#include "ElevatorManager.h"

USING_NS_CC;

ElevatorManager* ElevatorManager::instance = nullptr;

ElevatorManager* ElevatorManager::getInstance()
{
    if (!instance)
    {
        instance = new ElevatorManager();
    }
    return instance;
}

void ElevatorManager::createElevators(Scene* scene, int level)
{
	auto defaultPositions = defaultPositions1;
	switch (level)
	{
	case 1:
		defaultPositions = defaultPositions1;
		break;
	case 2:
		defaultPositions = defaultPositions2;
		break;
	case 3:
		defaultPositions = defaultPositions3;
		break;

	}


	int count = defaultPositions.size();
	for (int i = 0; i < count; ++i)
	{
		auto enemy = Elevator::create();
		enemy->setPos(defaultPositions[i]);
		scene->addChild(enemy);
	}
}
