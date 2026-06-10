#pragma once

#include <QString>
#include <QImage>

/**
 * @interface IPlugin
 * @brief Abstract contract for all LBS Studio extension plugins.
 *
 * Implement this interface to add custom filters or sources
 * without modifying the core pipeline.
 */
class IPlugin
{
public:
    virtual ~IPlugin() = default;

    /** @brief Human-readable plugin identifier. */
    virtual QString name() const = 0;

    /** @brief Plugin category string (e.g., "filter", "source"). */
    virtual QString category() const = 0;

    /** @brief Initialize plugin resources. Returns false on failure. */
    virtual bool initialize() = 0;

    /** @brief Release all plugin resources. */
    virtual void shutdown() = 0;

    /** @brief Process or generate a single video frame. */
    virtual QImage processFrame(const QImage& input) = 0;
};
