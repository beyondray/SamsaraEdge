#include "MenuLayer.h"
#include "ConstValue.h"
#include "PreGameScene.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d;
using namespace CocosDenshion;

//һ����̬��ʵ��
MenuLayer* MenuLayer::sharedMenu = nullptr;
//-----------------------------��getInstance()��--------------------------------
// Desc:��������
//------------------------------------------------------------------------------
MenuLayer* MenuLayer::getInstance()
{
	if (sharedMenu == nullptr)
	{
		sharedMenu = new MenuLayer();
		if (!sharedMenu || !sharedMenu->init())
		{
			CC_SAFE_DELETE(sharedMenu);
			sharedMenu = nullptr;
		}
	}
	return sharedMenu;
}

//-----------------------------��init()��---------------------------------
// Desc:�˵����ݳ�ʼ��
//------------------------------------------------------------------------
bool MenuLayer::init()
{
	if (!Layer::init())
		return false;
	
	//����Ⱦ�ߴ硿
	m_GLViewSize = Director::getInstance()->getVisibleSize();

	//����ǰ�÷֡�
	m_Score = Number::create(NOTHING_FNAME);
	m_Score->createDigitVec(PURPLENUMBER_FNAME, NUMBER_UNIT_WIDTH, NUMBER_UNIT_HEIGHT, NUMBER_GAPS);
	m_Score->toNumber(0);
	m_Score->setScale(1.2f);
	m_Score->setPosition(Vec2(m_GLViewSize.width / 2, 4 * m_GLViewSize.height / 5));
	m_Score->setAnchorPoint(Vec2(0.05f, 0.5f));
	this->addChild(m_Score, 1);

	//���������桿
	gameOverBkInit();

	//����������˵���
	gameOverMenuInit();

	return true;
}

//------------------------��gameOverBkInit()��----------------------------
// Desc:��Ϸ����������ܳ�ʼ��
//------------------------------------------------------------------------
void MenuLayer::gameOverBkInit()
{
	//GameOver������
	m_GameOverBk = Sprite::create(GAMEOVER_BK_FNAME);
	m_GameOverBk->setPosition(Vec2(m_GLViewSize.width / 2, m_GLViewSize.height + m_GameOverBk->getContentSize().height/2));
	m_GameOverBk->setVisible(false);
	this->addChild(m_GameOverBk);
	//һ�����֣���������������...
	auto failText = Sprite::create(STATICTEXT_FAIL_FNAME);
	failText->setAnchorPoint(Vec2::ZERO);
	failText->setPosition(Vec2(0, 60));
	failText->setScale(0.8f);
	m_GameOverBk->addChild(failText);
	//��ʾ������ʾ�йص��ı�
	auto maxScore = Sprite::create(STATICTEXT_MAXSCORE_FNAME);
	maxScore->setAnchorPoint(Vec2::ONE);
	maxScore->setPosition(Vec2(m_GLViewSize.width + 10, m_GLViewSize.height + 70));
	m_GameOverBk->addChild(maxScore);
	auto nowScore = Sprite::create(STATICTEXT_NOWSCORE_FNAME);
	nowScore->setAnchorPoint(Vec2(0.2f,1));
	nowScore->setPosition(Vec2(0, m_GLViewSize.height + 70));
	m_GameOverBk->addChild(nowScore);
	//���÷�
	m_MaxScore = Number::create(NOTHING_FNAME);
	m_MaxScore->createDigitVec(REDNUMBER_FNAME, NUMBER_UNIT_WIDTH, NUMBER_UNIT_HEIGHT, NUMBER_GAPS);
	m_MaxScore->setAnchorPoint(Vec2(0,1));
	m_MaxScore->setPosition(Vec2(m_GLViewSize.width-73, m_GLViewSize.height + 25));
	m_MaxScore->setScale(0.8f);
	m_GameOverBk->addChild(m_MaxScore);
	//"New"����
	m_NewText = Sprite::create(STATICTEXT_NEW_FNAME);
	m_NewText->setAnchorPoint(Vec2::ONE);
	m_NewText->setPosition(Vec2(m_GLViewSize.width -63, m_GLViewSize.height+28));
	m_NewText->setScale(0.8f);
	m_NewText->setVisible(false);
	m_GameOverBk->addChild(m_NewText);
}
//-------------------------��gameOverMenuInit()��--------------------------
// Desc:��Ϸ�����������ϵĲ˵����ʼ��
//-------------------------------------------------------------------------
void MenuLayer::gameOverMenuInit()
{
	//���˵���ť
	auto mainMenuUp = Sprite::create(BUTTON_MAINMENUUP_FNAME);
	auto mainMenuDown = Sprite::create(BUTTON_MAINMENUDOWN_FNAME);
	mainMenuDown->setPosition(Vec2(2, -6));
	auto menuItemMain = MenuItemSprite::create(mainMenuUp, mainMenuDown, CC_CALLBACK_1(MenuLayer::returnMain, this));
	menuItemMain->setAnchorPoint(Vec2(1, 0));
	menuItemMain->setPosition(Vec2(m_GLViewSize.width - 20, 0));
	//����һ�ְ�ť
	auto oneagainUp = Sprite::create(BUTTON_ONEAGAINUP_FNAME);
	auto oneagainDown = Sprite::create(BUTTON_ONEAGAINDOWN_FNAME);
	oneagainDown->setPosition(Vec2(8, 0));
	m_MenuItemOneagain = MenuItemSprite::create(oneagainUp, oneagainDown, CC_CALLBACK_1(MenuLayer::oneAgain, this));
	m_MenuItemOneagain->setAnchorPoint(Vec2(1, 0.5f));
	m_MenuItemOneagain->setPosition(Vec2(m_GLViewSize.width - 20, m_GLViewSize.height / 2));
	m_MenuItemOneagain->setOpacity(0);
	m_MenuItemOneagain->setVisible(false);
	//�����˵�
	auto endMenu = Menu::create(menuItemMain, m_MenuItemOneagain, nullptr);
	endMenu->ignoreAnchorPointForPosition(false);
	endMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	endMenu->setPosition(Vec2(0, m_GameOverBk->getContentSize().height / 2));
	m_GameOverBk->addChild(endMenu);
}

