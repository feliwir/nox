#include "DXTextureInterop.hpp"
#include <GL/wglew.h>
#include <iostream>
#include <cassert>
#include <D3D11_1.h>

nox::core::DXTextureInterop::DXTextureInterop() : m_glInteropTexture(nullptr)
{
	m_texture = std::make_shared<Texture>();
}

std::shared_ptr<nox::core::Texture> nox::core::DXTextureInterop::GetTexture()
{
	return m_texture;
}

void nox::core::DXTextureInterop::Initialize(AVD3D11VAFramesContext* hwctx)
{
	ID3D11Device* device = nullptr;
	hwctx->texture->GetDevice(&device);

	D3D11_FEATURE_DATA_D3D11_OPTIONS opts;
	device->CheckFeatureSupport(D3D11_FEATURE_D3D11_OPTIONS, &opts, sizeof(opts));
	printf("ExtendedResourceSharing = %d", opts.ExtendedResourceSharing);

	m_wglDXDevice = wglDXOpenDeviceNV(device);
	if (m_wglDXDevice != nullptr)
	{
		std::cout << "Initialized DX interop" << std::endl;
	}

	HANDLE shared_handle;
	IDXGIResource1* pResource = nullptr;
	HRESULT hr = hwctx->texture->QueryInterface(__uuidof(IDXGIResource1), (void**)&pResource);
	assert(hr == S_OK);
	hr = pResource->CreateSharedHandle(NULL,
		DXGI_SHARED_RESOURCE_READ,
		NULL, &shared_handle);

	GLuint glHandle = m_texture->GetHandle();
	m_texture->Bind();
	DWORD err = GetLastError();
	wglDXSetResourceShareHandleNV(hwctx->texture, shared_handle);
	m_glInteropTexture = wglDXRegisterObjectNV(m_wglDXDevice,
		hwctx->texture, glHandle,
		GL_TEXTURE_2D, WGL_ACCESS_READ_ONLY_NV);
	err = GetLastError();
}

bool nox::core::DXTextureInterop::IsValid()
{
	return m_glInteropTexture != nullptr;
}

void nox::core::DXTextureInterop::Process(AVHWFramesContext* fctx)
{
	if (fctx->format == AV_PIX_FMT_D3D11) {
		auto hwctx = static_cast<AVD3D11VAFramesContext*>(fctx->hwctx);

		// According to hwcontex_d3d11va.h, yuv420p means DXGI_FORMAT_420_OPAQUE,
		// which has no shader support.
		if (fctx->sw_format != AV_PIX_FMT_YUV420P)
			hwctx->BindFlags |= D3D11_BIND_SHADER_RESOURCE;

		Initialize(hwctx);

	
	}
}
