#ifndef __CUSTOM_ANIMATION_H__
#define __CUSTOM_ANIMATION_H__

#include "cocos2d.h"

class CustomAnimation : public cocos2d::Node
{
public:
    static CustomAnimation* create(const std::vector<std::string>& frameNames, float delay);
    virtual bool init(const std::vector<std::string>& frameNames, float delay);

    void start();
    void stop();

private:
    void updateAnimation(float dt);

    std::vector<std::string> frameNames;
    float delay;
    int currentFrameIndex;
    cocos2d::Sprite* sprite;
    bool isRunning;
};

#endif // __CUSTOM_ANIMATION_H__
