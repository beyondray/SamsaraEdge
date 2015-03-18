#include "PlayLayer.h"
#include "Random.h"
#include "MenuLayer.h"
#include "MagicCircle.h"
#include "MajorRole.h"
#include "Unit.h"
#include <SimpleAudioEngine.h>
#include <platform/android/jni/JniHelper.h>
#include <jni.h>

using namespace cocos2d;
using namespace CocosDenshion;

//------------------------------��init()��--------------------------------
// Desc����Ϸ���ݳ�ʼ��
//------------------------------------------------------------------------
bool PlayLayer::init()
{
	if (!Layer::init())
		return false;

	//��ʼ���������
	srand(time(NULL));

	//��Ⱦ�ߴ�
	m_GLViewSize = Director::getInstance()->getVisibleSize();
	
	//������������á�
	playDataInit();

	//����1,����2
	m_Bkgd1 = Sprite::create(BK1_FNAME);
	m_Bkgd2 = Sprite::create(BK2_FNAME);
	m_Bkgd1->setAnchorPoint(Vec2::ZERO);
	m_Bkgd2->setAnchorPoint(Vec2::ZERO);
	m_MoveOfBklength = m_Bkgd1->getContentSize().width;
	m_Bkgd2->setPosition(Vec2(m_MoveOfBklength - LINK_COEFF, 0));
	this->addChild(m_Bkgd1);
	m_Bkgd1->addChild(m_Bkgd2);

	//����
	m_MajorRole = MajorRole::create(MAJORROLE_WALK_FNAME, Rect(0, 0, 141, 167));
	m_MajorRole->setAnchorPoint(Vec2(0.5f, 0.4f));
	m_MajorRole->retain();

	//ħ�������ӹ켣
	Rect trackUnitRect = Rect(0, 0, TRACKUNIT_WIDTH, TRACKUNIT_HEIGHT);
	for (int i = 0; i < TRACKUNIT_NUMS; i++)
	{
		m_TrackUnit[i] = TrackUnit::create(TRACKUNIT_FNAME);
		m_TrackUnit[i]->init(TRACKUNIT_BASEFNAME, TRACKUNIT_POSTFIX, TRACKUNIT_PICS, trackUnitRect);
		m_TrackUnit[i]->setScale(TRACKUNIT_SCALE);
		m_TrackUnit[i]->retain();
		this->addChild(m_TrackUnit[i]);
		m_TrackUnit[i]->setVisible(false);
	}

	//������ӹ�Ч
	auto green = ParticleSystemQuad::create(GREEN_PARTICLE_FNAME);
	m_MajorRole->addChild(green);
	green->setPosition(Vec2(m_MajorRole->getContentSize().width*0.3f, m_MajorRole->getContentSize().height*0.4f));
	
	//�����ħ���Ź���
	for (int i = 0; i < MAGIC_CIR_TOTAL_NUM; i++)
	{
		m_MagicCir[i] = MagicCircle::create(MAGIC_FNAME, Rect(0, 0, MAGIC_WIDTH, MAGIC_HEIGHT));
		m_MagicCir[i]->setScale(MAGIC_SCALE);
		i <= MAGIC_CIR_TOTAL_NUM/2 ? m_Bkgd1->addChild(m_MagicCir[i]):m_Bkgd2->addChild(m_MagicCir[i]);
		m_MagicCir[i]->setPosition(randFloatTo(m_GLViewSize.width), randFloatTo(m_GLViewSize.height));	
		//��ת��������
		if (i % 2 != 0)
		{
			m_MagicCir[i]->setVisible(false);
			m_Blink[i] = nullptr;
		}
		else 
		{
			m_Blink[i] = m_MagicCir[i]->rotateAndHide(randFloat(MAGIC_ROTATE_RANDTIME_BEG, MAGIC_ROTATE_RANDTIME_END));
			m_Blink[i]->retain();
			m_MagicCir[i]->runAction(m_Blink[i]);
		}
	}
	//��ɫ�ͳߴ�
	m_MagicColor = m_MagicCir[0]->getColor();
	m_MagicSize = m_MagicCir[0]->getContentSize()*m_MagicCir[0]->getScale();

	//�¼������Ĵ�������
	TouchInitAndUpdate();
	keyEventInitAndUpdate();

	//��ʱ��
	this->schedule(schedule_selector(PlayLayer::BkUpdate));
	this->schedule(schedule_selector(PlayLayer::MagicUpdate), MAGIC_UPDATE_DELTA);
	this->schedule(schedule_selector(PlayLayer::TrackUpdate), TRACK_UNPDATE_DELTA);
	this->schedule(schedule_selector(PlayLayer::DeadJudgeUpdate));
	return true;

}

