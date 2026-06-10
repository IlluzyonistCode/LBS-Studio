#pragma once

#include <QDialog>

class QTabWidget;
class QComboBox;
class QSpinBox;
class QLineEdit;
class QCheckBox;
class QDialogButtonBox;

/**
 * @class SettingsDialog
 * @brief Modal settings dialog for output, encoding, and audio configuration.
 *
 * Organized into tabs: Output, Encoding, Audio, Advanced.
 * All values are persisted via QSettings on accept.
 */
class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget* parent = nullptr);

private slots:
    void accept() override;
    void restoreDefaults();

private:
    void setupUi();
    void setupOutputTab(QWidget* tab);
    void setupEncodingTab(QWidget* tab);
    void setupAudioTab(QWidget* tab);
    void setupAdvancedTab(QWidget* tab);

    void loadSettings();
    void saveSettings();

    /* Output */
    QComboBox* m_resolutionCombo;
    QComboBox* m_fpsCombo;
    QLineEdit* m_outputPathEdit;

    /* Encoding */
    QComboBox* m_codecCombo;
    QComboBox* m_presetCombo;
    QSpinBox*  m_bitrateSpinBox;
    QCheckBox* m_hardwareAccelCheck;

    /* Audio */
    QSpinBox*  m_sampleRateSpinBox;
    QComboBox* m_channelsCombo;
    QSpinBox*  m_audioBitrateSpinBox;

    /* Advanced */
    QSpinBox*  m_bufferSizeSpinBox;
    QCheckBox* m_verboseLogCheck;

    QDialogButtonBox* m_buttons;
};
