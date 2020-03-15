#include "DXTextureInterop.hpp"
#include <GL/wglew.h>
#include <iostream>

nox::core::DXTextureInterop::DXTextureInterop() : m_glInteropTexture(nullptr)
{
	m_texture = std::make_shared<Texture>();
}

std::shared_ptr<nox::core::Texture> nox::core::DXTextureInterop::GetTexture()
{
	return m_texture;
}

bool nox::core::DXTextureInterop::IsValid()
{
	return m_glInteropTexture != nullptr;
}

void nox::core::DXTextureInterop::Process(AVHWFramesContext* fctx)
{
	if (fctx->format == AV_PIX_FMT_D3D11) {
		AVD3D11VAFramesContext* hwctx = static_cast<AVD3D11VAFramesContext*>(fctx->hwctx);

		// According to hwcontex_d3d11va.h, yuv420p means DXGI_FORMAT_420_OPAQUE,
		// which has no shader support.
		if (fctx->sw_format != AV_PIX_FMT_YUV420P)
			hwctx->BindFlags |= D3D11_BIND_SHADER_RESOURCE;

		ID3D11Device* device = nullptr;
		hwctx->texture->GetDevice(&device);

		m_wglDXDevice = wglDXOpenDeviceNV(device);
		if (m_wglDXDevice != nullptr)
		{
			std::cout << "Initialized DX interop" << std::endl;
		}

		GLuint glHandle = m_texture->GetHandle();
		DWORD err = GetLastError();
		m_glInteropTexture = wglDXRegisterObjectNV(m_wglDXDevice, 
									hwctx->texture, glHandle,
									GL_TEXTURE_2D, WGL_ACCESS_READ_ONLY_NV);
		err = GetLastError();
	}
}
