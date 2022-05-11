#pragma once
#include <optional>

using namespace DirectX::Math;

class HumanView : IGameView
{
public:
    explicit HumanView(Color backgroundColor);
    ~HumanView() override;

    std::list<shared_ptr<IScreenElement>> ScreenElements;
    
    HRESULT OnRestore() override;
    HRESULT OnRender(float time, float elapsedTime) override;
    HRESULT OnLostDevice() override;
    GameViewType GetType() override { return GameViewType::Human; }
    GameViewId GetId() override { return _gameViewId; }

    bool LoadGame(xml::XMLElement* levelData);

    virtual void OnAttach(GameViewId viewId, std::optional<uint64_t> actorId)
    {
        _gameViewId = viewId;
        if (actorId.has_value())
            _actorId = actorId.value();
    }

    LRESULT CALLBACK OnMsgProc(AppMsg msg) override;
    void OnUpdate(int deltaMilliseconds) override;

    virtual void PushElement(shared_ptr<IScreenElement> element);
    virtual void RemoveElement(shared_ptr<IScreenElement> element);

    void TogglePause(bool active);

    bool InitAudio();
    virtual void SetCameraOffset(Vector4 offset);
    
protected:
    GameViewId _gameViewId;
    uint64_t _actorId;

    DWORD _currTick;
    DWORD _lastDraw;
    bool _runFullSpeed;

    IKeyboardHandler* _keyboardHandler;
    IPointerHandler* _pointerHandler;

    virtual void RenderText() {};

    virtual bool LoadGameDelegate(xml::XMLElement* levelData) { return true; }
private:
};
