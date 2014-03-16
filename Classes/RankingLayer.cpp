#include "RankingLayer.h"
#include "spine/Json.h"
#include "GameLayerLoader.h"

CCScene* RankingLayer::scene()
{
    CCScene* scene = RankingLayer::sceneWithScore(0);
    return scene;
}

CCScene* RankingLayer::sceneWithScore(int score)
{
    CCScene* scene = CCScene::create();
    RankingLayer* layer = RankingLayer::createWithScore(score);
    scene->addChild(layer);
    
    return scene;
}

RankingLayer* RankingLayer::createWithScore(int score)
{
    RankingLayer* layer = new RankingLayer();
    if (layer && layer->initWithScore(score))
    {
        layer->autorelease();
        return layer;
    }
    else
    {
        delete layer;
        layer = NULL;
        return NULL;
    }
    
}

bool RankingLayer::initWithScore(int score)
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    show(score);
    
    return true;
};

void RankingLayer::show(int score)
{
    myScore = score;
    
    // プリファレンスデータのユーザー名を取得
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    userName = userDefault->getStringForKey("userName", "");
    
    if (userName == "")
    {
        // ユーザー名登録に促す
        showRegister();
    }
    else
    {
        // ランキングに登録する
        registerRanking();
    }

}

void RankingLayer::showRegister()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    // Please register your name
    CCLabelTTF* label = CCLabelTTF::create("Please register your name", "04b19", 22);
    label->setPosition(ccp(size.width/2, size.height/4*3));
    this->addChild(label);

    CCSize editBoxSize = CCSizeMake(200, 50);
    CCEditBox* pEditBox = CCEditBox::create(editBoxSize,
                                            CCScale9Sprite::create("sprite9.png"));
    pEditBox->setPosition(ccp(size.width/2, size.height/2));
    pEditBox->setFontColor(ccBLUE);
    pEditBox->setPlaceHolder("Name:");
    pEditBox->setMaxLength(8);
    pEditBox->setReturnType(kKeyboardReturnTypeDone);
    pEditBox->setDelegate(this);
    this->addChild(pEditBox);
}

void RankingLayer::editBoxEditingDidBegin(CCEditBox* editBox) {
    CCLOG("did begin");
}

void RankingLayer::editBoxEditingDidEnd(CCEditBox* editBox) {
    CCLOG("did end : %s", editBox->getText());
    userName = editBox->getText();
    
    //ユーザーデータを保存
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    userDefault->setStringForKey("userName", userName);
    userDefault->flush();

    registerRanking();
}

void RankingLayer::editBoxTextChanged(CCEditBox* editBox, const std::string& text) {
    CCLOG("change %s", text.c_str());
}

void RankingLayer::editBoxReturn(CCEditBox* editBox) {
    CCLOG("return");
}

void RankingLayer::registerRanking()
{
    // Loading中を表示
    removeAllChildren();
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCLabelTTF* label = CCLabelTTF::create("Loading", "04b19", 22);
    label->setPosition(ccp(size.width/2, size.height/4*3));
    this->addChild(label);
    
    // セーブURL http://dreamlo.com/lb/-71rfB-JF0-SHPl86Gkokw4u2WXbq_-k2lVktH1xLskA/add/Carmine/200
    char url[100];
    sprintf(url, "http://dreamlo.com/lb/-71rfB-JF0-SHPl86Gkokw4u2WXbq_-k2lVktH1xLskA/add/%s/%i", userName.c_str(), myScore);
    CCString* ccurl = CCString::createWithFormat("http://dreamlo.com/lb/-71rfB-JF0-SHPl86Gkokw4u2WXbq_-k2lVktH1xLskA/add/%s/%i", userName.c_str(), myScore);
   
    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl(ccurl->getCString());
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, callfuncND_selector(RankingLayer::onSaveHttpRequestCompleted));
    request->setTag("save dreamlo");
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void RankingLayer::onSaveHttpRequestCompleted(cocos2d::CCNode *sender, void *data)
{
    CCHttpResponse *response = (CCHttpResponse*)data;
    CCHttpRequest* request = response->getHttpRequest();
    
    CCLOG("http request completed");
    CCLOG("url         :%s", request->getUrl());
    CCLOG("tag         :%s", request->getTag());
    CCLOG("status code :%d", response->getResponseCode());
    
    if (!response->isSucceed())
    {
        // Error表示
        CCLOG("error buffer: %s", response->getErrorBuffer());
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        CCLabelTTF* label = CCLabelTTF::create("ERROR", "04b19", 22);
        label->setPosition(ccp(size.width/2, size.height/2));
        this->addChild(label);
        return;
    }
    
    // 保存完了。ランキングを描画。
    requestRanking();

}

