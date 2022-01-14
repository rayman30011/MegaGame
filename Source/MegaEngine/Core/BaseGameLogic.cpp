#include <MegaEngineStd.h>

#include "BaseGameLogic.h"

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
}

void BaseGameLogic::OnRender(float time, float elapsedTime)
{
}
