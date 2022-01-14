#pragma once
#include <pch.h>

#include "../MegaEngine/Core/EngineApp.h"
#include "../MegaEngine/Core/BaseGameLogic.h"

class MegaGameApp : public EngineApp
{
public:
	const TCHAR* GetTitle() const override { return TEXT("MEGA GAME"); }
	const TCHAR* GetAppDirectory() const override { return TEXT(""); }
	HICON GetIcon() override;

	BaseGameLogic* CreateGame() override;
};

class MegaGameLogic : public BaseGameLogic
{
public:
	void Init() override;
};

extern MegaGameApp MegaGame;