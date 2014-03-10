//
//  ScoreScene.cpp
//  FlappyNyanko
//
//  Created by cocos2d-x on 14/03/09.
//
//

#include "ScoreScene.h"
#include "curl/curl.h"
#include "HttpRequest.h"
#include "HttpClient.h"
using namespace cocos2d;
using namespace std;

CCScene* ScoreScene::scene()
{
    CCScene* scene = CCScene::create();
    ScoreScene* layer = ScoreScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool ScoreScene::init()
{
    return true;
};

void ScoreScene::showScore(CCString* userName, int score)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    // saveData
    saveScore(userName, score);

    
    // GameOver
//    CCLabelTTF* label = CCLabelTTF::create("Game Over", "Marker Felt", 16);
//    label->setPosition(ccp(winSize.width/2, winSize.height - 100 + label->getContentSize().height));
//    this->addChild(label);
    
    // score
    CCString* scoreText = CCString::createWithFormat("%d", score);
    CCLabelTTF* scoreLabel = CCLabelTTF::create(scoreText->getCString(), "Marker Felt", 16);
    scoreLabel->setPosition(ccp(winSize.width/2, winSize.height - 100 + scoreLabel->getContentSize().height));
    this->addChild(scoreLabel);

    // best
    
    // new best
    
    // replay button
    
    // ranking button

}

static size_t onCurlWriteData(char *ptr, size_t size,
                              size_t nmemb, void *userdata)
{
    // 受信データをバッファに保存する
    std::vector<char> *buff = (std::vector<char>*)userdata;
    size_t sizes = size * nmemb;
    buff->insert(buff->end(), (char*)ptr, (char*)ptr+sizes);
    return sizes;
}

//bool ScoreScene::getTextFromUrl(const char* url,
//                              std::vector<char> *response)
//{
//    CURL *curl;
//    CURLcode res = CURLE_FAILED_INIT;
//    
//    curl = curl_easy_init();
//    if (curl)
//    {
//        curl_easy_setopt(curl, CURLOPT_URL, url);
//        // 受信データ保存関数を指定する
//        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,onCurlWriteData);
//        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
//        res = curl_easy_perform(curl);
//        curl_easy_cleanup(curl);
//    }
//    if (res == CURLE_OK && response->size())
//    {
//        return true;
//    }
//    return false;
//}

void ScoreScene::saveScore(CCString* userName, int score)
{
    // セーブURL http://dreamlo.com/lb/-71rfB-JF0-SHPl86Gkokw4u2WXbq_-k2lVktH1xLskA/add/Carmine/200
    CCString* url = CCString::createWithFormat("http://dreamlo.com/lb/-71rfB-JF0-SHPl86Gkokw4u2WXbq_-k2lVktH1xLskA/add/%s/%d","userName",score);
    
    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl(url->getCString());
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, callfuncND_selector(ScoreScene::onHttpRequestCompleted));
    request->setTag("save dreamlo");
    CCHttpClient::getInstance()->send(request);
    request->release();
}


void ScoreScene::onHttpRequestCompleted(cocos2d::CCNode *sender, void *data)
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
        return;
    }
    
    // 保存成功
    showSaveComplete();
    
    // dump data
//    std::vector<char> *buffer = response->getResponseData();
//    printf("Http Test, dump data: ");
//    for (unsigned int i = 0; i < buffer->size(); i++)
//    {
//        printf("%c", (*buffer)[i]);
//    }
//    printf("\n");
}

void ScoreScene::showSaveComplete()
{
    
}
