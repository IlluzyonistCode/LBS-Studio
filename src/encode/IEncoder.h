#pragma once

#include <QImage>
#include <QString>

/**
 * @interface IEncoder
 * @brief Abstract codec contract for video encoding backends.
 *
 * Implementations may wrap H.264/H.265 software (x264, x265)
 * or hardware paths (NVENC, AMF, QSV) behind this interface.
 */
class IEncoder
{
public:
    struct Config
    {
        int     width   { 1280        };
        int     height  { 720         };
        int     fps     { 60          };
        int     bitrate { 6000        }; /**< kbps */
        QString codec   { "H264"      };
        QString preset  { "ultrafast" };
    };

    virtual ~IEncoder() = default;

    /** @brief Apply or update codec configuration. Thread-safe. */
    virtual bool configure(const Config& config) = 0;

    /** @brief Submit a frame for encoding. Returns encoded byte count. */
    virtual qint64 encodeFrame(const QImage& frame) = 0;

    /** @brief Flush remaining frames and finalize output stream. */
    virtual void flush() = 0;

    /** @brief Encoder backend name (e.g., "x264", "NVENC"). */
    virtual QString backendName() const = 0;
};
