#include "PreGameScene.h"
#include "GameScene.h"
#include "OptionsScene.h"
#include "ConstValue.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d;
using namespace CocosDenshion;

//---------------------��init()��-----------------------
// Desc:��ʼ������
//------------------------------------------------------
bool PreGameScene::init()
{
	bool bResult=false;
	do 
	{	//��������
		CC_BREAK_IF(!Scene::init());
		//������
		auto layer = PreGameLayer::create();
		CC_BREAK_IF(!layer);
		//������ӵ�����
		this->addChild(layer); 
		//���������ɹ�
		bResult = true; 
	} while (0);
	return bResult;
}
//---------------------��init()��-----------------------
// Desc:��ʼ��Layer����
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
		//���ű�������
		SimpleAudioEngine::getInstance()->playBackgroundMusic(MUSIC_BK, true);
	}
	bOnce = false;

	//��OPENGL��Ⱦ��Χ�ĳߴ硿
	m_GLViewSize = Director::getInstance()->getVisibleSize();
			
	//���˵�������
	auto menuBk = Sprite::create(MENU_BK_FNAME);
	int width = menuBk->getContentSize().width;
	int heigth = menuBk->getContentSize().height;
	menuBk->setScale(m_GLViewSize.width/width, m_GLViewSize.height/heigth);
	menuBk->setAnchorPoint(Vec2::ZERO);
	menuBk->setTag(3);
	this->addChild(menuBk);

	//���趯�ľ��顿
	auto majorRole = MajorRole::create(MAJORROLE_JUMP_FNAME, Rect(0, 0, 167, 327));
	majorRole->setAnchorPoint(Vec2(0,0));
	majorRole->setPosition(Vec2(-majorRole->getContentSize().width*1.5f, 0));
	majorRole->setScale(1.3f);
	this->addChild(majorRole);
	//���鶯��
	auto waveup = majorRole->doSthAndMoveTo(major_Action::WAVEUP, Vec2(0, 0), 1.0f);
	auto jump = majorRole->doSthAndMoveTo(major_Action::JUMP, Vec2(m_GLViewSize.width / 8, 0), 1.0f);
	majorRole->runAction(Sequence::create(waveup, jump, CallFunc::create(CC_CALLBACK_0(PreGameLayer::WALK_CALL, this, majorRole)), nullptr));


	//����ť��˵���
	//startGame��Ϸ��ť
	auto startUp = Sprite::create(BUTTON_STARTUP_FNAME);
	auto startDown = Sprite::create(BUTTON_STARTDOWN_FNAME);
	auto menuItemStart = MenuItemSprite::create(startUp, startDown, CC_CALLBACK_1(PreGameLayer::menuStartCallBack, this));
	menuItemStart->setPosition(Vec2(0, m_GLViewSize.height / 6));
	menuItemStart->setScale(MENU_BUTTON_SCALE);
	menuItemStart->setName("start");
	startDown->setPosition(Vec2(2, 3));
	//options��Ϸ��ť
	auto optionsUp = Sprite::create(BUTTON_OPTIONSUP_FNAME);
	auto optionsDown = Sprite::create(BUTTON_OPTIONSDOWN_FNAME);
	auto menuItemOptions = MenuItemSprite::create(optionsUp, optionsDown, CC_CALLBACK_1(PreGameLayer::menuOptionsCallBack, this));
	menuItemOptions->setScale(MENU_BUTTON_SCALE);
	menuItemOptions->setName("options");	
	menuItemOptions->setAnchorPoint(Vec2(0.41f, 0.5f));
	optionsDown->setPosition(Vec2(2, 3));
	//quit��Ϸ��ť
	auto quitUp = Sprite::create(BUTTON_QUITUP_FNAME);
	auto quitDown = Sprite::create(BUTTON_QUITDOWN_FNAME);
	auto menuItemQuit = MenuItemSprite::create(quitUp, quitDown, CC_CALLBACK_1(PreGameLayer::menuQuitCallBack, this));
	menuItemQuit->setPosition(Vec2(0, -m_GLViewSize.height / 6));
	menuItemQuit->setScale(MENU_BUTTON_SCALE);
	menuItemQuit->setName("quit");
	menuItemQuit->setAnchorPoint(Vec2(0.27f, 0.5f));
	quitDown->setPosition(Vec2(2, 3));
	//�˵�
	m_Menu = Menu::create(menuItemStart,menuItemOptions, menuItemQuit,nullptr);
	this->addChild(m_Menu);

	return true;
}

//----------------------����ʼ��Ϸ��------------------------
void PreGameLayer::menuStartCallBack(cocos2d::Ref* pSender)
{
	//���ŵ����Ч
	SimpleAudioEngine::getInstance()->playEffect(EFFECT_CLICK);

	//�л�����Ϸ����
	auto scene = GameScene::create();
	auto transition = TransitionFadeTR::create(1.5f, scene);	
	Director::getInstance()->replaceScene(transition);
}

//------------------------��ѡ�--------------------------
void PreGameLayer::menuOptionsCallBack(cocos2d::Ref* pSender)
{
	//���ŵ����Ч
	SimpleAudioEngine::getInstance()->playEffect(EFFECT_CLICK);

	//�л���Options����
	auto scene = OptionsScene::create();
	auto transition = TransitionPageTurn::create(1.5f, scene, false);
	Director::getInstance()->replaceScene(transition);
	
}

//----------------------���˳���Ϸ��------------------------
void PreGameLayer::menuQuitCallBack(cocos2d::Ref* pSender)
{
	//���ŵ����Ч
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