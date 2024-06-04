#include "CustomAnimation.h"

USING_NS_CC;

CustomAnimation* CustomAnimation::create(const std::vector<std::string>& frameNames, float delay)
{
    CustomAnimation* pRet = new(std::nothrow) CustomAnimation();
    if (pRet && pRet->init(frameNames, delay))
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

bool CustomAnimation::init(const std::vector<std::string>& frameNames, float delay)
{
    if (!Node::init())
    {
        return false;
    }

    this->frameNames = frameNames;
    this->delay = delay;
    this->currentFrameIndex = 0;
    this->isRunning = false;

    sprite = Sprite::create(frameNames.at(0));
    sprite->setAnchorPoint(Vec2(0.5f, 0.5f)); // Ensure the anchor point is centered
    this->addChild(sprite);

    this->schedule(CC_SCHEDULE_SELECTOR(CustomAnimation::updateAnimation), delay);

    return true;
}

void CustomAnimation::start()
{
    this->isRunning = true;
}

void CustomAnimation::stop()
{
    this->isRunning = false;
}

void CustomAnimation::updateAnimation(float dt)
{
    if (isRunning)
    {
        currentFrameIndex = (currentFrameIndex + 1) % frameNames.size();
        sprite->setTexture(frameNames.at(currentFrameIndex));
    }
}
