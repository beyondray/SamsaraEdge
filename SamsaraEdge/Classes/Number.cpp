#include "Number.h"
#include "ConstValue.h"
using namespace cocos2d;
//======================================================
// Desc:根据文件名创建一个Number静态类型
//======================================================
Number* Number::create(std::string fileName)
{
	Number* number = new Number();
	if (number && number->initWithFile(fileName))
	{
		number->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(number);
	}
	return number;
}
//======================================================
// Desc:根据一张长图片创建数字数组
//======================================================
void Number::createDigitVec(std::string picfileName, size_t unitWidth, size_t unitHeight, size_t cutEveryGaps/* =0 */)
{
	int width = unitWidth - 2*cutEveryGaps;
	int height = unitHeight;
	Value number;
	for (int i = 0; i < 10; i++)
	{
		number = i;
		std::string name = picfileName + number.asString();
		if (!SpriteFrameCache::getInstance()->getSpriteFrameByName(name))
		{
			m_Digit[i] = SpriteFrame::create(picfileName, Rect(i*unitWidth+cutEveryGaps, 0, width, height));	
			SpriteFrameCache::getInstance()->addSpriteFrame(m_Digit[i], name);
		}
		else
		{
			m_Digit[i] = SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
		}
	}

}
//======================================================
// Desc:根据数字得到一张写有数字的精灵帧
//======================================================
inline SpriteFrame* Number::getDigit(int index)
{
	CCASSERT(index >= 0 && index < 10, "the index of NumberVec is illegal!");
	if (m_Digit[index] != nullptr)
		return m_Digit[index];
	return nullptr;
}
//======================================================
// Desc:转化为该数字（图片显示）
//======================================================
void Number::toNumber(size_t number)
{
	//构造数字化精灵列表
	std::list<SpriteFrame*> numberList;
	if (number == 0) numberList.push_back(m_Digit[0]);
	while (number!=0)
	{
		int digit = number%10;
		numberList.push_front(m_Digit[digit]);
		number = number / 10;
	}

	//构造依次排开的数字序列
	int i = 0;
	this->removeAllChildren();
	for (auto iter = numberList.begin(); iter != numberList.end();iter++)
	{
		auto digit = Sprite::createWithSpriteFrame(*iter);
		digit->setAnchorPoint(Vec2::ZERO);
		digit->setPosition(Vec2(i*(NUMBER_UNIT_WIDTH-NUMBER_GAPS), 0));
		this->addChild(digit);
		i++;
	}
	
}