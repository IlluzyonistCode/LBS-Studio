QT       += core gui widgets multimedia multimediawidgets concurrent

TARGET   = LBS_Studio
TEMPLATE = app

CONFIG  += c++20
CONFIG  -= app_bundle

QMAKE_CXXFLAGS += -O2 -ffast-math -Wall -Wextra -Wpedantic

INCLUDEPATH += \
    src/core    \
    src/capture \
    src/encode  \
    src/audio   \
    src/plugin  \
    src/util    \
    src/ui

HEADERS += \
    src/core/Engine.h              \
    src/core/FrameBuffer.h         \
    src/core/Pipeline.h            \
    src/capture/ICaptureSource.h   \
    src/capture/DesktopCapture.h   \
    src/encode/IEncoder.h          \
    src/encode/EncoderFactory.h    \
    src/audio/AudioMixer.h         \
    src/plugin/IPlugin.h           \
    src/plugin/PluginManager.h     \
    src/plugin/GrayscaleFilter.h   \
    src/util/Logger.h              \
    src/util/Stats.h               \
    src/util/AppSettings.h         \
    src/ui/MainWindow.h            \
    src/ui/PreviewWidget.h         \
    src/ui/ScenePanel.h            \
    src/ui/SourcePanel.h           \
    src/ui/AudioMixerWidget.h      \
    src/ui/StatsBar.h              \
    src/ui/ControlBar.h            \
    src/ui/SettingsDialog.h

SOURCES += \
    src/main.cpp                      \
    src/core/Engine.cpp               \
    src/core/FrameBuffer.cpp          \
    src/core/Pipeline.cpp             \
    src/capture/DesktopCapture.cpp    \
    src/encode/EncoderFactory.cpp     \
    src/audio/AudioMixer.cpp          \
    src/plugin/PluginManager.cpp      \
    src/plugin/GrayscaleFilter.cpp    \
    src/util/Logger.cpp               \
    src/util/Stats.cpp                \
    src/util/AppSettings.cpp          \
    src/ui/MainWindow.cpp             \
    src/ui/PreviewWidget.cpp          \
    src/ui/ScenePanel.cpp             \
    src/ui/SourcePanel.cpp            \
    src/ui/AudioMixerWidget.cpp       \
    src/ui/StatsBar.cpp               \
    src/ui/ControlBar.cpp             \
    src/ui/SettingsDialog.cpp

RESOURCES += resources/resources.qrc

win32: LIBS += -lgdi32 -ld3d11 -ldxgi -lole32 -loleaut32
