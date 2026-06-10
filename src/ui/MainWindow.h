#pragma once

#include <QMainWindow>
#include <memory>

class Engine;
class PluginManager;
class PreviewWidget;
class ScenePanel;
class SourcePanel;
class AudioMixerWidget;
class StatsBar;
class ControlBar;
class QAction;

/**
 * @class MainWindow
 * @brief Root application window.
 *
 * Composes all UI panels into a docked layout and wires them
 * to the Engine via Qt signals and slots. Contains no business
 * logic — delegates entirely to Engine for all media operations.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void onStart();
    void onStop();
    void onEngineError(const QString& message);

private:
    void setupUi();
    void setupMenuBar();
    void setupConnections();
    void applyStyleSheet();

    std::unique_ptr<Engine>        m_engine;
    std::unique_ptr<PluginManager> m_pluginManager;

    ControlBar*       m_controlBar;
    PreviewWidget*    m_preview;
    ScenePanel*       m_scenePanel;
    SourcePanel*      m_sourcePanel;
    AudioMixerWidget* m_audioMixer;
    StatsBar*         m_statsBar;

    QAction* m_startAction;
    QAction* m_stopAction;
};
