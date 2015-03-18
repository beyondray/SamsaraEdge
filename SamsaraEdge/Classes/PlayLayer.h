#pragma  once
#ifndef H_PLAY_LAYER
#define H_PLAY_LAYER

#include "ConstValue.h"
#include <cocos2d.h>
#include <list>
class MajorRole;
class MagicCircle;
class TrackUnit;

class PlayLayer :public cocos2d::Layer
{
public:
	PlayLayer() :m_bMagicPushLock(false),m_bMajorRDead(true), 
		m_bDragMagic(false),m_MagicRecord(nullptr){};
	virtual bool init() override;
	CREATE_FUNC(PlayLayer);
	
	const MajorRole* getMajorRole() const { return m_MajorRole; }
	bool isMajorDead()const{ return m_bMajorRDead; }				//是否死亡
	void MajorDeadManage();             //主角死亡管理(死亡变现，变量调整)
private:
	void playDataInit();				//游戏数据初始化
	
	void keyEventInitAndUpdate();		//键位事件监听
	void TouchInitAndUpdate();			//触屏事件监听
	void BkUpdate(float delta);			//背景更新
	void MagicUpdate(float delta);		//魔法阵更新
	void TrackUpdate(float delta);		//魔法阵之间的轨迹更新
	void DeadJudgeUpdate(float delta);	//主角死亡判断更新

	bool addMagicToListWithTest(cocos2d::Sprite* target);  //添加魔法阵节点到链表中（包含测试)
	void MovingToNode(cocos2d::Sprite* dest);	//移动到目标节点
	void maintainMagicList();					//魔法阵链表维护
	void setMajorDeadState(bool dead){ m_bMajorRDead = dead; }		//设置死亡状态
	void MajorDeadPerformance();									//死亡表现
	bool isInMagic(cocos2d::Sprite* magic, float radScale = 1.0f);  //主角是否在指定魔法阵内的判断！！
	bool reachMagicDead();				//是否因到达消失魔法阵而造成死亡
	bool stayMagicDead();				//是否因停留魔法阵造成死亡

private:
	float m_MoveOfBklength;               //循环滚动背景长度
	cocos2d::Size m_GLViewSize;	          //屏幕视口尺寸
	cocos2d::Size m_MagicSize;            //魔法阵尺寸
	cocos2d::Sprite* m_Bkgd1;			  //滚动背景1
	cocos2d::Sprite* m_Bkgd2;			  //滚动背景2
	
	MajorRole* m_MajorRole;							//主角
	TrackUnit * m_TrackUnit[80];					//轨迹单元
	MagicCircle* m_MagicCir[MAGIC_CIR_TOTAL_NUM];	//魔法阵
	cocos2d::Sprite* m_MagicRecord;					//下一个将要到达的魔法阵记录
	cocos2d::Action* m_Blink[MAGIC_CIR_TOTAL_NUM];	//魔法阵变换动作
	std::list<cocos2d::Sprite*> m_MagicList;		//魔法阵链表
	cocos2d::Color3B m_MagicColor;					//魔法阵原来的颜色

	bool m_bDragMagic;         //可拖动魔法阵
	bool m_bMagicPushLock;     //魔法阵添加到链表中的行为是否遭到封锁？（解锁后，会进行一次显示调用)
							   //只有主角安全到达目的地时才置为false解锁
	bool m_bMajorRDead;        //主角是否死亡

	friend class MenuLayer;
};

//--------------------------【MajorReachJudgeUpdate()】------------------------------
// Desc：主角到达下一个魔法阵的判断更新
//-----------------------------------------------------------------------------------
inline void PlayLayer::DeadJudgeUpdate(float delta)
{
	//【停留魔法阵的死亡判断】
	if (stayMagicDead())return;

	//【到达消失魔法阵的死亡判断】
	if (m_MagicRecord && reachMagicDead())return;
}
#endif

