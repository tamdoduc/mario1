#ifndef CHARACTER_H
#define CHARACTER_H

#include "cocos2d.h"
#include "Bullet.h" // Include Bullet header if it is in a different file

class Character : public cocos2d::Sprite
{
public:
    static Character* create();
    virtual bool init();
    void moveTo(const cocos2d::Vec2& position);
    void moveDirection(const bool m_isMoveLeft);
    void cancelMove();
    void update(float dt) override;
    void shoot();
    void jump();

private:
    bool isMove;
    bool isMoveLeft;
    bool isMoveRight;
    bool isJumping;
    int direction;
    float jumpForce;

    bool onContactBegin(cocos2d::PhysicsContact& contact);
};

#endif // CHARACTER_H
