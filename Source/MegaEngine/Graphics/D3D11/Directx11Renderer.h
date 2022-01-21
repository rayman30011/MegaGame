#pragma once

#include "../Interfaces.h"

#include <DXUTgui.h>
#include <SDKmisc.h>

class DirectX11Renderer : public IRenderer
{
public:
	static CDXUTDialogResourceManager DialogResourceManager;
	static CDXUTTextHelper* TextHelper;

	~DirectX11Renderer();
	
	void SetBackgroundColor(int r, int g, int b, int a) override;
	void OnRestore() override;
	void PreRender();
	void PostRender();
};