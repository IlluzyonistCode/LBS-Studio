#pragma once

#include <QObject>
#include <memory>

class Pipeline;
class AudioMixer;

/**
 * @class Engine
 * @brief Top-level application core.
 *
 * Owns and coordinates the capture Pipeline and AudioMixer.
 * Exposes a clean API to the UI layer, insulating it from
 * all subsystem implementation details.
 */
class Engine : public QObject
{
    Q_OBJECT

public:
    explicit Engine(QObject* parent = nullptr);
    ~Engine() override;

    bool startCapture();
    void stopCapture();

    bool isCapturing() const;

    Pipeline*   pipeline()   const;
    AudioMixer* audioMixer() const;

signals:
    void captureStarted();
    void captureStopped();
    void engineError(QString message);

private:
    std::unique_ptr<Pipeline>   m_pipeline;
    std::unique_ptr<AudioMixer> m_audioMixer;
};
