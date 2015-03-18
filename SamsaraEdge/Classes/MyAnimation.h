#pragma once

#ifndef H_MYANIMATION
#define H_MYANIMATION

#include <cocos2d.h>

class MyAnimation
{
public:
	MyAnimation(){};
	~MyAnimation(){};
	//��һ�Ŵ�ͼƬ��ʽ��������֡����
	cocos2d::Animation* createAnimate(const std::string& filename, size_t SpriteFrameNum, size_t unitWidth,
		size_t unitHeight, size_t lines, size_t columns, float delay, size_t loops, size_t cutEveryGaps = 0);
	cocos2d::Action* createAnimateWithHide(const std::string& filename, size_t SpriteFrameNum, size_t unitWidth,
		size_t unitHeight, size_t lines, size_t columns, float delay, size_t loops, size_t cutEveryGaps = 0);
	//�ö���ͼƬ��ʽ�������ж���֡(�ļ�����ʽ��baseFilename0.postfix, baseFilename1.postfix, baseFilename2.postfix,...)
	cocos2d::Animation* createAnimateWithMulPic(const std::string& baseFilename, const std::string& postfix, int picNums, cocos2d::Rect& rect,
		float delay, size_t loops);
	cocos2d::Action* createAnimateWithHide_MulPic(const std::string& baseFilename, const std::string& postfix, int picNums, cocos2d::Rect& rect,
		float delay, size_t loops);
};

#endif