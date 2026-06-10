#pragma once

#include <QWidget>
#include <QVector>
#include <QHBoxLayout>

class QSlider;
class QProgressBar;
class QPushButton;

/**
 * @class AudioMixerWidget
 * @brief Visual audio mixer with per-channel faders and VU meters.
 *
 * Receives peak level data from AudioMixer via the levelsUpdated signal
 * and renders vertical progress bars to visualize signal intensity.
 */
class AudioMixerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AudioMixerWidget(QWidget* parent = nullptr);

public slots:
    void updateLevels(QVector<float> peaks);

private:
    struct ChannelStrip
    {
        QSlider*      fader;
        QProgressBar* meter;
        QPushButton*  muteBtn;
    };

    void setupUi();
    void buildStrip(const QString& name, QHBoxLayout* row);

    QVector<ChannelStrip> m_strips;
};
