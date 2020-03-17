#pragma once
#include <memory>
#include <string_view>
#include <vector>

#include "IStream.hpp"
#include "nox-core_export.h"

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
};

namespace nox::core
{
class NOX_CORE_EXPORT Media
{
   public:
    Media(std::string_view filename);
    ~Media();

    bool IsValid();
    void Decode();

   private:
    AVFormatContext* m_formatCtx = nullptr;
    std::vector<std::shared_ptr<IStream>> m_streams;
};
} // namespace nox::core