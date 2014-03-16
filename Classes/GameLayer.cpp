#include "GameLayer.h"
#include "GameLayerLoader.h"
#include "ScoreLayer.h"
#include "stdio.h"
#include "math.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

GameLayer::GameLayer()
: cat(NULL)
, pipeBatch(NULL)
, spaceBatch(NULL)
, scoreLabel(NULL)
{
	duration = 2.0f;
	ax = 20.0f;
	isJump = false;
    pipeTag= 10000;
    spaceTag = 100;
    score = 0;
    lastHitTag = 0;
    isEnd = false;
}

GameLayer::~GameLayer()
{
}

bool GameLayer::init()
{
	CCLog("init");
    
    // BGM
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.5);
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("bgm.mp3");
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("bgm.mp3", true);
    
    // preload sound effect
    SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0);
    SimpleAudioEngine::sharedEngine()->preloadEffect("clear.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("jump5.wav");
    
	return true;
}

void GameLayer::onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader)
{
	CCLog("Load Node");
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    // cat
	cat = (CCSprite *)this->getChildByTag(1);
    
    // score label
    scoreLabel = CCLabelTTF::create("0", "04b19", 48);
    scoreLabel->setColor(ccc3(0,0,0));
    scoreLabel->setPosition(ccp(
                                size.width/2,
                                size.height - scoreLabel->getContentSize().height/2));
    this->addChild(scoreLabel);
	
	// batch node
	pipeBatch = CCSpriteBatchNode::create("pipe.png");
	this->addChild(pipeBatch);
    
    spaceBatch = CCSpriteBatchNode::create("space.png");
	this->addChild(spaceBatch);
    
    // start button
    CCMenuItemImage* btnStart = CCMenuItemImage::create("btn-start.png",
                                                        "btn-start-press.png",
                                                          this,
                                                          menu_selector(GameLayer::btnStartCallback));
    btnStart->setPosition(ccp(size.width/2, size.height/2));
    CCMenu* topMenu = CCMenu::create(btnStart, NULL);
    topMenu->setPosition(CCPointZero);
    
    this->addChild(topMenu);
    topMenu->setTag(-10);

}

void GameLayer::btnStartCallback(CCObject* sender)
{
    CCSprite* topMenu = (CCSprite *)this->getChildByTag(-10);
    this->removeChild(topMenu);
    
    // enable tap event
    this->setTouchEnabled(true);
    
	// pipe
	this->schedule(schedule_selector(GameLayer::setPipe), 2.0f);
	
	// cat
	this->schedule(schedule_selector(GameLayer::fallCat));
    
    // hit test
    this->schedule(schedule_selector(GameLayer::hitTest));
}

void GameLayer::hitTest()
{
    CCArray* spacenodes = spaceBatch->getChildren();
    CCObject* spacechild = NULL;
    CCARRAY_FOREACH(spacenodes, spacechild)
    {
//        CCLog("Tag Number is %d", ((CCNode*)spacechild)->getTag());
        int tagNum = ((CCNode*)spacechild)->getTag();
        
        CCRect spaceRect = ((CCNode*)spacechild)->boundingBox();
        CCRect catRect = CCRectMake(
                                       cat->getPosition().x - (cat->getContentSize().width/4),
                                       cat->getPosition().y - (cat->getContentSize().height/4),
                                       cat->getContentSize().width/2,
                                       cat->getContentSize().height/2);
        
        if (catRect.intersectsRect(spaceRect) && lastHitTag != tagNum) {
            lastHitTag = tagNum;
            CCLog("HIT !!!!!");
            // play sound effect
            SimpleAudioEngine::sharedEngine()->playEffect("clear.wav");
            score++;
            CCString* newscore = CCString::createWithFormat("%d", score);
            scoreLabel->setString(newscore->getCString());
        }
    }
    
    CCArray* pipenodes = pipeBatch->getChildren();
    CCObject* pipechild = NULL;
    CCARRAY_FOREACH(pipenodes, pipechild)
    {
//        CCLog("Tag Number is %d", ((CCNode*)pipechild)->getTag());
        CCRect pipeRect = ((CCNode*)pipechild)->boundingBox();
        CCRect catRect = CCRectMake(
                                    cat->getPosition().x - (cat->getContentSize().width/4),
                                    cat->getPosition().y - (cat->getContentSize().height/4),
                                    cat->getContentSize().width/2,
                                    cat->getContentSize().height/2);
        if (!isEnd && catRect.intersectsRect(pipeRect)) {
            isEnd = true;
            CCLog("DIE !!!!!");
            gameEnd();
        }
    }
}

