#include "BulletE.h"
#include <typeinfo>
#include <Enemy.h>
#include <Box.h>
#include <Character.h>
USING_NS_CC;

BulletE* BulletE::create()
{
	BulletE* pRet = new(std::nothrow) BulletE();
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

bool BulletE::init()
{
	if (!Sprite::init())
	{
		return false;
	}

	// Set the texture of the bullet
	this->setTexture("bulletE.png"); // Ensure you have a "bullet.png" file

	// Set the scale of the bullet
	int desiredWidth = 15;
	int desiredHeight = 15;
	this->setScale(desiredWidth / this->getContentSize().width, desiredHeight / this->getContentSize().height);

	// Set the speed of the bullet
	_speed = 300.0f; // Speed of the bullet

	// Set up physics body for collision detection
	auto bulletBody = PhysicsBody::createBox(this->getContentSize());
	bulletBody->setDynamic(false);
	bulletBody->setCategoryBitmask(0x01); // Category bitmask for bullet
	bulletBody->setCollisionBitmask(0x02); // Collision bitmask for bullet
	bulletBody->setContactTestBitmask(0x02); // Contact test bitmask for bullet
	this->setPhysicsBody(bulletBody);

	// Set up contact listener
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(BulletE::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
	traveledDistance = 0;
	CCLOG("INIT");
	return true;
}

void BulletE::shoot(int direction)
{
	CCLOG("SHOOTEEEE");
	_direction = direction;
	this->scheduleUpdate(); // Schedule the update method to move the bullet
}

void BulletE::update(float dt)
{
	Vec2 currentPosition = this->getPosition();
	currentPosition.x += _direction * _speed * dt;
	traveledDistance += _speed * dt;
	this->setPosition(currentPosition);
	if (traveledDistance >= 150.0f)
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

bool BulletE::onContactBegin(cocos2d::PhysicsContact& contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	if (nodeA != this && nodeB != this)
		return false;
	std::string nodeNameA = nodeA ? typeid(*nodeA).name() : "Unknown";
	std::string nodeNameB = nodeB ? typeid(*nodeB).name() : "Unknown";
	// Use dynamic casting to check if the nodes are of type Character
	Character* characterA = dynamic_cast<Character*>(nodeA);
	Character* characterB = dynamic_cast<Character*>(nodeB);

	// Check if either node is a Character
	if (characterA || characterB)
	{
		Character* character = characterA ? characterA : characterB;
		if (character)
		{
			CCLOG("Bullet2 Contact: Node A: (%s), Node B: (%s)", nodeNameA.c_str(), nodeNameB.c_str());
			character->OnLostHP();
		}
		return true;
	}
	return false; // Return false if the collision is not handled
}

