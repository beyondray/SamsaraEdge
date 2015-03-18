#pragma once
#ifndef H_CONST_VALUE
#define H_CONST_VALUE

#include <cocos2d.h>

//��һЩ������ֵ��
//����
const int LINK_COEFF = 1;
const int BK_MOVE_AVESECOND_SPEED = 60;
//�˵���
const float MENU_BUTTON_SCALE = 1.5f;

//ħ����
const int MAGIC_CIR_TOTAL_NUM = 30;
const int MAGIC_WIDTH = 192;
const int MAGIC_HEIGHT = 192;
const int MAGIC_PIC_NUM = 11;
const int MAGIC_PIC_LINES = 3;
const int MAGIC_PIC_COLUMNS = 5;
const int MAGIC_ANIMATE_LOOPS = 2;
const int MAGIC_PIC_CUTEVERYGAPS = 1;
const float MAGIC_UPDATE_DELTA = 1.0f;
const float MAGIC_SCALE = 0.7f;
const float MAGIC_UPDATE_MOVESCOPE = 100.0f;
const float MAGIC_ROTATE_RANDTIME_BEG = 4.5f;
const float MAGIC_ROTATE_RANDTIME_END = 6.5f;

//����
const int MAJOR_JUMP_PICS = 13;
const int MAJOR_WAVE_PICS = 10;
const int MAJOR_WAVEUP_PICS = 8;
const int MAJOR_WAVEDOWN_PICS = 8;
const int MAJOR_WAVERIGHT_PICS = 9;
const int MAJOR_WALK_PICS = 10;
const int MAJOR_BACK_PICS = 9;
const int MAJOR_DEAD_PICS = 8;
const int MAJOR_REBIRTH_PICS = 8;
const int MAJOR_EXCITE_PICS = 10;
const float MAJOR_WALK_TIME = 1.0f;
const float MAJOR_DROPTIME_IN_ONEHUNDRED_PIXELS = 0.3f;
const float MAJOR_MOVETIME_IN_ONEHUNDRED_PIXELS = 0.2f;

//���������
const int HUANG_UNIT_PICS = 4;
const int HUANG_UNIT_WIDTH = 208;
const int HUANG_UNIT_HEIGHT = 141;
const float HUANG_UNIT_SCALE = 0.3f;
const int XUAN_UNIT_PICS = 3;
const int XUAN_UNIT_WIDTH = 259;
const int XUAN_UNIT_HEIGHT = 162;
const float XUAN_UNIT_SCALE = 0.4f;
const int TRACKUNIT_NUMS = 80;
const float TRACK_DURATION_IN_ONEHUNDRED_PIXELS = MAJOR_MOVETIME_IN_ONEHUNDRED_PIXELS;
const float TRACK_UNPDATE_DELTA = TRACK_DURATION_IN_ONEHUNDRED_PIXELS*0.4f;

//����������
const int BEAM_UNIT_PICS = 4;
const int BEAM_UNIT_WIDTH = 339;
const int BEAM_UNIT_HEIGHT = 138;
const float BEAM_UNIT_SCALE = 0.4f;
const int BEAM_PIXELSPEED_PER_SECOND = 600;	
const int TORNADO_UNIT_PICS = 10;
const int TORNADO_UNIT_WIDTH = 192;
const int TORNADO_UNIT_HEIGHT = 192;
const float TORNADO_UNIT_SCALE = 1.3f;
const int TORNADO_PIXELSPEED_PER_SECOND = 150;
const int GHOST_UNIT_PICS = 19;
const int GHOST_UNIT_WIDTH = 234;
const int GHOST_UNIT_HEIGHT = 267;
const float GHOST_UNIT_SCALE = 0.7f;
const float GHOST_ATTACK_ANI_TIMES = 0.95f;

//����
const int NUMBER_UNIT_WIDTH = 24;
const int NUMBER_UNIT_HEIGHT = 46;
const int NUMBER_GAPS = 1;

