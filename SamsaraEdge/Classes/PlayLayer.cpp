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

//------------------------------【init()】--------------------------------
// Desc：游戏内容初始化
//------------------------------------------------------------------------
bool PlayLayer::init()
{
	if (!Layer::init())
		return false;

	//初始化随机种子
	srand(time(NULL));

	//渲染尺寸
	m_GLViewSize = Director::getInstance()->getVisibleSize();
	
	//【相关数据设置】
	playDataInit();

	//背景1,背景2
	m_Bkgd1 = Sprite::create(BK1_FNAME);
	m_Bkgd2 = Sprite::create(BK2_FNAME);
	m_Bkgd1->setAnchorPoint(Vec2::ZERO);
	m_Bkgd2->setAnchorPoint(Vec2::ZERO);
	m_MoveOfBklength = m_Bkgd1->getContentSize().width;
	m_Bkgd2->setPosition(Vec2(m_MoveOfBklength - LINK_COEFF, 0));
	this->addChild(m_Bkgd1);
	m_Bkgd1->addChild(m_Bkgd2);

	//主角
	m_MajorRole = MajorRole::create(MAJORROLE_WALK_FNAME, Rect(0, 0, 141, 167));
	m_MajorRole->setAnchorPoint(Vec2(0.5f, 0.4f));
	m_MajorRole->retain();

	//魔法阵连接轨迹
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

	//添加绿子光效
	auto green = ParticleSystemQuad::create(GREEN_PARTICLE_FNAME);
	m_MajorRole->addChild(green);
	green->setPosition(Vec2(m_MajorRole->getContentSize().width*0.3f, m_MajorRole->getContentSize().height*0.4f));
	
	//随机的魔法炫光阵
	for (int i = 0; i < MAGIC_CIR_TOTAL_NUM; i++)
	{
		m_MagicCir[i] = MagicCircle::create(MAGIC_FNAME, Rect(0, 0, MAGIC_WIDTH, MAGIC_HEIGHT));
		m_MagicCir[i]->setScale(MAGIC_SCALE);
		i <= MAGIC_CIR_TOTAL_NUM/2 ? m_Bkgd1->addChild(m_MagicCir[i]):m_Bkgd2->addChild(m_MagicCir[i]);
		m_MagicCir[i]->setPosition(randFloatTo(m_GLViewSize.width), randFloatTo(m_GLViewSize.height));	
		//旋转动画设置
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
	//颜色和尺寸
	m_MagicColor = m_MagicCir[0]->getColor();
	m_MagicSize = m_MagicCir[0]->getContentSize()*m_MagicCir[0]->getScale();

	//事件监听的触屏机制
	TouchInitAndUpdate();
	keyEventInitAndUpdate();

	//定时器
	this->schedule(schedule_selector(PlayLayer::BkUpdate));
	this->schedule(schedule_selector(PlayLayer::MagicUpdate), MAGIC_UPDATE_DELTA);
	this->schedule(schedule_selector(PlayLayer::TrackUpdate), TRACK_UNPDATE_DELTA);
	this->schedule(schedule_selector(PlayLayer::DeadJudgeUpdate));
	return true;

}

//---------------------------【playDataInit()】----------------------------
// Desc：游戏数据初始化
//-------------------------------------------------------------------------
void PlayLayer::playDataInit()
{
	//----------------------------------------
	//【场景数据初始化】
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

//-------------------------【keyEventInitAndUpdate()】----------------------
// Desc:事件监听初始化及更新
//--------------------------------------------------------------------------
void PlayLayer::keyEventInitAndUpdate()
{
	EventListenerKeyboard *listener = EventListenerKeyboard::create();
	typedef cocos2d::EventKeyboard::KeyCode KeyCode;
	listener->onKeyReleased = [&](cocos2d::EventKeyboard::KeyCode code, cocos2d::Event *event){
		switch (code)
		{
		case KeyCode::KEY_BACK://响应退格键 
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

//-----------------------------【TouchInitAndUpdate()】------------------------------
// Desc：事件派发的触屏初始化和更新
//-----------------------------------------------------------------------------------
void PlayLayer::TouchInitAndUpdate()
{
	//添加触屏监听
	auto m_TouchListener = EventListenerTouchOneByOne::create();
	m_TouchListener->setSwallowTouches(true);
	m_TouchListener->onTouchBegan = [&](Touch*touch, Event*event){
		//-----------------------------------------------------
		// 此处进行MagicLock
		//-----------------------------------------------------
		if (m_bMagicPushLock) return true;
		//-----------------------------------------------------
		//对非点中目标,不再进行触屏移动事件的发生
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		if (!target->isVisible())return false;
		Vec2 localPos = target->convertToNodeSpace(touch->getLocation());
		float radius = target->getContentSize().height / 2;
		if (pow(localPos.x - radius, 2) + pow(localPos.y - radius, 2) > radius*radius)
			return false;

		//初始化为不可拖拽
		m_bDragMagic = false;
		//将魔法阵添加到链表中（附加测试)
		addMagicToListWithTest(target);
		return true;
	};

	m_TouchListener->onTouchMoved = [&](Touch*touch, Event*event){
		//-----------------------------------------------------
		// 此处进行MagicLock
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
						if (m_bDragMagic) //重复点击主角位于的魔法阵
						{
							m_MagicCir[i]->setPosition(localPos);
							m_MajorRole->setPosition(getWorldPos(m_MagicCir[i]));
						}
					}
					else //将魔法阵添加到链表中（附加测试)
						addMagicToListWithTest(m_MagicCir[i]);
					break;
				}
			}
		}
	};
	//事件监听的场景图优先
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_TouchListener, m_MagicCir[0]);
	for (int i = 1; i < MAGIC_CIR_TOTAL_NUM; i++)
		_eventDispatcher->addEventListenerWithSceneGraphPriority(m_TouchListener->clone(), m_MagicCir[i]);
}


