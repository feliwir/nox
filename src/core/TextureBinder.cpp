#include "TextureBinder.hpp"
#include <iostream>

#include <libavutil/hwcontext.h>
#include <libavutil/hwcontext_d3d11va.h>
#include <libavutil/hwcontext_dxva2.h>
#include <GL/glew.h>
#include <GL/wglew.h>

int nox::core::TextureBinder::GetGLTexture(AVHWFramesContext* fctx)
{
	if (fctx->format == AV_PIX_FMT_D3D11) {
		AVD3D11VAFramesContext* hwctx = static_cast<AVD3D11VAFramesContext*>(fctx->hwctx);

		// According to hwcontex_d3d11va.h, yuv420p means DXGI_FORMAT_420_OPAQUE,
		// which has no shader support.
		if (fctx->sw_format != AV_PIX_FMT_YUV420P)
			hwctx->BindFlags |= D3D11_BIND_SHADER_RESOURCE;

		ID3D11Device* device = nullptr;
		hwctx->texture->GetDevice(&device);

		HANDLE wglDXDevice = wglDXOpenDeviceNV(device);
		if (wglDXDevice != nullptr)
		{
			std::cout << "Initialized DX interop" << std::endl;
		}
	}
	else if (fctx->format == AV_PIX_FMT_DXVA2_VLD)
	{
		AVDXVA2FramesContext* hwctx = static_cast<AVDXVA2FramesContext*>(fctx->hwctx);
		std::cout << hwctx->nb_surfaces;
	}

	return 0;
}
