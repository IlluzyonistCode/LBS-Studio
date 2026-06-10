#pragma once

#include <QString>
#include <QSize>

/**
 * @class AppSettings
 * @brief Typed accessor facade over QSettings.
 *
 * Centralizes all default values and key strings in one place,
 * preventing magic-string proliferation across the codebase.
 * All getters return sensible defaults when no stored value exists.
 */
class AppSettings
{
public:
    static AppSettings& instance();

    /* -- Output ---------------------------------------------------- */
    QSize   outputResolution() const;
    int     outputFps()        const;
    QString outputPath()       const;

    void setOutputResolution(const QSize& size);
    void setOutputFps(int fps);
    void setOutputPath(const QString& path);

    /* -- Encoding -------------------------------------------------- */
    QString encodingCodec()   const;
    QString encodingPreset()  const;
    int     encodingBitrate() const;
    bool    hardwareAccel()   const;

    void setEncodingCodec(const QString& codec);
    void setEncodingPreset(const QString& preset);
    void setEncodingBitrate(int kbps);
    void setHardwareAccel(bool enabled);

    /* -- Audio ----------------------------------------------------- */
    int     audioSampleRate() const;
    int     audioChannels()   const;
    int     audioBitrate()    const;

    void setAudioSampleRate(int hz);
    void setAudioChannels(int count);
    void setAudioBitrate(int kbps);

    /* -- Advanced -------------------------------------------------- */
    int  frameBufferSize() const;
    bool verboseLog()      const;

    void setFrameBufferSize(int frames);
    void setVerboseLog(bool enabled);

    /** @brief Persist all settings to disk immediately. */
    void sync();

private:
    AppSettings() = default;
    ~AppSettings()  = default;

    AppSettings(const AppSettings&)            = delete;
    AppSettings& operator=(const AppSettings&) = delete;
};
