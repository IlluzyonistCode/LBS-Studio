#include "MainWindow.h"
#include "Engine.h"
#include "Pipeline.h"
#include "AudioMixer.h"
#include "Stats.h"
#include "AppSettings.h"
#include "PluginManager.h"
#include "PreviewWidget.h"
#include "ScenePanel.h"
#include "SourcePanel.h"
#include "AudioMixerWidget.h"
#include "StatsBar.h"
#include "ControlBar.h"
#include "SettingsDialog.h"

#include <QMenuBar>
#include <QDockWidget>
#include <QSplitter>
#include <QVBoxLayout>
#include <QAction>
#include <QMessageBox>
#include <QCloseEvent>
#include <QFile>
#include <QApplication>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_engine(std::make_unique<Engine>(this))
    , m_pluginManager(std::make_unique<PluginManager>(this))
{
    setupUi();
    setupMenuBar();
    setupConnections();
    applyStyleSheet();

    m_pluginManager->discover("plugins");

    setWindowTitle("LBS Studio");
    resize(1280, 780);
}

MainWindow::~MainWindow() = default;

/* ------------------------------------------------------------------ */
/*  UI Construction                                                     */
/* ------------------------------------------------------------------ */

void MainWindow::setupUi()
{
    /* Central area: control bar stacked above preview */
    auto* centralWidget = new QWidget(this);
    auto* centralLayout = new QVBoxLayout(centralWidget);
    centralLayout->setContentsMargins(0, 0, 0, 0);
    centralLayout->setSpacing(0);

    m_controlBar = new ControlBar(centralWidget);
    m_preview    = new PreviewWidget(centralWidget);

    centralLayout->addWidget(m_controlBar);
    centralLayout->addWidget(m_preview, 1);

    setCentralWidget(centralWidget);

    /* Left dock: scenes + sources */
    auto* leftContent = new QWidget(this);
    auto* leftLayout  = new QVBoxLayout(leftContent);
    leftLayout->setContentsMargins(8, 8, 8, 8);
    leftLayout->setSpacing(0);

    m_scenePanel  = new ScenePanel(leftContent);
    m_sourcePanel = new SourcePanel(leftContent);

    auto* splitter = new QSplitter(Qt::Vertical, leftContent);
    splitter->addWidget(m_scenePanel);
    splitter->addWidget(m_sourcePanel);
    splitter->setSizes({ 220, 220 });

    leftLayout->addWidget(splitter);

    auto* leftDock = new QDockWidget("Scenes & Sources", this);
    leftDock->setWidget(leftContent);
    leftDock->setMinimumWidth(230);
    leftDock->setFeatures(QDockWidget::DockWidgetMovable |
                          QDockWidget::DockWidgetFloatable);
    addDockWidget(Qt::LeftDockWidgetArea, leftDock);

    /* Bottom dock: audio mixer */
    m_audioMixer = new AudioMixerWidget(this);

    auto* bottomDock = new QDockWidget("Audio Mixer", this);
    bottomDock->setWidget(m_audioMixer);
    bottomDock->setMaximumHeight(200);
    bottomDock->setFeatures(QDockWidget::DockWidgetMovable |
                            QDockWidget::DockWidgetFloatable);
    addDockWidget(Qt::BottomDockWidgetArea, bottomDock);

    /* Status bar */
    m_statsBar = new StatsBar(this);
    setStatusBar(m_statsBar);
}

