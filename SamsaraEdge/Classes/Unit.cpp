#include "Unit.h"
#include "ConstValue.h"
#include "Random.h"
using namespace cocos2d;

//------------------------��init()��----------------------------
// Desc:�������ݳ�ʼ����¼
//--------------------------------------------------------------
void Unit::init(const std::string& baseFName, const std::string& postfix, size_t unitPics, const cocos2d::Rect& unitRect)
{
	m_BaseFName = baseFName;
	m_Postfix = postfix;
	m_iUnitPics = unitPics;
	m_UnitRect = unitRect;
}

//------------------------��MovingToNode()��-----------------------------
// Desc:����һ���ƶ����̶�����λ�õĶ���Ч��
//-----------------------------------------------------------------------
FiniteTimeAction* TrackUnit::MovingToNode(const cocos2d::Sprite* target, float duration)
{
	Vec2 tarWorldPos = getWorldPos(target);
	Vec2 tarPosAfterBkMove = tarWorldPos - Vec2(duration*BK_MOVE_AVESECOND_SPEED, 0);
	auto moveToDest = Spawn::create(Repeat::create(createUnitAni(0.1f, 1), duration / (0.1f*m_iUnitPics)), MoveTo::create(duration, tarPosAfterBkMove), nullptr);
	return moveToDest;

}
//------------------------��MoveingToNodeThenHide()��----------------------------
// Desc:�ƶ����̶�����λ�ú�����
//-------------------------------------------------------------------------------
void TrackUnit::MovingToNodeThenHide(const cocos2d::Sprite* target, float duration)
{
	auto moveToDest = MovingToNode(target, duration);
	auto moveToDestAndHide = Sequence::create(moveToDest, Hide::create(), nullptr);
	this->stopAllActions();
	this->runAction(moveToDestAndHide);
}

//----------------------��RotateAndMoveToNodeThenHide()��------------------------
// Desc:��תһ���ǶȺ��ƶ����̶�����λ��������
//-------------------------------------------------------------------------------
void TrackUnit::RotateAndMoveToNodeThenHide(float rotateDuration, float deltaAngle,const cocos2d::Sprite* target, float moveDuration)
{	
	auto moveToDest = MovingToNode(target, moveDuration);
	auto rotateTo = RotateTo::create(rotateDuration, -CC_RADIANS_TO_DEGREES(deltaAngle));
	auto moveToDestAndHide = Sequence::create(rotateTo, moveToDest, Hide::create(), nullptr);
	this->stopAllActions();
	this->runAction(moveToDestAndHide);
}
//-----------------------------��MoveingByNode()��-------------------------------
// Desc:����һ���ع̶�����λ�÷����ƶ��Ķ���Ч��
//-------------------------------------------------------------------------------
FiniteTimeAction* BeamUnit::MovingByNode(const Vec2& tarWorldPos, float duration, float speed)
{
	Vec2 direction = tarWorldPos - getWorldPos(this);
	direction.normalize();
	auto moveByDest = Spawn::create(Repeat::create(createUnitAni(0.1f, 1), duration/(0.1f*m_iUnitPics)), MoveBy::create(duration, speed*duration*direction), nullptr);
	this->stopAllActions();
	return moveByDest;
}

//-----------------------��RotateAndMoveTByNodeThenHide()��-----------------------
// Desc:��תһ���ǶȺ��ع̶�����λ�÷����ƶ���Ȼ������
//--------------------------------------------------------------------------------
void BeamUnit::RotateAndMoveTByNodeThenHide(float rotateDuration, float deltaAngle,const Vec2& tarWorldPos, float moveDuration, float speed)
{
	auto rotateTo = RotateTo::create(rotateDuration, -CC_RADIANS_TO_DEGREES(deltaAngle));
	auto moveByDest = MovingByNode(tarWorldPos, moveDuration, speed);
	auto rotateAndmoveByDest = Sequence::create(rotateTo, moveByDest, Hide::create(),nullptr);
	this->stopAllActions();
	this->runAction(rotateAndmoveByDest);
}

//----------------------------��RandMoveThenHide()��------------------------------
// Desc:�������������ת�ƶ��󣬵�����λ�ã�Ȼ������
//--------------------------------------------------------------------------------
void TornadoUnit::RandMoveThenHide(Rect transitMoveRect, size_t transitPointNums, const cocos2d::Vec2& endWorldPos, float speed)
{
	//��ת�ƶ�
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
		//���ĵ�ǰλ�ã�׼����һ��ѭ��
		nowPos = nextPos;
	}
	//�ƶ����յ�
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

//--------------------------------��SlowHide()��----------------------------------
// Desc:��������ʧ���������
//--------------------------------------------------------------------------------
void TornadoUnit::SlowHide(float duration)
{
	auto repeatAni = Repeat::create(createUnitAni(0.1f, 1), duration/(0.1f*m_iUnitPics)+1);
	auto slowHide = Sequence::create(FadeOut::create(duration), Hide::create(), nullptr);
	auto hideWithAni = Spawn::create(repeatAni, slowHide, nullptr);
	auto hideWithAniThenAdustOpa = Sequence::create(hideWithAni, CallFunc::create(CC_CALLBACK_0(TornadoUnit::setOpacity, this, 255)), nullptr);
	this->runAction(hideWithAniThenAdustOpa);
}
//---------------------------��rotateAttackAndHide()��----------------------------
// Desc:��ת��һ������Ȼ����й���
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