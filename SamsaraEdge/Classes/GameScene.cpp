#include "GameScene.h"
#include "PlayLayer.h"
#include "AttackLayer.h"
#include "MenuLayer.h"
//=============================================
// Desc:��Ϸ������ʼ��
//=============================================
bool GameScene::init()
{
	bool bResult = false;
	do
	{
		//��������
		CC_BREAK_IF(!Scene::init());
		//PlayLayer��
		auto playLayer = PlayLayer::create();
		CC_BREAK_IF(!playLayer);
		this->addChild(playLayer);
		
		//AttackLayer��
		auto attackLayer = AttackLayer::create();
		CC_BREAK_IF(!attackLayer);
		this->addChild(attackLayer);
		attackLayer->setPlayLayer(playLayer);

		//MenuLayer��
		auto menuLayer = MenuLayer::getInstance();
		CC_BREAK_IF(!menuLayer);
		this->addChild(menuLayer);
		//���������ɹ�
		bResult = true;
	} while (0);
	return bResult;
}