//��һЩ�ļ�����
//����
const std::string GREEN_PARTICLE_FNAME = "images/green.plist";
//����
const std::string MENU_BK_FNAME = "images/cy0_h.jpg";
const std::string OPTIONS_BK_FNAME = "images/cy1_h.jpg";
const std::string GAMEOVER_BK_FNAME = "images/gameOverBk.png";
//��̬�ı�
const std::string STATICTEXT_MAXLUNHUISCORE_FNAME = "images/maxlunhuiScore.png";
const std::string STATICTEXT_BKMUSIC_FNAME = "images/bkMusic.png";
const std::string STATICTEXT_ON_FNAME = "images/on.png";
const std::string STATICTEXT_OFF_FNAME = "images/off.png";
const std::string STATICTEXT_NEW_FNAME = "images/new.png";
const std::string STATICTEXT_MAXSCORE_FNAME = "images/maxScore.png";
const std::string STATICTEXT_NOWSCORE_FNAME = "images/nowScore.png";
const std::string STATICTEXT_FAIL_FNAME = "images/gameFail.png";
//��ť����ҳ�棩
const std::string BUTTON_STARTUP_FNAME = "images/startUp.png";
const std::string BUTTON_STARTDOWN_FNAME = "images/startDown.png";
const std::string BUTTON_OPTIONSUP_FNAME = "images/optionsUp.png";
const std::string BUTTON_OPTIONSDOWN_FNAME = "images/optionsDown.png";
const std::string BUTTON_QUITUP_FNAME = "images/quitUp.png";
const std::string BUTTON_QUITDOWN_FNAME = "images/quitDown.png";
const std::string BUTTON_BACKDOWN_FNAME = "images/backDown.png";
const std::string BUTTON_BACKUP_FNAME = "images/backUp.png";
//��ť����Ϸ����ҳ�棩
const std::string BUTTON_MAINMENUUP_FNAME = "images/mainMenuUp.png";
const std::string BUTTON_MAINMENUDOWN_FNAME = "images/mainMenuDown.png";
const std::string BUTTON_ONEAGAINUP_FNAME = "images/oneagainUp.png";
const std::string BUTTON_ONEAGAINDOWN_FNAME = "images/oneagainDown.png";
//���棬��ѩ��ͼ
const std::string HUOYAN1_FNAME = "images/huoyan1_h.png";
const std::string HUOYAN2_FNAME = "images/huoyan2_h.png";
const std::string BINGXUE1_FNAME = "images/bingxue1_h.png";
const std::string BINGXUE2_FNAME = "images/bingxue2_h.png";
//����
const std::string NOTHING_FNAME = "images/nothing.png";
const std::string PURPLENUMBER_FNAME = "images/purpleNumbers.png";
const std::string REDNUMBER_FNAME = "images/redNumbers.png";
const std::string GREENNUMBER_FNAME = "images/greenNumbers.png";
//ħ����
const std::string MAGIC_FNAME = "images/MagicCircle_2.png";
//����
const std::string MAJORROLE_JUMP_FNAME = "images/major_jump.png";
const std::string MAJORROLE_WAVE_FNAME = "images/major_wave.png";
const std::string MAJORROLE_WAVEUP_FNAME = "images/major_waveup.png";
const std::string MAJORROLE_WAVERIGHT_FNAME = "images/major_waveright.png";
const std::string MAJORROLE_WAVEDOWN_FNAME = "images/major_wavedown.png";
const std::string MAJORROLE_WALK_FNAME = "images/major_walk.png";
const std::string MAJORROLE_BACK_FNAME = "images/major_back.png";
const std::string MAJORROLE_DEAD_FNAME = "images/major_dead.png";
const std::string MAJORROLE_REBIRTH_FNAME = "images/major_rebirth.png";
const std::string MAJORROLE_EXCITE_FNAME = "images/major_excit.png";
//���������
const std::string HUANG_UNIT_FNAME = "images/huang0.png";
const std::string HUANG_UNIT_BASEFNAME = "images/huang";
const std::string XUAN_UNIT_FNAME = "images/xuan0.png";
const std::string XUAN_UNIT_BASEFNAME = "images/xuan";
const std::string TRACKUNIT_POSTFIX = "png";
//����Ԫ���ļ�
const std::string BEAM_UNIT_FNAME = "images/beam0.png";
const std::string BEAM_UNIT_BASEFNAME = "images/beam";
const std::string BEAM_UNIT_POSTFIX = "png";
const std::string TORNADO_UNIT_FNAME = "images/tornado0.png";
const std::string TORNADO_UNIT_BASEFNAME = "images/tornado";
const std::string TORNADO_UNIT_POSTFIX = "png";
const std::string GHOST_UNIT_FNAME = "images/guihua0.png";
const std::string GHOST_UNIT_BASEFNAME = "images/guihua";
const std::string GHOST_UNIT_POSTFIX = "png";

//�ɱ仯�ı������켣��Ԫ
extern std::string BK1_FNAME;
extern std::string BK2_FNAME;
extern std::string TRACKUNIT_BASEFNAME;
extern std::string TRACKUNIT_FNAME;
extern int TRACKUNIT_PICS;
extern int TRACKUNIT_WIDTH;
extern int TRACKUNIT_HEIGHT;
extern float TRACKUNIT_SCALE;

//�������ļ���
extern char* EFFECT_CLICK;
extern char* EFFECT_CHOOSEMAP;
extern char* EFFECT_WAVE;
extern char* EFFECT_WAVERIGHT;
extern char* MUSIC_BK;
extern char* EFFECT_BACKJUMP;
extern char* EFFECT_ADDSCORE;
extern char* EFFECT_DEAD;
extern char* EFFECT_SCOREUP;
extern char* EFFECT_BIRTH;

//#define MOVE_PORTRAIT_MODE //�����ƶ�ģʽ��Ĭ��Ϊ������
//#define BK_BASE_HEIGHT_MODE  //���������߶���ȫ��ʾ��Ĭ��Ϊ����������ȣ�
const int BK_BASE_LENGTH = 840;   //�� BASE_XXX_MODE ��ϣ��ȫ����Ⱦ���ǲ��ֳ���

//һ������ĺ���������ռ��һ���ļ����������ˣ�
inline cocos2d::Vec2 getWorldPos(const cocos2d::Sprite* target)
{
	return target->getParent()->convertToWorldSpace(target->getPosition());
}
#endif