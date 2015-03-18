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
	bool isMajorDead()const{ return m_bMajorRDead; }				//�Ƿ�����
	void MajorDeadManage();             //������������(�������֣���������)
private:
	void playDataInit();				//��Ϸ���ݳ�ʼ��
	
	void keyEventInitAndUpdate();		//��λ�¼�����
	void TouchInitAndUpdate();			//�����¼�����
	void BkUpdate(float delta);			//��������
	void MagicUpdate(float delta);		//ħ�������
	void TrackUpdate(float delta);		//ħ����֮��Ĺ켣����
	void DeadJudgeUpdate(float delta);	//���������жϸ���

	bool addMagicToListWithTest(cocos2d::Sprite* target);  //���ħ����ڵ㵽�����У���������)
	void MovingToNode(cocos2d::Sprite* dest);	//�ƶ���Ŀ��ڵ�
	void maintainMagicList();					//ħ��������ά��
	void setMajorDeadState(bool dead){ m_bMajorRDead = dead; }		//��������״̬
	void MajorDeadPerformance();									//��������
	bool isInMagic(cocos2d::Sprite* magic, float radScale = 1.0f);  //�����Ƿ���ָ��ħ�����ڵ��жϣ���
	bool reachMagicDead();				//�Ƿ��򵽴���ʧħ������������
	bool stayMagicDead();				//�Ƿ���ͣ��ħ�����������

private:
	float m_MoveOfBklength;               //ѭ��������������
	cocos2d::Size m_GLViewSize;	          //��Ļ�ӿڳߴ�
	cocos2d::Size m_MagicSize;            //ħ����ߴ�
	cocos2d::Sprite* m_Bkgd1;			  //��������1
	cocos2d::Sprite* m_Bkgd2;			  //��������2
	
	MajorRole* m_MajorRole;							//����
	TrackUnit * m_TrackUnit[80];					//�켣��Ԫ
	MagicCircle* m_MagicCir[MAGIC_CIR_TOTAL_NUM];	//ħ����
	cocos2d::Sprite* m_MagicRecord;					//��һ����Ҫ�����ħ�����¼
	cocos2d::Action* m_Blink[MAGIC_CIR_TOTAL_NUM];	//ħ����任����
	std::list<cocos2d::Sprite*> m_MagicList;		//ħ��������
	cocos2d::Color3B m_MagicColor;					//ħ����ԭ������ɫ

	bool m_bDragMagic;         //���϶�ħ����
	bool m_bMagicPushLock;     //ħ������ӵ������е���Ϊ�Ƿ��⵽�������������󣬻����һ����ʾ����)
							   //ֻ�����ǰ�ȫ����Ŀ�ĵ�ʱ����Ϊfalse����
	bool m_bMajorRDead;        //�����Ƿ�����

	friend class MenuLayer;
};

//--------------------------��MajorReachJudgeUpdate()��------------------------------
// Desc�����ǵ�����һ��ħ������жϸ���
//-----------------------------------------------------------------------------------
inline void PlayLayer::DeadJudgeUpdate(float delta)
{
	//��ͣ��ħ����������жϡ�
	if (stayMagicDead())return;

	//��������ʧħ����������жϡ�
	if (m_MagicRecord && reachMagicDead())return;
}
#endif

