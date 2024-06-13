#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class UILayer : public cocos2d::Layer
{
public:
    virtual bool init();
    CREATE_FUNC(UILayer);

    void createControlButtons();
    void initHPUI(int HPMax);
    void renderHPUI(int HPleft, int HPMax);

    void setLeftButtonCallback(const std::function<void()>& callback);
    void setLeftButtonReleaseCallback(const std::function<void()>& callback);
    void setRightButtonCallback(const std::function<void()>& callback);
    void setRightButtonReleaseCallback(const std::function<void()>& callback);
    void setShootButtonCallback(const std::function<void()>& callback);
    void setJumpButtonCallback(const std::function<void()>& callback);

private:
    std::function<void()> leftButtonCallback;
    std::function<void()> leftButtonReleaseCallback;
    std::function<void()> rightButtonCallback;
    std::function<void()> rightButtonReleaseCallback;
    std::function<void()> shootButtonCallback;
    std::function<void()> jumpButtonCallback;

    std::vector<cocos2d::Sprite*> heartsMissing;
    std::vector<cocos2d::Sprite*> heartsFull;

    void onButtonTouchEvent(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type,
        const std::function<void()>& pressedCallback,
        const std::function<void()>& releasedCallback);

    float calculateScaleFactor();
};
