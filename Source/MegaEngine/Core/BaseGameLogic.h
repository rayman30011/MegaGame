#pragma once
#include "Interfaces.h"

class BaseGameLogic
{
public:
	void AddGameView(std::shared_ptr<IGameView> gameView);
protected:
	std::list<std::shared_ptr<IGameView>> _gameViews;
};