#include "UILayer.h"

USING_NS_CC;

bool UILayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	// Lấy kích thước của màn hình
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	// Create a LayerColor with black color and alpha 204 (0.8 * 255)
	fadeLayer = LayerColor::create(Color4B(0, 0, 0, 204));
	fadeLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
	fadeLayer->setContentSize(visibleSize);
	fadeLayer->setPosition(origin - visibleSize / 2);
	fadeLayer->setVisible(false); // Initially hidden
	this->addChild(fadeLayer); // Add the layer to the current layer

	createControlButtons();
	//hideAllButton();
	hideAllTutorial();
	return true;
}

void UILayer::createControlButtons()
{
	// Get the visible size of the screen
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// Define button sizes and positions
	Vec2 leftButtonPos = Vec2(-visibleSize.width / 2 + 50, -visibleSize.height / 2 + 50);
	Vec2 rightButtonPos = Vec2(-visibleSize.width / 2 + 150, -visibleSize.height / 2 + 50);
	Vec2 shootButtonPos = Vec2(visibleSize.width / 2 - 150, -visibleSize.height / 2 + 50);
	Vec2 jumpButtonPos = Vec2(visibleSize.width / 2 - 50, -visibleSize.height / 2 + 50);

	leftButton = ui::Button::create("left.png", "left.png");
	leftButton->setPosition(leftButtonPos);
	leftButton->setScale(SCALE);
	leftButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType type) {
		this->onButtonTouchEvent(pSender, type, leftButtonCallback, leftButtonReleaseCallback);
		});
	this->addChild(leftButton);

	rightButton = ui::Button::create("right.png", "right.png");
	rightButton->setPosition(rightButtonPos);
	rightButton->setScale(SCALE);
	rightButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType type) {
		this->onButtonTouchEvent(pSender, type, rightButtonCallback, rightButtonReleaseCallback);
		});
	this->addChild(rightButton);

	shootButton = ui::Button::create("attack.png", "attack.png");
	shootButton->setPosition(shootButtonPos);
	shootButton->setScale(SCALE);
	shootButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED) {
			if (shootButtonCallback) shootButtonCallback();
		}
		});
	this->addChild(shootButton);

	jumpButton = ui::Button::create("up.png", "up.png");
	jumpButton->setPosition(jumpButtonPos);
	jumpButton->setScale(SCALE);
	jumpButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED) {
			if (jumpButtonCallback) jumpButtonCallback();
		}
		});
	this->addChild(jumpButton);

	int desiredWidth = 50;
	int desiredHeight = 50;

	leftHandSprite = Sprite::create("hand.png");
	leftHandSprite->setAnchorPoint(Vec2(0.5f, 0.2f));
	leftHandSprite->setPosition(Vec2(0, 0)); // Đặt vị trí mong muốn của bàn tay
	leftHandSprite->setScale(-1 * desiredWidth / leftHandSprite->getContentSize().width, desiredHeight / leftHandSprite->getContentSize().height);
	leftButton->addChild(leftHandSprite);
	leftLabel = Label::createWithTTF("Touch this button to MOVE LEFT", "fonts/Marker Felt.ttf", 24);
	leftLabel->setAnchorPoint(Vec2(0, -1));
	leftLabel->setPosition(0, 0);
	leftButton->addChild(leftLabel);

	rightHandSprite = Sprite::create("hand.png");
	rightHandSprite->setAnchorPoint(Vec2(0.5f, 0.2f));
	rightHandSprite->setPosition(Vec2(0, 0)); // Đặt vị trí mong muốn của bàn tay
	rightHandSprite->setScale(-1 * desiredWidth / rightHandSprite->getContentSize().width, desiredHeight / rightHandSprite->getContentSize().height);
	rightButton->addChild(rightHandSprite);
	rightLabel = Label::createWithTTF("Touch this button to MOVE RIGHT", "fonts/Marker Felt.ttf", 24);
	rightLabel->setAnchorPoint(Vec2(0, -1));
	rightLabel->setPosition(0, 0);
	rightButton->addChild(rightLabel);


	jumpHandSprite = Sprite::create("hand.png");
	jumpHandSprite->setAnchorPoint(Vec2(0.5f, 0.2f));
	jumpHandSprite->setPosition(Vec2(0, 0)); // Đặt vị trí mong muốn của bàn tay
	jumpHandSprite->setScale(-1 * desiredWidth / jumpHandSprite->getContentSize().width, desiredHeight / jumpHandSprite->getContentSize().height);
	jumpButton->addChild(jumpHandSprite);
	jumpLabel = Label::createWithTTF("OH! The obstructive terrain!!\nTouch this button to JUMP over it", "fonts/Marker Felt.ttf", 24);
	jumpLabel->setAnchorPoint(Vec2(1, -1));
	jumpLabel->setPosition(0, 0);
	jumpButton->addChild(jumpLabel);

	shootHandSprite = Sprite::create("hand.png");
	shootHandSprite->setAnchorPoint(Vec2(0.5f, 0.2f));
	shootHandSprite->setPosition(Vec2(0, 0)); // Đặt vị trí mong muốn của bàn tay
	shootHandSprite->setScale(-1 * desiredWidth / jumpHandSprite->getContentSize().width, desiredHeight / jumpHandSprite->getContentSize().height);
	shootButton->addChild(shootHandSprite);
	shootLabel = Label::createWithTTF("The BOX is getting in your way!!!\n Touch the shoot button to destroy it", "fonts/Marker Felt.ttf", 24);
	shootLabel->setAnchorPoint(Vec2(1, -1));
	shootLabel->setPosition(0, 0);
	shootButton->addChild(shootLabel);
}

