#include "Bullet.h"
#include <typeinfo>
#include <Enemy.h>
#include <Box.h>
#include <Enemy2.h>
USING_NS_CC;

Bullet* Bullet::create()
{
	Bullet* pRet = new(std::nothrow) Bullet();
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

bool Bullet::init()
{
	if (!Sprite::init())
	{
		return false;
	}

	// Set the texture of the bullet
	this->setTexture("ice.png"); // Ensure you have a "bullet.png" file

	// Set the scale of the bullet
	int desiredWidth = 15;
	int desiredHeight = 15;
	this->setScale(desiredWidth / this->getContentSize().width, desiredHeight / this->getContentSize().height);

	// Set the speed of the bullet
	_speed = 300.0f; // Speed of the bullet

	// Set up physics body for collision detection
	auto bulletBody = PhysicsBody::createBox(this->getContentSize());
	bulletBody->setDynamic(false);
	bulletBody->setCategoryBitmask(0x08); // Category bitmask for bullet
	bulletBody->setCollisionBitmask(0x04); // Collision bitmask for bullet
	bulletBody->setContactTestBitmask(0x04); // Contact test bitmask for bullet
	this->setPhysicsBody(bulletBody);

	// Set up contact listener
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Bullet::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
	traveledDistance = 0;
	return true;
}

void Bullet::shoot(int direction)
{
	_direction = direction;
	this->scheduleUpdate(); // Schedule the update method to move the bullet
}

void Bullet::update(float dt)
{
	Vec2 currentPosition = this->getPosition();
	currentPosition.x += _direction * _speed * dt;
	traveledDistance +=  _speed * dt;
	this->setPosition(currentPosition);
	if (traveledDistance >= 250.0f)
	{
		this->removeFromParentAndCleanup(true);
	}
	// Remove the bullet if it goes off screen
	Size visibleSize = Director::getInstance()->getVisibleSize();
	if (currentPosition.x < 0 || currentPosition.x > visibleSize.width)
	{
		//this->removeFromParentAndCleanup(true);
	}
}

bool Bullet::onContactBegin(cocos2d::PhysicsContact& contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	if (nodeA != this && nodeB != this)
		return false;
	std::string nodeNameA = nodeA ? typeid(*nodeA).name() : "Unknown";
	std::string nodeNameB = nodeB ? typeid(*nodeB).name() : "Unknown";
	// Use dynamic casting to check if the nodes are of type Character
	Enemy* enemyA = dynamic_cast<Enemy*>(nodeA);
	Enemy* enemyB = dynamic_cast<Enemy*>(nodeB);

	// Check if either node is a Character
	if (enemyA || enemyB)
	{
		Enemy* enemy = enemyA ? enemyA : enemyB;
		if (enemy)
		{
			CCLOG("Ground Contact: Node A: (%s), Node B: (%s)", nodeNameA.c_str(), nodeNameB.c_str());
			enemy->removeFromParentAndCleanup(true);
		}
		return true;
	}
	Enemy2* enemy2A = dynamic_cast<Enemy2*>(nodeA);
	Enemy2* enemy2B = dynamic_cast<Enemy2*>(nodeB);

	// Check if either node is a Character
	if (enemy2A || enemy2B)
	{
		Enemy2* enemy2 = enemy2A ? enemy2A : enemy2B;
		if (enemy2)
		{
			CCLOG("Ground Contact: Node A: (%s), Node B: (%s)", nodeNameA.c_str(), nodeNameB.c_str());
			enemy2->removeFromParentAndCleanup(true);
		}
		return true;
	}
	Box* boxA = dynamic_cast<Box*>(nodeA);
	Box* boxB = dynamic_cast<Box*>(nodeB);

	// Check if either node is a Character
	if (boxA || boxB)
	{
		Box* box = boxA ? boxA : boxB;
		if (box)
		{
			CCLOG("Ground Contact: Node A: (%s), Node B: (%s)", nodeNameA.c_str(), nodeNameB.c_str());
			box->onDestruct();
		}
		return true;
	}

	return false; // Return false if the collision is not handled
}

