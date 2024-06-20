#include "Enemy2.h"
#include <Character.h>
#include <BulletE.h>

USING_NS_CC;

Enemy2* Enemy2::create()
{
	Enemy2* pRet = new(std::nothrow) Enemy2();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		return nullptr;
	}
}

bool Enemy2::init()
{
	if (!Sprite::initWithFile("E_Walk_1.png"))
	{
		return false;
	}

	int desiredWidth = 50;
	int desiredHeight = 50;
	float scaleFator = 1.2f;
	this->setScale(scaleFator * desiredWidth / this->getContentSize().width, scaleFator * desiredHeight / this->getContentSize().height);

	// Set default values
	isMovingLeft = true;
	moveSpeed = 25.0f; // Adjust move speed as needed
	moveRange = 160.0f; // Adjust move range as needed
	originalX = this->getPositionX();

	// Add physics body
	enemyBody = PhysicsBody::createBox(this->getContentSize());
	enemyBody->setDynamic(true);
	enemyBody->setCategoryBitmask(0x04);
	enemyBody->setCollisionBitmask(0x0B);
	enemyBody->setContactTestBitmask(0x0B);
	enemyBody->setGravityEnable(false);
	this->setPhysicsBody(enemyBody);

	// Set up contact listener

	createRunAnimation();

	this->scheduleUpdate();
	this->schedule(CC_SCHEDULE_SELECTOR(Enemy2::shootBullet), 2.0f);

	return true;
}

void Enemy2::setPos(cocos2d::Vec2 newPos)
{
	this->setPosition(newPos);
	originalX = newPos.x;
}

void Enemy2::update(float dt)
{
	if (isMovingLeft)
	{
		this->moveLeft();
		if (this->getPositionX() <= originalX - moveRange)
		{
			isMovingLeft = false;
		}
	}
	else
	{
		this->moveRight();
		if (this->getPositionX() >= originalX)
		{
			isMovingLeft = true;
		}
	}
}

void Enemy2::moveLeft()
{
	this->setPositionX(this->getPositionX() - moveSpeed * Director::getInstance()->getDeltaTime());
	this->setScaleX(1 * fabs(this->getScaleX()));
}

void Enemy2::moveRight()
{
	this->setPositionX(this->getPositionX() + moveSpeed * Director::getInstance()->getDeltaTime());
	this->setScaleX(-1 * fabs(this->getScaleX()));

}


void Enemy2::createRunAnimation()
{
	std::vector<std::string> frameNames;
	for (int i = 1; i <= 12; ++i) // Replace N with the number of frames
	{
		frameNames.push_back(StringUtils::format("E_Walk_%d.png", i));
	}

	runAnimate = CustomAnimation::create(frameNames, 0.1f);
	this->addChild(runAnimate);
	runAnimate->setPosition(this->getContentSize() / 2);
	runAnimate->setVisible(true);
	runAnimate->start();
	//// ẩn hình hiện tại
	this->setTextureCoords(cocos2d::Rect());
}
void Enemy2::shootBullet(float dt)
{
	auto bullet = BulletE::create();
	bullet->init();
	bullet->setPosition(this->getPosition());
	bullet->shoot(isMovingLeft ? -1 : 1);
	this->getParent()->addChild(bullet);

}
bool Enemy2::onContactBegin(cocos2d::PhysicsContact& contact)
{
	

	return false;
}