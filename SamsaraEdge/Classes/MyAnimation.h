#pragma once

#ifndef H_MYANIMATION
#define H_MYANIMATION

#include <cocos2d.h>

class MyAnimation
{
public:
	MyAnimation(){};
	~MyAnimation(){};
	//用一张大图片形式创建序列帧动画
	cocos2d::Animation* createAnimate(const std::string& filename, size_t SpriteFrameNum, size_t unitWidth,
		size_t unitHeight, size_t lines, size_t columns, float delay, size_t loops, size_t cutEveryGaps = 0);
	cocos2d::Action* createAnimateWithHide(const std::string& filename, size_t SpriteFrameNum, size_t unitWidth,
		size_t unitHeight, size_t lines, size_t columns, float delay, size_t loops, size_t cutEveryGaps = 0);
	//用多张图片形式创建序列动画帧(文件名格式：baseFilename0.postfix, baseFilename1.postfix, baseFilename2.postfix,...)
	cocos2d::Animation* createAnimateWithMulPic(const std::string& baseFilename, const std::string& postfix, int picNums, cocos2d::Rect& rect,
		float delay, size_t loops);
	cocos2d::Action* createAnimateWithHide_MulPic(const std::string& baseFilename, const std::string& postfix, int picNums, cocos2d::Rect& rect,
		float delay, size_t loops);
};

#endif