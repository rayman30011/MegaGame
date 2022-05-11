#pragma once
#include "UserInterface.h"

class UiMessageMox : public BaseUi
{
public:
    UiMessageMox(std::wstring message, std::wstring title, int buttonFlags = MB_OK);
    ~UiMessageMox();

    HRESULT OnRestore() override;
    int GetZOrder() const override { return 99; }
    LRESULT OnMsgProc(AppMsg msg) override;
    void SetVisible(bool visible) override {}
    bool IsVisible() const override { return true; }
    HRESULT OnRender(double fTime, float fElapsedTime) override;
    
    static void CALLBACK OnGuiEvent(UINT event, int controlId, CDXUTControl* control, void* context);
    static int Ask(MessageBoxQuestions question);
    
protected:
    CDXUTDialog _ui;
    int _buttonId;
};

