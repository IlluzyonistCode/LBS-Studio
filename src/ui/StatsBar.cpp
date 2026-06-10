#include "StatsBar.h"
#include "Stats.h"

#include <QLabel>

StatsBar::StatsBar(QWidget* parent)
    : QStatusBar(parent)
{
    m_fpsLabel     = new QLabel("FPS: --",     this);
    m_bitrateLabel = new QLabel("Bitrate: --", this);
    m_droppedLabel = new QLabel("Dropped: --", this);
    m_cpuLabel     = new QLabel("CPU: --%",    this);
    m_gpuLabel     = new QLabel("GPU: --%",    this);

    auto* sep1 = new QLabel("|", this);
    auto* sep2 = new QLabel("|", this);
    auto* sep3 = new QLabel("|", this);
    auto* sep4 = new QLabel("|", this);

    addPermanentWidget(m_fpsLabel);
    addPermanentWidget(sep1);
    addPermanentWidget(m_bitrateLabel);
    addPermanentWidget(sep2);
    addPermanentWidget(m_droppedLabel);
    addPermanentWidget(sep3);
    addPermanentWidget(m_cpuLabel);
    addPermanentWidget(sep4);
    addPermanentWidget(m_gpuLabel);

    setSizeGripEnabled(false);
}

void StatsBar::refresh()
{
    const Stats& s = Stats::instance();

    m_fpsLabel->setText(
        QString("FPS: %1").arg(s.fps(), 0, 'f', 1));

    m_bitrateLabel->setText(
        QString("Bitrate: %1 Mbps").arg(s.bitrateMbps(), 0, 'f', 2));

    m_droppedLabel->setText(
        QString("Dropped: %1").arg(s.droppedFrames()));

    m_cpuLabel->setText(
        QString("CPU: %1%").arg(static_cast<int>(s.cpuLoad())));

    m_gpuLabel->setText(
        QString("GPU: %1%").arg(static_cast<int>(s.gpuLoad())));
}
