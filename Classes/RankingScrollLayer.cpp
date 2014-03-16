#include "RankingScrollLayer.h"
#include "RankingThreadSprite.h"

bool RankingScrollLayer::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

    return true;
};

void RankingScrollLayer::createThereds(Json* entry)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    // array用意
    CCArray* array = CCArray::create();
    
    int rank = 0;
    string beforeScore = "";
    entryLength = Json_getSize(entry);
    for (int i = 0; i < entryLength; i++)
    {
        Json* user = Json_getItemAt(entry, i);
        string name = Json_getString(user, "name", "false");
        string score = Json_getString(user, "score", "0");
        if (beforeScore != score)
            rank = i+1;
        beforeScore = score;
        
        RankingThreadSprite* rt = RankingThreadSprite::createWithData(rank, score, name);
        array->addObject(rt); //配列に登録
        
        int yy = (i+0.5)*THREAD_HEIGHT;
        rt->setPosition(ccp(0, -yy));
        addChild(rt);
    }
}

int RankingScrollLayer::getContentHeight()
{
    int contentHeight = entryLength * THREAD_HEIGHT;
    return contentHeight;
}