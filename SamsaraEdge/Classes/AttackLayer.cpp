#include "AttackLayer.h"
#include "Unit.h"
#include "ConstValue.h"
#include "Random.h"
#include "MajorRole.h"
#include "PlayLayer.h"
#include "MenuLayer.h"

using namespace cocos2d;
typedef GhostUnit::Attack_Orient Attack_Orient;

const float AttackLayer::ms_GhostAttackWaitTimes = GHOST_ATTACK_ANI_TIMES*12/GHOST_UNIT_PICS;
//----------------------------��init()��--------------------------------
// Desc:AttackLayer���ʼ��
//----------------------------------------------------------------------
bool AttackLayer::init()
{
	if (!Layer::init())return false;

	//��Ⱦ�ߴ�
	m_GLViewSize = Director::getInstance()->getVisibleSize();
	
	//���ӹ���
	m_Beam = BeamUnit::create(BEAM_UNIT_FNAME);
	Rect beamRect = Rect(0, 0, BEAM_UNIT_WIDTH, BEAM_UNIT_HEIGHT);
	m_Beam->init(BEAM_UNIT_BASEFNAME, BEAM_UNIT_POSTFIX, BEAM_UNIT_PICS, beamRect);
	m_Beam->setScale(BEAM_UNIT_SCALE);
	m_Beam->setVisible(false);
	this->addChild(m_Beam);

	//�����
	m_Tornado = TornadoUnit::create(TORNADO_UNIT_FNAME);
	Rect tornadoRect = Rect(0, 0, TORNADO_UNIT_WIDTH, TORNADO_UNIT_HEIGHT);
	m_Tornado->init(TORNADO_UNIT_BASEFNAME, TORNADO_UNIT_POSTFIX, TORNADO_UNIT_PICS, tornadoRect);
	m_Tornado->setScale(TORNADO_UNIT_SCALE);
	m_Tornado->setVisible(false);
	this->addChild(m_Tornado);

	//���
	m_Ghost = GhostUnit::create(GHOST_UNIT_FNAME);
	Rect ghostRect = Rect(0, 0, GHOST_UNIT_WIDTH, GHOST_UNIT_HEIGHT);
	m_Ghost->init(GHOST_UNIT_BASEFNAME, GHOST_UNIT_POSTFIX, GHOST_UNIT_PICS, ghostRect);
	m_Ghost->setScale(GHOST_UNIT_SCALE);
	m_Ghost->setVisible(false);
	this->addChild(m_Ghost);

	this->schedule(schedule_selector(AttackLayer::AttackUpdate), 0.1f);
	this->schedule(schedule_selector(AttackLayer::ColliDetec), 0.1f);
	return true;
}
//----------------------------��randOutArea()��---------------------------------
// Desc:���һ���ڻ�����Χ���������(��������Χ����߿��߸�)
//------------------------------------------------------------------------------
Vec2 AttackLayer::randOutArea(float borderW, float borderH)
{
	static float halfWidth = m_GLViewSize.width / 2;
	static float halfHeight = m_GLViewSize.height / 2;
	float x = randFloat(0, 2*borderW);
	float y = randFloat(0, 2*borderH);
	x >= borderW ? x += halfWidth : x -= halfWidth;
	y >= borderH ? y += halfHeight : y -= halfHeight;
	return Vec2(x, y);
}
//----------------------------��shootBeamOnce()��-------------------------------
// Desc:�Թ����������Ϊ���о����͸���
//------------------------------------------------------------------------------
void AttackLayer::shootBeamOnce(float moveDuration)
{
	Vec2 randPos, tarWorldPos, direction;
	randPos = randOutArea(m_GLViewSize.width / 2, m_GLViewSize.height / 2);
	m_Beam->setPosition(randPos);
	tarWorldPos = getWorldPos(m_PlayLayer->getMajorRole());
	direction = tarWorldPos - randPos;
	m_Beam->RotateAndMoveTByNodeThenHide(0.1f, direction.getAngle(), tarWorldPos, moveDuration, BEAM_PIXELSPEED_PER_SECOND);
}
//----------------------------��BeamUpdate()��----------------------------------
// Desc:�Թ����������Ϊ���о����͸���
//------------------------------------------------------------------------------
void AttackLayer::BeamUpdate(float delta)
{
	//����Ĺ������	
	if (!m_Beam->isVisible())
	{
		int score = MenuLayer::getInstance()->getScore();
		if ((score >= 20 && score <= 30) || (score >= 40 && score <= 50) || score >= 60)
		{
			m_Beam->setVisible(true);
			shootBeamOnce(3.0f);
		}
	}
}
//----------------------------��rollTornadoOnce()��--------------------------------
// Desc:һ�������ĺ�ɨϯ��
//---------------------------------------------------------------------------------
void AttackLayer::rollTornadoOnce()
{
	Vec2 randPosBeg, randPosEnd;
	randPosBeg = randOutArea(m_GLViewSize.width / 3, m_GLViewSize.height / 3);
	randPosEnd = randOutArea(m_GLViewSize.width / 3, m_GLViewSize.height / 3);
	m_Tornado->setPosition(randPosBeg);
	Rect moveRect = Rect(0, 0, m_GLViewSize.width, m_GLViewSize.height);
	m_Tornado->RandMoveThenHide(moveRect, 7, randPosEnd, TORNADO_PIXELSPEED_PER_SECOND);
}

