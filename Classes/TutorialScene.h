#ifndef __TUTORIAL_SCENE_H__
#define __TUTORIAL_SCENE_H__

#include "cocos2d.h"
#include <Character.h>
#include <TileMap.h>
#include <Ground.h>
#include "ui/CocosGUI.h"
#include <EnemyManager.h>



class Tutorial : public cocos2d::Scene
{
public:
    static Tutorial* instance;
    static Tutorial* getInstance();

    static cocos2d::Scene* createScene();

    virtual bool init();

    void setPhysicWorld(cocos2d::PhysicsWorld* world);
    void initTouchEvent();
    void createControlButtons();
    void initHPUI();
    void renderHPUI(int HPleft);

    void menuCloseCallback(cocos2d::Ref* pSender);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void menuJumpCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    void menuShootCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    void menuLeftCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    void menuRightCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);

    void update(float dt);
    void OnCharacterDie(int HPLeft);
    void OnLose();

    CREATE_FUNC(Tutorial);

private:
    cocos2d::PhysicsWorld* world;
    Character* _character;
    TileMap* _tileMap;
    cocos2d::Layer* _uiLayer;
    std::vector<cocos2d::Sprite*> heartsMissing;
    std::vector<cocos2d::Sprite*> heartsFull;
    int HPMax = 5;
};

#endif // __TUTORIAL_SCENE_H__
