#include "Box.h"

USING_NS_CC;

Box* Box::create()
{
    Box* pRet = new(std::nothrow) Box();
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

bool Box::init()
{
    if (!Sprite::initWithFile("Tutorial/Box (2).png"))
    {
        return false;
    }

    int desiredWidth = 16;
    int desiredHeight = 16;

    //this->setScale(desiredWidth / this->getContentSize().width, desiredHeight / this->getContentSize().height);


    // Add physics body
    boxBody = PhysicsBody::createBox(this->getContentSize());
    boxBody->setDynamic(false);
    boxBody->setCategoryBitmask(0x04);
    boxBody->setCollisionBitmask(0x0B);
    boxBody->setContactTestBitmask(0x0B);
    boxBody->setGravityEnable(false);
    this->setPhysicsBody(boxBody);

    createDestructAnimation();

    this->scheduleUpdate();

    return true;
}

void Box::setPos(cocos2d::Vec2 newPos)
{
    this->setPosition(newPos);
}
void Box::createDestructAnimation()
{
    std::vector<std::string> frameNames;
    for (int i = 2; i <= 7; ++i) // Replace N with the number of frames
    {
        frameNames.push_back(StringUtils::format("Tutorial/Box (%d).png", i));
    }

    destructAnimate = CustomAnimation::create(frameNames, 0.15f);
    this->addChild(destructAnimate);
    destructAnimate->setPosition(this->getContentSize() / 2);
    destructAnimate->setVisible(false);
    destructAnimate->stop();
    //// ẩn hình hiện tại
   
}
void Box::onDestruct()
{
    this->setTextureCoords(cocos2d::Rect());
    destructAnimate->setVisible(true);
    destructAnimate->start();

    this->scheduleOnce([this](float) {
        this->removeFromParentAndCleanup(true);
        }, 0.15f*6, "reset_position_key");
}