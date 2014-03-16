#ifndef __GAME_LAYER__
#define __GAME_LAYER__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define GOAL_COUNT 100

class GameLayer : public CCLayer
, public CCNodeLoaderListener
{
protected:
	float duration;
	float ax;
	float isJump;
private:
	CCSprite* cat;
	CCSpriteBatchNode* pipeBatch;
	
	virtual void onNodeLoaded(cocos2d::CCNode * pNode, cocos2d::extension::CCNodeLoader * pNodeLoader);
	
	void fallCat(float delta);
	void setPipe();
	void movePipe();
	
public:
			
    GameLayer();
    virtual ~GameLayer();
    
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(GameLayer, create);
	
	bool init();
	
	void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	
	void jumCallback();
};

#endif /* __GAME_LAYER__ */