#include "ControlBar.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QStyle>

ControlBar::ControlBar(QWidget* parent)
    : QWidget(parent)
{
    setupUi();
    setupConnections();
}

void ControlBar::setupUi()
{
    setFixedHeight(52);

    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(12, 8, 12, 8);
    layout->setSpacing(10);

    m_streamBtn = new QPushButton("Start Streaming", this);
    m_streamBtn->setObjectName("startButton");
    m_streamBtn->setMinimumWidth(140);

    m_recordBtn = new QPushButton("Start Recording", this);
    m_recordBtn->setMinimumWidth(140);

    m_clockLabel = new QLabel("00:00:00", this);
    m_clockLabel->setStyleSheet(
        "font-family: monospace;"
        "font-size: 18px;"
        "color: #4A4B50;"
        "min-width: 90px;");
    m_clockLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    m_clockTimer = new QTimer(this);
    m_clockTimer->setInterval(1000);

    layout->addWidget(m_streamBtn);
    layout->addWidget(m_recordBtn);
    layout->addStretch();
    layout->addWidget(m_clockLabel);
}

void ControlBar::setupConnections()
{
    connect(m_streamBtn, &QPushButton::clicked,
            this,        &ControlBar::onStreamToggle);

    connect(m_recordBtn, &QPushButton::clicked,
            this,        &ControlBar::onRecordToggle);

    connect(m_clockTimer, &QTimer::timeout,
            this,         &ControlBar::tickClock);
}

/* ------------------------------------------------------------------ */
/*  Public API                                                          */
/* ------------------------------------------------------------------ */

void ControlBar::setStreamingActive(bool active)
{
    m_streaming = active;

    if (active)
    {
        m_streamBtn->setText("Stop Streaming");
        m_streamBtn->setObjectName("stopButton");

        m_elapsedSeconds = 0;
        m_clockTimer->start();
        m_clockLabel->setStyleSheet(
            "font-family: monospace;"
            "font-size: 18px;"
            "color: #2ECC71;"
            "min-width: 90px;");
    }
    else
    {
        m_streamBtn->setText("Start Streaming");
        m_streamBtn->setObjectName("startButton");

        if (!m_recording)
            m_clockTimer->stop();

        m_clockLabel->setStyleSheet(
            "font-family: monospace;"
            "font-size: 18px;"
            "color: #4A4B50;"
            "min-width: 90px;");
    }

    /* Force style recalculation after objectName change */
    m_streamBtn->style()->unpolish(m_streamBtn);
    m_streamBtn->style()->polish(m_streamBtn);
}

void ControlBar::setRecordingActive(bool active)
{
    m_recording = active;

    if (active)
    {
        m_recordBtn->setText("Stop Recording");
        m_recordBtn->setObjectName("stopButton");
        m_clockTimer->start();
    }
    else
    {
        m_recordBtn->setText("Start Recording");
        m_recordBtn->setObjectName("startButton");

        if (!m_streaming)
            m_clockTimer->stop();
    }

    m_recordBtn->style()->unpolish(m_recordBtn);
    m_recordBtn->style()->polish(m_recordBtn);
}

/* ------------------------------------------------------------------ */
/*  Slots                                                               */
/* ------------------------------------------------------------------ */

void ControlBar::onStreamToggle()
{
    if (!m_streaming)
        emit startStreamRequested();
    else
        emit stopStreamRequested();
}

void ControlBar::onRecordToggle()
{
    if (!m_recording)
        emit startRecordRequested();
    else
        emit stopRecordRequested();
}

void ControlBar::tickClock()
{
    ++m_elapsedSeconds;

    const int h = m_elapsedSeconds / 3600;
    const int m = (m_elapsedSeconds % 3600) / 60;
    const int s = m_elapsedSeconds % 60;

    m_clockLabel->setText(
        QString("%1:%2:%3")
            .arg(h, 2, 10, QChar('0'))
            .arg(m, 2, 10, QChar('0'))
            .arg(s, 2, 10, QChar('0')));
}
