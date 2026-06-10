#pragma once

#include <QObject>
#include <atomic>

/**
 * @class Stats
 * @brief Real-time performance counters for the capture pipeline.
 *
 * Counters are updated from worker threads using atomics,
 * then consumed by the UI thread via Qt signals at a fixed interval.
 */
class Stats : public QObject
{
    Q_OBJECT

public:
    static Stats& instance();

    void recordFrame();
    void recordDroppedFrame();
    void recordEncodedBytes(qint64 bytes);
    void setCpuLoad(float percent);
    void setGpuLoad(float percent);

    double  fps()           const;
    quint64 droppedFrames() const;
    double  bitrateMbps()   const;
    float   cpuLoad()       const;
    float   gpuLoad()       const;

    void reset();

signals:
    void updated();

private:
    Stats();
    ~Stats() override = default;

    Stats(const Stats&)            = delete;
    Stats& operator=(const Stats&) = delete;

    std::atomic<quint64> m_frameCount    { 0 };
    std::atomic<quint64> m_droppedFrames { 0 };
    std::atomic<qint64>  m_encodedBytes  { 0 };
    std::atomic<float>   m_cpuLoad       { 0.0f };
    std::atomic<float>   m_gpuLoad       { 0.0f };

    std::atomic<double> m_fps         { 0.0 };
    std::atomic<double> m_bitrateMbps { 0.0 };

    qint64 m_lastFrameCount   { 0 };
    qint64 m_lastEncodedBytes { 0 };
};
