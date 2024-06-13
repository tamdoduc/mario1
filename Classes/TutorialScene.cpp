#include "TutorialScene.h"
#include "MainMenuScene.h"
#include "Box.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "platform/CCFileUtils.h"

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
	_character->setScale(_character->getScale() * 2);
	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto background = Sprite::create("Background_tutorial2.jpeg");
	background->setScale(3);
	background->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	this->addChild(background, -1);

	_tileMap = TileMap::create("map_tutorial.tmx");
	if (_tileMap)
	{
		_tileMap->setScale(2.5f);
		_tileMap->setPosition(Vec2(0, 0));
		this->addChild(_tileMap, 0);
		_tileMap->addColliders();
	}

	CCLOG(("Done tile map"));
	auto box = Box::create();
	box->setPos(Vec2(736*2.5, 48*2.5));
	box->setScale(box
		->getScale() * 2.5);
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

	// Tạo một LayerColor với màu đen và alpha 204 (0.8 * 255)
	auto fadeLayer = LayerColor::create(Color4B(0, 0, 0, 204));

	// Đặt độ trong suốt ban đầu là 0 để thực hiện hiệu ứng fade in
	fadeLayer->setOpacity(0);

	// Thêm layer vào scene
	this->addChild(fadeLayer, 100); // 100 là z-order, đảm bảo layer này ở trên cùng

	// Tạo hành động fade in trong 2 giây
	auto fadeIn = FadeTo::create(2.0f, 204); // 2.0f là thời gian thực hiện, 204 là giá trị alpha

	// Thực hiện hành động
	fadeLayer->runAction(fadeIn);

	// Ẩn tất cả các nút điều khiển ban đầu
	setControlButtonsVisible(false);
	// Tạo sprite bàn tay và text hướng dẫn cho nút phải, nhưng ẩn chúng ban đầu
	auto rightHandSprite = Sprite::create("hand.png");
	rightHandSprite->setPosition(Vec2(350, 25));
	rightHandSprite->setScale(0.5f);
	int desiredWidth = 50*(visibleSize.width/960.0f);
	int desiredHeight = 50 * (visibleSize.width / 960.0f);

	rightHandSprite->setScale(2*desiredWidth / rightHandSprite->getContentSize().width,2* desiredHeight / rightHandSprite->getContentSize().height);
	rightHandSprite->setVisible(false); // Ẩn ban đầu
	
	fadeLayer->addChild(rightHandSprite, 101);

	auto rightLabel = Label::createWithTTF("Touch this button to MOVE RIGHT", "fonts/Marker Felt.ttf", 24);
	rightLabel->setPosition(Vec2(200, 200));
	rightLabel->setVisible(false); // Ẩn ban đầu
	fadeLayer->addChild(rightLabel, 101);

	_rightHandSprite = rightHandSprite;
	_rightLabel = rightLabel;
	_fadeLayer = fadeLayer;

	
	// Tạo sprite bàn tay và text hướng dẫn cho nhảy
	//jumpHandSprite->setAnchorPoint(Vec2(0, 1));


	auto jumpButton = ui::Button::create("up.png", "up.png");
	jumpButton->setPosition(Vec2(visibleSize.width/2-75, -visibleSize.height/2+200));
	jumpButton->addTouchEventListener(CC_CALLBACK_2(Tutorial::menuJumpCallback, this));
	_jumpButton = jumpButton;
	jumpButton->setScale(-1 * desiredWidth / jumpButton->getContentSize().width, desiredHeight / jumpButton->getContentSize().height);

	_uiLayer->addChild(jumpButton);
	auto jumpHandSprite = Sprite::create("hand.png");
	jumpHandSprite->setPosition(Vec2(visibleSize.width  - 100,  70)); // Đặt vị trí mong muốn của bàn tay
	jumpHandSprite->setScale(2*-1 * desiredWidth / jumpHandSprite->getContentSize().width,2* desiredHeight / jumpHandSprite->getContentSize().height);

	_fadeLayer->addChild(jumpHandSprite, 101); // 101 là z-order, trên fadeLayer
	_jumpHandSprite = jumpHandSprite;
	auto jumpLabel = Label::createWithTTF("OH! The obstructive terrain!!\nTouch this button to JUMP over it", "fonts/Marker Felt.ttf", 24);
	//jumpLabel->setAnchorPoint(Vec2(0, 1));
	jumpLabel->setPosition(Vec2(visibleSize.width  - 250,  70)); // Đặt vị trí bên cạnh bàn tay
	_fadeLayer->addChild(jumpLabel, 101); // 101 là z-order, trên fadeLayer

	_jumpLabel = jumpLabel;

	_jumpButton->setVisible(false);
	_jumpHandSprite->setVisible(false);
	_jumpLabel->setVisible(false);


	// Tạo sprite bàn tay và text hướng dẫn cho nhảy
	auto shootHandSprite = Sprite::create("hand.png");
	shootHandSprite->setPosition(Vec2(visibleSize.width - 100, 20)); // Đặt vị trí mong muốn của bàn tay


	auto shootButton = ui::Button::create("attack.png", "attack.png");
	shootButton->addTouchEventListener(CC_CALLBACK_2(Tutorial::menuShootCallback, this));
	shootButton->setPosition(Vec2(visibleSize.width / 2 - 75, -visibleSize.height / 2 + 50));
	shootButton->setScale(-1 * desiredWidth / shootButton->getContentSize().width, desiredHeight / shootButton->getContentSize().height);
	_shootButton = shootButton;
	_uiLayer->addChild(shootButton);
	shootHandSprite->setScale(-1 * desiredWidth / shootHandSprite->getContentSize().width, desiredHeight / shootHandSprite->getContentSize().height);
	_shootHandSprite = shootHandSprite;
	_fadeLayer->addChild(shootHandSprite, 101); // 101 là z-order, trên fadeLayer

	auto shootLabel = Label::createWithTTF("The BOX is getting in your way!!!\n Touch the shoot button to destroy it", "fonts/Marker Felt.ttf", 24);
	shootLabel->setPosition(Vec2(visibleSize.width - 300, 30)); // Đặt vị trí bên cạnh bàn tay
	_fadeLayer->addChild(shootLabel, 101); // 101 là z-order, trên fadeLayer
	_shootLabel = shootLabel;

	_shootButton->setVisible(false);
	_shootLabel->setVisible(false);
	_shootHandSprite->setVisible(false);



	// Lên lịch để hiện nút phải sau 2 giây
	this->scheduleOnce([this](float) {
		showRightControlTutorial();
		}, 2.0f, "ShowRightControlTutorial");
	isFirstTouchLeft = true;
	isFirstTouchRight = true;

	door = Door::create();
	door->init();
	door->setPosition(Vec2(850 * 2.5, 48 * 2.5));
	//door->setPosition(Vec2( 150, 50));
	this->addChild(door);
	door->setScale(door->getScale() * 2.5);
		door->setActionCallback([this]() {
		// Define what happens when the character collides with the door
		this->OnWin();
		});


	this->scheduleUpdate();

	return true;
}

