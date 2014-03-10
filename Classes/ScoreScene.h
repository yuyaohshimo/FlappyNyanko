#ifndef __SCORE_SCENE__
#define __SCORE_SCENE__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class ScoreScene : public CCLayer
{
private:
    void saveScore(CCString* userName, int score);
    void onHttpRequestCompleted(cocos2d::CCNode *sender, void *data);
    void showSaveComplete();
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(ScoreScene);
    
    void showScore(CCString* userName, int score);
};

#endif /* defined(__FlappyNyanko__ScoreScene__) */
