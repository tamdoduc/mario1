#ifndef __BOX_H__
#define __BOX_H__

#include "cocos2d.h"
#include "CustomAnimation.h"

class Box : public cocos2d::Sprite
{
public:
    static Box* create();
    virtual bool init() override;
    void setPos(cocos2d::Vec2 newPos);
    void onDestruct();
protected:
    cocos2d::PhysicsBody* boxBody;
    CustomAnimation* destructAnimate;

    void moveLeft();
    void moveRight();

    void createDestructAnimation();

};

#endif // __BOX_H__
