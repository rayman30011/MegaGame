#pragma once

#include "../../MegaEngine/UI/HumanView.h"
#include "../MegaEngine/Core/Interfaces.h"

class MegaGameHumanView : public HumanView
{
public:
    HRESULT OnRender(float time, float elapsedTime) override;
};