//---------------------------��playDataInit()��----------------------------
// Desc����Ϸ���ݳ�ʼ��
//-------------------------------------------------------------------------
void PlayLayer::playDataInit()
{
	//----------------------------------------
	//���������ݳ�ʼ����
	//----------------------------------------
	std::string selectedScene = UserDefault::getInstance()->getStringForKey("scene", "huoyan");
	if (selectedScene == "huoyan")
	{
		BK1_FNAME = HUOYAN1_FNAME;
		BK2_FNAME = HUOYAN2_FNAME;
		TRACKUNIT_FNAME = HUANG_UNIT_FNAME;
		TRACKUNIT_BASEFNAME = HUANG_UNIT_BASEFNAME;
		TRACKUNIT_PICS = HUANG_UNIT_PICS;
		TRACKUNIT_WIDTH = HUANG_UNIT_WIDTH;
		TRACKUNIT_HEIGHT = HUANG_UNIT_HEIGHT;
		TRACKUNIT_SCALE = HUANG_UNIT_SCALE;
	}
	else if (selectedScene == "bingxue")
	{
		BK1_FNAME = BINGXUE1_FNAME;
		BK2_FNAME = BINGXUE2_FNAME;
		TRACKUNIT_FNAME = XUAN_UNIT_FNAME;
		TRACKUNIT_BASEFNAME = XUAN_UNIT_BASEFNAME;
		TRACKUNIT_PICS = XUAN_UNIT_PICS;
		TRACKUNIT_WIDTH = XUAN_UNIT_WIDTH;
		TRACKUNIT_HEIGHT = XUAN_UNIT_HEIGHT;
		TRACKUNIT_SCALE = XUAN_UNIT_SCALE;
	}
}

//-------------------------��keyEventInitAndUpdate()��----------------------
// Desc:�¼�������ʼ��������
//--------------------------------------------------------------------------
void PlayLayer::keyEventInitAndUpdate()
{
	EventListenerKeyboard *listener = EventListenerKeyboard::create();
	typedef cocos2d::EventKeyboard::KeyCode KeyCode;
	listener->onKeyReleased = [&](cocos2d::EventKeyboard::KeyCode code, cocos2d::Event *event){
		switch (code)
		{
		case KeyCode::KEY_BACK://��Ӧ�˸�� 
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			JniMethodInfo info;
			bool bHave = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "exitGame", "()V");
			if (bHave)
				info.env->CallStaticVoidMethod(info.classID, info.methodID);
			else
				CCLOG("no exist");
#elif(CC_TARGET_PLATFORM != CC_PLATFORM_WINRT && CC_TARGET_PLATFORM != CC_PLATFORM_WP8 && CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
			Director::getInstance()->end();
			SimpleAudioEngine::getInstance()->end();
#endif
			break;
		}
	};
	_eventDispatcher->addEventListenerWithFixedPriority(listener, 1);
}

