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
	void shootBeamOnce(float moveDuration);//һ�ι������
	void rollTornadoOnce();				//һ��������趯
	void ghostAttackOnce();             //һ�ι�ֹ���
	void BeamUpdate(float delta);		//��������
	void TornadoUpdate(float delta);	//��������
	void GhostUpdate(float delta);		//��ָ���
	void AttackUpdate(float delta);		//��������

	bool rollMajorDetec(float delta);	//����Ƿ��������
	bool shootMajorDetec(float delta);	//����Ƿ���������
	bool GAttackMajorDetec(float delta); //������Ƿ񹥻�������
	void ColliDetec(float delta);		//��ײ���

	cocos2d::Vec2 randOutArea(float borderW, float borderH);	//���һ��������Χ���������
private:
	PlayLayer* m_PlayLayer;			//playLayer��
	cocos2d::Size m_GLViewSize;		//Opengl��Ⱦ������Ļ��Χ��

	BeamUnit* m_Beam;				//����
	TornadoUnit* m_Tornado;			//�����
	GhostUnit* m_Ghost;				//���

	float m_GhostComeUpTimes;			//��ֳ�������ʱ��
	cocos2d::Vec2 m_GhostAttackPos;     //��ֹ�����
	static const float ms_GhostAttackWaitTimes;	//��ֹ���ǰ�ۼƵȴ�ʱ��
};
#endif