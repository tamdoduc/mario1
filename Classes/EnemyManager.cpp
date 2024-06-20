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

void EnemyManager::createEnemies(Scene* scene, int level)
{
	auto defaultPositions = defaultPositions1;
	switch (level)
	{
	case 1:
		CCLOG("ENEMY LEVEL1");
		defaultPositions = defaultPositions1;
		break;
	case 2:
		CCLOG("ENEMY LEVEL2");
		defaultPositions = defaultPositions2;
		break;
	case 3:
		CCLOG("ENEMY LEVEL3");
		defaultPositions = defaultPositions3;
		break;

	}


	int count = defaultPositions.size();
	for (int i = 0; i < count; ++i)
	{
		auto enemy = Enemy::create();
		auto enemy2 = Enemy2::create();

		switch (level)
		{
		case 1:

			enemy->setPos(defaultPositions[i]);
			scene->addChild(enemy);
			break;
		case 2:
			enemy2->setPos(defaultPositions[i]);
			scene->addChild(enemy2);
			break;
		case 3:
			CCLOG("ENEMY LEVEL3");
			defaultPositions = defaultPositions3;
			break;

		}
		
	}
}
