#include "HelloWorldScene.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "platform/CCFileUtils.h"
USING_NS_CC;

HelloWorld* HelloWorld::instance = nullptr;

HelloWorld* HelloWorld::getInstance()
{
	if (!instance)
	{
		instance = new HelloWorld();
	}
	return instance;

}

Scene* HelloWorld::createScene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld();//->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	auto layer = HelloWorld::create();
	layer->setPhysicWorld(scene->getPhysicsWorld());
	scene->addChild(layer);
	return scene;
}

bool HelloWorld::init()
{
	if (!Scene::init())
	{
		return false;
	}
	level = getLevel();
	if (level > 2)
		level = 2;
	_character = Character::create();
	this->addChild(_character);
	_character->setScene(this);

	auto background = Sprite::create("background.jpg");
	background->setScale(1.3f);
	background->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(background, -1);
	std::string tileLevel = "Map" + std::to_string(level) + ".tmx";


	_tileMap = TileMap::create(tileLevel);
	if (_tileMap)
	{
		_tileMap->setPosition(Vec2(0, 0));
		this->addChild(_tileMap, 0);
		_tileMap->addColliders();
	}

	_uiLayer = UILayer::create();
	this->addChild(_uiLayer, 1);
	_uiLayer->initHPUI(_character->HP);
	_uiLayer->setLeftButtonCallback([this]() { _character->moveDirection(true); });
	_uiLayer->setLeftButtonReleaseCallback([this]() { _character->cancelMove(); });
	_uiLayer->setRightButtonCallback([this]() { _character->moveDirection(false); });
	_uiLayer->setRightButtonReleaseCallback([this]() { _character->cancelMove(); });
	_uiLayer->setShootButtonCallback([this]() { _character->shoot(); });
	_uiLayer->setJumpButtonCallback([this]() { _character->jump(); });

	this->getDefaultCamera()->setScale(2.0f);
	initTouchEvent();

	Ground* ground = Ground::create();
	ground->init();
	ground->setGround(Vec2(0, 0), Vec2(10000, 10));
	this->addChild(ground);

	Size mapSize = _tileMap->getMapSize();
	CCLOG("Map Size: width = %f, height = %f", mapSize.width, mapSize.height);
	EnemyManager::getInstance()->createEnemies(this, level);
	ElevatorManager::getInstance()->createElevators(this,level);

	door = Door::create();
	door->init();
	door->setPosition(lstPosDoor[level-1]);
	CCLOG("_tileMap->getMapSize() %f", _tileMap->getMapSize().width);
	this->addChild(door);
	//door->setScale(door->getScale());
	door->setActionCallback([this]() {
		// Define what happens when the character collides with the door
		this->OnWin();
		});

	CCLOG("CurrentLevel: %i", level);
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

void HelloWorld::update(float dt)
{
	if (_character)
	{
		_character->update(dt);

		// Get the current position of the character
		Vec2 characterPosition = _character->getPosition();

		auto camera = this->getDefaultCamera();

		// Calculate the minimum x position for the camera
		Size visibleSize = Director::getInstance()->getVisibleSize();
		float cameraHalfWidth = visibleSize.width / 2.0f;
		float cameraHalfHeight = visibleSize.height / 2.0f;
		//CCLOG("test %f", cameraHalfWidth);
		float minXPosition = cameraHalfWidth;
		float maxXPosition = _tileMap->getMapSize().width - cameraHalfWidth;
		float minYPosition = cameraHalfHeight;
		float maxYPosition = _tileMap->getMapSize().height - cameraHalfHeight;

		// Set the new x position of the camera, ensuring it's not less than minXPosition
		float newCameraX = std::max(characterPosition.x, minXPosition);
		newCameraX = std::min(newCameraX, maxXPosition);
		float newCameraY = std::max(characterPosition.y, minYPosition);
		newCameraY = std::min(newCameraY, maxYPosition);
		camera->setPosition(cocos2d::Vec2(newCameraX, newCameraY));

		// Update the UI layer's position based on the camera's position
		_uiLayer->setPosition(camera->getPosition());
	}
}

void HelloWorld::OnCharacterDie(int HPLeft)
{
	CCLOG("ONCHARACTERDIE %i", HPLeft);
	_uiLayer->renderHPUI(HPLeft, _character->HP);
}

void HelloWorld::OnLose()
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

void HelloWorld::OnWin()
{
	if (_uiLayer == nullptr)
	{
		CCLOG("_uiLayer is nullptr. Returning...");
		return;
	}

	auto loseLabel = Sprite::create("LevelComplete.jpg");
	int desiredWidth = 200;
	int desiredHeight = 200;
	loseLabel->setPosition(Vec2(0, 0));
	_uiLayer->addChild(loseLabel, 1);
	loseLabel->setScale(0);

	auto scaleAction = ScaleTo::create(2.0f, 5.0f);
	loseLabel->runAction(scaleAction);

	auto delayAction = DelayTime::create(3.0f);
	auto callFunc = CallFunc::create([&]() {
		incrementLevel();


	auto scene = MainMenuScene::create();
	TransitionFade* transition = TransitionFade::create(1.0f, scene);
	Director::getInstance()->replaceScene(transition);
		});

	auto sequence = Sequence::create(delayAction, callFunc, nullptr);
	this->runAction(sequence);
}
int HelloWorld::getLevel() {
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

// Function to increment the level value by 1 and save it back to the JSON file
void HelloWorld::incrementLevel() {
	int currentLevel = getLevel();
	if (currentLevel == -1) {
		CCLOG("Failed to get current level");
		return;
	}

	currentLevel += 1;

	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	document.AddMember("level", currentLevel, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);

	std::string filePath = cocos2d::FileUtils::getInstance()->getWritablePath() + "level.json";
	FILE* file = fopen(filePath.c_str(), "w");
	if (file) {
		fputs(buffer.GetString(), file);
		fclose(file);
	}
	else {
		CCLOG("Failed to open file for writing: %s", filePath.c_str());
	}
}
