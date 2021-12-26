#pragma once
#include "Interfaces.h"

class Actor;


class BaseGameLogic
{
public:
	void AddGameView(std::shared_ptr<IGameView> gameView);

	virtual weak_ptr<Actor> GetActor(uint64_t actorId);

protected:
	std::list<std::shared_ptr<IGameView>> _gameViews;

private:
	std::map<uint64_t, shared_ptr<Actor>> _actors;
};