#ifndef __ELEVATOR_MANAGER_H__
#define __ELEVATOR_MANAGER_H__

#include "cocos2d.h"
#include "Elevator.h"

class ElevatorManager
{
public:
    static ElevatorManager* getInstance();

    void createElevators(cocos2d::Scene* scene,int level);


private:
    ElevatorManager() = default;
    ~ElevatorManager() = default;

    static ElevatorManager* instance;

    std::vector<cocos2d::Vec2> defaultPositions1{};
    std::vector<cocos2d::Vec2> defaultPositions2{ cocos2d::Vec2(880, 50), cocos2d::Vec2(80, 300), cocos2d::Vec2(880, 600), cocos2d::Vec2(80, 800) };
    std::vector<cocos2d::Vec2> defaultPositions3;
};

#endif // __ELEVATOR_MANAGER_H__
