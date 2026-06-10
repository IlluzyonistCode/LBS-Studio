#pragma once

#include <QObject>
#include <QVector>
#include <QMutex>
#include <memory>
#include <atomic>

class QThread;

/**
 * @class AudioMixer
 * @brief High-priority audio processing thread with per-channel metering.
 *
 * Runs in its own QThread at TimeCriticalPriority to prevent A/V drift.
 * Emits metered peak levels for UI visualization at ~30 Hz.
 */
class AudioMixer : public QObject
{
    Q_OBJECT

public:
    struct Channel
    {
        QString name;
        float   volume { 1.0f  };
        bool    muted  { false };
    };

    explicit AudioMixer(QObject* parent = nullptr);
    ~AudioMixer() override;

    void addChannel(const Channel& channel);
    void setVolume(int channelIndex, float volume);
    void setMuted(int channelIndex, bool muted);

    int channelCount() const;

    void start();
    void stop();

signals:
    /** @brief Emits peak levels [0.0, 1.0] for each channel at ~30 Hz. */
    void levelsUpdated(QVector<float> peaks);

private slots:
    void process();

private:
    QVector<Channel>         m_channels;
    mutable QMutex           m_channelMutex;
    std::unique_ptr<QThread> m_thread;
    std::atomic<bool>        m_running { false };
};
