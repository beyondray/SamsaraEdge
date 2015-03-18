#pragma  once
#ifndef H_GAME_SCENE
#define H_GAME_SCENE

#include <cocos2d.h>

class GameScene:public cocos2d::Scene
{
public:
	virtual bool init();
	CREATE_FUNC(GameScene);
};


#endif