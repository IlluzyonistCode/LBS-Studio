#pragma once

#include "IPlugin.h"

/**
 * @class GrayscaleFilter
 * @brief Example plugin that converts frames to grayscale.
 *
 * Demonstrates the minimal IPlugin implementation required
 * for integration with the LBS Studio plugin system.
 *
 * To build as a standalone shared library, compile with:
 *   DEFINES += LBS_PLUGIN_BUILD
 * and export createPlugin() as the factory function.
 */
class GrayscaleFilter final : public IPlugin
{
public:
    GrayscaleFilter() = default;
    ~GrayscaleFilter() override = default;

    QString name()     const override;
    QString category() const override;

    bool   initialize() override;
    void   shutdown()   override;

    QImage processFrame(const QImage& input) override;
};

/* ------------------------------------------------------------------ */
/*  Plugin factory export (active when building as shared library)     */
/* ------------------------------------------------------------------ */

#ifdef LBS_PLUGIN_BUILD
extern "C" __declspec(dllexport) IPlugin* createPlugin();
#endif
