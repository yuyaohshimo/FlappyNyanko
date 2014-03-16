//
//  RankingScrollLayer.h
//  FlappyNyanko
//
//  Created by 竹ノ谷 知香 on 2014/03/13.
//
//

#ifndef __FlappyNyanko__RankingScrollLayer__
#define __FlappyNyanko__RankingScrollLayer__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "spine/Json.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d;
using namespace std;
using namespace extension;

class RankingScrollLayer : public CCLayer
{
private:
    int entryLength;
public:
    virtual bool init();
    CREATE_FUNC(RankingScrollLayer);
    void createThereds(Json* entry);
    int getContentHeight();
};



#endif /* defined(__FlappyNyanko__RankingScrollLayer__) */
