// Door.h
#ifndef __DOOR_H__
#define __DOOR_H__

#include "cocos2d.h"
#include "CustomAnimation.h"
#include "Character.h"

class Door : public cocos2d::Sprite
{
public:
    static Door* create();
    virtual bool init();
    void setActionCallback(const std::function<void()>& callback);


private:
    cocos2d::PhysicsBody* boxBody;
    bool isTouch = false;
    CustomAnimation* idleAnimation;
    std::function<void()> actionCallback;
    void createIdleAnimation();
    bool onContactBegin(cocos2d::PhysicsContact& contact);
};

#endif // __DOOR_H__
