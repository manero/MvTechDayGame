#include "GameScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* GameScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameScene *layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
//    screenSize.width
    
    CCSprite *heroi = CCSprite::create("CloseNormal.png");
    heroi->setPosition(ccp(heroi->boundingBox().getMaxX(), screenSize.height / 2.0f));
    heroi->setTag(666);
    this->addChild(heroi);
    
    this->schedule(schedule_selector(GameScene::update));

    return true;
}

void GameScene::update(float deltaTime) {
    CCNode *heroi = this->getChildByTag(666);
    heroi->setPosition(ccpAdd(heroi->getPosition(), ccp(1, 0)));
    
    
    if (checkForGameOver()) {
        CCDirector::sharedDirector()->replaceScene(GameScene::scene());
    }
    
}

bool GameScene::checkForGameOver() {
    CCNode *heroi = this->getChildByTag(666);
    if (heroi->getPosition().x > CCDirector::sharedDirector()->getWinSize().width) {
        return true;
    }
    return false;
}

void GameScene::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
