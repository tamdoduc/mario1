#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "cocos2d.h"

class Enemy : public cocos2d::Sprite
{
public:
    static Enemy* create();
    virtual bool init() override;
    virtual void update(float dt) override;
    void setPos(cocos2d::Vec2 newPos);

protected:
    bool isMovingLeft;
    float moveSpeed;
    float moveRange;
    float originalX;
    cocos2d::PhysicsBody* enemyBody;

    void moveLeft();
    void moveRight();
    bool onContactBegin(cocos2d::PhysicsContact& contact);

    //CREATE_FUNC(Enemy);
};

#endif // __ENEMY_H__
