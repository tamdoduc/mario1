#pragma once

#include "cocos2d.h"

class MyCamera : public cocos2d::Camera {
public:
    static MyCamera* create();

    virtual void visit(cocos2d::Renderer* renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) override;
};
