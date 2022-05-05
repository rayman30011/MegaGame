#include <MegaEngineStd.h>

#include "BaseGameLogic.h"

#include "Actors/Actor.h"
#include "Actors/ActorFactory.h"

bool BaseGameLogic::Init()
{
    _actorFactory = CreateActorFactory();
    return true;
}

void BaseGameLogic::AddGameView(std::shared_ptr<IGameView> gameView)
{
    _gameViews.push_back(gameView);
}

weak_ptr<Actor> BaseGameLogic::GetActor(uint64_t actorId)
{
    const auto it = _actors.find(actorId);
    if (it == _actors.end())
        return weak_ptr<Actor>();
    
    weak_ptr<Actor> weak(it->second);
    return weak;
}

void BaseGameLogic::OnUpdate(float time, float elapsedTime)
{
    for (const auto actor: _actors)
    {
        actor.second->Update(elapsedTime);
    }
}

void BaseGameLogic::OnRender(float time, float elapsedTime)
{
}

ActorFactory* BaseGameLogic::CreateActorFactory()
{
    return _NEW ActorFactory();
}
