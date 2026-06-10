#pragma once

#include <QWidget>

class QPushButton;
class QLabel;
class QLCDNumber;
class QTimer;

/**
 * @class ControlBar
 * @brief Primary transport controls: Stream, Record, and elapsed time display.
 *
 * Emits discrete signals for each action so MainWindow can delegate
 * cleanly to the Engine without ControlBar knowing about Engine internals.
 */
class ControlBar : public QWidget
{
    Q_OBJECT

public:
    explicit ControlBar(QWidget* parent = nullptr);

    void setStreamingActive(bool active);
    void setRecordingActive(bool active);

signals:
    void startStreamRequested();
    void stopStreamRequested();
    void startRecordRequested();
    void stopRecordRequested();

private slots:
    void onStreamToggle();
    void onRecordToggle();
    void tickClock();

private:
    void setupUi();
    void setupConnections();

    QPushButton* m_streamBtn;
    QPushButton* m_recordBtn;
    QLabel*      m_clockLabel;
    QTimer*      m_clockTimer;

    int  m_elapsedSeconds { 0 };
    bool m_streaming      { false };
    bool m_recording      { false };
};