void MainWindow::setupMenuBar()
{
    /* File --------------------------------------------------------- */
    auto* fileMenu = menuBar()->addMenu("File");

    fileMenu->addAction("Settings", this, [this]
    {
        SettingsDialog dlg(this);
        dlg.exec();
    });

    fileMenu->addSeparator();
    fileMenu->addAction("Exit", this, &QWidget::close);

    /* Capture ------------------------------------------------------ */
    auto* captureMenu = menuBar()->addMenu("Capture");

    m_startAction = new QAction("Start Streaming", this);
    m_stopAction  = new QAction("Stop Streaming",  this);

    m_startAction->setShortcut(QKeySequence(Qt::Key_F9));
    m_stopAction->setShortcut(QKeySequence(Qt::Key_F10));
    m_stopAction->setEnabled(false);

    connect(m_startAction, &QAction::triggered, this, &MainWindow::onStart);
    connect(m_stopAction,  &QAction::triggered, this, &MainWindow::onStop);

    captureMenu->addAction(m_startAction);
    captureMenu->addAction(m_stopAction);

    /* Plugins ------------------------------------------------------ */
    auto* pluginsMenu = menuBar()->addMenu("Plugins");

    pluginsMenu->addAction("Reload Plugins", this, [this]
    {
        m_pluginManager->unloadAll();
        m_pluginManager->discover("plugins");

        m_statsBar->showMessage(
            QString("  Plugins loaded: %1")
                .arg(m_pluginManager->count()), 3000);
    });

    pluginsMenu->addAction("Plugin List", this, [this]
    {
        const auto& plugins = m_pluginManager->plugins();
        QString list;

        for (const auto& p : plugins)
            list += QString("  [%1]  %2\n").arg(p->category()).arg(p->name());

        if (list.isEmpty())
            list = "No plugins loaded.";

        QMessageBox::information(this, "Loaded Plugins", list);
    });

    /* Help --------------------------------------------------------- */
    auto* helpMenu = menuBar()->addMenu("Help");

    helpMenu->addAction("About", this, [this]
    {
        QMessageBox::about(this, "LBS Studio",
            "LBS Studio v1.0\n\n"
            "High-performance streaming and recording engine.\n"
            "Built with Qt 6 and C++20.\n\n"
            "Modular SOLID architecture with plugin-extensible pipeline.\n"
            "Capture -> Filter -> Encode pipeline with bounded frame buffer.\n"
            "TimeCriticalPriority audio thread for zero A/V drift.");
    });
}

void MainWindow::setupConnections()
{
    /* Scene selection -> source panel */
    connect(m_scenePanel,  &ScenePanel::sceneSelected,
            m_sourcePanel, &SourcePanel::onSceneChanged);

    /* ControlBar transport */
    connect(m_controlBar, &ControlBar::startStreamRequested,
            this,         &MainWindow::onStart);

    connect(m_controlBar, &ControlBar::stopStreamRequested,
            this,         &MainWindow::onStop);

    connect(m_controlBar, &ControlBar::startRecordRequested, this, [this]
    {
        m_controlBar->setRecordingActive(true);
        m_statsBar->showMessage("  Recording...", 0);
    });

    connect(m_controlBar, &ControlBar::stopRecordRequested, this, [this]
    {
        m_controlBar->setRecordingActive(false);
        m_statsBar->showMessage("  Recording stopped.", 3000);
    });

    /* Pipeline frames -> preview (queued: cross-thread) */
    connect(m_engine->pipeline(), &Pipeline::frameReady,
            m_preview,            &PreviewWidget::setFrame,
            Qt::QueuedConnection);

    /* Audio levels -> mixer widget (queued: cross-thread) */
    connect(m_engine->audioMixer(), &AudioMixer::levelsUpdated,
            m_audioMixer,           &AudioMixerWidget::updateLevels,
            Qt::QueuedConnection);

    /* Stats refresh */
    connect(&Stats::instance(), &Stats::updated,
            m_statsBar,         &StatsBar::refresh);

    /* Engine lifecycle */
    connect(m_engine.get(), &Engine::captureStarted, this, [this]
    {
        m_startAction->setEnabled(false);
        m_stopAction->setEnabled(true);
        m_controlBar->setStreamingActive(true);
        m_statsBar->showMessage("  Streaming...", 0);
    });

    connect(m_engine.get(), &Engine::captureStopped, this, [this]
    {
        m_startAction->setEnabled(true);
        m_stopAction->setEnabled(false);
        m_controlBar->setStreamingActive(false);
        m_statsBar->showMessage("  Stopped.", 4000);
    });

    connect(m_engine.get(), &Engine::engineError,
            this,           &MainWindow::onEngineError);

    /* Plugin manager events */
    connect(m_pluginManager.get(), &PluginManager::pluginLoaded,
            this, [this](const QString& name)
    {
        m_statsBar->showMessage("  Plugin loaded: " + name, 2000);
    });
}

void MainWindow::applyStyleSheet()
{
    QFile qss(":/styles/dark.qss");

    if (qss.open(QIODevice::ReadOnly | QIODevice::Text))
        qApp->setStyleSheet(QString::fromUtf8(qss.readAll()));
}

/* ------------------------------------------------------------------ */
/*  Slots                                                               */
/* ------------------------------------------------------------------ */

void MainWindow::onStart()
{
    m_engine->startCapture();
}

void MainWindow::onStop()
{
    m_engine->stopCapture();
}

void MainWindow::onEngineError(const QString& message)
{
    QMessageBox::critical(this, "Engine Error", message);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    m_engine->stopCapture();
    AppSettings::instance().sync();
    event->accept();
}
