//
//  RankingThreadSprite.cpp
//  FlappyNyanko
//
//  Created by 竹ノ谷 知香 on 2014/03/12.
//
//

#include "RankingThreadSprite.h"


RankingThreadSprite::RankingThreadSprite()
{
}

RankingThreadSprite::~RankingThreadSprite()
{
}

RankingThreadSprite* RankingThreadSprite::createWithData(int rank, string score, string name)
{
    RankingThreadSprite *rt = new RankingThreadSprite();
    rt->myScore = score;
    rt->userName = name;
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCString* rankString = CCString::createWithFormat("%i.", rank);
    CCLabelTTF* rankLabel = CCLabelTTF::create(rankString->getCString(), "04b19", 30);
    CCLabelTTF* nameLabel = CCLabelTTF::create(name.c_str(), "04b19", 30);
    CCLabelTTF* scoreLabel = CCLabelTTF::create(score.c_str(), "04b19", 30);
    
    if (rank == 1)
        rankLabel->setColor(ccc3(0, 0, 0));
    else
        rankLabel->setColor(ccc3(0, 0, 0));
    nameLabel->setColor(ccc3(0, 0, 0));
    scoreLabel->setColor(ccc3(0, 0, 0));
    
    rankLabel->setPosition(ccp(winSize.width/4, 0));
    nameLabel->setPosition(ccp(rankLabel->getPositionX() + rankLabel->getContentSize().width/2 + nameLabel->getContentSize().width/2 + 10, 0));
    scoreLabel->setPosition(ccp(winSize.width/4*3 + scoreLabel->getContentSize().width/2, 0));
    
    rt->addChild(rankLabel);
    rt->addChild(nameLabel);
    rt->addChild(scoreLabel);
    
    return rt;
}

string RankingThreadSprite::getScore()
{
    return myScore;
}

string RankingThreadSprite::getUserName()
{
    return myScore;
}