//-----------------------------��TouchInitAndUpdate()��------------------------------
// Desc���¼��ɷ��Ĵ�����ʼ���͸���
//-----------------------------------------------------------------------------------
void PlayLayer::TouchInitAndUpdate()
{
	//��Ӵ�������
	auto m_TouchListener = EventListenerTouchOneByOne::create();
	m_TouchListener->setSwallowTouches(true);
	m_TouchListener->onTouchBegan = [&](Touch*touch, Event*event){
		//-----------------------------------------------------
		// �˴�����MagicLock
		//-----------------------------------------------------
		if (m_bMagicPushLock) return true;
		//-----------------------------------------------------
		//�Էǵ���Ŀ��,���ٽ��д����ƶ��¼��ķ���
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		if (!target->isVisible())return false;
		Vec2 localPos = target->convertToNodeSpace(touch->getLocation());
		float radius = target->getContentSize().height / 2;
		if (pow(localPos.x - radius, 2) + pow(localPos.y - radius, 2) > radius*radius)
			return false;

		//��ʼ��Ϊ������ק
		m_bDragMagic = false;
		//��ħ������ӵ������У����Ӳ���)
		addMagicToListWithTest(target);
		return true;
	};

	m_TouchListener->onTouchMoved = [&](Touch*touch, Event*event){
		//-----------------------------------------------------
		// �˴�����MagicLock
		//-----------------------------------------------------
		if (m_bMagicPushLock) return;
		//-----------------------------------------------------

		for (int i = 0; i < MAGIC_CIR_TOTAL_NUM; i++)
		{
			if (m_MagicCir[i]->isVisible())
			{
				Vec2 localPos = m_MagicCir[i]->getParent()->convertToNodeSpace(touch->getLocation());
				if (m_MagicCir[i]->getBoundingBox().containsPoint(localPos))
				{
					if (!m_MagicList.empty() && m_MagicList.back() == m_MagicCir[i])
					{
						if (m_bDragMagic) //�ظ��������λ�ڵ�ħ����
						{
							m_MagicCir[i]->setPosition(localPos);
							m_MajorRole->setPosition(getWorldPos(m_MagicCir[i]));
						}
					}
					else //��ħ������ӵ������У����Ӳ���)
						addMagicToListWithTest(m_MagicCir[i]);
					break;
				}
			}
		}
	};
	//�¼������ĳ���ͼ����
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_TouchListener, m_MagicCir[0]);
	for (int i = 1; i < MAGIC_CIR_TOTAL_NUM; i++)
		_eventDispatcher->addEventListenerWithSceneGraphPriority(m_TouchListener->clone(), m_MagicCir[i]);
}


//--------------------------------��BkUpdate()��-----------------------------------
// Desc����������
//---------------------------------------------------------------------------------
void PlayLayer::BkUpdate(float delta)
{
	//�����ƶ�
	static float accumTime = 1.0f;
	accumTime += delta;
	if(accumTime >= 1.0f)
	{
		auto moveAct = MoveBy::create(1.0f, Vec2(-BK_MOVE_AVESECOND_SPEED, 0));
		m_Bkgd1->runAction(moveAct);
		accumTime = 0.0f;
	}

	//�����ٽ�㣬����λ�ñ��
	if (m_Bkgd1->getPositionX() <= -m_MoveOfBklength && m_Bkgd2->getPositionX()>0)
	{		
		m_Bkgd1->setPositionX(m_MoveOfBklength - LINK_COEFF);
		m_Bkgd2->setPositionX(LINK_COEFF - m_MoveOfBklength);
	}
	else if (m_Bkgd1->getPositionX() <= 0 && m_Bkgd2->getPositionX()<0)
	{
		m_Bkgd2->setPositionX(m_MoveOfBklength - LINK_COEFF);
	}
}

//--------------------------------��TrackUpdate()��--------------------------------
// Desc��ħ����֮��Ĺ켣����
//---------------------------------------------------------------------------------
void PlayLayer::TrackUpdate(float delta)
{
	//�������ǵ�����Ŀ��Ĺ켣
	if (m_MagicRecord != nullptr)
	{
		if (!m_MagicList.empty())
		{
			Vec2 srcWorldPos = getWorldPos(m_MagicList.back());
			Vec2 destWorldPos = getWorldPos(m_MagicRecord);
			Vec2 dierction = destWorldPos - srcWorldPos;
			for (int i = 0; i < TRACKUNIT_NUMS; i++)
			{
				if (!m_TrackUnit[i]->isVisible())
				{
					m_TrackUnit[i]->setVisible(true);
					m_TrackUnit[i]->setPosition(srcWorldPos);
					m_TrackUnit[i]->RotateAndMoveToNodeThenHide(0.01f, (dierction).getAngle(),
						m_MagicRecord, dierction.length()*TRACK_DURATION_IN_ONEHUNDRED_PIXELS / 100);
					break;
				}
			}
		}
	}
	//����ħ����������ħ����֮��Ĺ켣
	for (auto iter = m_MagicList.begin(); iter != m_MagicList.end(); iter++)
	{
		auto nextIter = iter; nextIter++;
		if (nextIter != m_MagicList.end())
		{
			Vec2 srcWorldPos = getWorldPos((*iter));
			Vec2 destWorldPos = getWorldPos((*nextIter));
			Vec2 dierction = destWorldPos - srcWorldPos;
			for (int i = 0; i < TRACKUNIT_NUMS; i++)
			{
				if (!m_TrackUnit[i]->isVisible())
				{
					m_TrackUnit[i]->setVisible(true);
					m_TrackUnit[i]->setPosition(srcWorldPos);
					m_TrackUnit[i]->RotateAndMoveToNodeThenHide(0.01f, (dierction).getAngle(),
						(*nextIter), dierction.length()*TRACK_DURATION_IN_ONEHUNDRED_PIXELS / 100);
					break;
				}
			}
		}
	}
}

