#pragma once

#include <QStatusBar>

class QLabel;

/**
 * @class StatsBar
 * @brief Status bar displaying real-time pipeline performance metrics.
 *
 * Refreshed once per second via Stats::updated() signal.
 * Shows FPS, bitrate, dropped frames, and CPU/GPU load.
 */
class StatsBar : public QStatusBar
{
    Q_OBJECT

public:
    explicit StatsBar(QWidget* parent = nullptr);

public slots:
    void refresh();

private:
    QLabel* m_fpsLabel;
    QLabel* m_bitrateLabel;
    QLabel* m_droppedLabel;
    QLabel* m_cpuLabel;
    QLabel* m_gpuLabel;
};
