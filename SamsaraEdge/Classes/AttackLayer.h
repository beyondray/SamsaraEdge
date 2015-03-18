#ifndef H_ATTACKLAYER
#define H_ATTACKLAYER

#include <cocos2d.h>

class BeamUnit;
class TornadoUnit;
class GhostUnit;
class PlayLayer;

class AttackLayer:public cocos2d::Layer
{
public:
	AttackLayer():m_GhostComeUpTimes(0.0f){};
	virtual bool init()override;
	CREATE_FUNC(AttackLayer);

	void setPlayLayer(PlayLayer* layer){ m_PlayLayer = layer; };
	
private:
	void shootBeamOnce(float moveDuration);//一次光束射击
	void rollTornadoOnce();				//一次龙卷风舞动
	void ghostAttackOnce();             //一次鬼怪攻击
	void BeamUpdate(float delta);		//光束更新
	void TornadoUpdate(float delta);	//龙卷风更新
	void GhostUpdate(float delta);		//鬼怪更新
	void AttackUpdate(float delta);		//攻击更新

	bool rollMajorDetec(float delta);	//检测是否卷中主角
	bool shootMajorDetec(float delta);	//检测是否射中主角
	bool GAttackMajorDetec(float delta); //检测鬼怪是否攻击到主角
	void ColliDetec(float delta);		//碰撞检测

	cocos2d::Vec2 randOutArea(float borderW, float borderH);	//随机一个回型外围区域的向量
private:
	PlayLayer* m_PlayLayer;			//playLayer层
	cocos2d::Size m_GLViewSize;		//Opengl渲染区域（屏幕范围）

	BeamUnit* m_Beam;				//光束
	TornadoUnit* m_Tornado;			//龙卷风
	GhostUnit* m_Ghost;				//鬼怪

	float m_GhostComeUpTimes;			//鬼怪出现所花时间
	cocos2d::Vec2 m_GhostAttackPos;     //鬼怪攻击点
	static const float ms_GhostAttackWaitTimes;	//鬼怪攻击前累计等待时间
};
#endif