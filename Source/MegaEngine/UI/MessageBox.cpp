#include "MessageBox.h"
#include "d3d9.h"

UiMessageMox::UiMessageMox(std::wstring message, std::wstring title, int buttonFlags)
{
    _ui.Init(&g_DialogResourceManager);
    _ui.SetCallback(OnGuiEvent);

    RECT rc;
    SetRect(&rc, 0, 0, 0, 0);
    _ui.CalcTextRect(message.c_str(), _ui.GetDefaultElement(DXUT_CONTROL_STATIC, 0), &rc);
    int msgWidth = rc.right - rc.left;
    int msgHeight = rc.bottom - rc.top;

    int numButtons = 2;
    if ( (buttonFlags == MB_ABORTRETRYIGNORE) ||
        (buttonFlags == MB_CANCELTRYCONTINUE) ||
        (buttonFlags == MB_CANCELTRYCONTINUE) )
    {
        numButtons = 3;
    }
    else if (buttonFlags == MB_OK)
    {
        numButtons = 1;
    }

    const int btnWidth = GApp->ScreenWidth() * 0.15f;
    const int btnHeight = GApp->ScreenHeight() * 0.037f;
    const int border = GApp->ScreenWidth() * 0.043f;

    _width = max(msgWidth + 2 * border, btnWidth + 2 * border);
    _height = msgHeight + (numButtons * (btnHeight + border)) + (2 * border);

    _posX = (GApp->ScreenWidth() - _width) / 2;
    _posY = (GApp->ScreenHeight() - _height) / 2;
    _ui.SetLocation(_posX, _posY);

    _ui.SetSize(_width, _height);
    constexpr D3DCOLOR red = D3DCOLOR_ARGB(0xc0,0xff,0x00,0x00);
    _ui.SetBackgroundColors(red);

    const int iX = (_width - btnWidth) / 2;
    const int iY = _height - btnHeight - border;

    buttonFlags &= 0xF;

    if ((buttonFlags == MB_ABORTRETRYIGNORE) ||
        (buttonFlags == MB_CANCELTRYCONTINUE))
    {
        _ui.AddButton(IDCONTINUE, GApp->GetString(TEXT("IDS_CONTINUE")).c_str(), iX, iY - (2 * border), btnWidth, btnHeight);
        _ui.AddButton(IDTRYAGAIN, GApp->GetString(TEXT("IDS_TRYAGAIN")).c_str(), iX, iY - border, btnWidth, btnHeight);
        _ui.AddButton(IDCANCEL, GApp->GetString(TEXT("IDS_CANCEL")).c_str(), iX, iY, btnWidth, btnHeight);
    }
    else if (buttonFlags == MB_OKCANCEL)
    {
        _ui.AddButton(IDOK, GApp->GetString(TEXT("IDS_OK")).c_str(), iX, iY - border, btnWidth, btnHeight);
        _ui.AddButton(IDCANCEL, GApp->GetString(TEXT("IDS_CANCEL")).c_str(), iX, iY, btnWidth, btnHeight);
    }
    else if (buttonFlags == MB_RETRYCANCEL)
    {
        _ui.AddButton(IDRETRY, GApp->GetString(TEXT("IDS_RETRY")).c_str(), iX, iY - border, btnWidth, btnHeight);
        _ui.AddButton(IDCANCEL, GApp->GetString(TEXT("IDS_CANCEL")).c_str(), iX, iY, btnWidth, btnHeight);
    }
    else if (buttonFlags == MB_YESNO)
    {
        _ui.AddButton(IDYES, GApp->GetString(TEXT("IDS_YES")).c_str(), iX, iY - border, btnWidth, btnHeight);
        _ui.AddButton(IDNO, GApp->GetString(TEXT("IDS_NO")).c_str(), iX, iY, btnWidth, btnHeight);
    }
    else if (buttonFlags == MB_YESNOCANCEL)
    {
        _ui.AddButton(IDYES, GApp->GetString(TEXT("IDS_YES")).c_str(), iX, iY - (2 * border), btnWidth, btnHeight);
        _ui.AddButton(IDNO, GApp->GetString(TEXT("IDS_NO")).c_str(), iX, iY - border, btnWidth, btnHeight);
        _ui.AddButton(IDCANCEL, GApp->GetString(TEXT("IDS_CANCEL")).c_str(), iX, iY, btnWidth, btnHeight);
    }
    else
    {
        _ui.AddButton(IDOK, GApp->GetString(TEXT("IDS_OK")).c_str(), iX, iY, btnWidth, btnHeight);
    }
}

UiMessageMox::~UiMessageMox()
{
}

HRESULT UiMessageMox::OnRestore()
{
    _ui.SetLocation(_posX, _posY);
    _ui.SetSize(_width, _height);
    return S_OK;
}

HRESULT UiMessageMox::OnRender(double fTime, float fElapsedTime)
{
    _ui.OnRender(fElapsedTime);
    return S_OK;
}

LRESULT UiMessageMox::OnMsgProc(AppMsg msg)
{
    return _ui.MsgProc(msg.HWnd, msg.UMsg, msg.WParam, msg.LParam);
}

void UiMessageMox::OnGuiEvent(UINT event, int controlId, CDXUTControl* control, void* context)
{
    PostMessage(GApp->GetHWnd(), MSG_END_MODAL, 0, controlId);
}

int UiMessageMox::Ask(MessageBoxQuestions question)
{
}
