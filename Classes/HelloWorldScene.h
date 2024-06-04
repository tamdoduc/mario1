#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "MyCamera.h"
#include "Character.h"
#include "TileMap.h"
#include "Singleton.h"
#include "MainMenuScene.h"
#include "Ground.h"

class HelloWorld : public cocos2d::Scene
{
public:
    static HelloWorld* getInstance();
    virtual bool init();
    static cocos2d::Scene* createScene();
    // Implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

    void update(float dt) override;
    void menuCloseCallback(cocos2d::Ref* pSender);
    void initTouchEvent();
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void setPhysicWorld(cocos2d::PhysicsWorld* m_world);
    void createControlButtons();
    void cancelTouchCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    void menuJumpCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    void menuShootCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    void menuLeftCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    void menuRightCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    void OnCharacterDie(int HPLeft);
    void OnLose();
    void renderHPUI(int HPleft);
    void initHPUI();
    cocos2d::Layer* _uiLayer;

private:
    static HelloWorld* instance;
    Character* _character;
    std::vector<cocos2d::Sprite*> heartsFull;
    std::vector<cocos2d::Sprite*> heartsMissing;
    int HPMax =5;
    cocos2d::PhysicsWorld* world;
    TileMap* _tileMap;// Add a member variable for MyCamera
};
