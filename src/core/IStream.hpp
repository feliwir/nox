#pragma once

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
};

namespace nox::core
{
	enum class StreamType
	{
		Video,
		Audio,
		Subtitles
	};

	class IStream
	{
	public:
		inline IStream(AVStream*& stream) : m_stream(stream)
		{
		}

		virtual void Process(AVPacket& pkt) = 0;

		virtual float GetDuration() = 0;
		virtual float GetPosition() = 0;
		virtual StreamType GetType() = 0;

		inline int GetIndex() const {
			return m_stream->index;
		}

	protected:
		AVStream*& m_stream;
	};
}