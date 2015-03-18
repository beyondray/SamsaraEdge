#pragma once
#ifndef H_PREGAME_SCENE
#define H_PREGAME_SCENE

#include<cocos2d.h>
#include "MajorRole.h"


class PreGameScene: public cocos2d::Scene
{
public:
	virtual bool init() override;
	CREATE_FUNC(PreGameScene);
};

class PreGameLayer: public cocos2d::Layer
{
public:
	virtual bool init() override;
	CREATE_FUNC(PreGameLayer);
	//将场景作为其友元类
	friend class PreGameScene;
	
private:
	void WALK_CALL(MajorRole* majorRole)
	{
		majorRole->runAction(RepeatForever::create(majorRole->createWalkAni(0.1f, 1)));
	}
	void menuStartCallBack(cocos2d::Ref* pSender);
	void menuOptionsCallBack(cocos2d::Ref* pSender);
	void menuQuitCallBack(cocos2d::Ref* pSender);

private:
	cocos2d::Size m_GLViewSize;
	cocos2d::Menu* m_Menu;
};
#endif