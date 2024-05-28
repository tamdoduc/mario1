#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "MyCamera.h"
#include "Character.h"
#include "TileMap.h"

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

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
    void createGround();

private:
    cocos2d::PhysicsWorld* world;
    Character* _character;
    cocos2d::Layer* _uiLayer;
    MyCamera* _camera; // Add a member variable for MyCamera
};
