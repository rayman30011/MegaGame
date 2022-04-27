#include <pch.h>

#include "MegaGame.h"
#include "MegaGameView.h"
#include <iostream>

MegaGameApp MegaGame;

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
	std::cerr << "Log" << std::endl;

	BaseGameLogic* game = _NEW MegaGameLogic();
	game->Init();
	
	std::shared_ptr<IGameView> view(_NEW MegaGameHumanView());
	game->AddGameView(view);
	return game;
}

void MegaGameLogic::Init()
{

}
