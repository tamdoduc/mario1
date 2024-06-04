#include "Bullet.h"
#include <typeinfo>
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
	traveledDistance += _direction * _speed * dt;
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
	// Lấy tên thực sự của node A và node B
	std::string nodeNameA = nodeA ? typeid(*nodeA).name() : "Unknown";
	std::string nodeNameB = nodeB ? typeid(*nodeB).name() : "Unknown";

	// Hiển thị tên thực sự của node A và node B trong log
	CCLOG("Bullet Contact: Node A: (%s), Node B: (%s)", nodeNameA.c_str(), nodeNameB.c_str());

	// Kiểm tra xem va chạm có xảy ra với đối tượng Enemy hay không
	if (nodeNameA == "class Enemy" || nodeNameB == "class Enemy") // Giả sử Enemy có tag là 2
	{
		CCLOG("REMOVE enemy");
		// if (nodeNameA == "class Enemy")
		nodeA->removeFromParent();
		//else 
		nodeB->removeFromParent();

		// Loại bỏ viên đạn khi va chạm xảy ra với Enemy
		return true; // Chỉ định rằng va chạm đã được xử lý
	}
	//if (nodeNameA != "class Character" && nodeNameB != "class Character") // Giả sử Enemy có tag là 2
	//{
	//	CCLOG("REMOVE enemy");
	//	if (nodeNameA == "class Bullet")
	//		nodeA->removeFromParent();
	//	else
	//		nodeB->removeFromParent();

	//	// Loại bỏ viên đạn khi va chạm xảy ra với Enemy
	//	return true; // Chỉ định rằng va chạm đã được xử lý
	//}

	return false; // Trả về false nếu va chạm không được xử lý
}

