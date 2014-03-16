#ifndef __GAME_LAYER__
#define __GAME_LAYER__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class GameLayer : public CCLayer
, public CCNodeLoaderListener
{
protected:
	float duration;
	float ax;
	float isJump;
    int pipeTag;
    int spaceTag;
    int score;
    int lastHitTag;
    int isEnd;
private:
	CCSprite* cat;
	CCSpriteBatchNode* pipeBatch;
    CCSpriteBatchNode* spaceBatch;
    CCLabelTTF* scoreLabel;
	
	virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
	
    bool init();
    void btnStartCallback(CCObject* sender);
    void hitTest();
    void gameEnd();
    void showScore();
	void fallCat(float delta);
	void setPipe();
	void movePipe(CCSprite* pipe);
    void movePipeCallback(CCNode* pipeNode);
	void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	void jumCallback();
	
public:
			
    GameLayer();
    virtual ~GameLayer();
    
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(GameLayer, create);
	

};

#endif /* __GAME_LAYER__ */