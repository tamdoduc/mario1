#include "HelloWorldScene.h"
#include <EnemyManager.h>

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
Scene* HelloWorld::createScene()sssss
{
	
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld();;//->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
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

	_character = Character::create();
	this->addChild(_character);
	_character->setScene(this);

	auto background = Sprite::create("background.jpg");
	background->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(background, -1);

	// Tạo và thêm TileMap vào Scene
	_tileMap = TileMap::create("Map.tmx");
	if (_tileMap)
	{
		// Đặt vị trí cho tilemap
		_tileMap->setPosition(Vec2(0, 0)); // Thay Vec2(0, 0) bằng vị trí mong muốn
		this->addChild(_tileMap, 0);
		_tileMap->addColliders();
	}
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
	//OnLose();

	Size mapSize = _tileMap->getMapSize();
	CCLOG("Map Size: width = %f, height = %f", mapSize.width, mapSize.height);
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
void HelloWorld::initHPUI()
{
	Vec2 holderHeartPos = Vec2(-400, 200);
	int desiredWidth = 25;
	int desiredHeight = 25;

	for (int i = 0; i < HPMax; i++)
	{
		// Create missing hearts
		std::string missingHeartFile = "heartMissing.png";
		auto heartMissing = Sprite::create(missingHeartFile);
		heartMissing->setPosition(holderHeartPos + Vec2(30 * i, 0));
		heartMissing->setScale(desiredWidth / heartMissing->getContentSize().width, desiredHeight / heartMissing->getContentSize().height);
		heartMissing->setVisible(true);  // Initially visible
		_uiLayer->addChild(heartMissing);
		heartsMissing.push_back(heartMissing);

		// Create full hearts
		std::string fullHeartFile = "heart.png";
		auto heartFull = Sprite::create(fullHeartFile);
		heartFull->setPosition(holderHeartPos + Vec2(30 * i, 0));
		heartFull->setScale(desiredWidth / heartFull->getContentSize().width, desiredHeight / heartFull->getContentSize().height);
		heartFull->setVisible(true);  // Initially invisible
		_uiLayer->addChild(heartFull);
		heartsFull.push_back(heartFull);
	}
}

void HelloWorld::renderHPUI(int HPleft)
{
	for (int i = 0; i < HPMax; i++)
	{
		bool isHaveHP = (i + 1 <= HPleft);
		Sprite* missingHeart = heartsMissing[i];
		Sprite* fullHeart = heartsFull[i];

		// Check if sprites exist
		if (missingHeart && fullHeart)
		{
			// Set visibility based on HP
			missingHeart->setVisible(!isHaveHP);
			fullHeart->setVisible(isHaveHP);
		}
		else
		{
			CCLOG("Error: Missing heart sprite at index %d", i);
		}
	}
	//this->scheduleUpdate();
}

void HelloWorld::createControlButtons()
{

	Vec2 btnControllerLeft = Vec2(-500, -370);
	Vec2 btnControllerRight = Vec2(350, -350);
	auto leftButton = ui::Button::create("left.png", "left.png");
	leftButton->setPosition(btnControllerLeft + Vec2(50, 125));
	leftButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::menuLeftCallback, this));
	_uiLayer->addChild(leftButton);

	auto rightButton = ui::Button::create("right.png", "right.png");
	rightButton->setPosition(btnControllerLeft + Vec2(100, 125));
	rightButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::menuRightCallback, this));
	_uiLayer->addChild(rightButton);

	auto shootButton = ui::Button::create("attack.png", "attack.png");
	shootButton->setPosition(btnControllerRight + Vec2(100, 100));
	shootButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::menuShootCallback, this));
	_uiLayer->addChild(shootButton);

	auto jumpButton = ui::Button::create("up.png", "up.png");
	jumpButton->setPosition(btnControllerRight + Vec2(100, 150));
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
		float minXPosition = 480;
		float maxXPosition = this->_tileMap->getMapSize().width - 480;

		// Set the new x position of the camera, ensuring it's not less than minXPosition
		float newCameraX = std::max(characterPosition.x, minXPosition);
		newCameraX = std::min(newCameraX, maxXPosition);
		camera->setPosition(cocos2d::Vec2(newCameraX, camera->getPosition().y));
		Size mapSize = _tileMap->getMapSize();
		//CCLOG("Map Size: newCameraX = %f, max = %f", newCameraX, mapSize.width);
		// Update the UI layer's position based on the camera's position
		_uiLayer->setPosition(camera->getPosition());
	}
}

void HelloWorld::OnCharacterDie(int HPLeft)
{
	renderHPUI(HPLeft);
}
void HelloWorld::OnLose()
{
	// Kiểm tra xem _uiLayer có được khởi tạo không
	if (_uiLayer == nullptr)
	{
		CCLOG("_uiLayer là nullptr. Đang trở lại...");
		return;
	}

	// Tạo nhãn
	auto loseLabel = Sprite::create("GameOver.png");
	int desiredWidth = 200;
	int desiredHeight = 200;

	//Vec2 scale T(desiredWidth / loseLabel->getContentSize().width, desiredHeight / loseLabel->getContentSize().height);
	// Đặt vị trí của nhãn ở giữa màn hình
	loseLabel->setPosition(Vec2(0, 0));
	_uiLayer->addChild(loseLabel,1);
	// Bắt đầu với tỷ lệ 0
	loseLabel->setScale(0);

	// Hoạt ảnh thu phóng
	auto scaleAction = ScaleTo::create(1.0f, 0.2f); // Thu phóng đến 1 trong 1 giây
	loseLabel->runAction(scaleAction);

	// Trì hoãn trước khi chuyển sang MainMenuScene
	auto delayAction = DelayTime::create(3.0f); // Chờ 2 giây
	auto callFunc = CallFunc::create([&]() {
		// Chuyển sang MainMenuScene
		auto scene = MainMenuScene::create();
	TransitionFade* transition = TransitionFade::create(1.0f, scene);
	Director::getInstance()->replaceScene(transition);
		});

	// Chuỗi các hành động
	auto sequence = Sequence::create(delayAction, callFunc, nullptr);
	this->runAction(sequence);
}

