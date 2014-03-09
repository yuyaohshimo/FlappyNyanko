#ifndef __GAME_LAYER__
#define __GAME_LAYER__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define GOAL_COUNT 100

class GameLayer : public CCLayer
, public CCBSelectorResolver
, public CCBMemberVariableAssigner
{
    enum EFoot
    {
        FOOT_UNKNOWN = 0,
        FOOT_LEFTFOOT = 1,
        FOOT_RIGHTFOOT = 2,
    };
    
    int mCount;
    EFoot mBeforeFoot;
    float mTime;
    CCSprite* mBg1;
    CCSprite* mBg2;
    CCSprite* mBg3;
    CCBAnimationManager* mAnimationManager;
    CCMenuItemImage* mLeftButton;
    CCMenuItemImage* mRightButton;
    CCLabelTTF* mTimeLabel;
    CCLabelTTF* mDistanceLabel;
    
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName);
    virtual bool onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode);
    virtual bool onAssignCCBCustomProperty(CCObject* pTarget, const char* pMemberVariableName, CCBValue* pCCBValue);
    
    void tappedLeftFoot(CCObject* pSender, CCControlEvent pCCControlEvent);
    void tappedRightFoot(CCObject* pSender, CCControlEvent pCCControlEvent);
    void tappedFoot(EFoot currentFoot);
    void changeTappable();
    void tappedResetButton(CCObject* pSender, CCControlEvent pCCControlEvent);
    void timer(float delta);
    
public:
    GameLayer();
    virtual ~GameLayer();
    
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(GameLayer, create);
    
    void setAnimationManager(CCBAnimationManager *pAnimationManager);
};

#endif /* __GAME_LAYER__ */