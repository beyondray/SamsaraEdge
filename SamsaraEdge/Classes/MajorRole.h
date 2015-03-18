#pragma once

#ifndef H_MAJORROLE
#define H_MAJORROLE

#include "MyAnimation.h"
#include "ConstValue.h"
#include "MenuLayer.h"

USING_NS_CC;

enum major_Action
{
	JUMP = 0,
	WAVE = 1,
	WAVEUP = 2,
	WAVEDOWN = 3,
	WAVERIGHT = 4,
	BACK = 5,
	DEAD = 6,
	REBIRTH = 7,
	WALK = 8,
};
class MajorRole :public cocos2d::Sprite, public MyAnimation
{
public:
	MajorRole(){};
	static MajorRole* create(const std::string& filename, const cocos2d::Rect& rect=cocos2d::Rect(0, 0, -1, -1));
	//创建各种动画
	cocos2d::Animate* createJumpAni(float delay, size_t loops){ return  cocos2d::Animate::create(createAnimate(MAJORROLE_JUMP_FNAME, MAJOR_JUMP_PICS, 167, 327, 1, MAJOR_JUMP_PICS, delay, loops)); }
	cocos2d::Animate* createWaveAni(float delay, size_t loops){ return  cocos2d::Animate::create(createAnimate(MAJORROLE_WAVE_FNAME, MAJOR_WAVE_PICS, 290, 185, 1, MAJOR_WAVE_PICS, delay, loops)); }
	cocos2d::Animate* createBackAni(float delay, size_t loops){ return cocos2d::Animate::create(createAnimate(MAJORROLE_BACK_FNAME, MAJOR_BACK_PICS, 152, 177, 1, MAJOR_BACK_PICS, delay, loops)); }
	cocos2d::Animate* createWaveUpAni(float delay, size_t loops){ return  cocos2d::Animate::create(createAnimate(MAJORROLE_WAVEUP_FNAME, MAJOR_WAVE_PICS, 252, 205, 1, MAJOR_WAVEUP_PICS, delay, loops)); }
	cocos2d::Animate* createWaveDownAni(float delay, size_t loops){ return  cocos2d::Animate::create(createAnimate(MAJORROLE_WAVEDOWN_FNAME, MAJOR_WAVE_PICS, 197, 187, 1, MAJOR_WAVEDOWN_PICS, delay, loops)); }
	cocos2d::Animate* createWaveRightAni(float delay, size_t loops){ return  cocos2d::Animate::create(createAnimate(MAJORROLE_WAVERIGHT_FNAME, MAJOR_WAVE_PICS, 265, 142, 1, MAJOR_WAVERIGHT_PICS, delay, loops)); }
	cocos2d::Animate* createDeadAni(float delay, size_t loops){ return cocos2d::Animate::create(createAnimate(MAJORROLE_DEAD_FNAME, MAJOR_DEAD_PICS, 190, 165, 1, MAJOR_DEAD_PICS, delay, loops)); }
	cocos2d::Animate* createRebirthAni(float delay, size_t loops){ return cocos2d::Animate::create(createAnimate(MAJORROLE_REBIRTH_FNAME, MAJOR_REBIRTH_PICS, 195, 157, 1, MAJOR_REBIRTH_PICS, delay, loops)); }
	cocos2d::Animate* createExciteAni(float delay, size_t loops){ return cocos2d::Animate::create(createAnimate(MAJORROLE_EXCITE_FNAME, MAJOR_EXCITE_PICS, 140, 195, 1, MAJOR_EXCITE_PICS, delay, loops)); }
	cocos2d::Animate* createWalkAni(float delay, size_t loops){ return  cocos2d::Animate::create(createAnimate(MAJORROLE_WALK_FNAME, MAJOR_WALK_PICS, 141, 167, 1, MAJOR_WALK_PICS, delay, loops)); }
	//做某些动作，再跳跃到一个节点
	cocos2d::Spawn* doSthAndMoveTo(major_Action sth, cocos2d::Vec2 worldPos, float duration);
	
	//做某些动作，跳跃到一个节点并重复走动
	cocos2d::Sequence* doSthMovetoThenWalk(major_Action sth, cocos2d::Vec2 worldPos, float moveDuration);

	//重复走动(CALL_BACK)
	void  RepeatWalk_CALL()
	{
		auto repeatBackWalk = RepeatForever::create(Spawn::create(createWalkAni(0.1f, 1), MoveBy::create(1, Vec2(-BK_MOVE_AVESECOND_SPEED, 0)), nullptr));
		this->runAction(repeatBackWalk);
	}	
	//随着背景一直向后移动
	void alwaysRepeatBack_CALL()
	{
		auto repeatBack = RepeatForever::create(MoveBy::create(1, Vec2(-BK_MOVE_AVESECOND_SPEED, 0)));
		this->runAction(repeatBack);
	}
	//移动到一个节点并不停行走(带背景回退）
	void MovingToNodeThenWalk(major_Action sth, cocos2d::Vec2 tarWorldPos, float moveDuration);
	//背景回退（带游戏结束函数调用）
	void RepeatBack_CALL(bool* magicPushLock);
	//移动到一个节点(带背景回退函数调用）
	void MovingToNode(major_Action sth, cocos2d::Vec2 tarWorldPos, float moveDuration, bool* magicPushLock);
};

#endif