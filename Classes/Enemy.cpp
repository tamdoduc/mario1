#include "Enemy.h"

USING_NS_CC;

Enemy* Enemy::create()
{
    Enemy* pRet = new(std::nothrow) Enemy();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        return nullptr;
    }
}

bool Enemy::init()
{
    if (!Sprite::initWithFile("Fly_1.png"))
    {
        return false;
    }

    int desiredWidth = 25;
    int desiredHeight = 25;

    this->setScale(desiredWidth / this->getContentSize().width, desiredHeight / this->getContentSize().height);

    // Set default values
    isMovingLeft = true;
    moveSpeed = 50.0f; // Adjust move speed as needed
    moveRange = 80.0f; // Adjust move range as needed
    originalX = this->getPositionX();

    // Add physics body
    enemyBody = PhysicsBody::createBox(this->getContentSize());
    enemyBody->setDynamic(true);
    enemyBody->setCategoryBitmask(0x04);
    enemyBody->setCollisionBitmask(0x0B);
    enemyBody->setContactTestBitmask(0x0B);
    enemyBody->setGravityEnable(false);
    this->setPhysicsBody(enemyBody);

    // Set up contact listener
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(Enemy::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    createRunAnimation();

    this->scheduleUpdate();

    return true;
}

void Enemy::setPos(cocos2d::Vec2 newPos)
{
    this->setPosition(newPos);
    originalX = newPos.x;
}

void Enemy::update(float dt)
{
    if (isMovingLeft)
    {
        this->moveLeft();
        if (this->getPositionX() <= originalX - moveRange)
        {
            isMovingLeft = false;
        }
    }
    else
    {
        this->moveRight();
        if (this->getPositionX() >= originalX)
        {
            isMovingLeft = true;
        }
    }
}

void Enemy::moveLeft()
{
    this->setPositionX(this->getPositionX() - moveSpeed * Director::getInstance()->getDeltaTime());
    this->setScaleX(1 * fabs(this->getScaleX()));
}

void Enemy::moveRight()
{
    this->setPositionX(this->getPositionX() + moveSpeed * Director::getInstance()->getDeltaTime());
    this->setScaleX(-1 * fabs(this->getScaleX()));

}

bool Enemy::onContactBegin(cocos2d::PhysicsContact& contact)
{
    //auto nodeA = contact.getShapeA()->getBody()->getNode();
    //auto nodeB = contact.getShapeB()->getBody()->getNode();

    //// Check if collision is with bullet
    //if (nodeA->getTag() == 1 || nodeB->getTag() == 1) // Assuming bullet has tag 1
    //{
    //    this->removeFromParent();
    //    return true;
    //}

    return false;
}
void Enemy::createRunAnimation()
{
    std::vector<std::string> frameNames;
    for (int i = 1; i <= 6; ++i) // Replace N with the number of frames
    {
        frameNames.push_back(StringUtils::format("Fly_%d.png", i));
    }

    runAnimate = CustomAnimation::create(frameNames, 0.1f);
    this->addChild(runAnimate);
    runAnimate->setPosition(this->getContentSize() / 2);
    runAnimate->setVisible(true);
    runAnimate->start();
    //// ẩn hình hiện tại
    this->setTextureCoords(cocos2d::Rect());
}