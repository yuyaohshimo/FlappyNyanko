#ifndef __SCORE_SCENE__
#define __SCORE_SCENE__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d;
using namespace std;

class ScoreLayer : public CCLayer
{
private:
    int myScore;
    void showScore();
    void showRanking();
    void restart();
    
    // touch
    void ccTouchesBegan(CCSet* touches, CCEvent* event);
    void ccTouchesMoved(CCSet* touches, CCEvent* event);
    void ccTouchesEnded(CCSet* touches, CCEvent* event);

public:
    static cocos2d::CCScene* scene();
    static cocos2d::CCScene* sceneWithScore(int score);
    static ScoreLayer* createWithScore(int score);
    virtual bool initWithScore(int score);
};

#endif /* defined(__FlappyNyanko__ScoreScene__) */
