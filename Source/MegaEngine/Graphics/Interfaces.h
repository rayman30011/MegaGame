#pragma once

class IRenderer
{
public:
	virtual void SetBackgroundColor(int r, int g, int b, int a) = 0;
	virtual HRESULT OnRestore() = 0;
	virtual bool PreRender() = 0;
	virtual bool PostRender() = 0;
};