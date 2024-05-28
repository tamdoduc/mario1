#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Character.h"

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    void initTouchEvent();
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

    void createControlButtons();
    void menuLeftCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    void menuRightCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    void menuShootCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    void menuJumpCallback(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);

    void createGround();
    void update(float dt) override;
    void setPhysicWorld(cocos2d::PhysicsWorld* world);

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

private:
    Character* _character;
    cocos2d::PhysicsWorld* world;
};

#endif // __HELLOWORLD_SCENE_H__
