#include "MenuLayer.h"
#include "ConstValue.h"
#include "PreGameScene.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d;
using namespace CocosDenshion;

//一个静态的实例
MenuLayer* MenuLayer::sharedMenu = nullptr;
//-----------------------------【getInstance()】--------------------------------
// Desc:单例方法
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

//-----------------------------【init()】---------------------------------
// Desc:菜单内容初始化
//------------------------------------------------------------------------
bool MenuLayer::init()
{
	if (!Layer::init())
		return false;
	
	//【渲染尺寸】
	m_GLViewSize = Director::getInstance()->getVisibleSize();

	//【当前得分】
	m_Score = Number::create(NOTHING_FNAME);
	m_Score->createDigitVec(PURPLENUMBER_FNAME, NUMBER_UNIT_WIDTH, NUMBER_UNIT_HEIGHT, NUMBER_GAPS);
	m_Score->toNumber(0);
	m_Score->setScale(1.2f);
	m_Score->setPosition(Vec2(m_GLViewSize.width / 2, 4 * m_GLViewSize.height / 5));
	m_Score->setAnchorPoint(Vec2(0.05f, 0.5f));
	this->addChild(m_Score, 1);

	//【结束画面】
	gameOverBkInit();

	//【结束画面菜单】
	gameOverMenuInit();

	return true;
}

//------------------------【gameOverBkInit()】----------------------------
// Desc:游戏结束背景框架初始化
//------------------------------------------------------------------------
void MenuLayer::gameOverBkInit()
{
	//GameOver背景框
	m_GameOverBk = Sprite::create(GAMEOVER_BK_FNAME);
	m_GameOverBk->setPosition(Vec2(m_GLViewSize.width / 2, m_GLViewSize.height + m_GameOverBk->getContentSize().height/2));
	m_GameOverBk->setVisible(false);
	this->addChild(m_GameOverBk);
	//一段文字：大侠请重新来过...
	auto failText = Sprite::create(STATICTEXT_FAIL_FNAME);
	failText->setAnchorPoint(Vec2::ZERO);
	failText->setPosition(Vec2(0, 60));
	failText->setScale(0.8f);
	m_GameOverBk->addChild(failText);
	//显示分数显示有关的文本
	auto maxScore = Sprite::create(STATICTEXT_MAXSCORE_FNAME);
	maxScore->setAnchorPoint(Vec2::ONE);
	maxScore->setPosition(Vec2(m_GLViewSize.width + 10, m_GLViewSize.height + 70));
	m_GameOverBk->addChild(maxScore);
	auto nowScore = Sprite::create(STATICTEXT_NOWSCORE_FNAME);
	nowScore->setAnchorPoint(Vec2(0.2f,1));
	nowScore->setPosition(Vec2(0, m_GLViewSize.height + 70));
	m_GameOverBk->addChild(nowScore);
	//最大得分
	m_MaxScore = Number::create(NOTHING_FNAME);
	m_MaxScore->createDigitVec(REDNUMBER_FNAME, NUMBER_UNIT_WIDTH, NUMBER_UNIT_HEIGHT, NUMBER_GAPS);
	m_MaxScore->setAnchorPoint(Vec2(0,1));
	m_MaxScore->setPosition(Vec2(m_GLViewSize.width-73, m_GLViewSize.height + 25));
	m_MaxScore->setScale(0.8f);
	m_GameOverBk->addChild(m_MaxScore);
	//"New"字样
	m_NewText = Sprite::create(STATICTEXT_NEW_FNAME);
	m_NewText->setAnchorPoint(Vec2::ONE);
	m_NewText->setPosition(Vec2(m_GLViewSize.width -63, m_GLViewSize.height+28));
	m_NewText->setScale(0.8f);
	m_NewText->setVisible(false);
	m_GameOverBk->addChild(m_NewText);
}
//-------------------------【gameOverMenuInit()】--------------------------
// Desc:游戏结束背景框上的菜单项初始化
//-------------------------------------------------------------------------
void MenuLayer::gameOverMenuInit()
{
	//主菜单按钮
	auto mainMenuUp = Sprite::create(BUTTON_MAINMENUUP_FNAME);
	auto mainMenuDown = Sprite::create(BUTTON_MAINMENUDOWN_FNAME);
	mainMenuDown->setPosition(Vec2(2, -6));
	auto menuItemMain = MenuItemSprite::create(mainMenuUp, mainMenuDown, CC_CALLBACK_1(MenuLayer::returnMain, this));
	menuItemMain->setAnchorPoint(Vec2(1, 0));
	menuItemMain->setPosition(Vec2(m_GLViewSize.width - 20, 0));
	//再来一局按钮
	auto oneagainUp = Sprite::create(BUTTON_ONEAGAINUP_FNAME);
	auto oneagainDown = Sprite::create(BUTTON_ONEAGAINDOWN_FNAME);
	oneagainDown->setPosition(Vec2(8, 0));
	m_MenuItemOneagain = MenuItemSprite::create(oneagainUp, oneagainDown, CC_CALLBACK_1(MenuLayer::oneAgain, this));
	m_MenuItemOneagain->setAnchorPoint(Vec2(1, 0.5f));
	m_MenuItemOneagain->setPosition(Vec2(m_GLViewSize.width - 20, m_GLViewSize.height / 2));
	m_MenuItemOneagain->setOpacity(0);
	m_MenuItemOneagain->setVisible(false);
	//结束菜单
	auto endMenu = Menu::create(menuItemMain, m_MenuItemOneagain, nullptr);
	endMenu->ignoreAnchorPointForPosition(false);
	endMenu->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	endMenu->setPosition(Vec2(0, m_GameOverBk->getContentSize().height / 2));
	m_GameOverBk->addChild(endMenu);
}

