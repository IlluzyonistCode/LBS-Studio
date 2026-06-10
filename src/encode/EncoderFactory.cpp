#include "EncoderFactory.h"
#include "Logger.h"

/* ------------------------------------------------------------------ */
/*  Null encoder stub.                                                  */
/*  Replace with real x264/x265/NVENC/AMF codec integration.          */
/* ------------------------------------------------------------------ */

class NullEncoder final : public IEncoder
{
public:
    bool configure(const Config&) override { return true; }

    qint64 encodeFrame(const QImage&) override
    {
        /* Simulate 6000 kbps at 60 fps = ~12500 bytes per frame */
        return 12500;
    }

    void flush() override {}

    QString backendName() const override { return "NullEncoder (stub)"; }
};

/* ------------------------------------------------------------------ */

std::unique_ptr<IEncoder> EncoderFactory::create(Backend preference)
{
    if (preference == Backend::Auto)
    {
        if      (probeNVENC()) preference = Backend::NVENC;
        else if (probeAMF())   preference = Backend::AMF;
        else if (probeQSV())   preference = Backend::QSV;
        else                   preference = Backend::Software;
    }

    LOG_INFO("EncoderFactory",
             "Selected backend index: " +
             QString::number(static_cast<int>(preference)));

    return std::make_unique<NullEncoder>();
}

bool EncoderFactory::probeNVENC() { return false; }
bool EncoderFactory::probeAMF()   { return false; }
bool EncoderFactory::probeQSV()   { return false; }
