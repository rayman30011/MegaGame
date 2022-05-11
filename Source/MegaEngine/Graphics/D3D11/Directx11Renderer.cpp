#include <MegaEngineStd.h>

#include "Directx11Renderer.h"


DirectX11Renderer::~DirectX11Renderer()
{
	SAFE_DELETE(TextHelper);
}

void DirectX11Renderer::SetBackgroundColor(int r, int g, int b, int a)
{
	
}

HRESULT DirectX11Renderer::OnRestore()
{
	SAFE_DELETE(TextHelper);
	TextHelper = _NEW CDXUTTextHelper(DXUTGetD3D11Device(), DXUTGetD3D11DeviceContext(), &g_DialogResourceManager, 15);
	return S_OK;
}

bool DirectX11Renderer::PreRender()
{
	
}

bool DirectX11Renderer::PostRender()
{
}
