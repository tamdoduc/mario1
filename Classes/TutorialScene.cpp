#include "TutorialScene.h"
#include "MainMenuScene.h"
#include "Box.h"

USING_NS_CC;

Tutorial* Tutorial::instance = nullptr;

Tutorial* Tutorial::getInstance()
{
    if (!instance)
    {
        instance = new Tutorial();
    }
    return instance;
}

Scene* Tutorial::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld();//->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    auto layer = Tutorial::create();
    layer->setPhysicWorld(scene->getPhysicsWorld());
    scene->addChild(layer);
    return scene;
}

bool Tutorial::init()
{
    if (!Scene::init())
    {
        return false;
    }

    _character = Character::create();
    this->addChild(_character);
    _character->setScene(this);

    auto background = Sprite::create("Tutorial/Background_tutorial2.jpeg");
    background->setScale(1.3f);
    background->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
    this->addChild(background, -1);

    _tileMap = TileMap::create("map_tutorial.tmx");
    if (_tileMap)
    {
        _tileMap->setPosition(Vec2(0, 0));
        this->addChild(_tileMap, 0);
        _tileMap->addColliders();
    }
    auto box = Box::create();
    box->setPos(Vec2(736, 48));
    this->addChild(box);
    _uiLayer = Layer::create();
    this->addChild(_uiLayer, 1);
    this->getDefaultCamera()->setScale(2.0f);
    initTouchEvent();

    Ground* ground = Ground::create();
    ground->init();
    ground->setGround(Vec2(0, 0), Vec2(10000, 20));
    this->addChild(ground);

  

    createControlButtons();
    initHPUI();

    this->scheduleUpdate();

    return true;
}

void Tutorial::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void Tutorial::initTouchEvent()
{
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchEnded = CC_CALLBACK_2(Tutorial::onTouchEnded, this);
    touchListener->onTouchBegan = [](Touch* touch, Event* event) -> bool {
        CCLOG("Touch Began");
        return true;
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void Tutorial::onTouchEnded(Touch* touch, Event* event)
{
    CCLOG("OnTouchEnd");
    _character->cancelMove();
}

void Tutorial::setPhysicWorld(cocos2d::PhysicsWorld* m_world)
{
    world = m_world;
}

void Tutorial::initHPUI()
{
    Vec2 holderHeartPos = Vec2(-400, 200);
    int desiredWidth = 25;
    int desiredHeight = 25;

    for (int i = 0; i < HPMax; i++)
    {
        std::string missingHeartFile = "heartMissing.png";
        auto heartMissing = Sprite::create(missingHeartFile);
        heartMissing->setPosition(holderHeartPos + Vec2(30 * i, 0));
        heartMissing->setScale(desiredWidth / heartMissing->getContentSize().width, desiredHeight / heartMissing->getContentSize().height);
        heartMissing->setVisible(true);
        _uiLayer->addChild(heartMissing);
        heartsMissing.push_back(heartMissing);

        std::string fullHeartFile = "heart.png";
        auto heartFull = Sprite::create(fullHeartFile);
        heartFull->setPosition(holderHeartPos + Vec2(30 * i, 0));
        heartFull->setScale(desiredWidth / heartFull->getContentSize().width, desiredHeight / heartFull->getContentSize().height);
        heartFull->setVisible(true);
        _uiLayer->addChild(heartFull);
        heartsFull.push_back(heartFull);
    }
}

void Tutorial::renderHPUI(int HPleft)
{
    for (int i = 0; i < HPMax; i++)
    {
        bool isHaveHP = (i + 1 <= HPleft);
        Sprite* missingHeart = heartsMissing[i];
        Sprite* fullHeart = heartsFull[i];

        if (missingHeart && fullHeart)
        {
            missingHeart->setVisible(!isHaveHP);
            fullHeart->setVisible(isHaveHP);
        }
        else
        {
            CCLOG("Error: Missing heart sprite at index %d", i);
        }
    }
}

void Tutorial::createControlButtons()
{
    Vec2 btnControllerLeft = Vec2(-500, -370);
    Vec2 btnControllerRight = Vec2(350, -350);
    auto leftButton = ui::Button::create("left.png", "left.png");
    leftButton->setPosition(btnControllerLeft + Vec2(50, 125));
    leftButton->addTouchEventListener(CC_CALLBACK_2(Tutorial::menuLeftCallback, this));
    _uiLayer->addChild(leftButton);

    auto rightButton = ui::Button::create("right.png", "right.png");
    rightButton->setPosition(btnControllerLeft + Vec2(100, 125));
    rightButton->addTouchEventListener(CC_CALLBACK_2(Tutorial::menuRightCallback, this));
    _uiLayer->addChild(rightButton);

    auto shootButton = ui::Button::create("attack.png", "attack.png");
    shootButton->setPosition(btnControllerRight + Vec2(100, 100));
    shootButton->addTouchEventListener(CC_CALLBACK_2(Tutorial::menuShootCallback, this));
    _uiLayer->addChild(shootButton);

    auto jumpButton = ui::Button::create("up.png", "up.png");
    jumpButton->setPosition(btnControllerRight + Vec2(100, 150));
    jumpButton->addTouchEventListener(CC_CALLBACK_2(Tutorial::menuJumpCallback, this));
    _uiLayer->addChild(jumpButton);
}

void Tutorial::menuJumpCallback(Ref* pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        _character->jump();
    }
}

void Tutorial::menuShootCallback(Ref* pSender, ui::Widget::TouchEventType type)
{
    if (type == ui::Widget::TouchEventType::ENDED)
    {
        _character->shoot();
    }
}

void Tutorial::menuLeftCallback(Ref* pSender, ui::Widget::TouchEventType type)
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

void Tutorial::menuRightCallback(Ref* pSender, ui::Widget::TouchEventType type)
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

void Tutorial::update(float dt)
{
    if (_character)
    {
        _character->update(dt);

        Vec2 characterPosition = _character->getPosition();
        auto camera = this->getDefaultCamera();

        float cameraHalfWidth = camera->getContentSize().width;
        float minXPosition = 480;
        float maxXPosition = this->_tileMap->getMapSize().width - 480;

        float newCameraX = std::max(characterPosition.x, minXPosition);
        newCameraX = std::min(newCameraX, maxXPosition);
        camera->setPosition(cocos2d::Vec2(newCameraX, camera->getPosition().y));

        _uiLayer->setPosition(camera->getPosition());
    }
}

void Tutorial::OnCharacterDie(int HPLeft)
{
    renderHPUI(HPLeft);
}

void Tutorial::OnLose()
{
    if (_uiLayer == nullptr)
    {
        CCLOG("_uiLayer is nullptr. Returning...");
        return;
    }

    auto loseLabel = Sprite::create("GameOver.png");
    int desiredWidth = 200;
    int desiredHeight = 200;
    loseLabel->setPosition(Vec2(0, 0));
    _uiLayer->addChild(loseLabel, 1);
    loseLabel->setScale(0);

    auto scaleAction = ScaleTo::create(1.0f, 0.2f);
    loseLabel->runAction(scaleAction);

    auto delayAction = DelayTime::create(3.0f);
    auto callFunc = CallFunc::create([&]() {
        auto scene = MainMenuScene::create();
    TransitionFade* transition = TransitionFade::create(1.0f, scene);
    Director::getInstance()->replaceScene(transition);
        });

    auto sequence = Sequence::create(delayAction, callFunc, nullptr);
    this->runAction(sequence);
}
