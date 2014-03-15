#include "GameLayer.h"
#include "GameLayerLoader.h"

GameLayer::GameLayer()
: cat(NULL)
{
	duration = 2.0f;
	ax = 1.0f;
	isJump = false;
}

GameLayer::~GameLayer()
{
	CC_SAFE_RELEASE(cat);
}

bool GameLayer::init()
{
	CCLog("init");
	return true;
}

void GameLayer::onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader)
{
	CCLog("Load Node");
	cat = (CCSprite *)this->getChildByTag(1);
	
	this->schedule(schedule_selector(GameLayer::fallCat));

}

void GameLayer::timer(float delta)
{
}

void GameLayer::movePipe()
{
	
}

void GameLayer::fallCat()
{
	CCLog("fall");
	float catY = cat->getPosition().y;
	float newY = catY - ax;
	ax += 0.1f;
	cat->setPositionY(newY);
}

void GameLayer::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	CCLog("touch screen");
	
	// initialize ax
	ax = 0;
	
	cat->setTexture(CCTextureCache::sharedTextureCache()->addImage("cat_goal.png"));
	
	CCActionInterval* jump = CCMoveBy::create(duration/2, ccp(0, 150));
	CCActionInterval* eseout = CCEaseOut::create(jump, duration/4);
	CCCallFunc *jumpCallback = CCCallFunc::create(this, callfunc_selector(GameLayer::jumCallback));
	CCSequence* sequence = CCSequence::create(eseout, jumpCallback, NULL);

	cat->runAction(sequence);

}

void GameLayer::jumCallback()
{
	CCLog("jump callback");
	cat->setTexture(CCTextureCache::sharedTextureCache()->addImage("cat_normal.png"));
}