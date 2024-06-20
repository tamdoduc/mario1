#include "Elevator.h"

USING_NS_CC;

Elevator* Elevator::create()
{
    Elevator* pRet = new(std::nothrow) Elevator();
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

bool Elevator::init()
{
    if (!Sprite::initWithFile("elevator.png"))
    {
        return false;
    }
  //  CCLOQ("ELEVATOR");
    int desiredWidth = 50;
    int desiredHeight = 50;
    float scaleFator = 1.2f;
   // this->setScale(scaleFator * desiredWidth / this->getContentSize().width, scaleFator * desiredHeight / this->getContentSize().height);

    // Set default values
    isMovingUp = true;
    moveSpeed = 50.0f; // Adjust move speed as needed
    moveRange = 200.0f; // Adjust move range as needed
    originalY = this->getPositionY();

    // Add physics body
    elevatorBody = PhysicsBody::createBox(this->getContentSize());
    elevatorBody->setDynamic(false);
    elevatorBody->setCategoryBitmask(0x01);
    elevatorBody->setCollisionBitmask(0x0E);
    elevatorBody->setContactTestBitmask(0x0E);
    elevatorBody->setGravityEnable(false);
    this->setPhysicsBody(elevatorBody);

    // Set up contact listener
    //auto contactListener = EventListenerPhysicsContact::create();
   // _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    this->scheduleUpdate();

    return true;
}

void Elevator::setPos(cocos2d::Vec2 newPos)
{
    this->setPosition(newPos);
    originalY = newPos.y;
}

void Elevator::update(float dt)
{
    if (isMovingUp)
    {
        this->moveUp();
        if (this->getPositionY() >= originalY + moveRange)
        {
            isMovingUp = false;
        }
    }
    else
    {
        this->moveDown();
        if (this->getPositionY() <= originalY)
        {
            isMovingUp = true;
        }
    }
}

void Elevator::moveUp()
{
    this->setPositionY(this->getPositionY() + moveSpeed * Director::getInstance()->getDeltaTime());
    this->setScaleY(1 * fabs(this->getScaleY()));
}

void Elevator::moveDown()
{
    this->setPositionY(this->getPositionY() - moveSpeed * Director::getInstance()->getDeltaTime());
    this->setScaleY(1 * fabs(this->getScaleY()));
}