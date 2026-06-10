# LBS Studio

High-performance streaming and recording engine built with **Qt** and **C++20**.

---

## Architecture

```
src/
├── core/          Engine · Pipeline · FrameBuffer
├── capture/       ICaptureSource · DesktopCapture
├── encode/        IEncoder · EncoderFactory
├── audio/         AudioMixer
├── plugin/        IPlugin · PluginManager · GrayscaleFilter
├── util/          Logger · Stats · AppSettings
└── ui/            MainWindow · PreviewWidget · ScenePanel
                   SourcePanel · AudioMixerWidget
                   StatsBar · ControlBar · SettingsDialog
```

### Key Design Decisions

| Concern | Solution |
|---|---|
| Thread safety | `QMutex` + `std::atomic` everywhere |
| Memory | RAII, `unique_ptr` / `shared_ptr`, zero raw `delete` |
| Cross-thread frames | Bounded `FrameBuffer` (backpressure, 8-frame cap) |
| Audio/Video sync | `TimeCriticalPriority` audio thread |
| Extensibility | `IPlugin` interface — add filters without touching core |
| UI responsiveness | All heavy work off the main thread; `QueuedConnection` |
| Settings | `AppSettings` singleton over `QSettings` (typed, no magic strings) |

---

## Build

### Requirements

- Qt 6.x (Widgets, Multimedia, Concurrent)
- MinGW 11+ or Clang 14+ with C++20 support
- qmake

### Steps

```bash
cd LBS_Studio
qmake LBS_Studio.pro
make -j$(nproc)
```

Or open `LBS_Studio.pro` directly in **Qt Creator** and press **Build**.

---

## Plugin System

Implement `IPlugin` in a shared library and export a C factory:

```cpp
extern "C" __declspec(dllexport) IPlugin* createPlugin()
{
    return new MyFilter();
}
```

Drop the `.dll` / `.so` into the `plugins/` directory next to the executable.
On startup, `PluginManager::discover("plugins")` loads all valid libraries automatically.

---

## Extending the Pipeline

- **New capture source** — implement `ICaptureSource`, pass to `Engine::startCapture()`
- **New encoder backend** — implement `IEncoder`, register in `EncoderFactory::create()`
- **New video filter** — implement `IPlugin`, drop into `plugins/`

---

## File Naming Convention

All image and resource filenames describe content only — no lab or version numbers embedded in names.
