#ifndef __BULLET_H__
#define __BULLET_H__

#include "cocos2d.h"

class Bullet : public cocos2d::Sprite
{
public:
    static Bullet* create();
    virtual bool init() override;
    virtual void update(float dt) override;
    void shoot(int direction);

protected:
    int _direction;
    float _speed;
    float traveledDistance = 0.0f;
    bool onContactBegin(cocos2d::PhysicsContact& contact);

    //CREATE_FUNC(Bullet);
};

#endif // __BULLET_H__
