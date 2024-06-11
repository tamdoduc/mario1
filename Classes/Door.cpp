// Door.cpp
#include "Door.h"

USING_NS_CC;

Door* Door::create()
{
    Door* pRet = new(std::nothrow) Door();
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

bool Door::init()
{
    if (!Sprite::init())
    {
        return false;
    }
    this->setTexture("door_1.png");
    int desiredWidth = 50;
    int desiredHeight = 50;

    this->setScale(desiredWidth / this->getContentSize().width, desiredHeight / this->getContentSize().height);
    
    // Add physics body
    boxBody = PhysicsBody::createBox(this->getContentSize());
    boxBody->setDynamic(false);
    boxBody->setCategoryBitmask(0x04);
    boxBody->setCollisionBitmask(0x0B);
    boxBody->setContactTestBitmask(0x0B);
    boxBody->setGravityEnable(false);
    this->setPhysicsBody(boxBody);
    
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(Door::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    createIdleAnimation();
    return true;
}


void Door::createIdleAnimation()
{
    std::vector<std::string> frameNames;
    for (int i = 1; i <= 4; ++i) // Replace N with the number of frames
    {
        frameNames.push_back(StringUtils::format("door_%d.png", i));
    }

    idleAnimation = CustomAnimation::create(frameNames, 0.1f);
    this->addChild(idleAnimation);
    idleAnimation->setPosition(this->getContentSize() / 2);
    idleAnimation->setVisible(true);
    idleAnimation->start();
    //// ẩn hình hiện tại
    this->setTextureCoords(cocos2d::Rect());
}
void Door::setActionCallback(const std::function<void()>& callback)
{
    this->actionCallback = callback;
}

bool Door::onContactBegin(cocos2d::PhysicsContact& contact)
{
    if (isTouch)
        return false;
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();
    if (nodeA != this && nodeB != this)
        return false;
    std::string nodeNameA = nodeA ? typeid(*nodeA).name() : "Unknown";
    std::string nodeNameB = nodeB ? typeid(*nodeB).name() : "Unknown";
    // Use dynamic casting to check if the nodes are of type Character
    Character* characterA = dynamic_cast<Character*>(nodeA);
    Character* characterB = dynamic_cast<Character*>(nodeB);

    // Check if either node is a Character
    if (characterA || characterB)
    {
        Character* character = characterA ? characterA : characterB;
        if (character)
        {
            CCLOG("Door Contact: Node A: (%s), Node B: (%s)", nodeNameA.c_str(), nodeNameB.c_str());
            actionCallback();
            isTouch = true;
        }
        return true;
    }

    return false; // Return false if the collision is not handled
}