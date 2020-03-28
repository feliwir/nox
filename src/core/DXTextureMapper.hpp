#pragma once
#include "ITextureMapper.hpp"
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
};
#include <libavutil/hwcontext_d3d11va.h>
#include <libavutil/hwcontext_dxva2.h>

namespace nox::core
{
class DXTextureMapper : public ITextureMapper
{
   public:
    DXTextureMapper();

    // Derived from ITextureInterop
    virtual bool IsValid() override;
    virtual void Process(AVHWFramesContext* hwFramesContext) override;
    virtual std::shared_ptr<Texture> GetTexture() override;

   protected:
    void Initialize(int width, int height, AVD3D11VAFramesContext* fctx);
    void SetupDevice();
    void SetupTarget(D3D11_TEXTURE2D_DESC& orig);

    ID3D11Device* m_device;
    ID3D11DeviceContext* m_context;

    ID3D11Texture2D* m_d3dTexture;
    HANDLE m_wglDXDevice;
    HANDLE m_glInteropTexture;
    std::shared_ptr<Texture> m_texture;
};
} // namespace nox::core