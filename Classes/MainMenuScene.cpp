#include "MainMenuScene.h"
#include "HelloWorldScene.h"

USING_NS_CC;

Scene* MainMenuScene::createScene()
{
    return MainMenuScene::create();
}

// on "init" you need to initialize your instance
bool MainMenuScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Create Play Button
    auto playItem = MenuItemFont::create("Play", CC_CALLBACK_1(MainMenuScene::menuPlayCallback, this));
    playItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 50));

    // Create Exit Button
    auto exitItem = MenuItemFont::create("Exit", CC_CALLBACK_1(MainMenuScene::menuExitCallback, this));
    exitItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 50));

    // Create menu, it's an autorelease object
    auto menu = Menu::create(playItem, exitItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);


    auto background = Sprite::create("mainmenubg.jpg");
    background->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
    int desiredWidth = 500;
    int desiredHeight = 500;

    background->setScale(desiredWidth / background->getContentSize().width, desiredHeight / background->getContentSize().height);
    this->addChild(background, -1);

    return true;
}

void MainMenuScene::menuPlayCallback(Ref* pSender)
{
    auto scene = HelloWorld::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(0, 255, 255)));
}

void MainMenuScene::menuExitCallback(Ref* pSender)
{
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
