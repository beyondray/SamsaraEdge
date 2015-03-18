#pragma once
#ifndef H_NUMBER
#define H_NUMBER

#include<cocos2d.h>

class Number:public cocos2d::Sprite
{
public:
	CREATE_FUNC(Number);
	static Number* create(std::string fileName);
	void createDigitVec(std::string picfileName, size_t unitWidth, size_t unitHeight, size_t cutEveryGaps=0);
	cocos2d::SpriteFrame* getDigit(int index);
	void toNumber(size_t number);
private:
	cocos2d::CCSpriteFrame* m_Digit[10]; //0-9Ê®¸öÊý×Ö
};

#endif