#ifndef __ENEMY_MANAGER_H__
#define __ENEMY_MANAGER_H__

#include "cocos2d.h"
#include "Enemy.h"

class EnemyManager
{
public:
    static EnemyManager* getInstance();
    void createEnemies(cocos2d::Scene* scene);

private:
    EnemyManager() {}
    static EnemyManager* instance;
    const std::vector<cocos2d::Vec2> defaultPositions = { cocos2d::Vec2(500, 70), cocos2d::Vec2(900, 280) };
};

#endif // __ENEMY_MANAGER_H__
