/**
 * @file main.cpp
 * @brief LBS Studio application entry point.
 *
 * Bootstraps the Qt application, enables High DPI scaling,
 * initializes the logger, and launches the main window.
 */

#include "MainWindow.h"
#include "Logger.h"

#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char* argv[])
{
    QApplication::setHighDpiScaleFactorRoundingPolicy(
        Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    QApplication app(argc, argv);
    app.setApplicationName("LBS Studio");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("LBS");

    /* Default OpenGL surface for compositing layers */
    QSurfaceFormat fmt;
    fmt.setDepthBufferSize(24);
    fmt.setStencilBufferSize(8);
    fmt.setVersion(3, 3);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(fmt);

    /* Logger setup */
    Logger::instance().setLevel(Logger::Level::Debug);
    Logger::instance().setOutputFile("lbs_studio.log");

    LOG_INFO("Main", "LBS Studio starting...");

    MainWindow window;
    window.show();

    const int result = app.exec();

    LOG_INFO("Main", "LBS Studio shutdown complete.");
    return result;
}
