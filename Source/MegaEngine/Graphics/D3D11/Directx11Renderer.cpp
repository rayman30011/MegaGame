#include <MegaEngineStd.h>

#include "Directx11Renderer.h"

#include "DXUTgui.h"

DirectX11Renderer::~DirectX11Renderer()
{
	SAFE_DELETE(TextHelper);
}

void DirectX11Renderer::SetBackgroundColor(int r, int g, int b, int a)
{
	
}

void DirectX11Renderer::OnRestore()
{
	TextHelper = _NEW CDXUTTextHelper(DXUTGetD3D11Device(), DXUTGetD3D11DeviceContext(), &DialogResourceManager, 15);
}

void DirectX11Renderer::PreRender()
{
	
}

void DirectX11Renderer::PostRender()
{
}
