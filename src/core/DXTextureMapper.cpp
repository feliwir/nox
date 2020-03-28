#include "DXTextureMapper.hpp"

#include <D3D11_1.h>
#include <GL/wglew.h>

#include <cassert>
#include <iostream>

nox::core::DXTextureMapper::DXTextureMapper()
    : m_glInteropTexture(nullptr)
{
    m_texture = std::make_shared<Texture>();
}

std::shared_ptr<nox::core::Texture> nox::core::DXTextureMapper::GetTexture()
{
    return m_texture;
}

void nox::core::DXTextureMapper::SetupDevice()
{
    D3D11_FEATURE_DATA_D3D11_OPTIONS opts;
    m_device->CheckFeatureSupport(D3D11_FEATURE_D3D11_OPTIONS, &opts, sizeof(opts));
    printf("ExtendedResourceSharing = %d", opts.ExtendedResourceSharing);

    m_wglDXDevice = wglDXOpenDeviceNV(m_device);
    if (m_wglDXDevice != nullptr)
    {
        std::cout << "Initialized DX interop" << std::endl;
    }

    m_device->GetImmediateContext(&m_context);
}

void nox::core::DXTextureMapper::SetupTarget(D3D11_TEXTURE2D_DESC& orig)
{
    D3D11_TEXTURE2D_DESC desc = orig;
    desc.Usage = D3D11_USAGE_DEFAULT;
    ;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BindFlags = D3D11_BIND_RENDER_TARGET;
    desc.MiscFlags = D3D11_RESOURCE_MISC_SHARED;

    HRESULT err = m_device->CreateTexture2D(&desc, NULL, &m_d3dTexture);

    ID3D11DeviceContext* context = nullptr;
    m_device->GetImmediateContext(&context);
}

void nox::core::DXTextureMapper::Initialize(int width, int height, AVD3D11VAFramesContext* hwctx)
{
    ID3D11Device* device = nullptr;
    hwctx->texture->GetDevice(&m_device);
    D3D11_TEXTURE2D_DESC desc;
    hwctx->texture->GetDesc(&desc);

    SetupDevice();

    SetupTarget(desc);

    m_context->CopyResource(m_d3dTexture, hwctx->texture);
    HRESULT err = GetLastError();

    GLuint glHandle = m_texture->GetHandle();
    err = GetLastError();
    if (!wglDXSetResourceShareHandleNV(hwctx->texture, NULL))
    {
        err = GetLastError();
        std::cerr << "Failed to set share handle: " << err;
    }

    m_glInteropTexture =
        wglDXRegisterObjectNV(m_wglDXDevice, hwctx->texture, glHandle,
                              GL_RENDERBUFFER, WGL_ACCESS_READ_ONLY_NV);
    err = GetLastError();
}

bool nox::core::DXTextureMapper::IsValid()
{
    return m_glInteropTexture != nullptr;
}

void nox::core::DXTextureMapper::Process(AVHWFramesContext* fctx)
{
    if (fctx->format == AV_PIX_FMT_D3D11)
    {
        auto hwctx = static_cast<AVD3D11VAFramesContext*>(fctx->hwctx);

        // According to hwcontex_d3d11va.h, yuv420p means DXGI_FORMAT_420_OPAQUE,
        // which has no shader support.
        if (fctx->sw_format != AV_PIX_FMT_YUV420P)
            hwctx->BindFlags |= D3D11_BIND_SHADER_RESOURCE;

        Initialize(fctx->width, fctx->height, hwctx);
    }
}
