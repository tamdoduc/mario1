#include "Ground.h"

USING_NS_CC;

Ground* Ground::create()
{
    Ground* pRet = new(std::nothrow) Ground();
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

bool Ground::init()
{
    if (!Sprite::init())
    {
        return false;
    }

   
    // Set up contact listener
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(Ground::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    this->scheduleUpdate();

    return true;
}
void Ground::setGround(cocos2d::Vec2 newPos, cocos2d::Vec2 size)
{

    this->setPosition(newPos);
    // Set default values
    groundBody = PhysicsBody::createBox(Size(size));
    groundBody->setDynamic(false);
    groundBody->setCategoryBitmask(0x05);
    groundBody->setCollisionBitmask(0x02);
    groundBody->setContactTestBitmask(0x02);
    groundBody->setGravityEnable(false);
    this->setPhysicsBody(groundBody);

}

bool Ground::onContactBegin(cocos2d::PhysicsContact& contact)
{
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
            CCLOG("Ground Contact: Node A: (%s), Node B: (%s)", nodeNameA.c_str(), nodeNameB.c_str());
            character->OnDie();
        }
        return true;
    }

    return false; // Return false if the collision is not handled
}
