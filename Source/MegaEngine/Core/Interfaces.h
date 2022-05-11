#pragma once
#include "../Utilities/Math.h"

struct AppMsg;

template<class T>
struct SortBy_SharedPtr_Content
{
    bool operator()(const shared_ptr<T> &lhs, const shared_ptr<T> &rhs) const
    { return *lhs < *rhs; }
};

class IScreenElement
{
public:
    virtual HRESULT OnRestore() = 0;
    virtual HRESULT OnLostDevice() = 0;
    virtual HRESULT OnRender(double fTime, float fElapsedTime) = 0;
    virtual void OnUpdate(int deltaMilliseconds) = 0;

    virtual int GetZOrder() const = 0;
    virtual void SetZOrder(int const zOrder) = 0;
    virtual bool IsVisible() const = 0;
    virtual void SetVisible(bool visible) = 0;

    virtual LRESULT CALLBACK OnMsgProc(AppMsg msg)=0;

    virtual ~IScreenElement() { };
    virtual bool operator <(IScreenElement const& other) const { return GetZOrder() < other.GetZOrder(); }
};

enum class GameViewType
{
    Human,
    Remote,
    AI,
    Recorder,
    Other
};

typedef int GameViewId;

class IGameView
{
public:
    virtual ~IGameView() = default;
    virtual HRESULT OnRender(float time, float elapsedTime) = 0;
    virtual HRESULT OnRestore() = 0;
    virtual HRESULT OnLostDevice() = 0;
    virtual GameViewType GetType() = 0;
    virtual GameViewId GetId() = 0;
    virtual LRESULT CALLBACK OnMsgProc(AppMsg msg) = 0;
    virtual void OnUpdate(int deltaMilliseconds) = 0;
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