void UILayer::initHPUI(int HPMax)
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
		heartsMissing.push_back(heartMissing);
		this->addChild(heartsMissing[i]);
		
		std::string fullHeartFile = "heart.png";
		auto heartFull = Sprite::create(fullHeartFile);
		heartFull->setPosition(holderHeartPos + Vec2(30 * i, 0));
		heartFull->setScale(desiredWidth / heartFull->getContentSize().width, desiredHeight / heartFull->getContentSize().height);
		heartFull->setVisible(true);
		heartsFull.push_back(heartFull);
		this->addChild(heartsFull[i]);

	}
}

void UILayer::renderHPUI(int HPleft, int HPMax)
{
	CCLOG("HP LEFT%i", HPleft);
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

void UILayer::setLeftButtonCallback(const std::function<void()>& callback)
{
	leftButtonCallback = callback;
}

void UILayer::setLeftButtonReleaseCallback(const std::function<void()>& callback)
{
	leftButtonReleaseCallback = callback;
}

void UILayer::setRightButtonCallback(const std::function<void()>& callback)
{
	rightButtonCallback = callback;
}

void UILayer::setRightButtonReleaseCallback(const std::function<void()>& callback)
{
	rightButtonReleaseCallback = callback;
}

void UILayer::setShootButtonCallback(const std::function<void()>& callback)
{
	shootButtonCallback = callback;
}

void UILayer::setJumpButtonCallback(const std::function<void()>& callback)
{
	jumpButtonCallback = callback;
}

void UILayer::onButtonTouchEvent(Ref* pSender, ui::Widget::TouchEventType type,
	const std::function<void()>& pressedCallback,
	const std::function<void()>& releasedCallback)
{
	if (type == ui::Widget::TouchEventType::BEGAN || type == ui::Widget::TouchEventType::MOVED)
	{
		if (pressedCallback) pressedCallback();
	}
	else if (type == ui::Widget::TouchEventType::ENDED || type == ui::Widget::TouchEventType::CANCELED)
	{
		if (releasedCallback) releasedCallback();
	}
}
void UILayer::hideAllButton()
{
	leftButton->setVisible(false);
	rightButton->setVisible(false);
	jumpButton->setVisible(false);
	shootButton->setVisible(false);
}
void UILayer::hideAllTutorial()
{
	leftHandSprite->setVisible(false);
	leftLabel->setVisible(false);
	rightHandSprite->setVisible(false);
	rightLabel->setVisible(false);
	jumpHandSprite->setVisible(false);
	jumpLabel->setVisible(false);
	shootHandSprite->setVisible(false);
	shootLabel->setVisible(false);
}
void UILayer::showButton(int buttonType)
{
	switch (buttonType)
	{
	case 0:
		leftButton->setVisible(true);
		break;
	case 1:
		rightButton->setVisible(true);
		break;
	case 2:
		jumpButton->setVisible(true);
		break;
	case 3:
		shootButton->setVisible(true);
		break;
	default:
		break;
	}
}
void UILayer::showHandAndLabel(int buttonType)
{
	switch (buttonType)
	{
	case 0:
		leftHandSprite->setVisible(true);
		leftLabel->setVisible(true);
		break;
	case 1:
		rightHandSprite->setVisible(true);
		rightLabel->setVisible(true);
		break;
	case 2:
		jumpHandSprite->setVisible(true);
		jumpLabel->setVisible(true);

		break;
	case 3:
		shootHandSprite->setVisible(true);
		shootLabel->setVisible(true);

		break;
	default:
		break;
	}
}
void UILayer::showFadeLayer(bool isShow)
{
	if (!isShow)
	{
		fadeLayer->setVisible(false);
	}
	else
	{
		if (!fadeLayer->isVisible())
		{

			CCLOG("Fade");
			fadeLayer->setVisible(true);
			fadeLayer->setOpacity(0); // Set initial opacity to 0
			auto fadeIn = FadeTo::create(2.0f, 204); // Create fade-in action
			fadeLayer->runAction(fadeIn); // Run the fade-in action
		}
	}
}