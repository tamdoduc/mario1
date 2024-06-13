#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "UILayer.h"
#include "Character.h"
#include "TileMap.h"
#include "Ground.h"
#include "EnemyManager.h"
#include "MainMenuScene.h"

class HelloWorld : public cocos2d::Scene
{
public:
    static HelloWorld* getInstance();
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    void menuCloseCallback(cocos2d::Ref* pSender);
    void initTouchEvent();
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void setPhysicWorld(cocos2d::PhysicsWorld* m_world);
    void OnCharacterDie(int HPLeft);
    void OnLose();
    void update(float dt);

    CREATE_FUNC(HelloWorld);

private:
    HelloWorld() {}
    static HelloWorld* instance;
    
    Character* _character;
    TileMap* _tileMap;
    UILayer* _uiLayer;
    cocos2d::PhysicsWorld* world;
    
    const int HPMax = 3;
};

#endif // __HELLOWORLD_SCENE_H__
