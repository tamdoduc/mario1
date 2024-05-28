#include "MyCamera.h"

USING_NS_CC;

MyCamera* MyCamera::create() {
    MyCamera* ret = new MyCamera();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    else {
        delete ret;
        return nullptr;
    }
}

void MyCamera::visit(Renderer* renderer, const Mat4& parentTransform, uint32_t parentFlags) {
    // Set viewport size to 500x500
    auto visibleSize = Director::getInstance()->getVisibleSize();
    glViewport(0, 0, 500, 500);
    CCLOG("Camera");

    // Setup the projection matrix for 2D orthographic view
    Mat4 projection;
    Mat4::createOrthographicOffCenter(0, 500, 0, 500, -1, 1, &projection);
    this->setAdditionalProjection(projection);

    // Setup the view matrix to follow the character
    Mat4 view = Mat4::IDENTITY;
    this->setNodeToParentTransform(view);

    // Call visit on the children to draw them within the 500x500 region
    Camera::visit(renderer, parentTransform, parentFlags);

    // Restore viewport
    glViewport(0, 0, (GLsizei)visibleSize.width, (GLsizei)visibleSize.height);
}
