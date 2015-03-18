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
//----------------------------【init()】--------------------------------
// Desc:AttackLayer层初始化
//----------------------------------------------------------------------
bool AttackLayer::init()
{
	if (!Layer::init())return false;

	//渲染尺寸
	m_GLViewSize = Director::getInstance()->getVisibleSize();
	
	//粒子光束
	m_Beam = BeamUnit::create(BEAM_UNIT_FNAME);
	Rect beamRect = Rect(0, 0, BEAM_UNIT_WIDTH, BEAM_UNIT_HEIGHT);
	m_Beam->init(BEAM_UNIT_BASEFNAME, BEAM_UNIT_POSTFIX, BEAM_UNIT_PICS, beamRect);
	m_Beam->setScale(BEAM_UNIT_SCALE);
	m_Beam->setVisible(false);
	this->addChild(m_Beam);

	//龙卷风
	m_Tornado = TornadoUnit::create(TORNADO_UNIT_FNAME);
	Rect tornadoRect = Rect(0, 0, TORNADO_UNIT_WIDTH, TORNADO_UNIT_HEIGHT);
	m_Tornado->init(TORNADO_UNIT_BASEFNAME, TORNADO_UNIT_POSTFIX, TORNADO_UNIT_PICS, tornadoRect);
	m_Tornado->setScale(TORNADO_UNIT_SCALE);
	m_Tornado->setVisible(false);
	this->addChild(m_Tornado);

	//鬼怪
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
//----------------------------【randOutArea()】---------------------------------
// Desc:随机一个在回型外围区域的向量(参数：外围区域边宽，边高)
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
//----------------------------【shootBeamOnce()】-------------------------------
// Desc:对光束的射击行为进行决定和更新
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
//----------------------------【BeamUpdate()】----------------------------------
// Desc:对光束的射击行为进行决定和更新
//------------------------------------------------------------------------------
void AttackLayer::BeamUpdate(float delta)
{
	//随机的光束射击	
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
//----------------------------【rollTornadoOnce()】--------------------------------
// Desc:一次龙卷风的横扫席卷
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

//----------------------------【TornadoUpdate()】----------------------------------
// Desc:龙卷风更新
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

//---------------------------【ghostAttackOnce()】-------------------------------
// Desc:一次鬼怪攻击
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
//---------------------------【GhostUpdate()】-------------------------------
// Desc:鬼怪更新
//---------------------------------------------------------------------------
void AttackLayer::GhostUpdate(float delta)
{
	m_GhostComeUpTimes += delta;
	if (!m_PlayLayer->isMajorDead()) //主角未死
	{
		if (!m_Ghost->isVisible()) //处于就绪状态
		{
			if (m_GhostComeUpTimes >= 2.5f) //冷却2.5s以上
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
		else //处于攻击状态
		{
			//更新攻击位置
			m_GhostAttackPos.x -= delta*BK_MOVE_AVESECOND_SPEED;
		}		
	}
}

//----------------------------【AttackUpdate()】---------------------------------
// Desc:攻击性元素的更新
//------------------------------------------------------------------------------
void AttackLayer::AttackUpdate(float delta)
{
	//【光束更新】
	BeamUpdate(delta);

	//【龙卷风更新】
	TornadoUpdate(delta);

	//【鬼怪更新】
	GhostUpdate(delta);
}

//-------------------------------【shootMajor()】---------------------------------
// Desc:检测光束射击是否射中了主角
//--------------------------------------------------------------------------------
bool AttackLayer::shootMajorDetec(float delta)
{
	bool bShoot = (m_Beam->isVisible() && m_Beam->getBoundingBox().containsPoint(getWorldPos(m_PlayLayer->getMajorRole())));
	return bShoot;
}

//-------------------------------【rollMajor()】----------------------------------
// Desc:检测龙卷风是否卷中了主角
//--------------------------------------------------------------------------------
bool AttackLayer::rollMajorDetec(float delta)
{
	bool bRoll = (m_Tornado->isVisible() && m_Tornado->getBoundingBox().containsPoint(getWorldPos(m_PlayLayer->getMajorRole())));
	return bRoll;
}

//---------------------------【GAttackMajorDetec()】------------------------------
// Desc://检测鬼怪是否攻击到主角
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

//-------------------------------【ColliDetec()】---------------------------------
// Desc:与主角的碰撞检测
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