#include <MegaEngineStd.h>

#include "BaseGameLogic.h"

void BaseGameLogic::AddGameView(std::shared_ptr<IGameView> gameView)
{
}

weak_ptr<Actor> BaseGameLogic::GetActor(uint64_t actorId)
{
    return weak_ptr<Actor>();
}
