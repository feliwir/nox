#pragma once

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
};

namespace nox::core
{
class HWUtil
{
   public:
    static inline AVPixelFormat FindFmtByType(const enum AVHWDeviceType type)
    {
        enum AVPixelFormat fmt;

        switch (type)
        {
            case AV_HWDEVICE_TYPE_VAAPI:
                fmt = AV_PIX_FMT_VAAPI;
                break;
            case AV_HWDEVICE_TYPE_DXVA2:
                fmt = AV_PIX_FMT_DXVA2_VLD;
                break;
            case AV_HWDEVICE_TYPE_D3D11VA:
                fmt = AV_PIX_FMT_D3D11;
                break;
            case AV_HWDEVICE_TYPE_VDPAU:
                fmt = AV_PIX_FMT_VDPAU;
                break;
            case AV_HWDEVICE_TYPE_VIDEOTOOLBOX:
                fmt = AV_PIX_FMT_VIDEOTOOLBOX;
                break;
            default:
                fmt = AV_PIX_FMT_NONE;
                break;
        }

        return fmt;
    }
};
} // namespace nox::core