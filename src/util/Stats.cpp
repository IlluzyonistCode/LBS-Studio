#include "Stats.h"

#include <QTimer>

Stats::Stats()
{
    auto* timer = new QTimer(this);
    timer->setInterval(1000);

    connect(timer, &QTimer::timeout, this, [this]
    {
        const quint64 frames  = m_frameCount.load();
        const qint64  encoded = m_encodedBytes.load();

        m_fps.store(static_cast<double>(
            static_cast<qint64>(frames) - m_lastFrameCount));

        m_bitrateMbps.store(
            static_cast<double>(encoded - m_lastEncodedBytes) * 8.0 / 1'000'000.0);

        m_lastFrameCount   = static_cast<qint64>(frames);
        m_lastEncodedBytes = encoded;

        emit updated();
    });

    timer->start();
}

Stats& Stats::instance()
{
    static Stats inst;
    return inst;
}

void Stats::recordFrame()                { m_frameCount.fetch_add(1);    }
void Stats::recordDroppedFrame()         { m_droppedFrames.fetch_add(1); }
void Stats::recordEncodedBytes(qint64 b) { m_encodedBytes.fetch_add(b);  }
void Stats::setCpuLoad(float p)          { m_cpuLoad.store(p);           }
void Stats::setGpuLoad(float p)          { m_gpuLoad.store(p);           }

double  Stats::fps()           const { return m_fps.load();           }
quint64 Stats::droppedFrames() const { return m_droppedFrames.load(); }
double  Stats::bitrateMbps()   const { return m_bitrateMbps.load();   }
float   Stats::cpuLoad()       const { return m_cpuLoad.load();       }
float   Stats::gpuLoad()       const { return m_gpuLoad.load();       }

void Stats::reset()
{
    m_frameCount.store(0);
    m_droppedFrames.store(0);
    m_encodedBytes.store(0);
    m_lastFrameCount   = 0;
    m_lastEncodedBytes = 0;
}
