#include "UILayer.h"

USING_NS_CC;

bool UILayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	return true;
}

float UILayer::calculateScaleFactor()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	// Assuming a base size of 1280x720 for scaling
	float baseWidth = 1280.0f;
	float baseHeight = 720.0f;
	float scaleX = visibleSize.width / baseWidth;
	float scaleY = visibleSize.height / baseHeight;
	return std::min(scaleX, scaleY);
}

void UILayer::createControlButtons()
{
	// Get the visible size of the screen
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	float scaleFactor = calculateScaleFactor();

	// Define button sizes and positions
	Vec2 leftButtonPos = Vec2(-visibleSize.width / 2 + 50, -visibleSize.height / 2 + 50);
	Vec2 rightButtonPos = Vec2(-visibleSize.width / 2 + 150, -visibleSize.height / 2 + 50);
	Vec2 shootButtonPos = Vec2(visibleSize.width / 2 - 150, -visibleSize.height / 2 + 50);
	Vec2 jumpButtonPos = Vec2(visibleSize.width / 2 - 50, -visibleSize.height / 2 + 50);

	auto leftButton = ui::Button::create("left.png", "left.png");
	leftButton->setPosition(leftButtonPos);
	leftButton->setScale(scaleFactor);
	leftButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType type) {
		this->onButtonTouchEvent(pSender, type, leftButtonCallback, leftButtonReleaseCallback);
		});
	this->addChild(leftButton);

	auto rightButton = ui::Button::create("right.png", "right.png");
	rightButton->setPosition(rightButtonPos);
	rightButton->setScale(scaleFactor);
	rightButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType type) {
		this->onButtonTouchEvent(pSender, type, rightButtonCallback, rightButtonReleaseCallback);
		});
	this->addChild(rightButton);

	auto shootButton = ui::Button::create("attack.png", "attack.png");
	shootButton->setPosition(shootButtonPos);
	shootButton->setScale(scaleFactor);
	shootButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED) {
			if (shootButtonCallback) shootButtonCallback();
		}
		});
	this->addChild(shootButton);

	auto jumpButton = ui::Button::create("up.png", "up.png");
	jumpButton->setPosition(jumpButtonPos);
	jumpButton->setScale(scaleFactor);
	jumpButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED) {
			if (jumpButtonCallback) jumpButtonCallback();
		}
		});
	this->addChild(jumpButton);

	int desiredWidth = 50;
	int desiredHeight = 50;


	auto jumpHandSprite = Sprite::create("hand.png");
	jumpHandSprite->setAnchorPoint(Vec2(0.5f, 0));
	jumpHandSprite->setPosition(Vec2(0, 0)); // Đặt vị trí mong muốn của bàn tay
	jumpHandSprite->setScale(-1*desiredWidth / jumpHandSprite->getContentSize().width, desiredHeight / jumpHandSprite->getContentSize().height);
	jumpButton->addChild(jumpHandSprite);
	auto jumpLabel = Label::createWithTTF("OH! The obstructive terrain!!\nTouch this button to JUMP over it", "fonts/Marker Felt.ttf", 24);
	jumpLabel->setAnchorPoint(Vec2(1, -1));
	jumpLabel->setPosition(0,0);
	jumpButton->addChild(jumpLabel);
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
		this->addChild(heartMissing);
		heartsMissing.push_back(heartMissing);

		std::string fullHeartFile = "heart.png";
		auto heartFull = Sprite::create(fullHeartFile);
		heartFull->setPosition(holderHeartPos + Vec2(30 * i, 0));
		heartFull->setScale(desiredWidth / heartFull->getContentSize().width, desiredHeight / heartFull->getContentSize().height);
		heartFull->setVisible(true);
		this->addChild(heartFull);
		heartsFull.push_back(heartFull);
	}
}

void UILayer::renderHPUI(int HPleft, int HPMax)
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
