#include "Character.h"
#include "Bullet.h"

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

bool Character::init()
{
    if (!Sprite::init())
    {
        return false;
    }

    this->setTexture("player.png");

    int desiredWidth = 25;
    int desiredHeight = 25;

    this->setScale(desiredWidth / this->getContentSize().width, desiredHeight / this->getContentSize().height);

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    this->setPosition(Vec2(50, 100));

    PhysicsMaterial material;
    material.density = 0.01f;
    material.friction = 0.1f;
    material.restitution = 0.1f;

    auto characterBody = PhysicsBody::createBox(this->getContentSize(), material);
    characterBody->setDynamic(true);
    characterBody->setGravityEnable(true);
    characterBody->setMass(9.80f);
    characterBody->setRotationEnable(false);

    characterBody->setCategoryBitmask(0x01);
    characterBody->setCollisionBitmask(0x07);
    characterBody->setContactTestBitmask(0x07);

    this->setPhysicsBody(characterBody);

    isMove = false;
    isMoveLeft = false;
    isMoveRight = false;
    isJumping = false;
    direction = 1;
    jumpForce = 1500.0f;

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(Character::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    return true;
}

bool Character::onContactBegin(PhysicsContact& contact)
{
    CCLOG("Character Contact");
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();

    // Lấy tên thực sự của node A và node B
    std::string nodeNameA = nodeA ? typeid(*nodeA).name() : "Unknown";
    std::string nodeNameB = nodeB ? typeid(*nodeB).name() : "Unknown";

    // Hiển thị tên thực sự của node A và node B trong log
    CCLOG("Bullet Contact: Node A: (%s), Node B: (%s)", nodeNameA.c_str(), nodeNameB.c_str());

    // Kiểm tra xem va chạm có xảy ra với đối tượng Enemy hay không
    if (nodeNameA == "class Enemy" || nodeNameB == "class Enemy") // Giả sử Enemy có tag là 2
    {
        CCLOG("LOSE");
        //// if (nodeNameA == "class Enemy")
        //nodeA->removeFromParent();
        ////else 
        //nodeB->removeFromParent();

        // Loại bỏ viên đạn khi va chạm xảy ra với Enemy
        return true; // Chỉ định rằng va chạm đã được xử lý
    }

    isJumping = false;
    return true;
}

void Character::moveTo(const Vec2& position)
{
    this->setPosition(position);
}

void Character::moveDirection(const bool m_isMoveLeft)
{
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
}

void Character::update(float dt)
{
    if (isMove)
    {
        Vec2 currentPosition = this->getPosition();
        float movementSpeed = 25.0f;

        currentPosition.x += direction * movementSpeed * dt;

        this->setPosition(currentPosition);
    }
}

void Character::shoot()
{
    auto bullet = Bullet::create();
    bullet->setPosition(this->getPosition()+ cocos2d::Vec2(15*direction,0));
    this->getParent()->addChild(bullet);

    bullet->shoot(direction);
}

void Character::jump()
{
    CCLOG("isJumping %s", isJumping ? "true" : "false");
    if (!isJumping)
    {
        isJumping = true;
        this->getPhysicsBody()->applyImpulse(Vec2(0, jumpForce));
    }
}