void RankingLayer::requestRanking()
{
    // jsonをリクエストする
    char url[100] = "http://dreamlo.com/lb/531dacc66e51b403484b753f/json";
    
    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl(url);
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, callfuncND_selector(RankingLayer::onHttpRequestCompleted));
    request->setTag("save dreamlo");
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void RankingLayer::onHttpRequestCompleted(cocos2d::CCNode *sender, void *data)
{
    CCHttpResponse *response = (CCHttpResponse*)data;
    CCHttpRequest* request = response->getHttpRequest();
    
    CCLOG("http request completed");
    CCLOG("url         :%s", request->getUrl());
    CCLOG("tag         :%s", request->getTag());
    CCLOG("status code :%d", response->getResponseCode());
    
    if (!response->isSucceed())
    {
        // Error表示
        CCLOG("error buffer: %s", response->getErrorBuffer());
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        CCLabelTTF* label = CCLabelTTF::create("ERROR", "04b19", 22);
        label->setPosition(ccp(size.width/2, size.height/2));
        this->addChild(label);
        return;
    }
    
    vector<char> *buffer = response->getResponseData();
    string bufferString(buffer->begin(), buffer->end());
    
    // 文字列の取得
    showRanking(bufferString);
}

void RankingLayer::showRanking(string data)
{
    removeAllChildren();

    // JSONのパース
    Json* json = Json_create(data.c_str());
    
    // 文字列の取得
    Json* dreamlo = Json_getItem(json, "dreamlo");
    Json* leaderboard = Json_getItem(dreamlo, "leaderboard");
    Json* entry = Json_getItem(leaderboard, "entry");
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    // BackGround
    CCSprite* bg = CCSprite::create("ranking_back.png",CCRectMake(0, 0, 1024, 577));
    bg->setPosition(ccp(winSize.width/2, winSize.height/2));
    addChild(bg);

    // mask
    CCSprite* mask = CCSprite::create("ranking_mask.png", CCRectMake(0, 0, MASK_WIDTH, MASK_HEIGHT));
    mask->setPosition(ccp(winSize.width/2, winSize.height/2));
    
    rankingScrollLayer = RankingScrollLayer::create();
    rankingScrollLayer->createThereds(entry);
    rankingScrollLayer->setPosition(ccp(0,winSize.height/2 + MASK_HEIGHT/2));
    defaultRankingScrollY = rankingScrollLayer->getPositionY();
    
    // マスクする
    CCClippingNode *clippingNode = CCClippingNode::create();
    // マスクするノードを設定
    clippingNode->setStencil(mask);
    // マスクの閾値の設定
    clippingNode->setAlphaThreshold(0.01f);
    // マスクされるノードを設定(白いノード)
    clippingNode->addChild(rankingScrollLayer);
    
    // マスクしたノードを表示する
    clippingNode->setPosition(0,0);

    // restartボタン
    CCMenuItemImage* restartButton = CCMenuItemImage::create("restart.png", "restart.png", this, menu_selector(RankingLayer::restart));
    restartButton->setPosition(ccp(winSize.width/2, restartButton->getContentSize().height));
    
    CCMenu* footerButtons = CCMenu::create(restartButton, NULL);
    footerButtons->setPosition(ccp(0, 0));
    this->addChild(footerButtons);

    this->addChild(clippingNode);
    
    // ランキングのスクロールのために
    // touchに反応させる
    this->setTouchMode(kCCTouchesAllAtOnce);
    this->setTouchEnabled(true);

}

// ランキングViewをスクロールする

void RankingLayer::ccTouchesBegan(CCSet* touches, CCEvent* event)
{
    CCTouch* touch = (CCTouch*)touches->anyObject();
    CCPoint location = touch->getLocation();
    _lastTouchPoint = CCDirector::sharedDirector()->convertToGL(location);
}

void RankingLayer::ccTouchesMoved(CCSet* touches, CCEvent* event)
{
    CCTouch* touch = (CCTouch*)touches->anyObject();
    CCPoint location = touch->getLocation();
    location = CCDirector::sharedDirector()->convertToGL(location);
    
    CCPoint delta = CCPointMake(_lastTouchPoint.x - location.x, _lastTouchPoint.y - location.y);
    float nextY = rankingScrollLayer->getPosition().y+delta.y;
    _lastTouchPoint = location;

    rankingScrollLayer->setPosition(ccp(rankingScrollLayer->getPositionX(), nextY));
}

void RankingLayer::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    // 離されたときの位置をみて戻す
    float nextY;
    if (rankingScrollLayer->getPositionY() < defaultRankingScrollY || rankingScrollLayer->getContentHeight() < MASK_HEIGHT)
    {
        nextY = defaultRankingScrollY;
        
        CCMoveTo* scroll = CCMoveTo::create(1, ccp(rankingScrollLayer->getPosition().x, nextY));
        CCActionInterval* ease = CCEaseBounceOut::create(scroll);
        rankingScrollLayer->runAction(ease);
    }
    else if (rankingScrollLayer->getPositionY() > defaultRankingScrollY + rankingScrollLayer->getContentHeight() - MASK_HEIGHT)
    {
        nextY = defaultRankingScrollY + rankingScrollLayer->getContentHeight() - MASK_HEIGHT;
        
        CCMoveTo* scroll = CCMoveTo::create(1, ccp(rankingScrollLayer->getPosition().x, nextY));
        CCActionInterval* ease = CCEaseBounceOut::create(scroll);
        rankingScrollLayer->runAction(ease);
    }
}

void RankingLayer::restart()
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