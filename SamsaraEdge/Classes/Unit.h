#pragma once

#include "MyAnimation.h"

/*
**【CREATE_UNIT宏】
** Desc:根据图片文件名及区域创建一个类型对象
*/
#define CREATE_UNIT(__TYPE__) \
	static __TYPE__* create(const std::string& __FNAME__, \
	const cocos2d::Rect& __RECT__ = cocos2d::Rect(0, 0, -1, -1)) \
{\
__TYPE__*  pUnit = new __TYPE__();\
if (pUnit){\
	bool bInitSuc;\
	if (__RECT__.equals(cocos2d::Rect(0, 0, -1, -1))){\
		bInitSuc = pUnit->initWithFile(__FNAME__);\
	}\
	else{\
		bInitSuc = pUnit->initWithFile(__FNAME__, __RECT__);\
	}\
	if (bInitSuc){\
		pUnit->autorelease();\
		return pUnit;\
	}\
}\
CC_SAFE_DELETE(pUnit);\
return nullptr;\
}

//【base class:Unit】
class Unit :public cocos2d::Sprite, public MyAnimation
{
public:
	Unit(){};
	virtual ~Unit(){};
	CREATE_UNIT(Unit);
	//Desc:创建一个基于多张图片的动画效果
	cocos2d::Animate* createUnitAni(float delay, size_t loops){		
		return cocos2d::Animate::create(createAnimateWithMulPic(m_BaseFName,
			m_Postfix, m_iUnitPics,m_UnitRect, delay, loops));
	}
	void init(const std::string& baseFName, const std::string& postfix,
		size_t unitPics, const cocos2d::Rect& unitRect);
protected:
	std::string m_BaseFName;
	std::string m_Postfix;
	cocos2d::Rect m_UnitRect;
	int m_iUnitPics;
};

//【derived class:TrackUnit】
class TrackUnit:public Unit
{
public:
	CREATE_UNIT(TrackUnit);
	//移动到另一个节点之后隐藏
	cocos2d::FiniteTimeAction* MovingToNode(const cocos2d::Sprite* target, float duration);
	void MovingToNodeThenHide(const cocos2d::Sprite* target, float duration);
	void RotateAndMoveToNodeThenHide(float rotateDuration, float deltaAngle,const cocos2d::Sprite* target, float moveDuration);

};

//【derived class:BeamUnit】
class BeamUnit :public Unit
{
public:
	CREATE_UNIT(BeamUnit);
	cocos2d::FiniteTimeAction* MovingByNode(const cocos2d::Vec2& tarWorldPos,  float duration, float speed);
	void RotateAndMoveTByNodeThenHide(float rotateDuration, float deltaAnge,
		const cocos2d::Vec2& tarWorldPos, float moveDuration, float speed);
};

//【derived class:TornadoUnit】
class TornadoUnit :public Unit
{
public:
	CREATE_UNIT(TornadoUnit);
	void RandMoveThenHide(cocos2d::Rect transitMoveRect,size_t transitPointNums,
		const cocos2d::Vec2& endWorldPos, float speed);
	void SlowHide(float duration);
};

//【derived class:TornadoUnit】
class GhostUnit :public Unit
{
public:
	enum Attack_Orient
	{
		ATTACK_ORIENT_SE,
		ATTACK_ORIENT_SW,
		ATTACK_ORIENT_NE,
		ATTACK_ORIENT_NW
	};
public:
	CREATE_UNIT(GhostUnit);
	void rotateAttackAndHide(Attack_Orient attackOri, float duration=1.14f);

};