void GameLayer::gameEnd()
{
    CCLog("game end!");
    
    //scheduleをすべてキャンセル
    this->unschedule(schedule_selector(GameLayer::setPipe));
	this->unschedule(schedule_selector(GameLayer::fallCat));
    this->unschedule(schedule_selector(GameLayer::hitTest));
    this->setTouchEnabled(false);
    
    // ゲームエンドを表示する
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCLabelTTF* label = CCLabelTTF::create("GAME OVER!", "04b19", 80);
    label->setColor(ccc3(255, 0, 0));
    label->setPosition(ccp(size.width/2, size.height/2));
    this->addChild(label);
    
	cat->setTexture(CCTextureCache::sharedTextureCache()->addImage("cat_tumble.png"));
    
    CCTintTo* tint = CCTintTo::create(0.5f, 255, 0, 0);
    CCBlink* blink = CCBlink::create(0.5f, 10);
    CCMoveTo* move = CCMoveTo::create(0.5f, ccp(size.width/2, size.height/2));
    CCSpawn* animationSpawn = CCSpawn::create(move, tint, blink, NULL);
    CCCallFunc* actionMoveDone = CCCallFunc::create(this, callfunc_selector(GameLayer::showScore));
    
    CCSequence* animation = CCSequence::create(animationSpawn, actionMoveDone, NULL);
    cat->runAction(animation);
}

void GameLayer::showScore()
{
    CCScene* scoreScene = (CCScene*)ScoreLayer::createWithScore(score);
    CCScene* pScene = CCTransitionZoomFlipX::create(1.0f, scoreScene);
    if (pScene) CCDirector::sharedDirector()->replaceScene(pScene);
}

void GameLayer::setPipe()
{
	CCLog("set pipe");
	
	CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    int spaceH = cat->getContentSize().height + 50; // ネコの高さ+隙間
    int centerY = spaceH/2 + (int)(rand()*((int)size.height-spaceH/2+1.0)/(1.0+RAND_MAX));
	int pipeH = 500; // TODO
	int upperPipeY = pipeH + centerY + spaceH / 2;
	int underPipeY = centerY - spaceH / 2;
    
    upperPipeY = centerY + spaceH / 2;
    underPipeY = (centerY - spaceH / 2) - pipeH;
	
	CCSprite* upperPipe = CCSprite::createWithTexture(pipeBatch->getTexture());
	CCSprite* underPipe = CCSprite::createWithTexture(pipeBatch->getTexture());
    CCSprite* space = CCSprite::createWithTexture(spaceBatch->getTexture());
    
    upperPipe->setAnchorPoint(CCPointZero);
    underPipe->setAnchorPoint(CCPointZero);
    space->setAnchorPoint(ccp(0, 0.5));
    
	upperPipe->setPosition(ccp(size.width, upperPipeY));
	underPipe->setPosition(ccp(size.width, underPipeY));
    space->setPosition(ccp(size.width, centerY));
    
	pipeBatch->addChild(upperPipe);
	pipeBatch->addChild(underPipe);
    spaceBatch->addChild(space);
    
    upperPipe->setTag(pipeTag++);
    underPipe->setTag(pipeTag++);
    space->setTag(spaceTag++);
    
    GameLayer::movePipe(upperPipe);
    GameLayer::movePipe(underPipe);
    GameLayer::movePipe(space);

}

void GameLayer::movePipe(CCSprite* sprite)
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCActionInterval* move = CCMoveBy::create(duration, ccp(-size.width, 0));
    CCCallFuncN* movePipeCallback = CCCallFuncN::create(this, callfuncN_selector(GameLayer::movePipeCallback));
    CCSequence* sequence = CCSequence::create(move, movePipeCallback, NULL);
	
    sprite->runAction(sequence);
    
}

void GameLayer::movePipeCallback(CCNode* node)
{
    CCLog("move pipe callback");
    bool isCleanUp = true;
    node->getParent()->removeChild(node, isCleanUp);
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
    
    // play sound effect
    SimpleAudioEngine::sharedEngine()->playEffect("Jump5.wav");
	
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