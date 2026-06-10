#pragma once

#include <QImage>
#include <QString>
#include <functional>

/**
 * @interface ICaptureSource
 * @brief Abstraction over any video capture backend.
 *
 * Concrete implementations may wrap DXGI desktop duplication,
 * Windows Graphics Capture, webcams, or any other source.
 */
class ICaptureSource
{
public:
    using FrameCallback = std::function<void(const QImage&)>;

    virtual ~ICaptureSource() = default;

    /** @brief Human-readable source name. */
    virtual QString name() const = 0;

    /** @brief Attach a callback invoked on every captured frame. */
    virtual void setFrameCallback(FrameCallback callback) = 0;

    /** @brief Begin capture. Returns true if successful. */
    virtual bool start() = 0;

    /** @brief Halt capture and free GPU/OS resources. */
    virtual void stop() = 0;

    /** @brief True while actively capturing. */
    virtual bool isRunning() const = 0;
};
