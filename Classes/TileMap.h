#ifndef __TILEMAP_H__
#define __TILEMAP_H__

#include "cocos2d.h"

class TileMap : public cocos2d::Node
{
public:
    static TileMap* create(const std::string& tmxFile);
    virtual bool init(const std::string& tmxFile);
    void addColliders();

private:
    cocos2d::TMXTiledMap* _tileMap;
};

#endif // __TILEMAP_H__
