//
//  RankingThreadSprite.h
//  FlappyNyanko
//
//  Created by 竹ノ谷 知香 on 2014/03/12.
//
//

#ifndef __FlappyNyanko__RankingThreadSprite__
#define __FlappyNyanko__RankingThreadSprite__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d;
using namespace std;
using namespace extension;

#define THREAD_HEIGHT 30;

class RankingThreadSprite : public CCNode
{
private:
    string myScore;
    string userName;
    string getScore();
    string getUserName();
    
public:
    RankingThreadSprite();
    virtual ~RankingThreadSprite();
    
    static RankingThreadSprite* createWithData(int rank, string score, string userName);
};

#endif /* defined(__FlappyNyanko__RankingThreadSprite__) */
