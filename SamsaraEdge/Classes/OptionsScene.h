#pragma  once

#ifndef H_OPTIONS_SCENE
#define H_OPTIONS_SCENE

#include <cocos2d.h>


class OptionsScene:public cocos2d::Scene
{
public:
	virtual bool init() override;
	CREATE_FUNC(OptionsScene);
};

class OptionsLayer:public cocos2d::Layer
{
public:
	virtual bool init() override;	
	void BkAndTextInit();
	CREATE_FUNC(OptionsLayer);
private:
	void DrawHuoyanBingxueBox(cocos2d::Sprite* firstDrawTarget);
	void ChooseMapTouchInitAndUpdate();
	void BkMusicOnOfftouchInitAndUpdate();
	void menuBackCallBack(cocos2d::Ref* pSender);

private:
	cocos2d::Size m_GLViewSize;
	std::string m_SceneSelect; //Ñ¡ÔñµÄ³¡¾°
	cocos2d::Sprite* m_Huoyan;  //»ðÑæ³¡¾°
	cocos2d::Sprite* m_Bingxue; //±ùÑ©³¡¾°
	cocos2d::DrawNode* m_DrawPen; //»­±Ê
};

#endif