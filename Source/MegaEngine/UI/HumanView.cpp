#include <MegaEngineStd.h>
#include <timeapi.h>

#include "HumanView.h"

#include "Graphics/Interfaces.h"

constexpr uint32_t SCREEN_REFRESH_RATE = 1000 / 60;

HumanView::HumanView()
{
    
}

HumanView::~HumanView()
{
}

HRESULT HumanView::OnRestore()
{
    HRESULT hr;
    for (const auto& element : ScreenElements)
    {
        V_RETURN(element->OnRestore());
    }
    return S_OK;
}

void HumanView::OnRender(float time, float elapsedTime)
{
    _currTick = timeGetTime();
    if (_currTick == _lastDraw)
        return;

    HRESULT hr;

    if (_runFullSpeed || ((_currTick - _lastDraw) > SCREEN_REFRESH_RATE))
    {
        if (GApp->GetRenderer()->PreRender())
        {
            RenderText();
            
            ScreenElements.sort([](const auto& a, const auto& b) -> bool { return *a < *b; });

            for (const auto& elem : ScreenElements)
            {
                if (elem->IsVisible())
                    elem->OnRender(time, elapsedTime);
            }

            _lastDraw = _currTick;
        }

        GApp->GetRenderer()->PostRender();
    }
}

HRESULT HumanView::OnLostDevice()
{
    HRESULT hr;
    for (const auto& element : ScreenElements)
    {
        V_RETURN(element->OnRestore());
    }
    return S_OK;
}

bool HumanView::LoadGame(xml::XMLElement* levelData)
{
    return true;
}

LRESULT CALLBACK HumanView::OnMsgProc(AppMsg msg)
{
    for (const auto& element : ScreenElements)
    {
        if (element->IsVisible())
        {
            if (element->OnMsgProc(msg))
            {
                return 1;
            }
        }
        
    }

    LRESULT result = 0;
    switch (msg.UMsg)
    {
    case WM_KEYDOWN:
        if (_keyboardHandler)
        {
            result = _keyboardHandler->OnKeyDown(msg.WParam) ? 1 : 0;
        }
        break;
    case WM_KEYUP:
        if (_keyboardHandler)
        {
            result = _keyboardHandler->OnKeyUp(msg.WParam) ? 1 : 0;
        }
        break;
    case WM_MOUSEMOVE:
        if (_pointerHandler)
        {
            result = _pointerHandler->OnPointerMove(Vector2(LOWORD(msg.LParam), HIWORD(msg.LParam))) ? 1 : 0;
        }
        break;
    default:
        return 0;
    }
}

void HumanView::OnUpdate(int deltaMilliseconds)
{
    for (const auto& element : ScreenElements)
    {
        element->OnUpdate(deltaMilliseconds);
    }
}

void HumanView::PushElement(shared_ptr<IScreenElement> element)
{
    ScreenElements.push_back(element);
}

void HumanView::RemoveElement(shared_ptr<IScreenElement> element)
{
}

void HumanView::TogglePause(bool active)
{
}

bool HumanView::InitAudio()
{
    return true;
}

void HumanView::SetCameraOffset(Vector4 offset)
{
}