//--------------------------------【BkUpdate()】-----------------------------------
// Desc：背景更新
//---------------------------------------------------------------------------------
void PlayLayer::BkUpdate(float delta)
{
	//背景移动
	static float accumTime = 1.0f;
	accumTime += delta;
	if(accumTime >= 1.0f)
	{
		auto moveAct = MoveBy::create(1.0f, Vec2(-BK_MOVE_AVESECOND_SPEED, 0));
		m_Bkgd1->runAction(moveAct);
		accumTime = 0.0f;
	}

	//到达临界点，背景位置变更
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

//--------------------------------【TrackUpdate()】--------------------------------
// Desc：魔法阵之间的轨迹更新
//---------------------------------------------------------------------------------
void PlayLayer::TrackUpdate(float delta)
{
	//更新主角到欲到目标的轨迹
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
	//更新魔法阵链表中魔法阵之间的轨迹
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

//------------------------------【MagicUpdate()】----------------------------------
// Desc：魔法阵更新
//---------------------------------------------------------------------------------
void PlayLayer::MagicUpdate(float delta)
{
	//【维护魔法阵链表中的元素，将超出屏幕范围或不可见的元素移除】
	maintainMagicList();

	//【构造再次出现的随机魔法阵】
	//计算同时出现魔法阵的个数
	int  onceAppearNum = 0;
	float prob = randFloatTo(1.0f);
	if (prob < 0.05f)      //5%概率不出现
		return;
	if (prob < 0.3f)      //25%概率出现1个
		onceAppearNum = 1;
	else if (prob < 0.6f) //30%概率出现2个
		onceAppearNum = 2;
	else if (prob < 0.9f) //30%概率出现3个
		onceAppearNum = 3;
	else                   //10%概率出现4个
		onceAppearNum = 4;
	
	//出现次数累计
	int appearCount = 0;
	for (int i = 0; i < MAGIC_CIR_TOTAL_NUM; i++)
	{
		//第一次或动作已完成时执行
		if (m_Blink[i] == nullptr || m_Blink[i]->isDone())
		{
			Vec2 relativePos;
			Node* bk;
			while (1)
			{
				//设置可见范围内的随机位置
				float x = randFloat(MAGIC_UPDATE_MOVESCOPE, m_GLViewSize.width+MAGIC_UPDATE_MOVESCOPE);
				srand(rand());
				float y = randFloatTo(m_GLViewSize.height);

				//选定父背景，计算相对位置
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

			//新的魔法光阵
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
//--------------------------【stayMagicDead()】------------------------------
// Desc：是否因停留魔法阵造成死亡
//---------------------------------------------------------------------------
bool PlayLayer::stayMagicDead()
{
	//后来魔法阵链表不可见，或所处的元素不可见，判定死亡
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

//--------------------------【reachMagicDead()】------------------------------
// Desc：是否因到达消失魔法阵而造成死亡(未死亡则添加新魔法阵测试)
//----------------------------------------------------------------------------
bool PlayLayer::reachMagicDead()
{
	if (!m_bMajorRDead)
	{
		bool bInside = isInMagic(m_MagicRecord, 0.5f);
		if (bInside)
		{
			//主角到达时魔法阵不可见，则判定死亡
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

//-----------------------------【MajorDeadManage()】---------------------------------
// Desc:在给定时间下, 主角根据方向移动到另一个节点
//-----------------------------------------------------------------------------------
void PlayLayer::MajorDeadManage()
{
	m_bMajorRDead = true;
	MajorDeadPerformance();
	m_MagicRecord = nullptr;
	//MagicLock
	m_bMagicPushLock = true;
}

//------------------------------【maintainMagicList()】---------------------------------
// Desc：维护魔法阵链表
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

//----------------------------【MajorDeadPerformance()】----------------------------
// Desc：主角死亡后的表现及后期清理
//----------------------------------------------------------------------------------
void PlayLayer::MajorDeadPerformance()
{
	//【死亡坠落及显示结束画面】
	float height = m_GLViewSize.height / 9.0f;
	Vec2 deadPos = Vec2(m_MajorRole->getPosition().x, height);
	m_MajorRole->MovingToNode(major_Action::DEAD, deadPos,
		m_MajorRole->getPosition().y*MAJOR_DROPTIME_IN_ONEHUNDRED_PIXELS / 100, &m_bMagicPushLock);

	//【清空魔法阵链表】
	if (!m_MagicList.empty())
		m_MagicList.clear();

	return;
}

//----------------------------------【isInMagic()】----------------------------------
// Desc：主角是否在指定魔法阵内的判断！！
//-----------------------------------------------------------------------------------
bool PlayLayer::isInMagic(cocos2d::Sprite* magic, float radScale /* =1.0f */)
{
	Vec2 magicPos = getWorldPos(magic);
	Vec2 majorPos = m_MajorRole->getPosition();
	float radius = magic->getContentSize().height / 2;
	bool bInside = (pow(magicPos.x - majorPos.x, 2) + pow(magicPos.y - majorPos.y, 2) <= pow(radius*radScale, 2));
	return bInside;
}

//-----------------------------【MovingToNode()】---------------------------------
// Desc:在给定时间下, 主角根据方向移动到另一个节点
//--------------------------------------------------------------------------------
void PlayLayer::MovingToNode(cocos2d::Sprite* dest)
{
	Vec2 srcWorldPos = m_MajorRole->getPosition();
	Vec2 destWorldPos = getWorldPos(dest);
	Vec2 direction = destWorldPos - srcWorldPos;
	//根据方向判定选则动作
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
	//执行moving动作
	m_MajorRole->MovingToNodeThenWalk(sth, destWorldPos,direction.getLength()*MAJOR_MOVETIME_IN_ONEHUNDRED_PIXELS / 100);
}

//-------------------------【addMagicToListWithTest()】---------------------------
// Desc:在给定时间下, 主角根据方向移动到另一个节点
//--------------------------------------------------------------------------------
bool PlayLayer::addMagicToListWithTest(Sprite* target)
{
	//用颜色测试
	target->setColor(Color3B(0, 255, 0));

	//对位于的点重复点击时，魔法阵可拖拽
	if (!m_MagicList.empty() && m_MagicList.back() == target)m_bDragMagic = true;

	//0个时，直接加入元素并返回;1个时，元素不重复才加入，然后返回
	if (m_MagicList.size() <= 1)
	{
		if (m_MagicList.size() == 1 && m_MagicList.back() == target)
			return false;
		else if (m_MagicList.size() == 0)
		{
			//只有第一次才添加主角		
			if (m_MajorRole->getParent() == nullptr)
			{
				//设定主角
				this->addChild(m_MajorRole, 1);				
			}
			Vec2 tarWorldPos = getWorldPos(target);
			m_MajorRole->setPosition(tarWorldPos);	
			m_MajorRole->stopAllActions();
			auto backWalk = RepeatForever::create(Spawn::create(m_MajorRole->createWalkAni(0.1f, 1), 
				MoveBy::create(1, Vec2(-BK_MOVE_AVESECOND_SPEED, 0)), nullptr));
			m_MajorRole->runAction(backWalk);
			//主角诞生
			m_bMajorRDead = false;
			m_MagicList.push_back(target);
			//播放添加音效
			SimpleAudioEngine::getInstance()->playEffect(EFFECT_BIRTH);
			return true;
		}
		if (!m_bMagicPushLock)
		{
			//移动到另一个魔法阵节点
			MovingToNode(target);
			m_MagicRecord = target;
		}	
		//----------------------------------------------------------------------------
		//在此处进行MagicLock(第一进来时切换为锁定，第二次显示调用本函数时进行解锁
		//----------------------------------------------------------------------------
		m_bMagicPushLock = !m_bMagicPushLock;	
		if (m_bMagicPushLock)return true;
		//----------------------------------------------------------------------------

		m_MagicList.push_back(target);
		return true;
	}

	//若点击的目标与链表中后两个魔法阵指针不重复，则push_back
	auto back = m_MagicList.back();
	m_MagicList.pop_back();
	auto secondBack = m_MagicList.back();
	m_MagicList.push_back(back);

	bool bPushSuc = false;
	if (back != target && secondBack != target)
	{
		if (!m_bMagicPushLock)
		{
			//移动到另一个魔法阵节点
			MovingToNode(target);
			m_MagicRecord = target;
		}
		//----------------------------------------------------------------------------
		//在此处进行MagicLock(第一进来时切换为锁定，第二次显示调用本函数时进行解锁
		//----------------------------------------------------------------------------
		m_bMagicPushLock = !m_bMagicPushLock;
		if (m_bMagicPushLock)return true;
		//----------------------------------------------------------------------------

		m_MagicList.push_back(target);
		bPushSuc = true;
	}

	//如果操作成功，检查是否有可消除的元素
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
					//播放得分音效
					SimpleAudioEngine::getInstance()->playEffect(EFFECT_ADDSCORE);
				}
				break;
			}
		}
	}
	return bPushSuc;
}