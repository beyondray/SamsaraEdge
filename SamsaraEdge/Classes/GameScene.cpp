#include "GameScene.h"
#include "PlayLayer.h"
#include "AttackLayer.h"
#include "MenuLayer.h"
//=============================================
// Desc:游戏场景初始化
//=============================================
bool GameScene::init()
{
	bool bResult = false;
	do
	{
		//创建场景
		CC_BREAK_IF(!Scene::init());
		//PlayLayer层
		auto playLayer = PlayLayer::create();
		CC_BREAK_IF(!playLayer);
		this->addChild(playLayer);
		
		//AttackLayer层
		auto attackLayer = AttackLayer::create();
		CC_BREAK_IF(!attackLayer);
		this->addChild(attackLayer);
		attackLayer->setPlayLayer(playLayer);

		//MenuLayer层
		auto menuLayer = MenuLayer::getInstance();
		CC_BREAK_IF(!menuLayer);
		this->addChild(menuLayer);
		//上述操作成功
		bResult = true;
	} while (0);
	return bResult;
}