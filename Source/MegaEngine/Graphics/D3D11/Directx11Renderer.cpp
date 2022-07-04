#include <MegaEngineStd.h>

#include "Directx11Renderer.h"

CDXUTDialogResourceManager DirectX11Renderer::g_DialogResourceManager;

DirectX11Renderer::~DirectX11Renderer()
{
	SAFE_DELETE(TextHelper);
}

void DirectX11Renderer::SetBackgroundColor(int r, int g, int b, int a)
{
	_bgColor[0] = float(a) / 255.0f;
	_bgColor[1] = float(r) / 255.0f;
	_bgColor[2] = float(g) / 255.0f;
	_bgColor[3] = float(b) / 255.0f;
}

HRESULT DirectX11Renderer::OnRestore()
{
	SAFE_DELETE(TextHelper);
	TextHelper = _NEW CDXUTTextHelper(DXUTGetD3D11Device(), DXUTGetD3D11DeviceContext(), &g_DialogResourceManager, 15);
	return S_OK;
}

bool DirectX11Renderer::PreRender()
{
	if (DXUTGetD3D11DeviceContext() && DXUTGetD3D11RenderTargetView())
	{
		DXUTGetD3D11DeviceContext()->ClearRenderTargetView( DXUTGetD3D11RenderTargetView(), DirectX::Colors::LightBlue);
		DXUTGetD3D11DeviceContext()->ClearDepthStencilView( DXUTGetD3D11DepthStencilView(), D3D11_CLEAR_DEPTH, 1.0f, 0 );
	}
	return true;
}

bool DirectX11Renderer::PostRender()
{
	return true;
}