//------------------------------��MagicUpdate()��----------------------------------
// Desc��ħ�������
//---------------------------------------------------------------------------------
void PlayLayer::MagicUpdate(float delta)
{
	//��ά��ħ���������е�Ԫ�أ���������Ļ��Χ�򲻿ɼ���Ԫ���Ƴ���
	maintainMagicList();

	//�������ٴγ��ֵ����ħ����
	//����ͬʱ����ħ����ĸ���
	int  onceAppearNum = 0;
	float prob = randFloatTo(1.0f);
	if (prob < 0.05f)      //5%���ʲ�����
		return;
	if (prob < 0.3f)      //25%���ʳ���1��
		onceAppearNum = 1;
	else if (prob < 0.6f) //30%���ʳ���2��
		onceAppearNum = 2;
	else if (prob < 0.9f) //30%���ʳ���3��
		onceAppearNum = 3;
	else                   //10%���ʳ���4��
		onceAppearNum = 4;
	
	//���ִ����ۼ�
	int appearCount = 0;
	for (int i = 0; i < MAGIC_CIR_TOTAL_NUM; i++)
	{
		//��һ�λ��������ʱִ��
		if (m_Blink[i] == nullptr || m_Blink[i]->isDone())
		{
			Vec2 relativePos;
			Node* bk;
			while (1)
			{
				//���ÿɼ���Χ�ڵ����λ��
				float x = randFloat(MAGIC_UPDATE_MOVESCOPE, m_GLViewSize.width+MAGIC_UPDATE_MOVESCOPE);
				srand(rand());
				float y = randFloatTo(m_GLViewSize.height);

				//ѡ�����������������λ��
				Vec2 randPos = Vec2(x, y);	
				Vec2 worldPos1 = getWorldPos(m_Bkgd1);
				Vec2 worldPos2 = getWorldPos(m_Bkgd2);
				Rect rect(worldPos1.x, worldPos1.y, m_Bkgd1->getContentSize().width, m_Bkgd1->getContentSize().height);
				if (rect.containsPoint(randPos))
				{
					bk = m_Bkgd1;
					relativePos = randPos-worldPos1;
				}
				else
				{
					bk = m_Bkgd2;
					relativePos = randPos-worldPos2;
				}
				break;
			}

			//�µ�ħ������
			if (m_Blink[i]==nullptr)
			{
				m_Blink[i] = m_MagicCir[i]->rotateAndHide(randFloat(MAGIC_ROTATE_RANDTIME_BEG, MAGIC_ROTATE_RANDTIME_END));
				m_Blink[i]->retain();
			}	
			m_MagicCir[i]->setPosition(relativePos);		
			m_MagicCir[i]->setVisible(true);
			m_MagicCir[i]->runAction(m_Blink[i]);	
			m_MagicCir[i]->setColor(m_MagicColor);
			m_MagicCir[i]->setOpacityModifyRGB(false);
			m_MagicCir[i]->getParent()->removeChild(m_MagicCir[i], false);
			bk->addChild(m_MagicCir[i]);		
			appearCount++;
			if (appearCount >= onceAppearNum)
			{
				break;
			}
		}			
	}
}
//--------------------------��stayMagicDead()��------------------------------
// Desc���Ƿ���ͣ��ħ�����������
//---------------------------------------------------------------------------
bool PlayLayer::stayMagicDead()
{
	//����ħ���������ɼ�����������Ԫ�ز��ɼ����ж�����
	if (!m_bMajorRDead)
	{
		bool bInside = !m_MagicList.empty() && isInMagic(m_MagicList.back(), 0.2f);
		bool bDead = m_MagicList.empty() || (!m_MagicList.back()->isVisible() && bInside);
		if (bDead)
		{
			MajorDeadManage();
		}
	}
	return m_bMajorRDead;
}

