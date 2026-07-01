# LBS Studio

![C](https://img.shields.io/badge/C-A8B9CC.svg?style=flat-square&logo=C&logoColor=black)

## Overview

LBS Studio is a native C++ desktop application for screen capture and streaming, built with the Qt framework. It compiles to a standalone executable via qmake with Debug and Release configurations, and uses Qt resource bundling for UI assets.

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Project Structure](#project-structure)
- [Getting Started](#getting-started)
- [Contributing](#contributing)
- [License](#license)

---

## Features

|      | Component         | Details                                                                                                                                                                                                                                          |
| :--- | :---------------- | :----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| ⚙️  | **Architecture**  | <ul><li>Desktop GUI application built with **Qt framework** (`.pro` project file)</li><li>Dual build configuration: `Debug` / `Release` targets</li><li>C/C++ native application — compiled, not interpreted</li><li>Qt resource system via `resources.qrc` for bundled assets</li></ul> |
| 🔩 | **Code Quality**  | <ul><li>Mixed **C** and **C++** source files (`c`, `cpp`, `h`)</li><li>Separate object scripts per build target (`object_script.lbs_studio.debug`, `object_script.lbs_studio.release`)</li><li>No linter or static analysis tooling detected</li></ul> |
| 📄 | **Documentation** | <ul><li>`LICENSE` file present — project is openly licensed</li><li>No dedicated docs folder, wiki, or `README` detected</li><li>No inline doc generation tooling (e.g., Doxygen) identified</li></ul> |
| 🔌 | **Integrations**  | <ul><li>**Qt Framework** — core UI and application layer</li><li>Custom dark theme via `dark.qss` (Qt Style Sheet)</li><li>No external API or service integrations detected</li></ul> |
| 🧩 | **Modularity**    | <ul><li>Qt `.pro` file (`lbs studio.pro`) suggests single-project structure</li><li>Resource assets decoupled via `resources.qrc`</li><li>Styling isolated in `dark.qss` — theme is swappable</li><li>No submodule or multi-library structure detected</li></ul> |
| ⚡️  | **Performance**   | <ul><li>Native **C/C++** compilation — low-level performance potential</li><li>`Release` build target implies compiler optimizations enabled (e.g., `-O2`)</li><li>Qt's event-driven model for responsive UI</li></ul> |

---

## Project Structure

```
└── LBS Studio/
    ├── debug
    │   └── object_script.LBS_Studio.Debug
    ├── LBS Studio.pro
    ├── lbs_studio.log
    ├── LICENSE
    ├── Makefile
    ├── Makefile.Debug
    ├── Makefile.Release
    ├── README.md
    ├── release
    │   ├── AppSettings.o
    │   ├── AudioMixer.o
    │   ├── AudioMixerWidget.o
    │   ├── ControlBar.o
    │   ├── DesktopCapture.o
    │   ├── EncoderFactory.o
    │   ├── Engine.o
    │   ├── FrameBuffer.o
    │   ├── GrayscaleFilter.o
    │   ├── LBS_Studio.exe
    │   ├── Logger.o
    │   ├── main.o
    │   ├── MainWindow.o
    │   ├── moc_AudioMixer.cpp
    │   ├── moc_AudioMixer.o
    │   ├── moc_AudioMixerWidget.cpp
    │   ├── moc_AudioMixerWidget.o
    │   ├── moc_ControlBar.cpp
    │   ├── moc_ControlBar.o
    │   ├── moc_DesktopCapture.cpp
    │   ├── moc_DesktopCapture.o
    │   ├── moc_Engine.cpp
    │   ├── moc_Engine.o
    │   ├── moc_MainWindow.cpp
    │   ├── moc_MainWindow.o
    │   ├── moc_Pipeline.cpp
    │   ├── moc_Pipeline.o
    │   ├── moc_PluginManager.cpp
    │   ├── moc_PluginManager.o
    │   ├── moc_predefs.h
    │   ├── moc_PreviewWidget.cpp
    │   ├── moc_PreviewWidget.o
    │   ├── moc_ScenePanel.cpp
    │   ├── moc_ScenePanel.o
    │   ├── moc_SettingsDialog.cpp
    │   ├── moc_SettingsDialog.o
    │   ├── moc_SourcePanel.cpp
    │   ├── moc_SourcePanel.o
    │   ├── moc_Stats.cpp
    │   ├── moc_Stats.o
    │   ├── moc_StatsBar.cpp
    │   ├── moc_StatsBar.o
    │   ├── object_script.LBS_Studio.Release
    │   ├── Pipeline.o
    │   ├── PluginManager.o
    │   ├── PreviewWidget.o
    │   ├── qrc_resources.cpp
    │   ├── qrc_resources.o
    │   ├── ScenePanel.o
    │   ├── SettingsDialog.o
    │   ├── SourcePanel.o
    │   ├── Stats.o
    │   └── StatsBar.o
    ├── resources
    │   ├── resources.qrc
    │   └── styles
    └── src
        ├── audio
        ├── capture
        ├── core
        ├── encode
        ├── main.cpp
        ├── plugin
        ├── ui
        └── util
```

---

## Getting Started

### Prerequisites

- Python 3.10+ / Node.js 18+ *(depending on the stack above)*

### Installation

```sh
git clone "https://github.com/IlluzyonistCode/LBS-Studio"
cd "LBS-Studio"
qmake && make
```

### Usage

```sh
qmake && make
```

---

## Contributing

- [Report Issues](https://github.com/IlluzyonistCode/LBS-Studio/issues)
- [Submit Pull Requests](https://github.com/IlluzyonistCode/LBS-Studio/pulls)
- [Discussions](https://github.com/IlluzyonistCode/LBS-Studio/discussions)

---

## License

Distributed under the [AGPL-3.0](LICENSE) license.
