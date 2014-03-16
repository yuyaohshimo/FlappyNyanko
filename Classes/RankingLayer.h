//
//  RankingLayer.h
//  FlappyNyanko
//
//  Created by 竹ノ谷 知香 on 2014/03/11.
//
//

#ifndef __FlappyNyanko__RankingLayer__
#define __FlappyNyanko__RankingLayer__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "RankingScrollLayer.h"

#define MASK_WIDTH 1024
#define MASK_HEIGHT 314

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d;
using namespace std;
using namespace extension;

class RankingLayer : public CCLayer, public CCEditBoxDelegate
{
private:
    // register username
    void editBoxEditingDidBegin(CCEditBox* editBox);
    void editBoxEditingDidEnd(CCEditBox* editBox);
    void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    void editBoxReturn(CCEditBox* editBox);

    void onHttpRequestCompleted(cocos2d::CCNode *sender, void *data);
    void registerRanking();
    void showRegister();
    void requestRanking();
    void showRanking(string data);
    void onSaveHttpRequestCompleted(cocos2d::CCNode *sender, void *data);
    string userName;
    int myScore;
    CCRect absoluteBoundingBox();
    RankingScrollLayer* rankingScrollLayer;
    
    // touch
    int defaultRankingScrollY;
    void ccTouchesBegan(CCSet* touches, CCEvent* event);
    void ccTouchesMoved(CCSet* touches, CCEvent* event);
    void ccTouchesEnded(CCSet* touches, CCEvent* event);
    CCPoint _lastTouchPoint;
    
    void show(int score);
    void restart();
public:
    static cocos2d::CCScene* scene();
    static cocos2d::CCScene* sceneWithScore(int score);
    static RankingLayer* createWithScore(int score);
    virtual bool initWithScore(int score);
};

#endif /* defined(__FlappyNyanko__RankingLayer__) */
