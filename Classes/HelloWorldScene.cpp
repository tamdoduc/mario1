#include "HelloWorldScene.h"
#include <EnemyManager.h>

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld();//->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    auto layer = HelloWorld::create();
    layer->setPhysicWorld(scene->getPhysicsWorld());
    scene->addChild(layer);

    // Tạo và thêm TileMap vào Scene
    auto tileMap = TileMap::create("Map.tmx");
    if (tileMap)
    {
        // Đặt vị trí cho tilemap
        tileMap->setPosition(Vec2(0, 0)); // Thay Vec2(0, 0) bằng vị trí mong muốn
        scene->addChild(tileMap);
        tileMap->addColliders();
    }

    return scene;
}

bool HelloWorld::init()
{
    if (!Scene::init())
    {
        return false;
    }

    _character = Character::create();
    this->addChild(_character);

    _uiLayer = Layer::create();
    this->addChild(_uiLayer,100);

    createGround();
    initTouchEvent();
    createControlButtons();

    EnemyManager::getInstance()->createEnemies(this);

    this->scheduleUpdate();

    return true;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::initTouchEvent()
{
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    touchListener->onTouchBegan = [](Touch* touch, Event* event) -> bool {
        CCLOG("Touch Began");
        return true;
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event)
{
    CCLOG("OnTouchEnd");
    _character->cancelMove();
}

void HelloWorld::setPhysicWorld(cocos2d::PhysicsWorld* m_world)
{
    world = m_world;
}

void HelloWorld::createControlButtons()
{
    auto background = Sprite::create("background.jpg");
    background->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
    this->addChild(background, -1);

    Vec2 buttonControllPos = Vec2(-250, -250);
    auto leftButton = ui::Button::create("left.png", "left.png");
    leftButton->setPosition(buttonControllPos +  Vec2(50, 150));
    leftButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::menuLeftCallback, this));
    _uiLayer->addChild(leftButton);

    auto rightButton = ui::Button::create("right.png", "right.png");
    rightButton->setPosition(buttonControllPos + Vec2(150, 150));
    rightButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::menuRightCallback, this));
    _uiLayer->addChild(rightButton);

    auto shootButton = ui::Button::create("attack.png", "attack.png");
    shootButton->setPosition(buttonControllPos + Vec2(100, 100));
    shootButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::menuShootCallback, this));
    _uiLayer->addChild(shootButton);

    auto jumpButton = ui::Button::create("up.png", "up.png");
    jumpButton->setPosition(buttonControllPos + Vec2(100, 200));
    jumpButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::menuJumpCallback, this));
    _uiLayer->addChild(jumpButton);
}

void HelloWorld::cancelTouchCallback(Ref* pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        _character->cancelMove();
    }
}

void HelloWorld::menuJumpCallback(Ref* pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        _character->jump();
    }
}

void HelloWorld::menuShootCallback(Ref* pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        _character->shoot();
    }
}

void HelloWorld::menuLeftCallback(Ref* pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::BEGAN || type == ui::Widget::TouchEventType::MOVED)
    {
        _character->moveDirection(true);
    }
    else
    {
        _character->cancelMove();
    }
}

void HelloWorld::menuRightCallback(Ref* pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::BEGAN || type == ui::Widget::TouchEventType::MOVED)
    {
        _character->moveDirection(false);
    }
    else
    {
        _character->cancelMove();
    }
}

void HelloWorld::createGround()
{
    // Existing ground creation code
}

void HelloWorld::update(float dt)
{
    if (_character)
    {
        _character->update(dt);

        // Get the current position of the character
        Vec2 characterPosition = _character->getPosition();

        auto camera = this->getDefaultCamera();

        // Calculate the minimum x position for the camera
        float cameraHalfWidth = camera->getContentSize().width;
        float minXPosition = 250;

        // Set the new x position of the camera, ensuring it's not less than minXPosition
        float newCameraX = std::max(characterPosition.x, minXPosition);
        camera->setPosition(cocos2d::Vec2(newCameraX, camera->getPosition().y));

        // Update the UI layer's position based on the camera's position
        _uiLayer->setPosition(camera->getPosition());
    }
}
