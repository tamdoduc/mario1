#include "Character.h"
#include "HelloWorldScene.h"
#include "TutorialScene.h"

USING_NS_CC;

Character* Character::create()
{
	Character* pRet = new(std::nothrow) Character();

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
void Character::setScene(HelloWorld* helloWorldScene)
{
	this->helloWorldScene = helloWorldScene;
}
void Character::setScene(Tutorial* tutorialScene)
{
	this->tutorialScene = tutorialScene;
}
bool Character::init()
{
	if (!Sprite::init())
	{
		return false;
	}

	this->setTexture("Idle_2.png");

	int desiredWidth = 50;
	int desiredHeight = 50;
	this->setScale(desiredWidth / this->getContentSize().width, desiredHeight / this->getContentSize().height);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	startPos = Vec2(50, 70);
	this->setPosition(startPos);
	PhysicsMaterial material;
	//material.density = 0.01f;
	//material.friction = 0.1f;
	//material.restitution = 0.1f;

	characterBody = PhysicsBody::createBox(this->getContentSize(), material);
	characterBody->setDynamic(true);
	characterBody->setGravityEnable(true);
	characterBody->setMass(9.80f);
	characterBody->setRotationEnable(false);

	characterBody->setCategoryBitmask(0x02);
	characterBody->setCollisionBitmask(0x05);
	characterBody->setContactTestBitmask(0x05);

	this->setPhysicsBody(characterBody);

	isMove = false;
	isMoveLeft = false;
	isMoveRight = false;
	isJumping = false;
	direction = 1;
	jumpForce = 1200;

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Character::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	this->scheduleUpdate();

	createIdleAnimation(); // Create the walk animation
	createJumpAnimation(); // Create the walk animation
	createRunAnimation(); // Create the walk animation
	createDieAnimation(); // Create the walk animation
	createAppearAnimation();
	//jumpAnimate->setVisible(true);
	//jumpAnimate->start();


	ReSpawn(startPos);

	coolDown = ATTACKTIME;
	return true;
}
void Character::ReSpawn(cocos2d::Vec2 pos)
{
	appearAnimate->setVisible(true);
	appearAnimate->start();
	isPlaying = false;
	this->setPosition(pos);
	this->scheduleOnce([this](float) {
		isPlaying = true;
	appearAnimate->setVisible(false);
	appearAnimate->stop();
	idleAnimate->setVisible(true);
	idleAnimate->start();
		}, 1.05f, "appearing");
}
void Character::createAppearAnimation()
{
	std::vector<std::string> frameNames;
	for (int i = 1; i <= 7; ++i) // Replace N with the number of frames
	{
		frameNames.push_back(StringUtils::format("Appearing_%d.png", i));
	}

	appearAnimate = CustomAnimation::create(frameNames, 0.15f);
	this->addChild(appearAnimate);
	appearAnimate->setPosition(this->getContentSize() / 2);
	appearAnimate->setVisible(false);
	//idleAnimate->start();
	//// ẩn hình hiện tại
	this->setTextureCoords(cocos2d::Rect());
}
void Character::createIdleAnimation()
{
	std::vector<std::string> frameNames;
	for (int i = 1; i <= 11; ++i) // Replace N with the number of frames
	{
		frameNames.push_back(StringUtils::format("Idle_%d.png", i));
	}

	idleAnimate = CustomAnimation::create(frameNames, 0.1f);
    if (idleAnimate == nullptr) {
        cocos2d::log("Failed to create idle animation");
        return;
    }
	this->addChild(idleAnimate);
	idleAnimate->setPosition(this->getContentSize() / 2);
	idleAnimate->setVisible(false);
	//idleAnimate->start();
	//// ẩn hình hiện tại
	this->setTextureCoords(cocos2d::Rect());
}
void Character::createJumpAnimation()
{
	std::vector<std::string> frameNames;
	for (int i = 1; i <= 1; ++i) // Replace N with the number of frames
	{
		frameNames.push_back(StringUtils::format("Jump_%d.png", i));
	}

	jumpAnimate = CustomAnimation::create(frameNames, 0.1f);
	this->addChild(jumpAnimate);
	jumpAnimate->setPosition(this->getContentSize() / 2);
	jumpAnimate->setVisible(false);
	//jumpAnimate->start();
	//// ẩn hình hiện tại
   // this->setTextureCoords(cocos2d::Rect());
}
void Character::createRunAnimation()
{
	std::vector<std::string> frameNames;
	for (int i = 1; i <= 12; ++i) // Replace N with the number of frames
	{
		frameNames.push_back(StringUtils::format("Run_%d.png", i));
	}

	runAnimate = CustomAnimation::create(frameNames, 0.1f);
	this->addChild(runAnimate);
	runAnimate->setPosition(this->getContentSize() / 2);
	runAnimate->setVisible(false);
	//runAnimate->start();
	//// ẩn hình hiện tại
	//this->setTextureCoords(cocos2d::Rect());
}
void Character::createDieAnimation()
{
	std::vector<std::string> frameNames;
	for (int i = 1; i <= 7; ++i) // Replace N with the number of frames
	{
		frameNames.push_back(StringUtils::format("Hit_%d.png", i));
	}

	dieAnimate = CustomAnimation::create(frameNames, 0.3f);
	this->addChild(dieAnimate);
	dieAnimate->setPosition(this->getContentSize() / 2);
	dieAnimate->setVisible(false);
	//idleAnimate->start();
	//// ẩn hình hiện tại
	this->setTextureCoords(cocos2d::Rect());
}
void Character::moveDirection(const bool m_isMoveLeft)
{
	if (!isPlaying)
		return;
	if (!isMove && !isJumping)
	{
		CancelAllAnimation();

		runAnimate->setVisible(true);
		runAnimate->start();
	}
	isMove = true;
	isMoveLeft = m_isMoveLeft;
	isMoveRight = !m_isMoveLeft;
	direction = isMoveLeft ? -1 : 1;

	this->setScaleX(direction * fabs(this->getScaleX()));

}

void Character::cancelMove()
{
	isMove = false;
	isMoveLeft = false;
	isMoveRight = false;


	this->stopAllActions(); // Stop the walk animation
	CancelAllAnimation();
	if (isPlaying)
	{
		idleAnimate->setVisible(true);
		idleAnimate->start();
	}
}

void Character::update(float dt)
{
	if (!isPlaying)
		return;
	if (isMove)
	{
		Vec2 currentPosition = this->getPosition();
		float movementSpeed = 50.0f;

		currentPosition.x += direction * movementSpeed * dt;

		this->setPosition(currentPosition);

	}
	if (coolDown < ATTACKTIME)
		coolDown += dt;
}

void Character::shoot()
{
	if (!isPlaying)
		return;
	if (coolDown < ATTACKTIME)
		return;
	coolDown = 0;
	auto bullet = Bullet::create();
	bullet->setPosition(this->getPosition() + cocos2d::Vec2(15 * direction, 0));
	this->getParent()->addChild(bullet);

	bullet->shoot(direction);
}

void Character::jump()
{
	if (!isPlaying)
		return;
	CCLOG("isJumping %s", isJumping ? "true" : "false");
	if (!isJumping)
	{
		isJumping = true;
		this->getPhysicsBody()->applyImpulse(Vec2(0, jumpForce));

		runAnimate->setVisible(false);
		runAnimate->stop();
		idleAnimate->setVisible(false);
		idleAnimate->stop();

		jumpAnimate->setVisible(true);
		jumpAnimate->start();
	}
}

bool Character::onContactBegin(PhysicsContact& contact)
{

	if (!isPlaying)
		return true;
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();
	if (nodeA != this && nodeB != this)
		return false;
	std::string nodeNameA = nodeA ? typeid(*nodeA).name() : "Unknown";
	std::string nodeNameB = nodeB ? typeid(*nodeB).name() : "Unknown";
	// Use dynamic casting to check if the nodes are of type Character
	Enemy* enemyA = dynamic_cast<Enemy*>(nodeA);
	Enemy* enemyB = dynamic_cast<Enemy*>(nodeB);
	isJumping = false;
	// Check if either node is a Character
	if (enemyA || enemyB)
	{
		Enemy* enemy = enemyA ? enemyA : enemyB;
		if (enemy)
		{
			CCLOG("Ground Contact: Node A: (%s), Node B: (%s)", nodeNameA.c_str(), nodeNameB.c_str());
			this->OnLostHP();
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
			this->OnLostHP();
		}
		return true;
	}


	return true;
}

void Character::OnLostHP()
{
	if (!isPlaying)
		return;
	HP--;
	helloWorldScene->OnCharacterDie(HP);
	OnDie();
}
void Character::OnDie()
{
	// Animation Die

	isPlaying = false;
	CancelAllAnimation();
	dieAnimate->setVisible(true);
	dieAnimate->start();

	characterBody->setEnabled(false);

	// Schedule a method to reset the character's position after 2 seconds
	this->scheduleOnce([this](float) {
		if (HP > 0)
		OnRevive();
		else
		{
			helloWorldScene->OnLose();
		}
		}, 3.0f, "reset_position_key");
}
void Character::OnRevive()
{
	cancelMove();
	CancelAllAnimation();
	characterBody->setEnabled(true);
	ReSpawn(startPos);
}
void Character::CancelAllAnimation()
{
	idleAnimate->setVisible(false);
	idleAnimate->stop();
	jumpAnimate->setVisible(false);
	jumpAnimate->stop();
	runAnimate->setVisible(false);
	runAnimate->stop();
	dieAnimate->setVisible(false);
	dieAnimate->stop();
	appearAnimate->setVisible(false);
	appearAnimate->stop();
}
