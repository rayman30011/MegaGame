#pragma once

#include "../Interfaces.h"

class DirectX11Renderer : public IRenderer
{
public:
	static CDXUTDialogResourceManager g_DialogResourceManager;
	
	CDXUTTextHelper* TextHelper;

	~DirectX11Renderer();
	
	void SetBackgroundColor(int r, int g, int b, int a) override;
	HRESULT OnRestore() override;
	bool PreRender() override;
	bool PostRender() override;

private:
	float _bgColor[4] = {0.f};
};