void Tutorial::showRightControlTutorial()
{
	// Hiện nút phải
	_rightButton->setVisible(true);

	// Hiện bàn tay và text hướng dẫn cho nút phải
	_rightHandSprite->setVisible(true);
	_rightLabel->setVisible(true);

	// Thêm listener cho nút phải để chuyển sang bước hướng dẫn tiếp theo khi người dùng bấm nút
	_rightButton->addTouchEventListener(CC_CALLBACK_2(Tutorial::onRightButtonPressed, this));
	//_uiLayer->removeChild(_rightButton,false);
	//_fadeLayer->addChild(_rightButton, 101);
	_rightButton->setLocalZOrder(102);
}

void Tutorial::onRightButtonPressed(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (isFirstTouchRight)
	{
		isFirstTouchRight = false;

		showLeftControlTutorial();
		// Chuyển sang bước hướng dẫn nút trái
	}
	if (type == ui::Widget::TouchEventType::BEGAN || type == ui::Widget::TouchEventType::MOVED)
	{
		_character->moveDirection(false);
	}
	else
	{
		_character->cancelMove();
	}
}

void Tutorial::showLeftControlTutorial()
{
	// Ẩn hướng dẫn nút phải
	_rightHandSprite->setVisible(false);
	_rightLabel->setVisible(false);

	// Hiện nút trái và text hướng dẫn
	auto leftHandSprite = Sprite::create("hand.png");
	leftHandSprite->setPosition(Vec2(150, 100));
	int desiredWidth = 50;
	int desiredHeight = 50;

	leftHandSprite->setScale(2*desiredWidth / leftHandSprite->getContentSize().width,2* desiredHeight / leftHandSprite->getContentSize().height);
	_fadeLayer->addChild(leftHandSprite, 101);

	auto leftLabel = Label::createWithTTF("Touch this button to MOVE LEFT", "fonts/Marker Felt.ttf", 24);
	leftLabel->setPosition(Vec2(140, 200));
	_fadeLayer->addChild(leftLabel, 101);

	_leftHandSprite = leftHandSprite;
	_leftLabel = leftLabel;

	// Hiện nút trái
	_leftButton->setVisible(true);

	// Thêm listener cho nút trái
	_leftButton->addTouchEventListener(CC_CALLBACK_2(Tutorial::onLeftButtonPressed, this));
}

