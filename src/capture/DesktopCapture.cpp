#include "DesktopCapture.h"
#include "Logger.h"
#include "Stats.h"

#include <QThread>
#include <QElapsedTimer>
#include <QImage>

/* ------------------------------------------------------------------ */
/*  DXGI Desktop Duplication stub.                                     */
/*  Replace the frame synthesis block with actual D3D11/DXGI calls    */
/*  for production deployment on Windows.                              */
/* ------------------------------------------------------------------ */

DesktopCapture::DesktopCapture(int adapterIndex, int outputIndex, QObject* parent)
    : QObject(parent)
    , m_adapterIndex(adapterIndex)
    , m_outputIndex(outputIndex)
{
    LOG_INFO("DesktopCapture",
             QString("Initialized adapter=%1 output=%2")
                 .arg(adapterIndex)
                 .arg(outputIndex));
}

DesktopCapture::~DesktopCapture()
{
    stop();
}

QString DesktopCapture::name() const
{
    return QString("Desktop [Adapter %1, Output %2]")
               .arg(m_adapterIndex)
               .arg(m_outputIndex);
}

void DesktopCapture::setFrameCallback(FrameCallback callback)
{
    QMutexLocker lock(&m_callbackMutex);
    m_callback = std::move(callback);
}

bool DesktopCapture::start()
{
    if (m_running.load())
        return false;

    m_running.store(true);

    m_thread = std::make_unique<QThread>();

    connect(m_thread.get(), &QThread::started, [this]
    {
        constexpr int targetMs = 16; // ~60 fps

        QElapsedTimer timer;

        while (m_running.load())
        {
            timer.start();

            /* Stub: synthesize a 1280x720 black frame */
            QImage frame(1280, 720, QImage::Format_RGBA8888);
            frame.fill(Qt::black);

            {
                QMutexLocker lock(&m_callbackMutex);
                if (m_callback)
                    m_callback(frame);
            }

            Stats::instance().recordFrame();

            const int elapsed   = static_cast<int>(timer.elapsed());
            const int remaining = targetMs - elapsed;

            if (remaining > 0)
                QThread::msleep(static_cast<unsigned long>(remaining));
        }

        QThread::currentThread()->quit();
    });

    m_thread->start(QThread::HighPriority);

    LOG_INFO("DesktopCapture", "Capture thread started.");
    return true;
}

void DesktopCapture::stop()
{
    if (!m_running.load())
        return;

    m_running.store(false);

    if (m_thread && m_thread->isRunning())
        m_thread->wait(3000);

    m_thread.reset();

    LOG_INFO("DesktopCapture", "Capture stopped.");
}

bool DesktopCapture::isRunning() const
{
    return m_running.load();
}
