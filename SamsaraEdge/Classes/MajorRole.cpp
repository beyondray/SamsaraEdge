#include "MajorRole.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d;
using namespace CocosDenshion;

//============================================================
// Desc:静态create方法
//============================================================
MajorRole* MajorRole::create(const std::string& filename, const Rect& rect)
{
	MajorRole* sprite = new (std::nothrow)MajorRole();
	if (sprite)
	{
		bool bInitSuc;
		if (rect.equals(Rect(0, 0, -1, -1)))
		{
			bInitSuc = sprite->initWithFile(filename);
		}
		else
		{
			bInitSuc = sprite->initWithFile(filename, rect);
		}
		if (bInitSuc)
		{
			sprite->autorelease();
			return sprite;
		}
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}
//============================================================
// Desc:做某些动作，再跳跃到一个节点
//============================================================
cocos2d::Spawn* MajorRole::doSthAndMoveTo(major_Action sth, cocos2d::Vec2 worldPos, float duration)
{
	Animate* sthAnimation;
	switch (sth)
	{
	case JUMP:
		sthAnimation = createJumpAni(duration / MAJOR_JUMP_PICS, 1);
		//播放后跳音效
		SimpleAudioEngine::getInstance()->playEffect(EFFECT_BACKJUMP);
		break;
	case WAVE:
		sthAnimation = createWaveAni(duration / MAJOR_WAVE_PICS, 1);
		//播放挥舞音效
		SimpleAudioEngine::getInstance()->playEffect(EFFECT_WAVE);
		break;
	case WAVEUP:
		sthAnimation = createWaveUpAni(duration / MAJOR_WAVEUP_PICS, 1);
		//播放挥舞音效
		SimpleAudioEngine::getInstance()->playEffect(EFFECT_WAVE);
		break;
	case WAVEDOWN:
		sthAnimation = createWaveDownAni(duration / MAJOR_WAVEDOWN_PICS, 1);
		//播放挥舞音效
		SimpleAudioEngine::getInstance()->playEffect(EFFECT_WAVE);
		break;
	case WAVERIGHT:
		sthAnimation = createWaveRightAni(duration / MAJOR_WAVERIGHT_PICS, 1);
		//播放挥舞音效
		SimpleAudioEngine::getInstance()->playEffect(EFFECT_WAVERIGHT);
		break;
	case BACK:
		sthAnimation = createBackAni(duration / MAJOR_BACK_PICS, 1);
		break;
	case DEAD:
		sthAnimation = createDeadAni(duration / MAJOR_DEAD_PICS, 1);
		//【播放死亡音效】
		SimpleAudioEngine::getInstance()->playEffect(EFFECT_DEAD);
		break;
	case REBIRTH:
		sthAnimation = createRebirthAni(duration / MAJOR_REBIRTH_PICS, 1);
		break;
	case WALK:
		sthAnimation = createWalkAni(duration / MAJOR_WALK_PICS, 1);
		break;
	default:
		sthAnimation = createWaveAni(duration / MAJOR_WAVE_PICS, 1);
		//播放挥舞音效
		SimpleAudioEngine::getInstance()->playEffect(EFFECT_WAVE);
		break;
	}
	
	//加速度减小为0的Sine变加速运动
	auto move = EaseSineOut::create(cocos2d::MoveTo::create(duration, worldPos));		
	auto doSthAndMove = cocos2d::Spawn::create(move, sthAnimation, nullptr);
	return doSthAndMove;
}
//============================================================
// Desc:做某些动作，跳跃到一个节点并重复走动
//============================================================
Sequence* MajorRole::doSthMovetoThenWalk(major_Action sth, cocos2d::Vec2 worldPos, float moveDuration)
{
	auto doSthAndMove = doSthAndMoveTo(sth, worldPos, moveDuration);
	auto doSthMoveToThenWalk = Sequence::create(doSthAndMove, CCCallFunc::create(CC_CALLBACK_0(MajorRole::RepeatWalk_CALL, this)), nullptr);
	return doSthMoveToThenWalk;
}
//============================================================
// Desc:移动到一个节点并不停行走(带背景回退）
//============================================================
void MajorRole::MovingToNodeThenWalk(major_Action sth, cocos2d::Vec2 tarWorldPos, float moveDuration)
{
	this->stopAllActions();
	Vec2 tarPosAfterBackMove = tarWorldPos - Vec2(moveDuration*BK_MOVE_AVESECOND_SPEED, 0);
	this->runAction(this->doSthMovetoThenWalk(sth, tarPosAfterBackMove, moveDuration));
}
//============================================================
// Desc:背景回退（带游戏结束函数调用）
//============================================================
void MajorRole::RepeatBack_CALL(bool* magicPushLock)
{
	auto delay = DelayTime::create(2.0f);
	auto backAndGameOver = Sequence::create(CallFunc::create(CC_CALLBACK_0(MajorRole::alwaysRepeatBack_CALL, this)),
		delay, CallFunc::create(CC_CALLBACK_0(MenuLayer::runGameOverAni, MenuLayer::getInstance(), magicPushLock)), nullptr);
	this->runAction(backAndGameOver);
}
//============================================================
// Desc:移动到一个节点(带背景回退函数调用）
//============================================================
void MajorRole::MovingToNode(major_Action sth, cocos2d::Vec2 tarWorldPos, float moveDuration, bool* magicPushLock)
{
	this->stopAllActions();
	Vec2 tarPosAfterBackMove = tarWorldPos - Vec2(moveDuration*BK_MOVE_AVESECOND_SPEED, 0);
	auto doSthAndMove = doSthAndMoveTo(sth, tarPosAfterBackMove, moveDuration);
	auto doSthMoveToAndBack = Sequence::create(doSthAndMove, CallFunc::create(CC_CALLBACK_0(MajorRole::RepeatBack_CALL, this, magicPushLock)), nullptr);
	this->runAction(doSthMoveToAndBack);
}

