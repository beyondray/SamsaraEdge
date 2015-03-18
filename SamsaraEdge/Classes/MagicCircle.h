#pragma  once
#ifndef H_MAGIC_CIRCLE
#define H_MAGIC_CIRCLE

#include "MyAnimation.h"

class MagicCircle :public cocos2d::Sprite, public MyAnimation
{
public:
	static MagicCircle* create(const std::string& filename, const cocos2d::Rect& rect);
	cocos2d::Action* rotateAndHide(float rotateTime = 2.0f);
};

#endif