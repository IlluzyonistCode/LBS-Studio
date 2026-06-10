#include "GrayscaleFilter.h"
#include "Logger.h"

QString GrayscaleFilter::name()     const { return "Grayscale Filter"; }
QString GrayscaleFilter::category() const { return "filter";           }

bool GrayscaleFilter::initialize()
{
    LOG_INFO("GrayscaleFilter", "Initialized.");
    return true;
}

void GrayscaleFilter::shutdown()
{
    LOG_INFO("GrayscaleFilter", "Shutdown.");
}

QImage GrayscaleFilter::processFrame(const QImage& input)
{
    return input.convertToFormat(QImage::Format_Grayscale8)
                .convertToFormat(QImage::Format_RGBA8888);
}

/* ------------------------------------------------------------------ */
/*  Factory export                                                      */
/* ------------------------------------------------------------------ */

#ifdef LBS_PLUGIN_BUILD
extern "C" __declspec(dllexport) IPlugin* createPlugin()
{
    return new GrayscaleFilter();
}
#endif
