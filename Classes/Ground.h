#ifndef __GROUND_H__
#define __GROUND_H__

#include "cocos2d.h"
#include "Character.h"

class Ground : public cocos2d::Sprite
{
public:
    static Ground* create();
    virtual bool init() override;
    void setGround(cocos2d::Vec2 newPos, cocos2d::Vec2 size);
protected:
    cocos2d::PhysicsBody* groundBody;

    bool onContactBegin(cocos2d::PhysicsContact& contact);

    //CREATE_FUNC(Enemy);
};

#endif // __GROUND_H__
