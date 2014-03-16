#include "GameLayer.h"
#include "GameLayerLoader.h"

GameLayer::GameLayer()
: cat(NULL)
{
	duration = 2.0f;
	ax = 10.0f;
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
	
	// batch node
	pipeBatch = CCSpriteBatchNode::create("pipe.png");
	this->addChild(pipeBatch);

	// pipe
//	this->schedule(schedule_selector(GameLayer::setPipe), 2.0f);
	GameLayer::setPipe();
	
	// cat
	this->schedule(schedule_selector(GameLayer::fallCat));

}

void GameLayer::setPipe()
{
	CCLog("set pipe");
	
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	int centerY = (int)(CCRANDOM_0_1()*size.height + 1);
	int space = cat->getContentSize().height + 50; // ネコの高さ+隙間
	int pipeH = 500; // TODO
	int upperPipeY = pipeH + centerY + space / 2;
	int underPipeY = centerY - space / 2;
	
	CCSprite* upperPipe = CCSprite::createWithTexture(pipeBatch->getTexture());
	CCSprite* underPipe = CCSprite::createWithTexture(pipeBatch->getTexture());
	
	upperPipe->setPosition(ccp(size.width -200, upperPipeY));
	underPipe->setPosition(ccp(size.width -200, underPipeY));
	
	pipeBatch->addChild(upperPipe);
	pipeBatch->addChild(underPipe);

}

void GameLayer::movePipe()
{
	
}

void GameLayer::fallCat(float delta)
{
	if (isJump) return;
	
//	CCLog("fall");
//	CCLog("%f", delta);
	float catY = cat->getPosition().y;
	float newY = catY - ax;
	ax += (delta * 10.0f);
	cat->setPositionY(newY);
}

void GameLayer::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	CCLog("touch screen");
	
	// initialize ax
	ax = 0;
	
	isJump = true;
	
	cat->setTexture(CCTextureCache::sharedTextureCache()->addImage("cat_goal.png"));
	
	CCActionInterval* jump = CCMoveBy::create(duration/4, ccp(0, 150));
	CCActionInterval* eseout = CCEaseOut::create(jump, duration/4);
	CCCallFunc *jumpCallback = CCCallFunc::create(this, callfunc_selector(GameLayer::jumCallback));
	CCSequence* sequence = CCSequence::create(eseout, jumpCallback, NULL);

	cat->runAction(sequence);

}

void GameLayer::jumCallback()
{
	CCLog("jump callback");
	cat->setTexture(CCTextureCache::sharedTextureCache()->addImage("cat_normal.png"));
	
	isJump = false;
}