//------------------------【runSetScoreAni()】----------------------------
// Desc:运行取得新得分时的动画
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
//---------------------------【newComeUp()】------------------------------
// Desc:更新最大分记录时的new字样动画显示
//------------------------------------------------------------------------
void MenuLayer::newComeUp(float beginScale /* = 4.0f */, float duration /* = 0.6f */)
{
	auto scale = m_NewText->getScale();
	m_NewText->setVisible(true);
	m_NewText->setScale(beginScale);
	m_NewText->runAction(cocos2d::EaseCubicActionOut::create(cocos2d::ScaleTo::create(duration, scale)));
}
//-------------------------【oneAgainFadeIn()】---------------------------
// Desc:oneAgain菜单项渐显
//------------------------------------------------------------------------
void MenuLayer::oneAginFadeIn()
{
	auto show = Show::create();
	auto oneAgainFadeIn = cocos2d::FadeIn::create(0.3f);
	m_MenuItemOneagain->runAction(Sequence::create(show, oneAgainFadeIn, nullptr));
}
//---------------------------【runGameOverAni()】-------------------------
// Desc:弹出GameOver画面及菜单(带分数显示)
// 【附加额外的最大分数据更新】
//------------------------------------------------------------------------
void MenuLayer::runGameOverAni(bool* magicPushLock)
{
	//使分数不可见
	m_Score->setVisible(false);
	//得到新数据及上次最大得分显示
	int maxScore = UserDefault::getInstance()->getIntegerForKey("maxScore", 0);
	int score = MenuLayer::getInstance()->getScore();
	m_MaxScore->toNumber(maxScore);
	
	//返回一个弹下GameOver的画面
	m_GameOverBk->setVisible(true);
	cocos2d::Sequence* sumSeq;
	auto bounce = EaseBounceInOut::create(MoveTo::create(2.0f, Vec2(m_GLViewSize.width / 2, m_GLViewSize.height / 2)));
	auto zeroUptoScore = zeroUpdateToScore(2.0f);
	auto fadeInFunc = CallFunc::create(CC_CALLBACK_0(MenuLayer::oneAginFadeIn, this));
	if (score > maxScore)
	{
		//【新数据更新】
		UserDefault::getInstance()->setIntegerForKey("maxScore", score);
		UserDefault::getInstance()->flush();
		// 添加【最大得分变化动作】,【new显示】,【oneAgain渐显】
		auto maxScorFunc = CallFunc::create(CC_CALLBACK_0(MenuLayer::updateMaxScore, this, score));
		auto newComeUpFunc = CallFunc::create(CC_CALLBACK_0(MenuLayer::newComeUp, this, 4.0f, 1.0f));
		auto fade_New_Score = Spawn::create(fadeInFunc, newComeUpFunc, maxScorFunc, nullptr);
		sumSeq = Sequence::create(bounce, zeroUptoScore, fade_New_Score, nullptr);
	}
	else
		sumSeq = Sequence::create(bounce, zeroUptoScore, fadeInFunc, nullptr);
	m_GameOverBk->runAction(sumSeq);

	//更改MagicLock
	m_bMagicPushLock = magicPushLock;
}
//---------------------------【zeroUpdateToScore()】------------------------------
// Desc:从零更新到当前分数的动画显示(需要时显示new字样动画）
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
	//创建准备后在从零更新的复合动作
	auto prepareAndzeroUptoScore = Sequence::create(CallFunc::create([&](){
		m_Score->setScale(1.1f);
		m_Score->setVisible(true);
		m_Score->setPosition(Vec2(m_GLViewSize.width*0.16f, m_GLViewSize.height - 30));
		m_Score->setAnchorPoint(cocos2d::Vec2(0, 0.5f));
	}), zeroUptoScore, nullptr);
	return prepareAndzeroUptoScore;
}
//---------------------------【toNumberAndPlayEffect()】------------------------------
// Desc:更新分数及播放得分音效
//------------------------------------------------------------------------------------
void MenuLayer::toNumberAndPlayEffect(int number)
{
	m_Score->toNumber(number);
	SimpleAudioEngine::getInstance()->playEffect(EFFECT_SCOREUP);
}
//--------------------------【recoverInit()】-----------------------------
// Desc:恢复内容相关的一些初始化数据
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
//---------------------------【returnMain()】------------------------------
// Desc:返回主菜单（按钮效果）
//-------------------------------------------------------------------------
void MenuLayer::returnMain(cocos2d::Ref* pSender)
{
	//播放点击音效
	SimpleAudioEngine::getInstance()->playEffect(EFFECT_CLICK);

	//恢复初始状态
	recoverInit();
	m_GameOverBk->setPosition(Vec2(m_GLViewSize.width / 2, m_GLViewSize.height + m_GameOverBk->getContentSize().height / 2));
	m_GameOverBk->setVisible(false);

	//切换到预备场景
	auto scene = PreGameScene::create();
	auto transition = TransitionFlipAngular::create(1.5f, scene);
	Director::getInstance()->replaceScene(transition);
}
//---------------------------【oneAgin()】---------------------------------
// Desc:再来一局（按钮效果）
//-------------------------------------------------------------------------
void MenuLayer::oneAgain(cocos2d::Ref* pSender)
{
	//播放点击音效
	SimpleAudioEngine::getInstance()->playEffect(EFFECT_CLICK);

	//先使分数不可见
	m_Score->setVisible(false);
	//弹回GameOver背景,并设置分数为0
	auto bounceAndOneAgain = Sequence::create(EaseBounceInOut::create(MoveTo::create(2.0f, Vec2(m_GLViewSize.width / 2,
		m_GLViewSize.height + m_GameOverBk->getContentSize().height / 2))), CallFunc::create([&](){
		recoverInit();		
	}),nullptr);

	m_GameOverBk->runAction(bounceAndOneAgain);
	*m_bMagicPushLock = false;
}