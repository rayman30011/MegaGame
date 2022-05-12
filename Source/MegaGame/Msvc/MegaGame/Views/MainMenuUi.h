#pragma once
#include "../../../../MegaEngine/UI/UserInterface.h"

class MainMenuUi : public BaseUi
{
public:
    MainMenuUi();
    ~MainMenuUi() override;

    HRESULT OnRestore() override;
    HRESULT OnLostDevice() override;
    void SetZOrder(int const zOrder) override {};
    int GetZOrder() const override { return 1; }
    
    LRESULT CALLBACK OnMsgProc(AppMsg msg) override;
    
    
    HRESULT OnRender(double fTime, float fElapsedTime) override;
    
    static void CALLBACK OnGuiEvent(UINT event, int controlId, CDXUTControl* control, void *userContext);
protected:
    CDXUTDialog _ui;
    class DirectX11Renderer* _renderer;
};