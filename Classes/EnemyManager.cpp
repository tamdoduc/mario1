#include "EnemyManager.h"

USING_NS_CC;

EnemyManager* EnemyManager::instance = nullptr;

EnemyManager* EnemyManager::getInstance()
{
    if (!instance)
    {
        instance = new EnemyManager();
    }
    return instance;
}

void EnemyManager::createEnemies(Scene* scene)
{
    int count = defaultPositions.size();
    for (int i = 0; i < count; ++i)
    {
        auto enemy = Enemy::create();
        enemy->setPos(defaultPositions[i]);
        scene->addChild(enemy);
    }
}
