#include "GameLayer.h"
#include "GameLayerLoader.h"

GameLayer::GameLayer()
: mBg1(NULL)
, mBg2(NULL)
, mBg3(NULL)
, mAnimationManager(NULL)
, mLeftButton(NULL)
, mRightButton(NULL)
, mTimeLabel(NULL)
, mDistanceLabel(NULL)
{
    //mCount = 0;
    //mBeforeFoot = FOOT_UNKNOWN;
    //mTime = 0;
}

GameLayer::~GameLayer()
{
    CC_SAFE_RELEASE(mBg1);
    CC_SAFE_RELEASE(mBg2);
    CC_SAFE_RELEASE(mBg3);
    CC_SAFE_RELEASE_NULL(mAnimationManager);
    CC_SAFE_RELEASE(mLeftButton);
    CC_SAFE_RELEASE(mRightButton);
    CC_SAFE_RELEASE(mTimeLabel);
    CC_SAFE_RELEASE(mDistanceLabel);
}

SEL_MenuHandler GameLayer::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "tappedLeftFoot", GameLayer::tappedLeftFoot);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "tappedRightFoot", GameLayer::tappedRightFoot);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "tappedReset", GameLayer::tappedResetButton);
    
    return NULL;
}

void GameLayer::tappedLeftFoot(CCObject* pSender, CCControlEvent pCCControlEvent)
{
    tappedFoot(FOOT_LEFTFOOT);
}

void GameLayer::tappedRightFoot(CCObject* pSender, CCControlEvent pCCControlEvent)
{
    tappedFoot(FOOT_RIGHTFOOT);
}

SEL_CCControlHandler GameLayer::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName)
{
    return NULL;
}

bool GameLayer::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBg1", CCSprite*, this->mBg1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBg2", CCSprite*, this->mBg2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBg3", CCSprite*, this->mBg3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mLeftButton", CCMenuItemImage*, this->mLeftButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mRightButton", CCMenuItemImage*, this->mRightButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mTime", CCLabelTTF*, this->mTimeLabel);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mDistance", CCLabelTTF*, this->mDistanceLabel);
    
    return false;
}

void GameLayer::tappedFoot(EFoot currentFoot)
{
    if (mCount == 0)
        schedule(schedule_selector(GameLayer::timer));
    
    if (mBeforeFoot == currentFoot)
    {
        mAnimationManager->runAnimationsForSequenceNamedTweenDuration("Tumble", 0);
        mBeforeFoot = FOOT_UNKNOWN;
        mLeftButton->setEnabled(false);
        mRightButton->setEnabled(false);
        scheduleOnce(schedule_selector(GameLayer::changeTappable), 1.0);
    }
    else
    {
        mCount++;
        mDistanceLabel->setString(CCString::createWithFormat("%.1fm", mCount * 0.5)->getCString());
        if (mCount >= GOAL_COUNT)
        {
            unschedule(schedule_selector(GameLayer::timer));
            mAnimationManager->runAnimationsForSequenceNamedTweenDuration("Goal", 0);
            mLeftButton->setEnabled(false);
            mRightButton->setEnabled(false);
            return;
        }
        
        mBeforeFoot = currentFoot;
        
        switch (currentFoot)
        {
            case FOOT_LEFTFOOT:
                mAnimationManager->runAnimationsForSequenceNamedTweenDuration("Leftfoot", 0);
                break;
                
            case FOOT_RIGHTFOOT:
                mAnimationManager->runAnimationsForSequenceNamedTweenDuration("Rightfoot", 0);
                break;
                
            default:
                return;
        }
        
        const float moveTime = 1.0 / 3;
        float moveDistanceForBG3 = (this->mBg3->getContentSize().width - CCDirector::sharedDirector()->getWinSize().width) / GOAL_COUNT;
        
        CCMoveBy* move1 = CCMoveBy::create(moveTime, ccp(-moveDistanceForBG3 / 4, 0));
        this->mBg1->runAction(move1);
        
        CCMoveBy* move2 = CCMoveBy::create(moveTime, ccp(-moveDistanceForBG3 / 2, 0));
        this->mBg2->runAction(move2);
        
        CCMoveBy* move3 = CCMoveBy::create(moveTime, ccp(-moveDistanceForBG3, 0));
        this->mBg3->runAction(move3);
        
        float moveDistanceForCat = CCDirector::sharedDirector()->getWinSize().width / 2 / GOAL_COUNT;
        
        CCNode* cat = this->getChildByTag(1);
        CCMoveBy* move = CCMoveBy::create(moveTime, ccp(moveDistanceForCat, 0));
        cat->runAction(move);
    }
}

void GameLayer::setAnimationManager(CCBAnimationManager *pAnimationManager)
{
    CC_SAFE_RELEASE_NULL(mAnimationManager);
    mAnimationManager = pAnimationManager;
    CC_SAFE_RETAIN(mAnimationManager);
}

void GameLayer::changeTappable()
{
    mLeftButton->setEnabled(true);
    mRightButton->setEnabled(true);
    mAnimationManager->runAnimationsForSequenceNamedTweenDuration("Normal", 0);
}

void GameLayer::timer(float delta)
{
    mTime += delta;
    mTimeLabel->setString(CCString::createWithFormat("%.2fs", mTime)->getCString());
}

void GameLayer::tappedResetButton(CCObject* pSender, CCControlEvent pCCControlEvent)
{
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("GameLayer", GameLayerLoader::loader());
    CCBReader * ccbReader = new CCBReader(ccNodeLoaderLibrary);
    CCNode* node = ccbReader->readNodeGraphFromFile("GameLayer.ccbi");
    ((GameLayer*)node)->setAnimationManager(ccbReader->getAnimationManager());
    
    CCScene * scene = CCScene::create();
    if(node != NULL)
        scene->addChild(node);
    ccbReader->release();
    CCDirector::sharedDirector()->replaceScene(scene);
}

bool GameLayer::onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, CCBValue* pCCBValue)
{
    bool bRet = false;
    if (pTarget == this)
    {
        if (0 == strcmp(pMemberVariableName, "mCount"))
        {
            this->mCount = pCCBValue->getIntValue();
            bRet = true;
        }
        else if ( 0 == strcmp(pMemberVariableName, "mBeforeFoot"))
        {
            this->mBeforeFoot = (EFoot)pCCBValue->getIntValue();
            bRet = true;
        }
        else if ( 0 == strcmp(pMemberVariableName, "mTime"))
        {
            this->mTime = pCCBValue->getFloatValue();
            bRet = true;
        }
    }
    
    return bRet;
}