//----------------------------��TornadoUpdate()��----------------------------------
// Desc:��������
//---------------------------------------------------------------------------------
void AttackLayer::TornadoUpdate(float delta)
{
	int score = MenuLayer::getInstance()->getScore();
	if (!m_Tornado->isVisible())
	{		
		if ((score >= 30 && score <= 40) || (score >= 50 && score <= 60) || score >= 70)
		{
			m_Tornado->setVisible(true);
			rollTornadoOnce();
		}
	}
	else
	{
		if (score == 0)m_Tornado->SlowHide(0.5f);
	}
}

//---------------------------��ghostAttackOnce()��-------------------------------
// Desc:һ�ι�ֹ���
//-------------------------------------------------------------------------------
void AttackLayer::ghostAttackOnce()
{
	Vec2 thePos = m_PlayLayer->getMajorRole()->getPosition();
	m_GhostAttackPos = thePos;
	GhostUnit::Attack_Orient attackOrient = static_cast<GhostUnit::Attack_Orient>(randIntTo(3));
	int offsetX = 30, offsetY = 50;
	switch (attackOrient)
	{
	case Attack_Orient::ATTACK_ORIENT_NE:
		thePos += Vec2(-offsetX, -offsetY);
		break;
	case Attack_Orient::ATTACK_ORIENT_NW:
		thePos += Vec2(offsetX, -offsetY);
		break;
	case Attack_Orient::ATTACK_ORIENT_SE:
		thePos += Vec2(-offsetX, offsetY);
		break;
	case Attack_Orient::ATTACK_ORIENT_SW:
		thePos += Vec2(offsetX, offsetY);
		break;
	}
	m_Ghost->setPosition(thePos);
	m_Ghost->rotateAttackAndHide(attackOrient, GHOST_ATTACK_ANI_TIMES);
}
//---------------------------��GhostUpdate()��-------------------------------
// Desc:��ָ���
//---------------------------------------------------------------------------
void AttackLayer::GhostUpdate(float delta)
{
	m_GhostComeUpTimes += delta;
	if (!m_PlayLayer->isMajorDead()) //����δ��
	{
		if (!m_Ghost->isVisible()) //���ھ���״̬
		{
			if (m_GhostComeUpTimes >= 2.5f) //��ȴ2.5s����
			{
				int score = MenuLayer::getInstance()->getScore();
				if ((score >= 10 && score <= 20) || (score >= 40 && score <= 60) || score >= 70)
				{
					m_Ghost->setVisible(true);
					m_GhostComeUpTimes = 0.0f;
					ghostAttackOnce();
				}
			}
		}
		else //���ڹ���״̬
		{
			//���¹���λ��
			m_GhostAttackPos.x -= delta*BK_MOVE_AVESECOND_SPEED;
		}		
	}
}

//----------------------------��AttackUpdate()��---------------------------------
// Desc:������Ԫ�صĸ���
//------------------------------------------------------------------------------
void AttackLayer::AttackUpdate(float delta)
{
	//���������¡�
	BeamUpdate(delta);

	//���������¡�
	TornadoUpdate(delta);

	//����ָ��¡�
	GhostUpdate(delta);
}

//-------------------------------��shootMajor()��---------------------------------
// Desc:����������Ƿ�����������
//--------------------------------------------------------------------------------
bool AttackLayer::shootMajorDetec(float delta)
{
	bool bShoot = (m_Beam->isVisible() && m_Beam->getBoundingBox().containsPoint(getWorldPos(m_PlayLayer->getMajorRole())));
	return bShoot;
}

//-------------------------------��rollMajor()��----------------------------------
// Desc:���������Ƿ����������
//--------------------------------------------------------------------------------
bool AttackLayer::rollMajorDetec(float delta)
{
	bool bRoll = (m_Tornado->isVisible() && m_Tornado->getBoundingBox().containsPoint(getWorldPos(m_PlayLayer->getMajorRole())));
	return bRoll;
}

//---------------------------��GAttackMajorDetec()��------------------------------
// Desc://������Ƿ񹥻�������
//--------------------------------------------------------------------------------
bool AttackLayer::GAttackMajorDetec(float delta)
{
	bool bHitMajor = false;
	if (m_Ghost->isVisible() && m_GhostComeUpTimes >= ms_GhostAttackWaitTimes)
	{
		Rect majorRect = m_PlayLayer->getMajorRole()->getBoundingBox();
		float borderW = m_PlayLayer->getMajorRole()->getContentSize().width / 4;
		float borderH = m_PlayLayer->getMajorRole()->getContentSize().height / 4;
		Rect newRect = Rect(majorRect.getMinX() + borderW, majorRect.getMinY(),
			2*borderW, 3*borderH);
		if (newRect.containsPoint(m_GhostAttackPos))
			bHitMajor = true;
	}
	return bHitMajor;
}

//-------------------------------��ColliDetec()��---------------------------------
// Desc:�����ǵ���ײ���
//--------------------------------------------------------------------------------
void AttackLayer::ColliDetec(float delta)
{
	if (!m_PlayLayer->isMajorDead())
	{
		if (shootMajorDetec(delta) || rollMajorDetec(delta) || GAttackMajorDetec(delta))
		{
			m_PlayLayer->MajorDeadManage();
		}
	}
}