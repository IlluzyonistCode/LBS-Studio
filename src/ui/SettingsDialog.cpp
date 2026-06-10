#include "SettingsDialog.h"

#include <QTabWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QSettings>

SettingsDialog::SettingsDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Settings");
    setMinimumSize(520, 420);
    setModal(true);

    setupUi();
    loadSettings();
}

/* ------------------------------------------------------------------ */
/*  UI Construction                                                     */
/* ------------------------------------------------------------------ */

void SettingsDialog::setupUi()
{
    auto* root = new QVBoxLayout(this);
    root->setSpacing(12);
    root->setContentsMargins(16, 16, 16, 16);

    auto* tabs = new QTabWidget(this);

    auto* outputTab   = new QWidget(tabs);
    auto* encodingTab = new QWidget(tabs);
    auto* audioTab    = new QWidget(tabs);
    auto* advancedTab = new QWidget(tabs);

    setupOutputTab(outputTab);
    setupEncodingTab(encodingTab);
    setupAudioTab(audioTab);
    setupAdvancedTab(advancedTab);

    tabs->addTab(outputTab,   "Output");
    tabs->addTab(encodingTab, "Encoding");
    tabs->addTab(audioTab,    "Audio");
    tabs->addTab(advancedTab, "Advanced");

    m_buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok |
        QDialogButtonBox::Cancel |
        QDialogButtonBox::RestoreDefaults,
        this);

    connect(m_buttons, &QDialogButtonBox::accepted,
            this,      &SettingsDialog::accept);

    connect(m_buttons, &QDialogButtonBox::rejected,
            this,      &QDialog::reject);

    connect(m_buttons->button(QDialogButtonBox::RestoreDefaults),
            &QPushButton::clicked,
            this, &SettingsDialog::restoreDefaults);

    root->addWidget(tabs);
    root->addWidget(m_buttons);
}

void SettingsDialog::setupOutputTab(QWidget* tab)
{
    auto* layout = new QVBoxLayout(tab);
    layout->setSpacing(16);

    /* Resolution & FPS group */
    auto* videoGroup  = new QGroupBox("Video Output", tab);
    auto* videoLayout = new QFormLayout(videoGroup);
    videoLayout->setSpacing(10);

    m_resolutionCombo = new QComboBox(videoGroup);
    m_resolutionCombo->addItems({
        "1920x1080", "1280x720", "2560x1440", "3840x2160", "854x480"
    });

    m_fpsCombo = new QComboBox(videoGroup);
    m_fpsCombo->addItems({ "60", "30", "24", "25", "50" });

    videoLayout->addRow("Resolution:", m_resolutionCombo);
    videoLayout->addRow("Frame Rate:", m_fpsCombo);

    /* Output path group */
    auto* pathGroup  = new QGroupBox("Recording Path", tab);
    auto* pathLayout = new QHBoxLayout(pathGroup);

    m_outputPathEdit = new QLineEdit(pathGroup);
    m_outputPathEdit->setPlaceholderText("Select output directory...");

    auto* browseBtn = new QPushButton("Browse", pathGroup);
    connect(browseBtn, &QPushButton::clicked, this, [this]
    {
        const QString dir = QFileDialog::getExistingDirectory(
            this, "Select Output Directory",
            m_outputPathEdit->text());

        if (!dir.isEmpty())
            m_outputPathEdit->setText(dir);
    });

    pathLayout->addWidget(m_outputPathEdit);
    pathLayout->addWidget(browseBtn);

    layout->addWidget(videoGroup);
    layout->addWidget(pathGroup);
    layout->addStretch();
}

void SettingsDialog::setupEncodingTab(QWidget* tab)
{
    auto* layout = new QVBoxLayout(tab);
    layout->setSpacing(16);

    auto* group       = new QGroupBox("Codec Settings", tab);
    auto* formLayout  = new QFormLayout(group);
    formLayout->setSpacing(10);

    m_codecCombo = new QComboBox(group);
    m_codecCombo->addItems({ "H.264", "H.265 (HEVC)", "AV1" });

    m_presetCombo = new QComboBox(group);
    m_presetCombo->addItems({
        "ultrafast", "superfast", "veryfast",
        "faster", "fast", "medium", "slow", "veryslow"
    });

    m_bitrateSpinBox = new QSpinBox(group);
    m_bitrateSpinBox->setRange(500, 50000);
    m_bitrateSpinBox->setSingleStep(500);
    m_bitrateSpinBox->setSuffix(" kbps");

    m_hardwareAccelCheck = new QCheckBox("Enable hardware acceleration", group);

    formLayout->addRow("Codec:",              m_codecCombo);
    formLayout->addRow("Encoder preset:",     m_presetCombo);
    formLayout->addRow("Video bitrate:",      m_bitrateSpinBox);
    formLayout->addRow("",                    m_hardwareAccelCheck);

    layout->addWidget(group);
    layout->addStretch();
}

