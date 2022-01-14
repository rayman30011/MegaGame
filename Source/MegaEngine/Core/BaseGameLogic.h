#pragma once
#include "Interfaces.h"

class Actor;


class BaseGameLogic
{
public:
	virtual void Init() = 0;

	inline auto GetViews() const { return _gameViews; }
	void AddGameView(std::shared_ptr<IGameView> gameView);
	virtual weak_ptr<Actor> GetActor(uint64_t actorId);
	
	virtual void OnUpdate(float time, float elapsedTime);
	virtual void OnRender(float time, float elapsedTime);

protected:
	std::list<std::shared_ptr<IGameView>> _gameViews;

private:
	std::map<uint64_t, shared_ptr<Actor>> _actors;
};
