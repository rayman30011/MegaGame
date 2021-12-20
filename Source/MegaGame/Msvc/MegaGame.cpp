#include "MegaGame.h"
#include "pch.h"

#include "MegaGame.h"
#include "MegaGameView.h"

MegaGameApp MegaGame;

#pragma

INT WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR cmdLine, int cmdShow)
{
	return MegaEngineMain(instance, prevInstance, cmdLine, cmdShow);
}

HICON MegaGameApp::GetIcon()
{
	return LoadIcon(GetInstance(), MAKEINTRESOURCE(IDI_APPLICATION));
}

BaseGameLogic* MegaGameApp::CreateGame()
{
	BaseGameLogic* game = _NEW MegaGameLogic();
	std::shared_ptr<IGameView> view(_NEW MegaGameHumanView());
	game->AddGameView(view);
	return game;
}
