#pragma once

#include "IEncoder.h"
#include <memory>

/**
 * @class EncoderFactory
 * @brief Creates encoder instances based on availability and preference.
 *
 * Probes hardware capabilities at startup and falls back gracefully
 * to software encoding when acceleration is unavailable.
 */
class EncoderFactory
{
public:
    enum class Backend { Software, NVENC, AMF, QSV, Auto };

    static std::unique_ptr<IEncoder> create(Backend preference = Backend::Auto);

private:
    static bool probeNVENC();
    static bool probeAMF();
    static bool probeQSV();
};
