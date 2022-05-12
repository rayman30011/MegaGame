#include <pch.h>
#include <iostream>

#include "MegaGameView.h"

#include "MegaGame/Views/MainMenuUi.h"

MegaGameHumanView::MegaGameHumanView() : HumanView()
{
    _runFullSpeed = true;
    _mainMenuUi = std::make_shared<MainMenuUi>();
    PushElement(_mainMenuUi);
}

MegaGameHumanView::~MegaGameHumanView()
{
}
