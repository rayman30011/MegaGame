#pragma once
#include <pch.h>

#include "../MegaEngine/Core/EngineApp.h"
#include "../MegaEngine/Core/BaseGameLogic.h"

class MegaGameApp : public EngineApp
{
public:
	virtual const TCHAR* GetTitle() const override { return TEXT("MEGA GAME"); }
	virtual const TCHAR* GetAppDirectory() const override { return TEXT(""); }
	virtual HICON GetIcon() override;

	virtual BaseGameLogic* CreateGame() override;
};

class MegaGameLogic : public BaseGameLogic
{

};

extern MegaGameApp MegaGame;