//--------------------------��reachMagicDead()��------------------------------
// Desc���Ƿ��򵽴���ʧħ������������(δ�����������ħ�������)
//----------------------------------------------------------------------------
bool PlayLayer::reachMagicDead()
{
	if (!m_bMajorRDead)
	{
		bool bInside = isInMagic(m_MagicRecord, 0.5f);
		if (bInside)
		{
			//���ǵ���ʱħ���󲻿ɼ������ж�����
			if (!m_MagicRecord->isVisible())
			{
				MajorDeadManage();
			}
			else
			{
				addMagicToListWithTest(m_MagicRecord);
				m_MagicRecord = nullptr;
			}	
		}
	}
	return m_bMajorRDead;
}

//-----------------------------��MajorDeadManage()��---------------------------------
// Desc:�ڸ���ʱ����, ���Ǹ��ݷ����ƶ�����һ���ڵ�
//-----------------------------------------------------------------------------------
void PlayLayer::MajorDeadManage()
{
	m_bMajorRDead = true;
	MajorDeadPerformance();
	m_MagicRecord = nullptr;
	//MagicLock
	m_bMagicPushLock = true;
}

//------------------------------��maintainMagicList()��---------------------------------
// Desc��ά��ħ��������
//--------------------------------------------------------------------------------------
void PlayLayer::maintainMagicList()
{
	static float locaWidth = m_MagicSize.width / 2;
	static float limitWidthRight = m_GLViewSize.width + locaWidth;
	static float limitWidthLeft = -locaWidth;
	for (auto iterSp = m_MagicList.begin(); iterSp != m_MagicList.end();)
	{
		float posX = (getWorldPos(*iterSp)).x;
		if (posX > limitWidthRight || posX < limitWidthLeft || !(*iterSp)->isVisible())
		{
			iterSp = m_MagicList.erase(iterSp);
		}
		else
		{
			iterSp++;
		}
	}
}

//----------------------------��MajorDeadPerformance()��----------------------------
// Desc������������ı��ּ���������
//----------------------------------------------------------------------------------
void PlayLayer::MajorDeadPerformance()
{
	//������׹�估��ʾ�������桿
	float height = m_GLViewSize.height / 9.0f;
	Vec2 deadPos = Vec2(m_MajorRole->getPosition().x, height);
	m_MajorRole->MovingToNode(major_Action::DEAD, deadPos,
		m_MajorRole->getPosition().y*MAJOR_DROPTIME_IN_ONEHUNDRED_PIXELS / 100, &m_bMagicPushLock);

	//�����ħ��������
	if (!m_MagicList.empty())
		m_MagicList.clear();

	return;
}

//----------------------------------��isInMagic()��----------------------------------
// Desc�������Ƿ���ָ��ħ�����ڵ��жϣ���
//-----------------------------------------------------------------------------------
bool PlayLayer::isInMagic(cocos2d::Sprite* magic, float radScale /* =1.0f */)
{
	Vec2 magicPos = getWorldPos(magic);
	Vec2 majorPos = m_MajorRole->getPosition();
	float radius = magic->getContentSize().height / 2;
	bool bInside = (pow(magicPos.x - majorPos.x, 2) + pow(magicPos.y - majorPos.y, 2) <= pow(radius*radScale, 2));
	return bInside;
}

//-----------------------------��MovingToNode()��---------------------------------
// Desc:�ڸ���ʱ����, ���Ǹ��ݷ����ƶ�����һ���ڵ�
//--------------------------------------------------------------------------------
void PlayLayer::MovingToNode(cocos2d::Sprite* dest)
{
	Vec2 srcWorldPos = m_MajorRole->getPosition();
	Vec2 destWorldPos = getWorldPos(dest);
	Vec2 direction = destWorldPos - srcWorldPos;
	//���ݷ����ж�ѡ����
	major_Action sth;
	int randDom = randIntTo(1);
	if (direction.x > 0)
	{
		if (direction.y > direction.x*0.577f)
		{
			randDom == 0 ? sth = major_Action::WAVE : sth = major_Action::WAVEUP;
		}
		else if (direction.y < -direction.x*0.577)
		{
			randDom == 0 ? sth = major_Action::WAVE : sth = major_Action::WAVEDOWN;
		}
		else
		{
			randDom == 0 ? sth = major_Action::WAVE : sth = major_Action::WAVERIGHT;
		}		
	}
	else
	{
		sth = major_Action::JUMP;
	}
	//ִ��moving����
	m_MajorRole->MovingToNodeThenWalk(sth, destWorldPos,direction.getLength()*MAJOR_MOVETIME_IN_ONEHUNDRED_PIXELS / 100);
}

