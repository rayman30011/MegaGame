#include <pch.h>

#include "MainMenuUi.h"

#include "../../../../MegaEngine/Graphics/D3D11/Directx11Renderer.h"

enum MENU_IDC
{
    IDC_SAMPLE
};

MainMenuUi::MainMenuUi()
{
    _ui.Init(&DirectX11Renderer::g_DialogResourceManager);
    _ui.SetCallback(OnGuiEvent);

    _ui.AddButton(IDC_SAMPLE, L"Sample", 10, 10, 150, 150);
    _ui.SetLocation(10, 10);
    _ui.SetSize(200, 200);
}

MainMenuUi::~MainMenuUi()
{
    DirectX11Renderer::g_DialogResourceManager.UnregisterDialog(&_ui);
}

HRESULT MainMenuUi::OnRestore()
{
    _ui.SetLocation(10, 10);
    _ui.SetSize(150, 150);
    return S_OK;
}

HRESULT MainMenuUi::OnLostDevice()
{
    return S_OK;
}

LRESULT MainMenuUi::OnMsgProc(AppMsg msg)
{
    return S_OK;
}

HRESULT MainMenuUi::OnRender(double fTime, float fElapsedTime)
{
    _ui.OnRender(fElapsedTime);
    return S_OK;
}

void MainMenuUi::OnGuiEvent(UINT event, int controlId, CDXUTControl* control, void* userContext)
{
}
