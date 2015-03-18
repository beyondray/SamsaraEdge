#include "PreGameScene.h"
#include "GameScene.h"
#include "OptionsScene.h"
#include "ConstValue.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d;
using namespace CocosDenshion;

//---------------------【init()】-----------------------
// Desc:初始化场景
//------------------------------------------------------
bool PreGameScene::init()
{
	bool bResult=false;
	do 
	{	//创建场景
		CC_BREAK_IF(!Scene::init());
		//创建层
		auto layer = PreGameLayer::create();
		CC_BREAK_IF(!layer);
		//将层添加到场景
		this->addChild(layer); 
		//上述操作成功
		bResult = true; 
	} while (0);
	return bResult;
}
//---------------------【init()】-----------------------
// Desc:初始化Layer内容
//------------------------------------------------------
bool PreGameLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	bool bPlayMusic = UserDefault::getInstance()->getBoolForKey("bkgdMusic", true);
	static bool bOnce = true;
	if (bOnce && bPlayMusic)
	{
		//播放背景音乐
		SimpleAudioEngine::getInstance()->playBackgroundMusic(MUSIC_BK, true);
	}
	bOnce = false;

	//【OPENGL渲染范围的尺寸】
	m_GLViewSize = Director::getInstance()->getVisibleSize();
			
	//【菜单背景】
	auto menuBk = Sprite::create(MENU_BK_FNAME);
	int width = menuBk->getContentSize().width;
	int heigth = menuBk->getContentSize().height;
	menuBk->setScale(m_GLViewSize.width/width, m_GLViewSize.height/heigth);
	menuBk->setAnchorPoint(Vec2::ZERO);
	menuBk->setTag(3);
	this->addChild(menuBk);

	//【舞动的精灵】
	auto majorRole = MajorRole::create(MAJORROLE_JUMP_FNAME, Rect(0, 0, 167, 327));
	majorRole->setAnchorPoint(Vec2(0,0));
	majorRole->setPosition(Vec2(-majorRole->getContentSize().width*1.5f, 0));
	majorRole->setScale(1.3f);
	this->addChild(majorRole);
	//精灵动画
	auto waveup = majorRole->doSthAndMoveTo(major_Action::WAVEUP, Vec2(0, 0), 1.0f);
	auto jump = majorRole->doSthAndMoveTo(major_Action::JUMP, Vec2(m_GLViewSize.width / 8, 0), 1.0f);
	majorRole->runAction(Sequence::create(waveup, jump, CallFunc::create(CC_CALLBACK_0(PreGameLayer::WALK_CALL, this, majorRole)), nullptr));


	//【按钮项及菜单】
	//startGame游戏按钮
	auto startUp = Sprite::create(BUTTON_STARTUP_FNAME);
	auto startDown = Sprite::create(BUTTON_STARTDOWN_FNAME);
	auto menuItemStart = MenuItemSprite::create(startUp, startDown, CC_CALLBACK_1(PreGameLayer::menuStartCallBack, this));
	menuItemStart->setPosition(Vec2(0, m_GLViewSize.height / 6));
	menuItemStart->setScale(MENU_BUTTON_SCALE);
	menuItemStart->setName("start");
	startDown->setPosition(Vec2(2, 3));
	//options游戏按钮
	auto optionsUp = Sprite::create(BUTTON_OPTIONSUP_FNAME);
	auto optionsDown = Sprite::create(BUTTON_OPTIONSDOWN_FNAME);
	auto menuItemOptions = MenuItemSprite::create(optionsUp, optionsDown, CC_CALLBACK_1(PreGameLayer::menuOptionsCallBack, this));
	menuItemOptions->setScale(MENU_BUTTON_SCALE);
	menuItemOptions->setName("options");	
	menuItemOptions->setAnchorPoint(Vec2(0.41f, 0.5f));
	optionsDown->setPosition(Vec2(2, 3));
	//quit游戏按钮
	auto quitUp = Sprite::create(BUTTON_QUITUP_FNAME);
	auto quitDown = Sprite::create(BUTTON_QUITDOWN_FNAME);
	auto menuItemQuit = MenuItemSprite::create(quitUp, quitDown, CC_CALLBACK_1(PreGameLayer::menuQuitCallBack, this));
	menuItemQuit->setPosition(Vec2(0, -m_GLViewSize.height / 6));
	menuItemQuit->setScale(MENU_BUTTON_SCALE);
	menuItemQuit->setName("quit");
	menuItemQuit->setAnchorPoint(Vec2(0.27f, 0.5f));
	quitDown->setPosition(Vec2(2, 3));
	//菜单
	m_Menu = Menu::create(menuItemStart,menuItemOptions, menuItemQuit,nullptr);
	this->addChild(m_Menu);

	return true;
}

//----------------------【开始游戏】------------------------
void PreGameLayer::menuStartCallBack(cocos2d::Ref* pSender)
{
	//播放点击音效
	SimpleAudioEngine::getInstance()->playEffect(EFFECT_CLICK);

	//切换到游戏场景
	auto scene = GameScene::create();
	auto transition = TransitionFadeTR::create(1.5f, scene);	
	Director::getInstance()->replaceScene(transition);
}

//------------------------【选项】--------------------------
void PreGameLayer::menuOptionsCallBack(cocos2d::Ref* pSender)
{
	//播放点击音效
	SimpleAudioEngine::getInstance()->playEffect(EFFECT_CLICK);

	//切换到Options场景
	auto scene = OptionsScene::create();
	auto transition = TransitionPageTurn::create(1.5f, scene, false);
	Director::getInstance()->replaceScene(transition);
	
}

//----------------------【退出游戏】------------------------
void PreGameLayer::menuQuitCallBack(cocos2d::Ref* pSender)
{
	//播放点击音效
	SimpleAudioEngine::getInstance()->playEffect(EFFECT_CLICK);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
#endif
	Director::getInstance()->end();
	SimpleAudioEngine::getInstance()->end();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}