void SettingsDialog::setupAudioTab(QWidget* tab)
{
    auto* layout = new QVBoxLayout(tab);
    layout->setSpacing(16);

    auto* group      = new QGroupBox("Audio Settings", tab);
    auto* formLayout = new QFormLayout(group);
    formLayout->setSpacing(10);

    m_sampleRateSpinBox = new QSpinBox(group);
    m_sampleRateSpinBox->setRange(22050, 192000);
    m_sampleRateSpinBox->setSingleStep(100);
    m_sampleRateSpinBox->setSuffix(" Hz");

    m_channelsCombo = new QComboBox(group);
    m_channelsCombo->addItems({ "Stereo (2ch)", "Mono (1ch)", "5.1 (6ch)" });

    m_audioBitrateSpinBox = new QSpinBox(group);
    m_audioBitrateSpinBox->setRange(64, 320);
    m_audioBitrateSpinBox->setSingleStep(32);
    m_audioBitrateSpinBox->setSuffix(" kbps");

    formLayout->addRow("Sample rate:",   m_sampleRateSpinBox);
    formLayout->addRow("Channels:",      m_channelsCombo);
    formLayout->addRow("Audio bitrate:", m_audioBitrateSpinBox);

    layout->addWidget(group);
    layout->addStretch();
}

void SettingsDialog::setupAdvancedTab(QWidget* tab)
{
    auto* layout = new QVBoxLayout(tab);
    layout->setSpacing(16);

    auto* group      = new QGroupBox("Pipeline", tab);
    auto* formLayout = new QFormLayout(group);
    formLayout->setSpacing(10);

    m_bufferSizeSpinBox = new QSpinBox(group);
    m_bufferSizeSpinBox->setRange(2, 64);
    m_bufferSizeSpinBox->setSuffix(" frames");

    m_verboseLogCheck = new QCheckBox("Enable verbose logging", group);

    formLayout->addRow("Frame buffer size:", m_bufferSizeSpinBox);
    formLayout->addRow("",                   m_verboseLogCheck);

    layout->addWidget(group);
    layout->addStretch();
}

/* ------------------------------------------------------------------ */
/*  Persistence                                                         */
/* ------------------------------------------------------------------ */

void SettingsDialog::loadSettings()
{
    QSettings s("LBS", "LBS_Studio");

    m_resolutionCombo->setCurrentText(s.value("output/resolution", "1280x720").toString());
    m_fpsCombo->setCurrentText(s.value("output/fps", "60").toString());
    m_outputPathEdit->setText(s.value("output/path", "").toString());

    m_codecCombo->setCurrentText(s.value("encoding/codec", "H.264").toString());
    m_presetCombo->setCurrentText(s.value("encoding/preset", "veryfast").toString());
    m_bitrateSpinBox->setValue(s.value("encoding/bitrate", 6000).toInt());
    m_hardwareAccelCheck->setChecked(s.value("encoding/hwaccel", true).toBool());

    m_sampleRateSpinBox->setValue(s.value("audio/samplerate", 48000).toInt());
    m_channelsCombo->setCurrentText(s.value("audio/channels", "Stereo (2ch)").toString());
    m_audioBitrateSpinBox->setValue(s.value("audio/bitrate", 160).toInt());

    m_bufferSizeSpinBox->setValue(s.value("advanced/buffersize", 8).toInt());
    m_verboseLogCheck->setChecked(s.value("advanced/verboselog", false).toBool());
}

void SettingsDialog::saveSettings()
{
    QSettings s("LBS", "LBS_Studio");

    s.setValue("output/resolution", m_resolutionCombo->currentText());
    s.setValue("output/fps",        m_fpsCombo->currentText());
    s.setValue("output/path",       m_outputPathEdit->text());

    s.setValue("encoding/codec",   m_codecCombo->currentText());
    s.setValue("encoding/preset",  m_presetCombo->currentText());
    s.setValue("encoding/bitrate", m_bitrateSpinBox->value());
    s.setValue("encoding/hwaccel", m_hardwareAccelCheck->isChecked());

    s.setValue("audio/samplerate", m_sampleRateSpinBox->value());
    s.setValue("audio/channels",   m_channelsCombo->currentText());
    s.setValue("audio/bitrate",    m_audioBitrateSpinBox->value());

    s.setValue("advanced/buffersize", m_bufferSizeSpinBox->value());
    s.setValue("advanced/verboselog", m_verboseLogCheck->isChecked());
}

void SettingsDialog::accept()
{
    saveSettings();
    QDialog::accept();
}

void SettingsDialog::restoreDefaults()
{
    m_resolutionCombo->setCurrentText("1280x720");
    m_fpsCombo->setCurrentText("60");
    m_outputPathEdit->clear();

    m_codecCombo->setCurrentText("H.264");
    m_presetCombo->setCurrentText("veryfast");
    m_bitrateSpinBox->setValue(6000);
    m_hardwareAccelCheck->setChecked(true);

    m_sampleRateSpinBox->setValue(48000);
    m_channelsCombo->setCurrentText("Stereo (2ch)");
    m_audioBitrateSpinBox->setValue(160);

    m_bufferSizeSpinBox->setValue(8);
    m_verboseLogCheck->setChecked(false);
}
