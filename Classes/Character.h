#ifndef CHARACTER_H
#define CHARACTER_H

#include "Bullet.h" // Include Bullet header if it is in a different file
#include "cocos2d.h"
#include "CustomAnimation.h"
#include "Enemy.h"
#include "Enemy2.h"

class HelloWorld;
class Tutorial;


class Character : public cocos2d::Sprite
{
public:
    static Character* create();
    virtual bool init();
    void setScene(HelloWorld* helloWorldScene);
    void setScene(Tutorial* tutorialScene);
    void moveTo(const cocos2d::Vec2& position);
    void moveDirection(const bool m_isMoveLeft);
    void cancelMove();
    void update(float dt);
    void shoot();
    void jump();

    bool onContactBegin(cocos2d::PhysicsContact& contact);
    void OnRevive();
    void OnDie();
    void ReSpawn(cocos2d::Vec2 pos);
    void CancelAllAnimation();
    int HP = 5;

    bool isPlaying = false;
    void OnLostHP();

private:

    bool isMove;
    bool isMoveLeft;
    bool isMoveRight;
    bool isJumping;
    int direction;
    float jumpForce;
    float coolDown;
    cocos2d::Vec2 startPos;
    const float ATTACKTIME = 0.5f;
    cocos2d::PhysicsBody* characterBody;

    CustomAnimation* idleAnimate;
    CustomAnimation* jumpAnimate;
    CustomAnimation* runAnimate;
    CustomAnimation* dieAnimate;
    CustomAnimation* appearAnimate;
    HelloWorld* helloWorldScene;
    Tutorial* tutorialScene;

    void createIdleAnimation();
    void createJumpAnimation();
    void createRunAnimation();
    void createDieAnimation();
    void createAppearAnimation();
};

#endif // CHARACTER_H
