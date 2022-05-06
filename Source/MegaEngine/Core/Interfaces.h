#pragma once
#include "../Utilities/Math.h"

class IGameView
{
public:
    virtual void OnRender(float time, float elapsedTime) = 0;
};

class Resource;
class ResHandler;

class IResourceLoader
{
public:
    virtual std::string GetPattern() = 0;
    virtual bool UseRawFile() = 0;
    virtual bool AddNullZero() { return false; }
    virtual size_t GetLoadedResourceSize(char* rawBuffer, size_t rawSize) = 0;
    virtual bool LoadResource(char* rawBuffer, uint32_t rawSize, shared_ptr<ResHandler> handle) = 0;
};

class IResourceFile
{
public:
    virtual bool Open() = 0;
    virtual int GetRawResourceSize(const Resource& resource) = 0;
    virtual int GetRawResource(const Resource& resource, char* buffer) = 0;
    virtual int GetNumResources() const = 0;
    virtual std::string GetResourceName(int num) const = 0;
};

class IKeyboardHandler
{
public:
    virtual bool OnKeyDown(uint32_t code);
    virtual bool OnKeyUp(uint32_t code);
};

class IPointerHandler
{
public:
    virtual bool OnPointerMove(const DirectX::Math::Vector2& mousePos) = 0;
    virtual bool OnPointerButtonDown(const DirectX::Math::Vector2& mousePos, const std::string& buttonName) = 0;
    virtual bool OnPointerButtonUp(const DirectX::Math::Vector2& mousePos, const std::string& buttonName) = 0;
    virtual int GetPointerRadius(const DirectX::Math::Vector2& mousePos, const std::string& buttonName) = 0;
};

class IJoystickHandler
{
public:
    virtual bool OnTrigger(const std::string& buttonName, int pressure) = 0;
    virtual bool OnButtonDown(const std::string& buttonName, int pressure) = 0;
    virtual bool OnButtonUp(const std::string& buttonName) = 0;
    virtual bool OnDirectionalPad(const std::string& direction) = 0;
    virtual bool OnThumbstick(const std::string& stickName, float x, float y) = 0;
};