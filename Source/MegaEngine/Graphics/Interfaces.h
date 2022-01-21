#pragma once

class IRenderer
{
public:
	virtual void SetBackgroundColor(int r, int g, int b, int a) = 0;
	virtual void OnRestore() = 0;
};