#include "Media.hpp"
#include "VideoStream.hpp"

nox::core::Media::Media(std::string_view filename)
{
	m_formatCtx = avformat_alloc_context();

	// Open video file
	if (avformat_open_input(&m_formatCtx, filename.data(), NULL, NULL) != 0)
		return; // Couldn't open file

	// Retrieve stream information
	if (avformat_find_stream_info(m_formatCtx, NULL) < 0)
		return; // Couldn't find stream information

	// Dump information about file onto standard error
	av_dump_format(m_formatCtx, 0, filename.data(), 0);

	for (int i = 0; i < m_formatCtx->nb_streams; i++)
	{
		AVStream*& stream = m_formatCtx->streams[i];
		AVCodecParameters* codecParams = stream->codecpar;

		switch (codecParams->codec_type)
		{
		case AVMEDIA_TYPE_VIDEO:
			m_streams.push_back(std::make_shared<VideoStream>(stream));
		default:
			break;
		}
	}

	Decode();
}

nox::core::Media::~Media()
{
	avformat_close_input(&m_formatCtx);
}

void nox::core::Media::Decode()
{
	AVPacket packet;
	int ret = 0;

	while (ret >= 0)
	{
		if ((ret = av_read_frame(m_formatCtx, &packet)) < 0)
			break;

		for (const auto& stream : m_streams)
		{
			if (stream->GetIndex() == packet.stream_index)
			{
				stream->Process(packet);
			}
		}

		av_packet_unref(&packet);
	}
}

bool nox::core::Media::IsValid()
{
	return true;
}
