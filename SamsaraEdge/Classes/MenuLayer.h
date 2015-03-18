#pragma once
#ifndef H_MENU_LAYER
#define H_MENU_LAYER

#include <cocos2d.h>
#include "Number.h"

//静态类型的菜单层
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

	//在外部禁用构造函数，析构函数，Create函数
	MenuLayer() :m_Score(nullptr),m_iScore(0){};
	virtual ~MenuLayer(){};
	CREATE_FUNC(MenuLayer);

private:
	Number* m_Score;        //当前得分
	Number* m_MaxScore;     //最大得分
	int m_iScore;           //当前分数
	cocos2d::Sprite* m_NewText;						//new文本字样
	cocos2d::MenuItemSprite* m_MenuItemOneagain;	//再来一局菜单按钮

	cocos2d::Size m_GLViewSize;		//OpengGl渲染区域（屏幕范围）
	cocos2d::Sprite* m_GameOverBk;  //游戏结束时弹出的背景框
	bool* m_bMagicPushLock;			//记录magicLock状态，并实时改变
};

#endif