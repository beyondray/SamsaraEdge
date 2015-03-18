#include "MagicCircle.h"
#include "ConstValue.h"
using namespace cocos2d;
//=============================================
// Desc:静态create方法
//=============================================
MagicCircle* MagicCircle::create(const std::string& filename, const Rect& rect)
{
	MagicCircle* sprite = new MagicCircle();
	if (sprite && sprite->initWithFile(filename, rect))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
	
}
//=============================================
// Desc:旋转一段时间后隐藏
//=============================================
Action* MagicCircle::rotateAndHide(float rotateTime /* = 2.0f */)
{
	auto bigger = ScaleTo::create(0.2f, 1.0f);
	auto smaller = ScaleTo::create(0.2f, MAGIC_SCALE);
	auto scaleChange = Sequence::create(bigger, smaller, nullptr);
	auto scaleTint = Spawn::create(RotateBy::create(1.5f, 3600), scaleChange, TintTo::create(1.5f, 255, 0, 0), nullptr);
	return Sequence::create(RotateBy::create(rotateTime, 12000), scaleTint, Hide::create(), nullptr);
}

