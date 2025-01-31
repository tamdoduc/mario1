﻿#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class MainMenuScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    // a selector callback
    void menuPlayCallback(cocos2d::Ref* pSender);
    void menuExitCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(MainMenuScene);
};

#endif // __MAIN_MENU_SCENE_H__
