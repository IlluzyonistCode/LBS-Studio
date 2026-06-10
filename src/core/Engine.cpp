#include "Engine.h"
#include "Pipeline.h"
#include "AudioMixer.h"
#include "DesktopCapture.h"
#include "EncoderFactory.h"
#include "Logger.h"

Engine::Engine(QObject* parent)
    : QObject(parent)
    , m_pipeline(std::make_unique<Pipeline>(this))
    , m_audioMixer(std::make_unique<AudioMixer>(this))
{
    connect(m_pipeline.get(), &Pipeline::error,
            this, &Engine::engineError);

    m_audioMixer->addChannel({ "Desktop Audio", 0.8f, false });
    m_audioMixer->addChannel({ "Microphone",    0.7f, false });

    LOG_INFO("Engine", "Engine initialized.");
}

Engine::~Engine()
{
    stopCapture();
}

bool Engine::startCapture()
{
    auto source  = std::make_unique<DesktopCapture>(0, 0);
    auto encoder = EncoderFactory::create();

    m_pipeline->setCaptureSource(std::move(source));
    m_pipeline->setEncoder(std::move(encoder));

    if (!m_pipeline->start())
        return false;

    m_audioMixer->start();

    emit captureStarted();
    LOG_INFO("Engine", "Capture started.");
    return true;
}

void Engine::stopCapture()
{
    if (!m_pipeline->isActive())
        return;

    m_pipeline->stop();
    m_audioMixer->stop();

    emit captureStopped();
    LOG_INFO("Engine", "Capture stopped.");
}

bool Engine::isCapturing() const
{
    return m_pipeline->isActive();
}

Pipeline*   Engine::pipeline()   const { return m_pipeline.get();   }
AudioMixer* Engine::audioMixer() const { return m_audioMixer.get(); }
