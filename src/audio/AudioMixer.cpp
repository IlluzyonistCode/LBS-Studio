#include "AudioMixer.h"
#include "Logger.h"

#include <QThread>
#include <QTimer>
#include <cmath>
#include <random>

AudioMixer::AudioMixer(QObject* parent)
    : QObject(parent)
{
}

AudioMixer::~AudioMixer()
{
    stop();
}

void AudioMixer::addChannel(const Channel& channel)
{
    QMutexLocker lock(&m_channelMutex);
    m_channels.append(channel);
}

void AudioMixer::setVolume(int index, float volume)
{
    QMutexLocker lock(&m_channelMutex);
    if (index >= 0 && index < m_channels.size())
        m_channels[index].volume = std::clamp(volume, 0.0f, 1.0f);
}

void AudioMixer::setMuted(int index, bool muted)
{
    QMutexLocker lock(&m_channelMutex);
    if (index >= 0 && index < m_channels.size())
        m_channels[index].muted = muted;
}

int AudioMixer::channelCount() const
{
    QMutexLocker lock(&m_channelMutex);
    return m_channels.size();
}

void AudioMixer::start()
{
    if (m_running.load())
        return;

    m_running.store(true);

    m_thread = std::make_unique<QThread>();

    /* Move this object to the worker thread so process() runs there */
    moveToThread(m_thread.get());

    connect(m_thread.get(), &QThread::started, this, [this]
    {
        auto* meterTimer = new QTimer(this);
        meterTimer->setInterval(33); // ~30 Hz
        connect(meterTimer, &QTimer::timeout, this, &AudioMixer::process);
        meterTimer->start();
    });

    connect(m_thread.get(), &QThread::finished, this, [this]
    {
        /* Move back to main thread after worker finishes */
        moveToThread(QThread::currentThread());
    });

    m_thread->start(QThread::TimeCriticalPriority);

    LOG_INFO("AudioMixer", "Audio thread started.");
}

void AudioMixer::stop()
{
    if (!m_running.load())
        return;

    m_running.store(false);

    if (m_thread && m_thread->isRunning())
    {
        m_thread->quit();
        m_thread->wait(2000);
    }

    m_thread.reset();

    LOG_INFO("AudioMixer", "Audio thread stopped.");
}

void AudioMixer::process()
{
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    QMutexLocker lock(&m_channelMutex);

    QVector<float> peaks;
    peaks.reserve(m_channels.size());

    for (const auto& ch : m_channels)
    {
        const float raw  = dist(rng) * (ch.muted ? 0.0f : ch.volume);
        const float peak = std::sqrt(raw); // gamma for visual balance
        peaks.append(peak);
    }

    emit levelsUpdated(peaks);
}
