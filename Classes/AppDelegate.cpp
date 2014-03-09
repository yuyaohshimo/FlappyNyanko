#include "AppDelegate.h"
#include "AppMacros.h"
#include "cocos-ext.h"
#include "GameLayerLoader.h"

USING_NS_CC;
USING_NS_CC_EXT;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
	
    // デザインサイズの設定
    pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionNoBorder);
    
    CCSize frameSize = pEGLView->getFrameSize();
    
    std::vector<std::string> searchPath;
    
    if (frameSize.height > largeResource.size.height)
    {
        // iPad Retina用リソースを使用
        searchPath.push_back(xlargeResource.directory);
        pDirector->setContentScaleFactor(MIN(xlargeResource.size.height / designResolutionSize.height, xlargeResource.size.width / designResolutionSize.width));
    }
    else if (frameSize.height > smallResource.size.height)
    {
        // iPad用リソースを使用
        searchPath.push_back(largeResource.directory);
        pDirector->setContentScaleFactor(MIN(largeResource.size.height / designResolutionSize.height, largeResource.size.width / designResolutionSize.width));
    }
    else
    {
        // iPhone用リソースを使用
        searchPath.push_back(smallResource.directory);
        pDirector->setContentScaleFactor(MIN(smallResource.size.height / designResolutionSize.height, smallResource.size.width / designResolutionSize.width));
    }
    
    // リソースディレクトリを指定
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPath);
    pDirector->setDisplayStats(true);
    pDirector->setAnimationInterval(1.0 / 60);

    // CocosBuilderのファイルを読込みゲーム画面を生成する
    CCNodeLoaderLibrary* ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("GameLayer", GameLayerLoader::loader());
    CCBReader* ccbReader = new CCBReader(ccNodeLoaderLibrary);
    CCNode* node = ccbReader->readNodeGraphFromFile("GameLayer.ccbi");
    ((GameLayer*)node)->setAnimationManager(ccbReader->getAnimationManager());
    
    // シーンを用意し、ゲーム画面を設置する
    CCScene* pScene = CCScene::create();
    if (node != NULL)
        pScene->addChild(node);
    ccbReader->release();
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
