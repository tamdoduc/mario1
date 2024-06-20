#include "MainMenuScene.h"
#include "HelloWorldScene.h"
#include "TutorialScene.h"
#include "platform/CCFileUtils.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "platform/CCFileUtils.h"

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
    level = getLevel();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    std::string s = "Play";
    if (level!=-1)
        s = "Level: " + std::to_string(level);
    auto playItem = MenuItemFont::create(s, CC_CALLBACK_1(MainMenuScene::menuPlayCallback, this));
    playItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 + 100));

    // Create Exit Button
    auto exitItem = MenuItemFont::create("Exit", CC_CALLBACK_1(MainMenuScene::menuExitCallback, this));
    exitItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 50));

    // Create menu, it's an autorelease object
    auto menu = Menu::create( playItem, exitItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);


    auto background = Sprite::create("Background_tutorial2.jpeg");
    background->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
    int desiredWidth = 960;
    int desiredHeight = 540;

    background->setScale(desiredWidth / background->getContentSize().width, desiredHeight / background->getContentSize().height);
    this->addChild(background, -1);

    return true;
}

void MainMenuScene::menuPlayCallback(Ref* pSender)
{
    CCLOG("Start test");

    std::string filePath = cocos2d::FileUtils::getInstance()->getWritablePath() + "level.json";
    auto fileUtils = cocos2d::FileUtils::getInstance();

    Scene* scene;

    // Check if the file exists
    if (fileUtils->isFileExist(filePath))
    {
        // File exists, load HelloWorld scene
        scene = HelloWorld::createScene();
        //scene = Tutorial::createScene();
    }
    else
    {
        // File does not exist, load Tutorial scene
        scene = Tutorial::createScene();
       // scene = HelloWorld::createScene();
    }
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene, Color3B(0, 255, 255)));
}

void MainMenuScene::menuExitCallback(Ref* pSender)
{
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
int MainMenuScene::getLevel() {
    std::string filePath = cocos2d::FileUtils::getInstance()->getWritablePath() + "level.json";
    std::string fileContent = cocos2d::FileUtils::getInstance()->getStringFromFile(filePath);

    if (fileContent.empty()) {
        CCLOG("File is empty or does not exist: %s", filePath.c_str());
        return -1; // return a default value or handle the error as needed
    }

    rapidjson::Document document;
    if (document.Parse(fileContent.c_str()).HasParseError()) {
        CCLOG("Failed to parse JSON file: %s", filePath.c_str());
        return -1; // return a default value or handle the error as needed
    }

    if (!document.HasMember("level") || !document["level"].IsInt()) {
        CCLOG("JSON does not have 'level' member or 'level' is not an integer");
        return -1; // return a default value or handle the error as needed
    }

    return document["level"].GetInt();
}