//-------------------------��addMagicToListWithTest()��---------------------------
// Desc:�ڸ���ʱ����, ���Ǹ��ݷ����ƶ�����һ���ڵ�
//--------------------------------------------------------------------------------
bool PlayLayer::addMagicToListWithTest(Sprite* target)
{
	//����ɫ����
	target->setColor(Color3B(0, 255, 0));

	//��λ�ڵĵ��ظ����ʱ��ħ�������ק
	if (!m_MagicList.empty() && m_MagicList.back() == target)m_bDragMagic = true;

	//0��ʱ��ֱ�Ӽ���Ԫ�ز�����;1��ʱ��Ԫ�ز��ظ��ż��룬Ȼ�󷵻�
	if (m_MagicList.size() <= 1)
	{
		if (m_MagicList.size() == 1 && m_MagicList.back() == target)
			return false;
		else if (m_MagicList.size() == 0)
		{
			//ֻ�е�һ�β��������		
			if (m_MajorRole->getParent() == nullptr)
			{
				//�趨����
				this->addChild(m_MajorRole, 1);				
			}
			Vec2 tarWorldPos = getWorldPos(target);
			m_MajorRole->setPosition(tarWorldPos);	
			m_MajorRole->stopAllActions();
			auto backWalk = RepeatForever::create(Spawn::create(m_MajorRole->createWalkAni(0.1f, 1), 
				MoveBy::create(1, Vec2(-BK_MOVE_AVESECOND_SPEED, 0)), nullptr));
			m_MajorRole->runAction(backWalk);
			//���ǵ���
			m_bMajorRDead = false;
			m_MagicList.push_back(target);
			//���������Ч
			SimpleAudioEngine::getInstance()->playEffect(EFFECT_BIRTH);
			return true;
		}
		if (!m_bMagicPushLock)
		{
			//�ƶ�����һ��ħ����ڵ�
			MovingToNode(target);
			m_MagicRecord = target;
		}	
		//----------------------------------------------------------------------------
		//�ڴ˴�����MagicLock(��һ����ʱ�л�Ϊ�������ڶ�����ʾ���ñ�����ʱ���н���
		//----------------------------------------------------------------------------
		m_bMagicPushLock = !m_bMagicPushLock;	
		if (m_bMagicPushLock)return true;
		//----------------------------------------------------------------------------

		m_MagicList.push_back(target);
		return true;
	}

	//�������Ŀ���������к�����ħ����ָ�벻�ظ�����push_back
	auto back = m_MagicList.back();
	m_MagicList.pop_back();
	auto secondBack = m_MagicList.back();
	m_MagicList.push_back(back);

	bool bPushSuc = false;
	if (back != target && secondBack != target)
	{
		if (!m_bMagicPushLock)
		{
			//�ƶ�����һ��ħ����ڵ�
			MovingToNode(target);
			m_MagicRecord = target;
		}
		//----------------------------------------------------------------------------
		//�ڴ˴�����MagicLock(��һ����ʱ�л�Ϊ�������ڶ�����ʾ���ñ�����ʱ���н���
		//----------------------------------------------------------------------------
		m_bMagicPushLock = !m_bMagicPushLock;
		if (m_bMagicPushLock)return true;
		//----------------------------------------------------------------------------

		m_MagicList.push_back(target);
		bPushSuc = true;
	}

	//��������ɹ�������Ƿ��п�������Ԫ��
	if (bPushSuc)
	{
		for (auto iter = --m_MagicList.rend(); iter != m_MagicList.rbegin(); iter--)
		{
			if (*iter == target)
			{
				m_MagicList.pop_back();
				int score = MenuLayer::getInstance()->getScore();
				while (m_MagicList.back() != target)
				{
					m_MagicList.back()->setVisible(false);
					m_MagicList.pop_back();
					MenuLayer::getInstance()->runSetScoreAni(++score);
					//���ŵ÷���Ч
					SimpleAudioEngine::getInstance()->playEffect(EFFECT_ADDSCORE);
				}
				break;
			}
		}
	}
	return bPushSuc;
}