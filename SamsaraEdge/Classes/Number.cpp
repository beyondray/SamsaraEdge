#include "Number.h"
#include "ConstValue.h"
using namespace cocos2d;
//======================================================
// Desc:�����ļ�������һ��Number��̬����
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
// Desc:����һ�ų�ͼƬ������������
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
// Desc:�������ֵõ�һ��д�����ֵľ���֡
//======================================================
inline SpriteFrame* Number::getDigit(int index)
{
	CCASSERT(index >= 0 && index < 10, "the index of NumberVec is illegal!");
	if (m_Digit[index] != nullptr)
		return m_Digit[index];
	return nullptr;
}
//======================================================
// Desc:ת��Ϊ�����֣�ͼƬ��ʾ��
//======================================================
void Number::toNumber(size_t number)
{
	//�������ֻ������б�
	std::list<SpriteFrame*> numberList;
	if (number == 0) numberList.push_back(m_Digit[0]);
	while (number!=0)
	{
		int digit = number%10;
		numberList.push_front(m_Digit[digit]);
		number = number / 10;
	}

	//���������ſ�����������
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