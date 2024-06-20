#ifndef __ENEMY2_H__
#define __ENEMY2_H__

#include "cocos2d.h"
#include "CustomAnimation.h"

class Enemy2 : public cocos2d::Sprite
{
public:
    static Enemy2* create();
    virtual bool init() override;
    virtual void update(float dt) override;
    void setPos(cocos2d::Vec2 newPos);
protected:
    bool isMovingLeft;
    float moveSpeed;
    float moveRange;
    float originalX;
    cocos2d::PhysicsBody* enemyBody;
    CustomAnimation* runAnimate;

    void moveLeft();
    void moveRight();
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    void createRunAnimation();
    void shootBullet(float dt);

    //CREATE_FUNC(Enemy);
};

#endif // __ENEMY2_H__
