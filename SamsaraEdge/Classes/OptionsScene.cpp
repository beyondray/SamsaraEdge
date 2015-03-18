#include "OptionsScene.h"
#include "PreGameScene.h"
#include "ConstValue.h"
#include "Number.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d;
using namespace CocosDenshion;

//---------------------【init()】-----------------------
// Desc:初始化场景
//------------------------------------------------------
bool OptionsScene::init()
{
	bool bResult = false;
	do 
	{
		//创建场景
		CC_BREAK_IF(!Scene::init());
		//创建层
		auto layer = OptionsLayer::create();
		CC_BREAK_IF(!layer);
		this->addChild(layer);
		bResult = true;
	} while (0);
	return bResult;
}

//---------------------【init()】-----------------------
// Desc:初始化Layer内容
//------------------------------------------------------
bool OptionsLayer::init()
{
	//预先初始化
	if (!Layer::init())
		return false;
	//渲染尺寸
	m_GLViewSize = Director::getInstance()->getVisibleSize();

	//背景及其上的静态文本
	BkAndTextInit();

	//场景数据
	m_SceneSelect = UserDefault::getInstance()->getStringForKey("scene", "huoyan");

	//选择场景
	m_Huoyan = Sprite::create(HUOYAN1_FNAME);
	m_Bingxue = Sprite::create(BINGXUE1_FNAME);
	m_DrawPen = DrawNode::create();
	this->addChild(m_DrawPen);
	this->addChild(m_Huoyan);
	this->addChild(m_Bingxue);

	//设定比例和位置
	float scaleX = 0.25f;
	auto huo_or_bing = m_SceneSelect == "huoyan" ?m_Huoyan:m_Bingxue;
	for (int i = 0; i < 2; i++)
	{
		huo_or_bing->setScaleX(scaleX);
		float wantContentHeight = huo_or_bing->getContentSize().width*m_GLViewSize.height / m_GLViewSize.width;
		huo_or_bing->setScaleY(scaleX*wantContentHeight / huo_or_bing->getContentSize().height);
		huo_or_bing == m_Huoyan?huo_or_bing->setPosition(Vec2(4*m_GLViewSize.width / 5, 3*m_GLViewSize.height / 4)):
			huo_or_bing->setPosition(Vec2(4 * m_GLViewSize.width / 5, 2 * m_GLViewSize.height / 5));
		huo_or_bing = (huo_or_bing == m_Huoyan?m_Bingxue:m_Huoyan);
	}
	//画边界框
	DrawHuoyanBingxueBox(huo_or_bing);

	//【总菜单】--back
	//back按钮
	auto backDown = Sprite::create(BUTTON_BACKDOWN_FNAME);
	auto backUp = Sprite::create(BUTTON_BACKUP_FNAME);
	backDown->setPosition(Vec2(2, 3));
	auto menuItemBack = MenuItemSprite::create(backUp, backDown, CC_CALLBACK_1(OptionsLayer::menuBackCallBack, this));
	menuItemBack->setAnchorPoint(Vec2(0.5f,0));
	menuItemBack->setPosition(Vec2(m_GLViewSize.width, 0));
	menuItemBack->setScale(1.1f);

	//【菜单】
	auto menu = Menu::create(menuItemBack, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	//触屏设置
	ChooseMapTouchInitAndUpdate();
	BkMusicOnOfftouchInitAndUpdate();
	return true;
}
//======================================================
// Desc:背景和文本初始化
//======================================================
void OptionsLayer::BkAndTextInit()
{
	//背景
	auto optionsBk = Sprite::create(OPTIONS_BK_FNAME);
	optionsBk->setAnchorPoint(Vec2(0, 0));
	this->addChild(optionsBk);

	//“最大轮回分”的静态的文本
	auto maxScoreText = Sprite::create(STATICTEXT_MAXLUNHUISCORE_FNAME);
	maxScoreText->setAnchorPoint(Vec2(0, 1));
	maxScoreText->setPosition(Vec2(0, m_GLViewSize.height));
	optionsBk->addChild(maxScoreText);

	//“最大轮回分”数字图片
	int maxScore = UserDefault::getInstance()->getIntegerForKey("maxScore", 0);
	CCLOG("%d", maxScore);
	auto number = Number::create(NOTHING_FNAME);
	number->createDigitVec(GREENNUMBER_FNAME, NUMBER_UNIT_WIDTH, NUMBER_UNIT_HEIGHT, NUMBER_GAPS);
	number->toNumber(maxScore);
	this->addChild(number);
	number->setAnchorPoint(Vec2(0, 1));
	number->setScale(1.3f);
	number->setPosition(Vec2(maxScoreText->getContentSize().width, m_GLViewSize.height));

	//“背景音乐”的静态文本
	auto backMusicText = Sprite::create(STATICTEXT_BKMUSIC_FNAME);
	backMusicText->setAnchorPoint(Vec2(0, 1));
	backMusicText->setPosition(Vec2(0, m_GLViewSize.height - maxScoreText->getContentSize().height));
	optionsBk->addChild(backMusicText);

	//On,Off
	auto On = Sprite::create(STATICTEXT_ON_FNAME);
	On->setName("On");
	auto Off = Sprite::create(STATICTEXT_OFF_FNAME);
	Off->setName("Off");
	On->setAnchorPoint(Vec2(0, 1));
	On->setPosition(Vec2(backMusicText->getContentSize().width-20, m_GLViewSize.height - maxScoreText->getContentSize().height));
	Off->setAnchorPoint(Vec2(0, 1));
	Off->setPosition(Vec2(backMusicText->getContentSize().width-20, m_GLViewSize.height - maxScoreText->getContentSize().height));
	this->addChild(On);
	this->addChild(Off);
	bool bPlayMusic = UserDefault::getInstance()->getBoolForKey("bkgdMusic", true);
	bPlayMusic ? Off->setVisible(false) : On->setVisible(false);
}

//======================================================
// Desc:Back按钮
//======================================================
void OptionsLayer::menuBackCallBack(cocos2d::Ref* pSender)
{
	//播放点击音效
	SimpleAudioEngine::getInstance()->playEffect(EFFECT_CLICK);

	//设置选择场景的数据,背景音乐是否播放数据
	UserDefault::getInstance()->setStringForKey("scene", m_SceneSelect);
	UserDefault::getInstance()->setBoolForKey("bkgdMusic", this->getChildByName("On")->isVisible());
	UserDefault::getInstance()->flush();

	//切换到预备场景
	auto scene = PreGameScene::create();
	auto transition = TransitionMoveInT::create(1.5f, scene);
	Director::getInstance()->replaceScene(transition);
}

//======================================================
// Desc:点击On,Off音乐切换按钮的监听
//======================================================
void OptionsLayer::BkMusicOnOfftouchInitAndUpdate()
{
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [&](Touch*touch, Event*event){
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		if (target->isVisible() && target->getBoundingBox().containsPoint(target->getParent()->convertToNodeSpace(touch->getLocation())))
		{
			target->setVisible(false);
			//播放音效
			SimpleAudioEngine::getInstance()->playEffect(EFFECT_CLICK);
			bool bMusicPlayed = SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying();
			if (target->getName() == "On")
			{
				this->getChildByName("Off")->setVisible(true);
				if (bMusicPlayed)
				{
					SimpleAudioEngine::getInstance()->stopBackgroundMusic();
				}	
			}
			else if (target->getName() == "Off")
			{
				this->getChildByName("On")->setVisible(true);
				SimpleAudioEngine::getInstance()->playBackgroundMusic(MUSIC_BK, true);
			}
			return true;
		}
		return false;
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this->getChildByName("On"));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener->clone(), this->getChildByName("Off"));
}
//======================================================
// Desc:点击选择地图的监听
//======================================================
void OptionsLayer::ChooseMapTouchInitAndUpdate()
{
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [&](Touch*touch, Event*event){
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		if (target->getBoundingBox().containsPoint(target->getParent()->convertToNodeSpace(touch->getLocation())))
		{
			//播放地图选择音效
			SimpleAudioEngine::getInstance()->playEffect(EFFECT_CHOOSEMAP, false, 1.0f);
			//线框更新
			DrawHuoyanBingxueBox(target);
			//转换场景
			if (target == m_Bingxue)
			{			
				m_SceneSelect = "bingxue";
			}
			else
			{				
				m_SceneSelect = "huoyan";
			}
			return true;
		}
		return false;
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, m_Huoyan);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener->clone(), m_Bingxue);
}
//======================================================
// Desc:火焰和冰雪地图外围框的画制
//======================================================
void OptionsLayer::DrawHuoyanBingxueBox(Sprite* firstDrawTarget)
{
	for (int i = 0; i < 2; i++)
	{
		auto rect = firstDrawTarget->getBoundingBox();
		float radius = 2.0f;
		auto color = i==0? Color4F::GREEN : Color4F::WHITE;
		m_DrawPen->drawSegment(Vec2(rect.getMinX(), rect.getMinY()), Vec2(rect.getMaxX(), rect.getMinY()), radius, color);
		m_DrawPen->drawSegment(Vec2(rect.getMaxX(), rect.getMinY()), Vec2(rect.getMaxX(), rect.getMaxY()), radius, color);
		m_DrawPen->drawSegment(Vec2(rect.getMaxX(), rect.getMaxY()), Vec2(rect.getMinX(), rect.getMaxY()), radius, color);
		m_DrawPen->drawSegment(Vec2(rect.getMinX(), rect.getMaxY()), Vec2(rect.getMinX(), rect.getMinY()), radius, color);
		firstDrawTarget = (firstDrawTarget == m_Huoyan? m_Bingxue:m_Huoyan);
	}
}