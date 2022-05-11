#pragma once
#include <optional>

constexpr DWORD QUIT_NO_PROMPT = MAKELPARAM(-1,-1);
constexpr UINT MSG_END_MODAL = (WM_USER+100);

enum MessageBoxQuestions {
    QUESTION_WHERES_THE_CD,
    QUESTION_QUIT_GAME,
};

class BaseUi : public IScreenElement
{
public:
    void OnUpdate(int deltaMilliseconds) override {}
    bool IsVisible() const override { return _visible; }
    void SetVisible(bool visible) override { _visible = visible;}
    
protected:
    int _posX = 0, _posY = 0;
    int _width = 100, _height = 100;
    std::optional<int> _result;
    bool _visible = true;
};

