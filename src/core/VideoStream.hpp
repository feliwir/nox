#pragma once
#include "IStream.hpp"
#include "ITextureInterop.hpp"

namespace nox::core
{
	class VideoStream : public IStream
	{
	public:
		VideoStream(AVStream*& stream);
		~VideoStream();

		virtual float GetDuration() override;
		virtual float GetPosition() override;
		virtual StreamType GetType() override;

	protected:
		static AVPixelFormat GetHardwareFormat(
			AVCodecContext* ctx,
			const AVPixelFormat* pix_fmts);
		bool InitializeHardwareDevice();
		// Derived from IStream
		virtual void Process(AVPacket& pkt) override;

		// Basic AVCodec members
		AVCodec* m_codec;
		AVCodecContext* m_codecCtx;

		// Hardware decoding
		AVBufferRef* m_hwDeviceCtx;
		static AVHWDeviceType m_hwDeviceType;
		AVPixelFormat m_hwPixelFormat;
		bool m_usingHwDecode;
		std::unique_ptr<ITextureInterop> m_texInterop;
	};
}