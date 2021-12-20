#pragma once

#include "../Interfaces.h"

class DirectX11Renderer : public IRenderer
{
public:
	virtual void SetBackgroupColor(int r, int g, int b, int a) override;
	virtual void OnRestore() override;
};