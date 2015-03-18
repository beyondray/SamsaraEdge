#include "OptionsScene.h"
#include "PreGameScene.h"
#include "ConstValue.h"
#include "Number.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d;
using namespace CocosDenshion;

//---------------------��init()��-----------------------
// Desc:��ʼ������
//------------------------------------------------------
bool OptionsScene::init()
{
	bool bResult = false;
	do 
	{
		//��������
		CC_BREAK_IF(!Scene::init());
		//������
		auto layer = OptionsLayer::create();
		CC_BREAK_IF(!layer);
		this->addChild(layer);
		bResult = true;
	} while (0);
	return bResult;
}

//---------------------��init()��-----------------------
// Desc:��ʼ��Layer����
//------------------------------------------------------
bool OptionsLayer::init()
{
	//Ԥ�ȳ�ʼ��
	if (!Layer::init())
		return false;
	//��Ⱦ�ߴ�
	m_GLViewSize = Director::getInstance()->getVisibleSize();

	//���������ϵľ�̬�ı�
	BkAndTextInit();

	//��������
	m_SceneSelect = UserDefault::getInstance()->getStringForKey("scene", "huoyan");

	//ѡ�񳡾�
	m_Huoyan = Sprite::create(HUOYAN1_FNAME);
	m_Bingxue = Sprite::create(BINGXUE1_FNAME);
	m_DrawPen = DrawNode::create();
	this->addChild(m_DrawPen);
	this->addChild(m_Huoyan);
	this->addChild(m_Bingxue);

	//�趨������λ��
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
	//���߽��
	DrawHuoyanBingxueBox(huo_or_bing);

	//���ܲ˵���--back
	//back��ť
	auto backDown = Sprite::create(BUTTON_BACKDOWN_FNAME);
	auto backUp = Sprite::create(BUTTON_BACKUP_FNAME);
	backDown->setPosition(Vec2(2, 3));
	auto menuItemBack = MenuItemSprite::create(backUp, backDown, CC_CALLBACK_1(OptionsLayer::menuBackCallBack, this));
	menuItemBack->setAnchorPoint(Vec2(0.5f,0));
	menuItemBack->setPosition(Vec2(m_GLViewSize.width, 0));
	menuItemBack->setScale(1.1f);

	//���˵���
	auto menu = Menu::create(menuItemBack, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	//��������
	ChooseMapTouchInitAndUpdate();
	BkMusicOnOfftouchInitAndUpdate();
	return true;
}
//======================================================
// Desc:�������ı���ʼ��
//======================================================
void OptionsLayer::BkAndTextInit()
{
	//����
	auto optionsBk = Sprite::create(OPTIONS_BK_FNAME);
	optionsBk->setAnchorPoint(Vec2(0, 0));
	this->addChild(optionsBk);

	//������ֻط֡��ľ�̬���ı�
	auto maxScoreText = Sprite::create(STATICTEXT_MAXLUNHUISCORE_FNAME);
	maxScoreText->setAnchorPoint(Vec2(0, 1));
	maxScoreText->setPosition(Vec2(0, m_GLViewSize.height));
	optionsBk->addChild(maxScoreText);

	//������ֻط֡�����ͼƬ
	int maxScore = UserDefault::getInstance()->getIntegerForKey("maxScore", 0);
	CCLOG("%d", maxScore);
	auto number = Number::create(NOTHING_FNAME);
	number->createDigitVec(GREENNUMBER_FNAME, NUMBER_UNIT_WIDTH, NUMBER_UNIT_HEIGHT, NUMBER_GAPS);
	number->toNumber(maxScore);
	this->addChild(number);
	number->setAnchorPoint(Vec2(0, 1));
	number->setScale(1.3f);
	number->setPosition(Vec2(maxScoreText->getContentSize().width, m_GLViewSize.height));

	//���������֡��ľ�̬�ı�
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
// Desc:Back��ť
//======================================================
void OptionsLayer::menuBackCallBack(cocos2d::Ref* pSender)
{
	//���ŵ����Ч
	SimpleAudioEngine::getInstance()->playEffect(EFFECT_CLICK);

	//����ѡ�񳡾�������,���������Ƿ񲥷�����
	UserDefault::getInstance()->setStringForKey("scene", m_SceneSelect);
	UserDefault::getInstance()->setBoolForKey("bkgdMusic", this->getChildByName("On")->isVisible());
	UserDefault::getInstance()->flush();

	//�л���Ԥ������
	auto scene = PreGameScene::create();
	auto transition = TransitionMoveInT::create(1.5f, scene);
	Director::getInstance()->replaceScene(transition);
}

//======================================================
// Desc:���On,Off�����л���ť�ļ���
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
			//������Ч
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
// Desc:���ѡ���ͼ�ļ���
//======================================================
void OptionsLayer::ChooseMapTouchInitAndUpdate()
{
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [&](Touch*touch, Event*event){
		auto target = static_cast<Sprite*>(event->getCurrentTarget());
		if (target->getBoundingBox().containsPoint(target->getParent()->convertToNodeSpace(touch->getLocation())))
		{
			//���ŵ�ͼѡ����Ч
			SimpleAudioEngine::getInstance()->playEffect(EFFECT_CHOOSEMAP, false, 1.0f);
			//�߿����
			DrawHuoyanBingxueBox(target);
			//ת������
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
// Desc:����ͱ�ѩ��ͼ��Χ��Ļ���
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