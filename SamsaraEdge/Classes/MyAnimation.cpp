#include "MyAnimation.h"
using namespace cocos2d;

//======================================================
// Desc:创建一个动画
//======================================================
Animation* MyAnimation::createAnimate(const std::string& filename, size_t SpriteFrameNum, size_t unitWidth,
	size_t unitHeight, size_t lines, size_t columns, float delay, size_t loops, size_t cutEveryGaps /* = 0 */)
{
	Vector<SpriteFrame*> spVec;
	int gridWidth = unitWidth - 2 * cutEveryGaps;
	int gridHeight = unitHeight - 2 * cutEveryGaps;
	size_t iCount = 0;
	for (size_t i = 0; i < lines; i++)
	{
		for (size_t j = 0; j < columns; j++)
		{
			auto sp = SpriteFrame::create(filename, Rect(j*unitWidth + cutEveryGaps, i*unitHeight + cutEveryGaps, gridWidth, gridHeight));
			spVec.pushBack(sp);
			iCount++;
			if (iCount >= SpriteFrameNum)
				break;
		}
	}
	auto animation = Animation::createWithSpriteFrames(spVec, delay, loops);
	return animation;
}
//======================================================
// Desc:创建一个动画并附带隐藏效果
//======================================================
cocos2d::Action* MyAnimation::createAnimateWithHide(const std::string& filename, size_t SpriteFrameNum,
	size_t unitWidth, size_t unitHeight, size_t lines, size_t columns, float delay, size_t loops,
	size_t cutEveryGaps /* = 0 */)
{
	auto animation = createAnimate(filename, SpriteFrameNum, unitWidth, unitHeight, lines, columns, delay, loops, cutEveryGaps);
	auto action = Sequence::create(Animate::create(animation), Hide::create(), nullptr);
	return action;
}
//============================================================================
// Desc:用多张图片形式创建序列动画帧(【文件名格式】：
// baseFilename0.postfix, baseFilename1.postfix, baseFilename2.postfix,...)
//============================================================================
cocos2d::Animation* MyAnimation::createAnimateWithMulPic(const std::string& baseFilename, 
	const std::string& postfix, int picNums, cocos2d::Rect& rect, float delay, size_t loops)
{
	Vector<SpriteFrame*> spFrameVec;
	std::string trueFilename;
	Value val;
	for (int i = 0; i < picNums; i++)
	{
		val = i;
		trueFilename = baseFilename + val.asString() +"."+postfix;
		auto sp = SpriteFrame::create(trueFilename, rect);
		spFrameVec.pushBack(sp);
	}
	return Animation::createWithSpriteFrames(spFrameVec, delay, loops);
}
//============================================================
// Desc:用"多张图片"的形式创建一个动画并附带隐藏效果
//============================================================
cocos2d::Action* MyAnimation::createAnimateWithHide_MulPic(const std::string& baseFilename, 
	const std::string& postfix, int picNums, cocos2d::Rect& rect, float delay, size_t loops)
{
	auto animation = createAnimateWithMulPic(baseFilename, postfix, picNums, rect, delay, loops);
	auto action = Sequence::create(Animate::create(animation), Hide::create(), nullptr);
	return action;
}