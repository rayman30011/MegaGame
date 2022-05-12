#pragma once

#include "../../MegaEngine/UI/HumanView.h"

class MegaGameHumanView : public HumanView
{
public:
    MegaGameHumanView();
    ~MegaGameHumanView() override;
private:
    shared_ptr<class MainMenuUi> _mainMenuUi;
};
