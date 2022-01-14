#pragma once

class IGameView
{
public:
    virtual void OnRender(float time, float elapsedTime) = 0;
};