#include "AudioMixerWidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QProgressBar>
#include <QPushButton>

AudioMixerWidget::AudioMixerWidget(QWidget* parent)
    : QWidget(parent)
{
    setupUi();
}

void AudioMixerWidget::setupUi()
{
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(8, 4, 8, 4);
    root->setSpacing(6);

    auto* label = new QLabel("AUDIO MIXER", this);
    label->setObjectName("sectionLabel");
    root->addWidget(label);

    auto* stripsRow = new QHBoxLayout;
    stripsRow->setSpacing(12);
    root->addLayout(stripsRow);

    buildStrip("Desktop", stripsRow);
    buildStrip("Mic",     stripsRow);

    stripsRow->addStretch();
}

void AudioMixerWidget::buildStrip(const QString& name, QHBoxLayout* row)
{
    auto* container = new QWidget(this);
    auto* layout    = new QVBoxLayout(container);
    layout->setSpacing(4);
    layout->setContentsMargins(4, 4, 4, 4);

    auto* nameLabel = new QLabel(name, container);
    nameLabel->setAlignment(Qt::AlignHCenter);

    auto* meter = new QProgressBar(container);
    meter->setOrientation(Qt::Vertical);
    meter->setRange(0, 100);
    meter->setValue(0);
    meter->setTextVisible(false);
    meter->setFixedWidth(12);
    meter->setMinimumHeight(80);

    auto* fader = new QSlider(Qt::Vertical, container);
    fader->setRange(0, 100);
    fader->setValue(80);
    fader->setMinimumHeight(80);

    auto* muteBtn = new QPushButton("M", container);
    muteBtn->setCheckable(true);
    muteBtn->setFixedSize(28, 28);

    auto* metersRow = new QHBoxLayout;
    metersRow->setSpacing(4);
    metersRow->addWidget(meter);
    metersRow->addWidget(fader);

    layout->addWidget(nameLabel);
    layout->addLayout(metersRow);
    layout->addWidget(muteBtn, 0, Qt::AlignHCenter);

    row->addWidget(container);

    m_strips.append({ fader, meter, muteBtn });
}

void AudioMixerWidget::updateLevels(QVector<float> peaks)
{
    for (int i = 0; i < peaks.size() && i < m_strips.size(); ++i)
    {
        const int value = static_cast<int>(peaks[i] * 100.0f);
        m_strips[i].meter->setValue(value);
    }
}
