#pragma once
#ifndef H_CONST_VALUE
#define H_CONST_VALUE

#include <cocos2d.h>

//【一些定义数值】
//背景
const int LINK_COEFF = 1;
const int BK_MOVE_AVESECOND_SPEED = 60;
//菜单项
const float MENU_BUTTON_SCALE = 1.5f;

//魔法阵
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

//主角
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

//黄链，紫璇
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

//光束，龙卷
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

//数字
const int NUMBER_UNIT_WIDTH = 24;
const int NUMBER_UNIT_HEIGHT = 46;
const int NUMBER_GAPS = 1;

//【一些文件名】
//粒子
const std::string GREEN_PARTICLE_FNAME = "images/green.plist";
//背景
const std::string MENU_BK_FNAME = "images/cy0_h.jpg";
const std::string OPTIONS_BK_FNAME = "images/cy1_h.jpg";
const std::string GAMEOVER_BK_FNAME = "images/gameOverBk.png";
//静态文本
const std::string STATICTEXT_MAXLUNHUISCORE_FNAME = "images/maxlunhuiScore.png";
const std::string STATICTEXT_BKMUSIC_FNAME = "images/bkMusic.png";
const std::string STATICTEXT_ON_FNAME = "images/on.png";
const std::string STATICTEXT_OFF_FNAME = "images/off.png";
const std::string STATICTEXT_NEW_FNAME = "images/new.png";
const std::string STATICTEXT_MAXSCORE_FNAME = "images/maxScore.png";
const std::string STATICTEXT_NOWSCORE_FNAME = "images/nowScore.png";
const std::string STATICTEXT_FAIL_FNAME = "images/gameFail.png";
//按钮（主页面）
const std::string BUTTON_STARTUP_FNAME = "images/startUp.png";
const std::string BUTTON_STARTDOWN_FNAME = "images/startDown.png";
const std::string BUTTON_OPTIONSUP_FNAME = "images/optionsUp.png";
const std::string BUTTON_OPTIONSDOWN_FNAME = "images/optionsDown.png";
const std::string BUTTON_QUITUP_FNAME = "images/quitUp.png";
const std::string BUTTON_QUITDOWN_FNAME = "images/quitDown.png";
const std::string BUTTON_BACKDOWN_FNAME = "images/backDown.png";
const std::string BUTTON_BACKUP_FNAME = "images/backUp.png";
//按钮（游戏结束页面）
const std::string BUTTON_MAINMENUUP_FNAME = "images/mainMenuUp.png";
const std::string BUTTON_MAINMENUDOWN_FNAME = "images/mainMenuDown.png";
const std::string BUTTON_ONEAGAINUP_FNAME = "images/oneagainUp.png";
const std::string BUTTON_ONEAGAINDOWN_FNAME = "images/oneagainDown.png";
//火焰，冰雪地图
const std::string HUOYAN1_FNAME = "images/huoyan1_h.png";
const std::string HUOYAN2_FNAME = "images/huoyan2_h.png";
const std::string BINGXUE1_FNAME = "images/bingxue1_h.png";
const std::string BINGXUE2_FNAME = "images/bingxue2_h.png";
//数字
const std::string NOTHING_FNAME = "images/nothing.png";
const std::string PURPLENUMBER_FNAME = "images/purpleNumbers.png";
const std::string REDNUMBER_FNAME = "images/redNumbers.png";
const std::string GREENNUMBER_FNAME = "images/greenNumbers.png";
//魔法阵
const std::string MAGIC_FNAME = "images/MagicCircle_2.png";
//主角
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
//黄链，紫璇
const std::string HUANG_UNIT_FNAME = "images/huang0.png";
const std::string HUANG_UNIT_BASEFNAME = "images/huang";
const std::string XUAN_UNIT_FNAME = "images/xuan0.png";
const std::string XUAN_UNIT_BASEFNAME = "images/xuan";
const std::string TRACKUNIT_POSTFIX = "png";
//攻击元素文件
const std::string BEAM_UNIT_FNAME = "images/beam0.png";
const std::string BEAM_UNIT_BASEFNAME = "images/beam";
const std::string BEAM_UNIT_POSTFIX = "png";
const std::string TORNADO_UNIT_FNAME = "images/tornado0.png";
const std::string TORNADO_UNIT_BASEFNAME = "images/tornado";
const std::string TORNADO_UNIT_POSTFIX = "png";
const std::string GHOST_UNIT_FNAME = "images/guihua0.png";
const std::string GHOST_UNIT_BASEFNAME = "images/guihua";
const std::string GHOST_UNIT_POSTFIX = "png";

//可变化的背景及轨迹单元
extern std::string BK1_FNAME;
extern std::string BK2_FNAME;
extern std::string TRACKUNIT_BASEFNAME;
extern std::string TRACKUNIT_FNAME;
extern int TRACKUNIT_PICS;
extern int TRACKUNIT_WIDTH;
extern int TRACKUNIT_HEIGHT;
extern float TRACKUNIT_SCALE;

//【音乐文件】
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

//#define MOVE_PORTRAIT_MODE //竖屏移动模式（默认为横屏）
//#define BK_BASE_HEIGHT_MODE  //依靠背景高度完全显示（默认为依赖背景宽度）
const int BK_BASE_LENGTH = 840;   //在 BASE_XXX_MODE 中希望全部渲染的那部分长度

//一个额外的函数（不想占用一个文件，放这里了）
inline cocos2d::Vec2 getWorldPos(const cocos2d::Sprite* target)
{
	return target->getParent()->convertToWorldSpace(target->getPosition());
}
#endif