#include "AppSettings.h"

#include <QSettings>

/* ------------------------------------------------------------------ */
/*  Helpers                                                             */
/* ------------------------------------------------------------------ */

static QSettings& cfg()
{
    static QSettings s("LBS", "LBS_Studio");
    return s;
}

/* ------------------------------------------------------------------ */

AppSettings& AppSettings::instance()
{
    static AppSettings inst;
    return inst;
}

/* -- Output -------------------------------------------------------- */

QSize AppSettings::outputResolution() const
{
    const QString raw = cfg().value("output/resolution", "1280x720").toString();
    const QStringList parts = raw.split('x');

    if (parts.size() == 2)
        return QSize(parts[0].toInt(), parts[1].toInt());

    return QSize(1280, 720);
}

int AppSettings::outputFps() const
{
    return cfg().value("output/fps", 60).toInt();
}

QString AppSettings::outputPath() const
{
    return cfg().value("output/path", "").toString();
}

void AppSettings::setOutputResolution(const QSize& size)
{
    cfg().setValue("output/resolution",
                   QString("%1x%2").arg(size.width()).arg(size.height()));
}

void AppSettings::setOutputFps(int fps)    { cfg().setValue("output/fps",  fps);  }
void AppSettings::setOutputPath(const QString& p) { cfg().setValue("output/path", p); }

/* -- Encoding ------------------------------------------------------ */

QString AppSettings::encodingCodec()   const { return cfg().value("encoding/codec",   "H.264").toString();     }
QString AppSettings::encodingPreset()  const { return cfg().value("encoding/preset",  "veryfast").toString();  }
int     AppSettings::encodingBitrate() const { return cfg().value("encoding/bitrate", 6000).toInt();           }
bool    AppSettings::hardwareAccel()   const { return cfg().value("encoding/hwaccel", true).toBool();          }

void AppSettings::setEncodingCodec(const QString& c)  { cfg().setValue("encoding/codec",   c);       }
void AppSettings::setEncodingPreset(const QString& p) { cfg().setValue("encoding/preset",  p);       }
void AppSettings::setEncodingBitrate(int kbps)        { cfg().setValue("encoding/bitrate", kbps);    }
void AppSettings::setHardwareAccel(bool e)            { cfg().setValue("encoding/hwaccel", e);       }

/* -- Audio --------------------------------------------------------- */

int AppSettings::audioSampleRate() const { return cfg().value("audio/samplerate", 48000).toInt(); }
int AppSettings::audioChannels()   const { return cfg().value("audio/channels",   2).toInt();     }
int AppSettings::audioBitrate()    const { return cfg().value("audio/bitrate",    160).toInt();   }

void AppSettings::setAudioSampleRate(int hz)    { cfg().setValue("audio/samplerate", hz);    }
void AppSettings::setAudioChannels(int count)   { cfg().setValue("audio/channels",   count); }
void AppSettings::setAudioBitrate(int kbps)     { cfg().setValue("audio/bitrate",    kbps);  }

/* -- Advanced ------------------------------------------------------ */

int  AppSettings::frameBufferSize() const { return cfg().value("advanced/buffersize", 8).toInt();      }
bool AppSettings::verboseLog()      const { return cfg().value("advanced/verboselog", false).toBool(); }

void AppSettings::setFrameBufferSize(int f) { cfg().setValue("advanced/buffersize", f); }
void AppSettings::setVerboseLog(bool e)     { cfg().setValue("advanced/verboselog", e); }

void AppSettings::sync() { cfg().sync(); }
