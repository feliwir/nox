#pragma once
#include "ITextureInterop.hpp"
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
};
#include <libavutil/hwcontext_d3d11va.h>
#include <libavutil/hwcontext_dxva2.h>

namespace nox::core
{
class DXTextureInterop : public ITextureInterop
{
   public:
    DXTextureInterop();

    // Derived from ITextureInterop
    virtual bool IsValid() override;
    virtual void Process(AVHWFramesContext* hwFramesContext) override;
    virtual std::shared_ptr<Texture> GetTexture() override;

   protected:
    void Initialize(AVD3D11VAFramesContext* fctx);

    ID3D11Device* m_device;
    HANDLE m_wglDXDevice;
    HANDLE m_glInteropTexture;
    std::shared_ptr<Texture> m_texture;
};
} // namespace nox::core