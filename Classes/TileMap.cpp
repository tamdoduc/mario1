#include "TileMap.h"

USING_NS_CC;

TileMap* TileMap::create(const std::string& tmxFile)
{
    TileMap* ret = new (std::nothrow) TileMap();
    if (ret && ret->init(tmxFile))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        delete ret;
        return nullptr;
    }
}

bool TileMap::init(const std::string& tmxFile)
{
    if (!Node::init())
    {
        return false;
    }

    // Tải tệp TMX
    _tileMap = TMXTiledMap::create(tmxFile);
    if (_tileMap)
    {
        this->addChild(_tileMap);
        this->addColliders();
    }
    else
    {
        return false;
    }

    return true;
}

void TileMap::addColliders()
{
    // Giả sử lớp đối tượng tên là "Colliders"
    auto objectGroup = _tileMap->getObjectGroup("Colliders");
    CCLOG("objectGroup1");
    if (objectGroup)
    {
        CCLOG("objectGroup2");
        auto& objects = objectGroup->getObjects();
        for (auto& obj : objects)
        {
            ValueMap& dict = obj.asValueMap();
            float x = dict["x"].asFloat();
            float y = dict["y"].asFloat();
            float width = dict["width"].asFloat();
            float height = dict["height"].asFloat();

            // Tạo PhysicsBody cho collider
            auto collider = PhysicsBody::createBox(Size(width, height), PhysicsMaterial(0.1f, 1.0f, 0.0f));
            collider->setDynamic(false);
            collider->setCategoryBitmask(0x01);
            collider->setCollisionBitmask(0x01);
            collider->setContactTestBitmask(0x01);
            CCLOG("aaa %f",width);
            // Tạo node đại diện cho collider và thêm vào tilemap
            auto node = Node::create();
            node->setPosition(Vec2(x + width / 2, y + height / 2));
            node->setPhysicsBody(collider);
            this->addChild(node);
        }
    }
}

