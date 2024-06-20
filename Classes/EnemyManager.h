#ifndef __ENEMY_MANAGER_H__
#define __ENEMY_MANAGER_H__

#include "cocos2d.h"
#include "Enemy.h"
#include "Enemy2.h"

class EnemyManager
{
public:
    static EnemyManager* getInstance();
    void createEnemies(cocos2d::Scene* scene, int level);

private:
    EnemyManager() {}
    static EnemyManager* instance;
    const std::vector<cocos2d::Vec2> defaultPositions1 = { cocos2d::Vec2(500, 70), cocos2d::Vec2(900, 280), cocos2d::Vec2(1200, 280), cocos2d::Vec2(2000, 280) };
    const std::vector<cocos2d::Vec2> defaultPositions2 = { cocos2d::Vec2(540, 50), cocos2d::Vec2(440, 300), cocos2d::Vec2(540, 550), cocos2d::Vec2(440, 800), cocos2d::Vec2(540, 1050) };
    const std::vector<cocos2d::Vec2> defaultPositions3 = { };
};

#endif // __ENEMY_MANAGER_H__
