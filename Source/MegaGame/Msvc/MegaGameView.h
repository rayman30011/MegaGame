#pragma once

#include "../MegaEngine/Core/Interfaces.h"

class MegaGameHumanView : public IGameView
{
public:
    void OnRender(float time, float elapsedTime) override;
};