//------------------------��runSetScoreAni()��----------------------------
// Desc:����ȡ���µ÷�ʱ�Ķ���
//------------------------------------------------------------------------
void MenuLayer::runSetScoreAni(int score)
{
	m_iScore = score;
	m_Score->toNumber(m_iScore);
	float centerScale = m_Score->getChildrenCount() / 20.0f;
	m_Score->setAnchorPoint(cocos2d::Vec2(centerScale, 0.5f));
	m_Score->setScale(4.0f);
	m_Score->runAction(cocos2d::EaseCubicActionOut::create(cocos2d::ScaleTo::create(0.6f, 1.2f)));
}
//---------------------------��newComeUp()��------------------------------
// Desc:�������ּ�¼ʱ��new����������ʾ
//------------------------------------------------------------------------
void MenuLayer::newComeUp(float beginScale /* = 4.0f */, float duration /* = 0.6f */)
{
	auto scale = m_NewText->getScale();
	m_NewText->setVisible(true);
	m_NewText->setScale(beginScale);
	m_NewText->runAction(cocos2d::EaseCubicActionOut::create(cocos2d::ScaleTo::create(duration, scale)));
}
//-------------------------��oneAgainFadeIn()��---------------------------
// Desc:oneAgain�˵����
//------------------------------------------------------------------------
void MenuLayer::oneAginFadeIn()
{
	auto show = Show::create();
	auto oneAgainFadeIn = cocos2d::FadeIn::create(0.3f);
	m_MenuItemOneagain->runAction(Sequence::create(show, oneAgainFadeIn, nullptr));
}
//---------------------------��runGameOverAni()��-------------------------
// Desc:����GameOver���漰�˵�(��������ʾ)
// �����Ӷ�����������ݸ��¡�
//------------------------------------------------------------------------
void MenuLayer::runGameOverAni(bool* magicPushLock)
{
	//ʹ�������ɼ�
	m_Score->setVisible(false);
	//�õ������ݼ��ϴ����÷���ʾ
	int maxScore = UserDefault::getInstance()->getIntegerForKey("maxScore", 0);
	int score = MenuLayer::getInstance()->getScore();
	m_MaxScore->toNumber(maxScore);
	
	//����һ������GameOver�Ļ���
	m_GameOverBk->setVisible(true);
	cocos2d::Sequence* sumSeq;
	auto bounce = EaseBounceInOut::create(MoveTo::create(2.0f, Vec2(m_GLViewSize.width / 2, m_GLViewSize.height / 2)));
	auto zeroUptoScore = zeroUpdateToScore(2.0f);
	auto fadeInFunc = CallFunc::create(CC_CALLBACK_0(MenuLayer::oneAginFadeIn, this));
	if (score > maxScore)
	{
		//�������ݸ��¡�
		UserDefault::getInstance()->setIntegerForKey("maxScore", score);
		UserDefault::getInstance()->flush();
		// ��ӡ����÷ֱ仯������,��new��ʾ��,��oneAgain���ԡ�
		auto maxScorFunc = CallFunc::create(CC_CALLBACK_0(MenuLayer::updateMaxScore, this, score));
		auto newComeUpFunc = CallFunc::create(CC_CALLBACK_0(MenuLayer::newComeUp, this, 4.0f, 1.0f));
		auto fade_New_Score = Spawn::create(fadeInFunc, newComeUpFunc, maxScorFunc, nullptr);
		sumSeq = Sequence::create(bounce, zeroUptoScore, fade_New_Score, nullptr);
	}
	else
		sumSeq = Sequence::create(bounce, zeroUptoScore, fadeInFunc, nullptr);
	m_GameOverBk->runAction(sumSeq);

	//����MagicLock
	m_bMagicPushLock = magicPushLock;
}
//---------------------------��zeroUpdateToScore()��------------------------------
// Desc:������µ���ǰ�����Ķ�����ʾ(��Ҫʱ��ʾnew����������
//--------------------------------------------------------------------------------
FiniteTimeAction* MenuLayer::zeroUpdateToScore(float duration)
{
	Vector<FiniteTimeAction*> seqVec;
	float delayInterval = duration / m_iScore ;
	if (delayInterval > 0.1f)delayInterval = 0.1f;
	for (int i = 0; i <= m_iScore; i++)
	{
		auto call = CallFunc::create(CC_CALLBACK_0(MenuLayer::toNumberAndPlayEffect, this, i));
		auto delay =  DelayTime::create(delayInterval);
		seqVec.pushBack(call);
		seqVec.pushBack(delay);
	}
	auto zeroUptoScore = Sequence::create(seqVec);
	//����׼�����ڴ�����µĸ��϶���
	auto prepareAndzeroUptoScore = Sequence::create(CallFunc::create([&](){
		m_Score->setScale(1.1f);
		m_Score->setVisible(true);
		m_Score->setPosition(Vec2(m_GLViewSize.width*0.16f, m_GLViewSize.height - 30));
		m_Score->setAnchorPoint(cocos2d::Vec2(0, 0.5f));
	}), zeroUptoScore, nullptr);
	return prepareAndzeroUptoScore;
}
//---------------------------��toNumberAndPlayEffect()��------------------------------
// Desc:���·��������ŵ÷���Ч
//------------------------------------------------------------------------------------
void MenuLayer::toNumberAndPlayEffect(int number)
{
	m_Score->toNumber(number);
	SimpleAudioEngine::getInstance()->playEffect(EFFECT_SCOREUP);
}
//--------------------------��recoverInit()��-----------------------------
// Desc:�ָ�������ص�һЩ��ʼ������
//------------------------------------------------------------------------
void MenuLayer::recoverInit()
{
	m_Score->setScale(1.2f);
	m_Score->setPosition(Vec2(m_GLViewSize.width / 2, 4 * m_GLViewSize.height / 5));
	m_Score->setAnchorPoint(Vec2(0.05f, 0.5f));
	m_iScore = 0;
	runSetScoreAni(m_iScore);
	m_NewText->setVisible(false);
	m_MenuItemOneagain->setOpacity(0);
	m_MenuItemOneagain->setVisible(false);
	m_Score->setVisible(true);
}
//---------------------------��returnMain()��------------------------------
// Desc:�������˵�����ťЧ����
//-------------------------------------------------------------------------
void MenuLayer::returnMain(cocos2d::Ref* pSender)
{
	//���ŵ����Ч
	SimpleAudioEngine::getInstance()->playEffect(EFFECT_CLICK);

	//�ָ���ʼ״̬
	recoverInit();
	m_GameOverBk->setPosition(Vec2(m_GLViewSize.width / 2, m_GLViewSize.height + m_GameOverBk->getContentSize().height / 2));
	m_GameOverBk->setVisible(false);

	//�л���Ԥ������
	auto scene = PreGameScene::create();
	auto transition = TransitionFlipAngular::create(1.5f, scene);
	Director::getInstance()->replaceScene(transition);
}
//---------------------------��oneAgin()��---------------------------------
// Desc:����һ�֣���ťЧ����
//-------------------------------------------------------------------------
void MenuLayer::oneAgain(cocos2d::Ref* pSender)
{
	//���ŵ����Ч
	SimpleAudioEngine::getInstance()->playEffect(EFFECT_CLICK);

	//��ʹ�������ɼ�
	m_Score->setVisible(false);
	//����GameOver����,�����÷���Ϊ0
	auto bounceAndOneAgain = Sequence::create(EaseBounceInOut::create(MoveTo::create(2.0f, Vec2(m_GLViewSize.width / 2,
		m_GLViewSize.height + m_GameOverBk->getContentSize().height / 2))), CallFunc::create([&](){
		recoverInit();		
	}),nullptr);

	m_GameOverBk->runAction(bounceAndOneAgain);
	*m_bMagicPushLock = false;
}