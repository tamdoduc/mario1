#include "TutorialScene.h"
#include "MainMenuScene.h"
#include "Box.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "platform/CCFileUtils.h"
#include "HelloWorldScene.h"


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
	//_character->setScale(_character->getScale() * 2);
	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto background = Sprite::create("Background_tutorial2.jpeg");
	background->setScale(3);
	background->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(background, -1);

	_tileMap = TileMap::create("map_tutorial.tmx");
	if (_tileMap)
	{
		_tileMap->setScale(1.0f);
		_tileMap->setPosition(Vec2(0, 0));
		this->addChild(_tileMap, 0);
		_tileMap->addColliders();
	}

	CCLOG(("Done tile map"));
	auto box = Box::create();
	box->setPos(Vec2(736, 48));
	box->setScale(box
		->getScale() * 1.2f);
	this->addChild(box);
	CCLOG(("Done box"));
	initTouchEvent();

	Ground* ground = Ground::create();
	ground->init();
	ground->setGround(Vec2(0, 0), Vec2(10000, 20));
	this->addChild(ground);
	CCLOG(("Done ground"));
	_uiLayer = UILayer::create();
	this->addChild(_uiLayer, 1);
	_uiLayer->initHPUI(_character->HP);
	_uiLayer->setLeftButtonCallback([this]() {
		if (isFirstTouchLeft)
		{
			isFirstTouchLeft = false;
			_uiLayer->hideAllTutorial();
			_uiLayer->showFadeLayer(false);
		}
	_character->moveDirection(true);
		});
	_uiLayer->setLeftButtonReleaseCallback([this]() { _character->cancelMove(); });
	_uiLayer->setRightButtonCallback([this]() {
		if (isFirstTouchRight)
		{
			isFirstTouchRight = false;
			_uiLayer->hideAllTutorial();
			showLeftControlTutorial();
		}
	_character->moveDirection(false);
		});
	_uiLayer->setRightButtonReleaseCallback([this]() { _character->cancelMove(); });
	_uiLayer->setShootButtonCallback([this]() {
		if (isFirstTouchShoot)
		{
			isFirstTouchShoot = false;
			_uiLayer->hideAllTutorial();
			_uiLayer->showFadeLayer(false);
		}
	_character->shoot(); });
	_uiLayer->setJumpButtonCallback([this]() {
		if (isFirstTouchJump)
		{
			isFirstTouchJump = false;
			_uiLayer->hideAllTutorial();
			_uiLayer->showFadeLayer(false);
		}
	_character->jump(); });
	_uiLayer->hideAllButton();
	// Lên lịch để hiện nút phải sau 2 giây
	this->scheduleOnce([this](float) {
		showRightControlTutorial();
		}, 2.0f, "ShowRightControlTutorial");
	isFirstTouchLeft = true;
	isFirstTouchRight = true;
	CCLOG(("Done UI"));
	door = Door::create();
	door->init();
	door->setPosition(Vec2(850, 48));
	//door->setPosition(Vec2( 150, 50));
	this->addChild(door);
	//door->setScale(door->getScale());
	door->setActionCallback([this]() {
		// Define what happens when the character collides with the door
		this->OnWin();
		});

	CCLOG(("Done Door"));
	this->scheduleUpdate();
	this->getDefaultCamera()->setScale(2.0f);

	return true;
}
void Tutorial::showRightControlTutorial()
{
	_uiLayer->showFadeLayer(true);
	_uiLayer->hideAllTutorial();
	_uiLayer->showButton(1);
	_uiLayer->showHandAndLabel(1);
}
void Tutorial::showLeftControlTutorial()
{
	_uiLayer->hideAllTutorial();
	_uiLayer->showButton(0);
	_uiLayer->showHandAndLabel(0);
}

void Tutorial::showJumpTutorial()
{
	_uiLayer->showFadeLayer(true);
	_uiLayer->hideAllTutorial();
	_uiLayer->showButton(2);
	_uiLayer->showHandAndLabel(2);
}
void Tutorial::showShootTutorial()
{
	_uiLayer->showFadeLayer(true);

	_uiLayer->hideAllTutorial();
	_uiLayer->showButton(3);
	_uiLayer->showHandAndLabel(3);
}
void Tutorial::setControlButtonsVisible(bool visible)
{
	_leftButton->setVisible(visible);
	_rightButton->setVisible(visible);
	_shootButton->setVisible(visible);
	_jumpButton->setVisible(visible);
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

void Tutorial::update(float dt)
{
	if (_character)
	{
		_character->update(dt);

		Vec2 characterPosition = _character->getPosition();
		auto camera = this->getDefaultCamera();

		Size visibleSize = Director::getInstance()->getVisibleSize();
		float cameraHalfWidth = camera->getContentSize().width;
		float minXPosition = visibleSize.width / 2;
		float maxXPosition = 960 - visibleSize.width / 4;

		float newCameraX = std::max(characterPosition.x, minXPosition);
		newCameraX = std::min(newCameraX, maxXPosition);
		//camera->setPosition(cocos2d::Vec2(newCameraX, camera->getPosition().y));
		if (characterPosition.x >= 450 && isFirstTouchJump)
		{
			showJumpTutorial();
		}
		if (characterPosition.x >= 600 && isFirstTouchShoot)
		{
			showShootTutorial();
		}
		_uiLayer->setPosition(camera->getPosition());
	}
}

void Tutorial::OnCharacterDie(int HPLeft)
{
	//renderHPUI(HPLeft);
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

void Tutorial::OnWin()
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
		// Save the level to a JSON file
		rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	document.AddMember("level", 1, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);

	std::string filePath = cocos2d::FileUtils::getInstance()->getWritablePath() + "level.json";
	FILE* file = fopen(filePath.c_str(), "w");
	if (file)
	{
		fputs(buffer.GetString(), file);
		fclose(file);
	}
	else
	{
		CCLOG("Failed to open file for writing: %s", filePath.c_str());
	}


	auto scene = MainMenuScene::create();
	TransitionFade* transition = TransitionFade::create(1.0f, scene);
	Director::getInstance()->replaceScene(transition);
		});

	auto sequence = Sequence::create(delayAction, callFunc, nullptr);
	this->runAction(sequence);
}