void Tutorial::onLeftButtonPressed(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (isFirstTouchLeft)
	{
		isFirstTouchLeft = false;


		// Ẩn hướng dẫn nút trái
		_leftHandSprite->setVisible(false);
		_leftLabel->setVisible(false);

		// Có thể thêm các
		_fadeLayer->setVisible(false);

	}
	if (type == ui::Widget::TouchEventType::BEGAN || type == ui::Widget::TouchEventType::MOVED)
	{
		_character->moveDirection(true);
	}
	else
	{
		_character->cancelMove();
	}
}
void Tutorial::setControlButtonsVisible(bool visible)
{
	_leftButton->setVisible(visible);
	_rightButton->setVisible(visible);
	_shootButton->setVisible(visible);
	_jumpButton->setVisible(visible);
}

void Tutorial::createControlButtons()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 btnControllerLeft = Vec2(-visibleSize.width / 2, -visibleSize.height / 2);
	Vec2 btnControllerRight = Vec2(visibleSize.width / 2, -visibleSize.height / 2);

	int desiredWidth = 50 * (visibleSize.width / 960.0f);
	int desiredHeight = 50 * (visibleSize.width / 960.0f);
	_leftButton = ui::Button::create("left.png", "left.png");
	_leftButton->setPosition(btnControllerLeft + Vec2(50, 50));
	_leftButton->setScale(  desiredWidth / _leftButton->getContentSize().width, desiredHeight / _leftButton->getContentSize().height);
	
	_uiLayer->addChild(_leftButton);

	_rightButton = ui::Button::create("right.png", "right.png");
	_rightButton->setPosition(btnControllerLeft + Vec2(200, 50));
	_uiLayer->addChild(_rightButton);
	_rightButton->setScale(  desiredWidth / _rightButton->getContentSize().width, desiredHeight / _rightButton->getContentSize().height);

	_shootButton = ui::Button::create("attack.png", "attack.png");
	_shootButton->setPosition(btnControllerRight + Vec2(100, 100));
	_uiLayer->addChild(_shootButton);

	_jumpButton = ui::Button::create("up.png", "up.png");
	_jumpButton->setPosition(btnControllerRight + Vec2(100, 150));
	_uiLayer->addChild(_jumpButton);
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
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 holderHeartPos = Vec2(-visibleSize.width/2+10, 200);
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


void Tutorial::menuJumpCallback(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (isFirstTouchJump)
	{
		isFirstTouchJump = false;
		_jumpHandSprite->setVisible(false);
		_jumpLabel->setVisible(false);

		// Có thể thêm các
		_fadeLayer->setVisible(false);
	}
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		_character->jump();
	}
}

void Tutorial::menuShootCallback(Ref* pSender, ui::Widget::TouchEventType type)
{
	if (isFirstTouchShoot)
	{
		isFirstTouchShoot = false;
		_shootHandSprite->setVisible(false);
		_shootLabel->setVisible(false);

		// Có thể thêm các
		_fadeLayer->setVisible(false);
	}
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

		Size visibleSize = Director::getInstance()->getVisibleSize();
		float cameraHalfWidth = camera->getContentSize().width;
		float minXPosition = visibleSize.width/2;
		float maxXPosition = this->_tileMap->getMapSize().width*2 - visibleSize.width/2;

		float newCameraX = std::max(characterPosition.x, minXPosition);
		newCameraX = std::min(newCameraX, maxXPosition);
		camera->setPosition(cocos2d::Vec2(newCameraX, camera->getPosition().y));
		if (characterPosition.x >= 450*2.5 && isFirstTouchJump)
		{
			showJumpTutorial();
		}
		if (characterPosition.x >= 600*2.5 && isFirstTouchShoot)
		{
			showShootTutorial();
		}
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

void Tutorial::showJumpTutorial()
{
	CCLOG("SHOW JUMP");
	// Tạo hành động fade in trong 2 giây
	//_fadeLayer->setOpacity(0);
	auto fadeIn = FadeTo::create(2.0f, 204); // 2.0f là thời gian thực hiện, 204 là giá trị alpha
	// Thực hiện hành động
	_fadeLayer->setVisible(true);
	_fadeLayer->runAction(fadeIn);

	_jumpButton->setVisible(true);
	_jumpHandSprite->setVisible(true);
	_jumpLabel->setVisible(true);
}
void Tutorial::showShootTutorial()
{

	// Tạo hành động fade in trong 2 giây
	//_fadeLayer->setOpacity(0);
	auto fadeIn = FadeTo::create(2.0f, 204); // 2.0f là thời gian thực hiện, 204 là giá trị alpha
	// Thực hiện hành động
	_fadeLayer->setVisible(true);
	_fadeLayer->runAction(fadeIn);

	_shootButton->setVisible(true);
	_shootLabel->setVisible(true);
	_shootHandSprite->setVisible(true);


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