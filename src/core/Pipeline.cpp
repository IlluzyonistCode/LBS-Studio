#include "Pipeline.h"
#include "ICaptureSource.h"
#include "IEncoder.h"
#include "IPlugin.h"
#include "FrameBuffer.h"
#include "Stats.h"
#include "Logger.h"

#include <QThread>
#include <algorithm>

Pipeline::Pipeline(QObject* parent)
    : QObject(parent)
    , m_buffer(std::make_unique<FrameBuffer>(8))
{
}

Pipeline::~Pipeline()
{
    stop();
}

void Pipeline::setCaptureSource(std::unique_ptr<ICaptureSource> source)
{
    m_source = std::move(source);

    m_source->setFrameCallback([this](const QImage& frame)
    {
        onFrame(frame);
    });
}

void Pipeline::setEncoder(std::unique_ptr<IEncoder> encoder)
{
    m_encoder = std::move(encoder);
}

void Pipeline::addPlugin(std::shared_ptr<IPlugin> plugin)
{
    m_plugins.push_back(std::move(plugin));
}

void Pipeline::removePlugin(const QString& name)
{
    m_plugins.erase(
        std::remove_if(m_plugins.begin(), m_plugins.end(),
            [&name](const auto& p) { return p->name() == name; }),
        m_plugins.end());
}

bool Pipeline::start()
{
    if (m_active.load())
        return false;

    if (!m_source || !m_encoder)
    {
        emit error("Pipeline requires both a capture source and an encoder.");
        return false;
    }

    m_active.store(true);

    /* Reset buffer for fresh run */
    m_buffer = std::make_unique<FrameBuffer>(8);

    /* Encode thread */
    m_encodeThread = std::make_unique<QThread>();

    connect(m_encodeThread.get(), &QThread::started, [this]
    {
        encodeLoop();
        QThread::currentThread()->quit();
    });

    m_encodeThread->start(QThread::HighPriority);

    m_source->start();

    LOG_INFO("Pipeline", "Pipeline started.");
    return true;
}

void Pipeline::stop()
{
    if (!m_active.load())
        return;

    m_active.store(false);

    if (m_source)
        m_source->stop();

    m_buffer->close();

    if (m_encodeThread && m_encodeThread->isRunning())
        m_encodeThread->wait(3000);

    m_encodeThread.reset();

    if (m_encoder)
        m_encoder->flush();

    LOG_INFO("Pipeline", "Pipeline stopped.");
}

bool Pipeline::isActive() const
{
    return m_active.load();
}

void Pipeline::onFrame(const QImage& raw)
{
    const QImage processed = applyPlugins(raw);

    emit frameReady(processed);

    m_buffer->push(processed);
}

QImage Pipeline::applyPlugins(const QImage& frame)
{
    QImage result = frame;

    for (auto& plugin : m_plugins)
        result = plugin->processFrame(result);

    return result;
}

void Pipeline::encodeLoop()
{
    while (m_active.load() || m_buffer->size() > 0)
    {
        auto optFrame = m_buffer->tryPop();

        if (!optFrame.has_value())
        {
            QThread::msleep(1);
            continue;
        }

        const qint64 bytes = m_encoder->encodeFrame(*optFrame);
        Stats::instance().recordEncodedBytes(bytes);
    }
}
