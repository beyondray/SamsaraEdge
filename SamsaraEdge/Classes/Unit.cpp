#include "Unit.h"
#include "ConstValue.h"
#include "Random.h"
using namespace cocos2d;

//------------------------【init()】----------------------------
// Desc:基本内容初始化记录
//--------------------------------------------------------------
void Unit::init(const std::string& baseFName, const std::string& postfix, size_t unitPics, const cocos2d::Rect& unitRect)
{
	m_BaseFName = baseFName;
	m_Postfix = postfix;
	m_iUnitPics = unitPics;
	m_UnitRect = unitRect;
}

//------------------------【MovingToNode()】-----------------------------
// Desc:创建一个移动到固定精灵位置的动画效果
//-----------------------------------------------------------------------
FiniteTimeAction* TrackUnit::MovingToNode(const cocos2d::Sprite* target, float duration)
{
	Vec2 tarWorldPos = getWorldPos(target);
	Vec2 tarPosAfterBkMove = tarWorldPos - Vec2(duration*BK_MOVE_AVESECOND_SPEED, 0);
	auto moveToDest = Spawn::create(Repeat::create(createUnitAni(0.1f, 1), duration / (0.1f*m_iUnitPics)), MoveTo::create(duration, tarPosAfterBkMove), nullptr);
	return moveToDest;

}
//------------------------【MoveingToNodeThenHide()】----------------------------
// Desc:移动到固定精灵位置后隐藏
//-------------------------------------------------------------------------------
void TrackUnit::MovingToNodeThenHide(const cocos2d::Sprite* target, float duration)
{
	auto moveToDest = MovingToNode(target, duration);
	auto moveToDestAndHide = Sequence::create(moveToDest, Hide::create(), nullptr);
	this->stopAllActions();
	this->runAction(moveToDestAndHide);
}

//----------------------【RotateAndMoveToNodeThenHide()】------------------------
// Desc:旋转一个角度后，移动到固定精灵位置再隐藏
//-------------------------------------------------------------------------------
void TrackUnit::RotateAndMoveToNodeThenHide(float rotateDuration, float deltaAngle,const cocos2d::Sprite* target, float moveDuration)
{	
	auto moveToDest = MovingToNode(target, moveDuration);
	auto rotateTo = RotateTo::create(rotateDuration, -CC_RADIANS_TO_DEGREES(deltaAngle));
	auto moveToDestAndHide = Sequence::create(rotateTo, moveToDest, Hide::create(), nullptr);
	this->stopAllActions();
	this->runAction(moveToDestAndHide);
}
//-----------------------------【MoveingByNode()】-------------------------------
// Desc:创建一个沿固定精灵位置方向移动的动画效果
//-------------------------------------------------------------------------------
FiniteTimeAction* BeamUnit::MovingByNode(const Vec2& tarWorldPos, float duration, float speed)
{
	Vec2 direction = tarWorldPos - getWorldPos(this);
	direction.normalize();
	auto moveByDest = Spawn::create(Repeat::create(createUnitAni(0.1f, 1), duration/(0.1f*m_iUnitPics)), MoveBy::create(duration, speed*duration*direction), nullptr);
	this->stopAllActions();
	return moveByDest;
}

//-----------------------【RotateAndMoveTByNodeThenHide()】-----------------------
// Desc:旋转一个角度后，沿固定精灵位置方向移动，然后隐藏
//--------------------------------------------------------------------------------
void BeamUnit::RotateAndMoveTByNodeThenHide(float rotateDuration, float deltaAngle,const Vec2& tarWorldPos, float moveDuration, float speed)
{
	auto rotateTo = RotateTo::create(rotateDuration, -CC_RADIANS_TO_DEGREES(deltaAngle));
	auto moveByDest = MovingByNode(tarWorldPos, moveDuration, speed);
	auto rotateAndmoveByDest = Sequence::create(rotateTo, moveByDest, Hide::create(),nullptr);
	this->stopAllActions();
	this->runAction(rotateAndmoveByDest);
}

//----------------------------【RandMoveThenHide()】------------------------------
// Desc:在区域内随机周转移动后，到最终位置，然后隐藏
//--------------------------------------------------------------------------------
void TornadoUnit::RandMoveThenHide(Rect transitMoveRect, size_t transitPointNums, const cocos2d::Vec2& endWorldPos, float speed)
{
	//中转移动
	Vector<FiniteTimeAction*> actArr;
	Vec2 nowPos = getWorldPos(this), nextPos, direction;
	for (size_t i = 0; i < transitPointNums; i++)
	{
		float x = randFloat(transitMoveRect.getMinX(), transitMoveRect.getMaxX());
		float y = randFloat(transitMoveRect.getMinY(), transitMoveRect.getMaxY());
		nextPos = Vec2(x, y);
		direction = nextPos - nowPos;
		auto moveToDest = MoveTo::create(direction.length() / speed, nextPos);
		actArr.pushBack(moveToDest);
		//更改当前位置，准备下一轮循环
		nowPos = nextPos;
	}
	//移动到终点
	direction = endWorldPos - nowPos;
	auto moveToEnd = MoveTo::create(direction.length() / speed, endWorldPos);
	actArr.pushBack(moveToEnd);
	actArr.pushBack(Hide::create());

	auto moveSeq = Sequence::create(actArr);
	auto repeatAni = Repeat::create(createUnitAni(0.1f, 1), 500);
	auto tornadoMove = Spawn::create(moveSeq, repeatAni, nullptr);
	this->stopAllActions();
	this->runAction(tornadoMove);
}

//--------------------------------【SlowHide()】----------------------------------
// Desc:缓慢的消失，最后隐藏
//--------------------------------------------------------------------------------
void TornadoUnit::SlowHide(float duration)
{
	auto repeatAni = Repeat::create(createUnitAni(0.1f, 1), duration/(0.1f*m_iUnitPics)+1);
	auto slowHide = Sequence::create(FadeOut::create(duration), Hide::create(), nullptr);
	auto hideWithAni = Spawn::create(repeatAni, slowHide, nullptr);
	auto hideWithAniThenAdustOpa = Sequence::create(hideWithAni, CallFunc::create(CC_CALLBACK_0(TornadoUnit::setOpacity, this, 255)), nullptr);
	this->runAction(hideWithAniThenAdustOpa);
}
//---------------------------【rotateAttackAndHide()】----------------------------
// Desc:旋转到一个面向，然后进行攻击
//--------------------------------------------------------------------------------
void GhostUnit::rotateAttackAndHide(Attack_Orient attackOri, float duration/* =1.14f */)
{
	float angle = 0.0f;
	switch (attackOri)
	{
	case GhostUnit::ATTACK_ORIENT_SE:
		angle = 0.0f;
		break;
	case GhostUnit::ATTACK_ORIENT_SW:
		angle = 60.0f;
		break;
	case GhostUnit::ATTACK_ORIENT_NE:
		angle = -120.0f;
		break;
	case GhostUnit::ATTACK_ORIENT_NW:
		angle = 180.0f;
		break;
	default:
		break;
	}
	auto rotateTo = RotateTo::create(0.0f, angle);
	auto backMove = MoveBy::create(duration, Vec2(-duration*BK_MOVE_AVESECOND_SPEED, 0));
	auto ani = createUnitAni(duration/m_iUnitPics, 1);
	auto attack = Spawn::create(backMove, ani, nullptr);
	auto aniAndHide = Sequence::create(rotateTo, attack, Hide::create(), nullptr);
	this->runAction(aniAndHide);
}