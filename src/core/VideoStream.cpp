#include "VideoStream.hpp"
#include "HWUtil.hpp"
#include "TextureBinder.hpp"
#include <functional>
#include <iostream>

AVHWDeviceType nox::core::VideoStream::m_hwDeviceType = AV_HWDEVICE_TYPE_D3D11VA;

AVPixelFormat nox::core::VideoStream::GetHardwareFormat(AVCodecContext* ctx,
	const AVPixelFormat* pix_fmts)
{
	VideoStream* video = (VideoStream*)ctx->opaque;
	const enum AVPixelFormat* p;

	for (p = pix_fmts; *p != -1; p++) 
	{
		if (*p == video->m_hwPixelFormat)
			return *p;
	}

	video->m_usingHwDecode = false;
	AVPixelFormat fallback = avcodec_default_get_format(ctx, pix_fmts);

	std::cerr << "Failed to get HW surface format.\n";
	return fallback;
}

nox::core::VideoStream::VideoStream(AVStream*& stream) : IStream(stream), 
	m_hwDeviceCtx(nullptr), m_hwPixelFormat(AV_PIX_FMT_NONE), m_usingHwDecode(true)
{
	int ret = 0;
	AVCodecContext* codecCtx = m_stream->codec;
	if (!(m_codec = avcodec_find_decoder(codecCtx->codec_id)))
		return;

	if (!(m_codecCtx = avcodec_alloc_context3(m_codec)))
		return;

	if (avcodec_parameters_to_context(m_codecCtx, stream->codecpar) < 0)
		return;

	for (int i = 0;; i++) 
	{
		const AVCodecHWConfig* config = avcodec_get_hw_config(m_codec, i);
		if (!config) {
			fprintf(stderr, "Decoder %s does not support device type %s.\n",
				codecCtx->codec->name, av_hwdevice_get_type_name(m_hwDeviceType));
			return;
		}

		if (config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX &&
			config->device_type == m_hwDeviceType) {
			m_hwPixelFormat = config->pix_fmt;
			break;
		}
	}
	m_codecCtx->opaque = this;
	m_codecCtx->get_format = GetHardwareFormat;

	if (!InitializeHardwareDevice())
		return;

	if ((ret = avcodec_open2(m_codecCtx, m_codec, NULL)) < 0) {

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

	if ((err = av_hwdevice_ctx_create(&m_hwDeviceCtx, m_hwDeviceType,
		NULL, NULL, 0)) < 0) {
		std::cerr << "Failed to create specified HW device." << std::endl;
		return false;

	}
	m_codecCtx->hw_device_ctx = av_buffer_ref(m_hwDeviceCtx);
	return true; 
}

void nox::core::VideoStream::Process(AVPacket& pkt)
{
	AVFrame* frame = NULL;
	int ret = 0;

	ret = avcodec_send_packet(m_codecCtx, &pkt);
	if (ret < 0)
	{
		//ERROR
		return;
	}

	while (ret >= 0) {
		if (!(frame = av_frame_alloc())) {
			//ERROR
			return;
		}

		ret = avcodec_receive_frame(m_codecCtx, frame);

		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
			av_frame_free(&frame);
			return;

		}
		else if (ret < 0) {
			std::cerr << "Error while decoding" << std::endl;
			//ERROR
		}
		AVHWFramesContext* fctx = (AVHWFramesContext*)frame->hw_frames_ctx->data;
		std::cout << fctx->width << "-" << fctx->height;
		int tex = TextureBinder::GetGLTexture(fctx);
		
	}
}
