#ifndef __ELEVATOR_H__
#define __ELEVATOR_H__

#include "cocos2d.h"

class Elevator : public cocos2d::Sprite
{
public:
    static Elevator* create();

    virtual bool init() override;

    void setPos(cocos2d::Vec2 newPos);
    void update(float dt) override;

private:
    void moveUp();
    void moveDown();

    cocos2d::PhysicsBody* elevatorBody;
    bool isMovingUp;
    float moveSpeed;
    float moveRange;
    float originalY;
};

#endif // __ELEVATOR_H__
