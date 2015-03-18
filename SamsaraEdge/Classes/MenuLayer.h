#pragma once
#ifndef H_MENU_LAYER
#define H_MENU_LAYER

#include <cocos2d.h>
#include "Number.h"

//��̬���͵Ĳ˵���
class MenuLayer : public cocos2d::Layer
{
public:	
	static MenuLayer* getInstance();						
	void runGameOverAni(bool* magicLock);					
	int getScore(){ return m_iScore; }
	void toNumber(int number){ m_Score->toNumber(number); }
	void runSetScoreAni(int score);
	static MenuLayer* sharedMenu;

private:
	virtual bool init() override;
	void toNumberAndPlayEffect(int number);
	void gameOverBkInit();
	void gameOverMenuInit();
	void returnMain(cocos2d::Ref* pSender);
	void oneAgain(cocos2d::Ref* pSender);	
	void oneAginFadeIn();
	
	cocos2d::FiniteTimeAction* zeroUpdateToScore(float duration);
	void updateMaxScore(int score){ m_MaxScore->toNumber(score); }
	void newComeUp(float beginScale = 4.0f, float duration = 0.6f);
	void recoverInit();

	//���ⲿ���ù��캯��������������Create����
	MenuLayer() :m_Score(nullptr),m_iScore(0){};
	virtual ~MenuLayer(){};
	CREATE_FUNC(MenuLayer);

private:
	Number* m_Score;        //��ǰ�÷�
	Number* m_MaxScore;     //���÷�
	int m_iScore;           //��ǰ����
	cocos2d::Sprite* m_NewText;						//new�ı�����
	cocos2d::MenuItemSprite* m_MenuItemOneagain;	//����һ�ֲ˵���ť

	cocos2d::Size m_GLViewSize;		//OpengGl��Ⱦ������Ļ��Χ��
	cocos2d::Sprite* m_GameOverBk;  //��Ϸ����ʱ�����ı�����
	bool* m_bMagicPushLock;			//��¼magicLock״̬����ʵʱ�ı�
};

#endif