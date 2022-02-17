#pragma once

class IGameView
{
public:
    virtual void OnRender(float time, float elapsedTime) = 0;
};

class IResourceFile
{
public:
    virtual bool Open() = 0;
    virtual int GetRawResourceSize() = 0;
};