//
//  ScoreLayer.cpp
//  FlappyNyanko
//
//  Created by cocos2d-x on 14/03/09.
//
//

#include "ScoreLayer.h"
#include "curl/curl.h"
#include "RankingLayer.h"
#include "GameLayerLoader.h"


CCScene* ScoreLayer::scene()
{
    return ScoreLayer::sceneWithScore(0);
}

CCScene* ScoreLayer::sceneWithScore(int score)
{
    CCScene* scene = CCScene::create();
    ScoreLayer* layer = ScoreLayer::createWithScore(score);
    scene->addChild(layer);
    
    return scene;
}

ScoreLayer* ScoreLayer::createWithScore(int score)
{
    ScoreLayer *pRet = new ScoreLayer();
    if (pRet && pRet->initWithScore(score))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool ScoreLayer::initWithScore(int score)
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    myScore = score;
    
    showScore();
    
    return true;
};

void ScoreLayer::showScore()
{
    // プリファレンスデータのベストスコアを取得
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    int bsetScore = userDefault->getIntegerForKey("bestScore", 0);
    
    // テスト用　デフォルトに戻す
//    userDefault->setIntegerForKey("bestScore", 0);
//    userDefault->setStringForKey("userName", "");
//    userDefault->flush();

    bool best;
    if (myScore > bsetScore)
    {
        best = true;
        // ベストスコアを登録
        userDefault->setIntegerForKey("bestScore", myScore);
        userDefault->flush();
        bsetScore = myScore;
    }
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    // BackGround
    CCSprite* bg = CCSprite::create("score_back.png",CCRectMake(0, 0, 1024, 577));
    bg->setPosition(ccp(winSize.width/2, winSize.height/2));
    addChild(bg);
    
    // Score
    CCLabelTTF* label = CCLabelTTF::create("Score", "04b19", 48);
    label->setColor(ccc3(0, 0, 0));
    label->setPosition(ccp(winSize.width/2 - label->getContentSize().width/2 -10, 370));
    this->addChild(label);
    
    // score
    CCString* scoreText = CCString::createWithFormat("%i", myScore);
    CCLabelTTF* scoreLabel = CCLabelTTF::create(scoreText->getCString(), "04b19", 48);
    scoreLabel->setColor(ccc3(0, 0, 0));
    scoreLabel->setPosition(ccp(winSize.width/2 + scoreLabel->getContentSize().width/2 + 10, 370));
    this->addChild(scoreLabel);
    
    // best
    CCLabelTTF* bestLabel = CCLabelTTF::create("Best", "04b19", 48);
    bestLabel->setColor(ccc3(0, 0, 0));
    bestLabel->setPosition(ccp(winSize.width/2 - bestLabel->getContentSize().width/2 -10, 290));
    this->addChild(bestLabel);
    
    // bestScore
    CCString* bestText = CCString::createWithFormat("%i", bsetScore);
    CCLabelTTF* bestScoreText = CCLabelTTF::create(bestText->getCString(), "04b19", 48);
    bestScoreText->setColor(ccc3(0, 0, 0));
    bestScoreText->setPosition(ccp(winSize.width/2 + bestScoreText->getContentSize().width/2 +10, 290));
    this->addChild(bestScoreText);
    
    // new best
    if (best)
    {
        CCSprite* newIcon = CCSprite::create("new.png",CCRectMake(0, 0, 75, 43));
        newIcon->setPosition(ccp(bestLabel->getPositionX() - bestLabel->getContentSize().width/2 - newIcon->getContentSize().width/2 - 10, bestLabel->getPositionY()));
        addChild(newIcon);
    }

    // buttons
    CCMenuItemImage* rankingButton = CCMenuItemImage::create("ranking.png", "ranking.png", this, menu_selector(ScoreLayer::showRanking));
    rankingButton->setPosition(ccp(winSize.width/2 + rankingButton->getContentSize().width, winSize.height/5));
    CCMenuItemImage* restartButton = CCMenuItemImage::create("restart.png", "restart.png", this, menu_selector(ScoreLayer::restart));
    restartButton->setPosition(ccp(winSize.width/2 - restartButton->getContentSize().width, winSize.height/5));

    CCMenu* footerButtons = CCMenu::create(restartButton, rankingButton, NULL);
    footerButtons->setPosition(ccp(0, 0));
    this->addChild(footerButtons);
    
    
    // にゃんこで遊ぶ
    CCSprite* nyan = CCSprite::create("cat_normal.png");
    nyan->setScale(0.5f);
    nyan->setPosition(ccp(nyan->getContentSize().width/2, nyan->getContentSize().height/2));
    addChild(nyan);
}

void ScoreLayer::showRanking()
{
    CCScene* rankingScene = (CCScene*)RankingLayer::sceneWithScore(myScore);
    CCScene* pScene = CCTransitionZoomFlipX::create(1.0f, rankingScene);
    if (pScene) CCDirector::sharedDirector()->replaceScene(pScene);
}

void ScoreLayer::restart()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    
    // CocosBuilderのファイルを読込みゲーム画面を生成する
    CCNodeLoaderLibrary* ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("GameLayer", GameLayerLoader::loader());
    CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
    CCNode* node = ccbReader->readNodeGraphFromFile("GameLayer.ccbi");
    
    // シーンを用意し、ゲーム画面を設置する
    CCScene* pScene = CCScene::create();
    if (node != NULL)
        pScene->addChild(node);
    ccbReader->release();
    CCScene* newScene = CCTransitionZoomFlipX::create(1.0f, pScene);
    pDirector->replaceScene(newScene);
}

void ScoreLayer::ccTouchesBegan(CCSet* touches, CCEvent* event)
{
    
}
void ScoreLayer::ccTouchesMoved(CCSet* touches, CCEvent* event)
{
    
}
void ScoreLayer::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    
}
