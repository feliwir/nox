#include "VideoStream.hpp"

#include <functional>
#include <iostream>

#include "HWUtil.hpp"
#include "Platform.hpp"
#ifdef NOX_PLATFORM_WIN
#include "DXTextureInterop.hpp"
AVHWDeviceType nox::core::VideoStream::m_hwDeviceType =
    AV_HWDEVICE_TYPE_D3D11VA;
#else
AVHWDeviceType nox::core::VideoStream::m_hwDeviceType = AV_HWDEVICE_TYPE_VDPAU;
#endif

AVPixelFormat nox::core::VideoStream::GetHardwareFormat(
    AVCodecContext* ctx,
    const AVPixelFormat* pix_fmts)
{
    AVPixelFormat selected = AV_PIX_FMT_NONE;
    auto* video = (VideoStream*)ctx->opaque;
    const enum AVPixelFormat* p;
    AVBufferRef* new_frames_ctx = nullptr;

    for (p = pix_fmts; *p != -1; p++)
    {
        if (*p == video->m_hwPixelFormat)
        {
            if (avcodec_get_hw_frames_parameters(ctx, video->m_hwDeviceCtx, *p,
                                                 &new_frames_ctx) < 0)
            {
                break;
            }
            ctx->hw_frames_ctx = av_buffer_ref(new_frames_ctx);
            selected = *p;
            break;
        }
    }

    if (selected == AV_PIX_FMT_NONE)
    {
        video->m_usingHwDecode = false;
        AVPixelFormat fallback = avcodec_default_get_format(ctx, pix_fmts);

        std::cerr << "Failed to get HW surface format.\n";
    }
    else
    {
#ifdef NOX_PLATFORM_WIN
        if (selected == AV_PIX_FMT_D3D11)
        {
            auto fctx = (AVHWFramesContext*)ctx->hw_frames_ctx->data;
            auto hwctx = static_cast<AVD3D11VAFramesContext*>(fctx->hwctx);
            hwctx->MiscFlags = D3D11_RESOURCE_MISC_SHARED_NTHANDLE |
                               D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX;
        }
#else

#endif
    }
    return selected;
}

nox::core::VideoStream::VideoStream(AVStream*& stream)
    : IStream(stream),
      m_hwDeviceCtx(nullptr),
      m_hwPixelFormat(AV_PIX_FMT_NONE),
      m_usingHwDecode(true)
{
    int ret = 0;
    AVCodecParameters* codecPar = m_stream->codecpar;
    if (!(m_codec = avcodec_find_decoder(codecPar->codec_id)))
        return;

    if (!(m_codecCtx = avcodec_alloc_context3(m_codec)))
        return;

    if (avcodec_parameters_to_context(m_codecCtx, stream->codecpar) < 0)
        return;

    for (int i = 0;; i++)
    {
        const AVCodecHWConfig* config = avcodec_get_hw_config(m_codec, i);
        if (!config)
        {
            fprintf(stderr, "Decoder %s does not support device type %s.\n",
                    avcodec_get_name(codecPar->codec_id),
                    av_hwdevice_get_type_name(m_hwDeviceType));
            return;
        }

        if (config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX &&
            config->device_type == m_hwDeviceType)
        {
            m_hwPixelFormat = config->pix_fmt;
            break;
        }
    }
    m_codecCtx->opaque = this;
    m_codecCtx->get_format = GetHardwareFormat;

    if (!InitializeHardwareDevice())
        return;

    if ((ret = avcodec_open2(m_codecCtx, m_codec, nullptr)) < 0)
    {
    }
}

nox::core::VideoStream::~VideoStream()
{
    avcodec_free_context(&m_codecCtx);
    av_buffer_unref(&m_hwDeviceCtx);
}

float nox::core::VideoStream::GetDuration()
{
    return 0.0f;
}

float nox::core::VideoStream::GetPosition()
{
    return 0.0f;
}

nox::core::StreamType nox::core::VideoStream::GetType()
{
    return StreamType::Video;
}

bool nox::core::VideoStream::InitializeHardwareDevice()
{
    int err = 0;
    AVDictionary* opts = NULL; // "create" an empty dictionary
    av_dict_set_int(&opts, "debug", 1, 0); // add an entry

    if ((err = av_hwdevice_ctx_create(&m_hwDeviceCtx, m_hwDeviceType, nullptr, opts,
                                      0)) < 0)
    {
        std::cerr << "Failed to create specified HW device." << std::endl;
        return false;
    }
    m_codecCtx->hw_device_ctx = av_buffer_ref(m_hwDeviceCtx);

    switch (m_hwDeviceType)
    {
#ifdef NOX_PLATFORM_WIN
        case AV_HWDEVICE_TYPE_D3D11VA:
            m_texInterop = std::make_unique<DXTextureInterop>();
            break;
#endif
    }
    return true;
}

void nox::core::VideoStream::Process(AVPacket& pkt)
{
    AVFrame* frame = nullptr;
    int ret = 0;

    ret = avcodec_send_packet(m_codecCtx, &pkt);
    if (ret < 0)
    {
        // ERROR
        return;
    }

    while (ret >= 0)
    {
        if (!(frame = av_frame_alloc()))
        {
            // ERROR
            return;
        }

        ret = avcodec_receive_frame(m_codecCtx, frame);

        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
        {
            av_frame_free(&frame);
            return;
        }
        else if (ret < 0)
        {
            std::cerr << "Error while decoding" << std::endl;
            // ERROR
        }
        auto fctx = (AVHWFramesContext*)frame->hw_frames_ctx->data;
        m_texInterop->Process(fctx);
    }
}
