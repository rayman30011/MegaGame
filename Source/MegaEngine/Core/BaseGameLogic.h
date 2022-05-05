#pragma once
#include "Interfaces.h"

class Actor;
class ActorFactory;

class BaseGameLogic
{
public:
	virtual bool Init();

	inline auto GetViews() const { return _gameViews; }
	void AddGameView(std::shared_ptr<IGameView> gameView);
	virtual weak_ptr<Actor> GetActor(uint64_t actorId);
	
	virtual void OnUpdate(float time, float elapsedTime);
	virtual void OnRender(float time, float elapsedTime);

protected:
	virtual ActorFactory* CreateActorFactory();
	
protected:
	std::list<std::shared_ptr<IGameView>> _gameViews;
	ActorFactory* _actorFactory = nullptr;

	std::map<uint64_t, shared_ptr<Actor>> _actors;
};
