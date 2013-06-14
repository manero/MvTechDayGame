#include "GameScene.h"
#include "SimpleAudioEngine.h"

#define ENEMY_TAG 1000

using namespace cocos2d;

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
    
    CCSprite *heroi = CCSprite::create("CloseNormal.png");
    heroi->setPosition(ccp(heroi->boundingBox().getMaxX(), screenSize.height / 2.0f));
    heroi->setTag(666);
    this->addChild(heroi);
    
    this->schedule(schedule_selector(GameScene::update));
    this->schedule(schedule_selector(GameScene::spawnEnemy), 1.0f);

    setTouchMode(kCCTouchesOneByOne);
    registerWithTouchDispatcher();
    
    
    yOffsetValue = 0;
    numberOfEnemies = 0;
    return true;
}

void GameScene::update(float deltaTime) {
    CCNode *heroi = this->getChildByTag(666);
    heroi->setPosition(ccpAdd(heroi->getPosition(), ccp(1, yOffsetValue)));

    decayYOffsetValue();
    
    if (checkForGameOver()) {
        goToGameOverScene();
    }
    
    if (checkForVictory()) {
        goToVictoryScene();
    }
}

void GameScene::spawnEnemy() {
    CCNode *heroi = this->getChildByTag(666);
    CCSprite *enemy = CCSprite::create("CloseSelected.png");
    enemy->setPosition(ccp(heroi->getPosition().x + (3 * heroi->boundingBox().size.width), heroi->getPosition().y));
    enemy->setTag(ENEMY_TAG + numberOfEnemies);
    CCLog("criando inimigo na posição... x: %f y:%f", enemy->getPosition().x, enemy->getPosition().y);
    this->addChild(enemy);
    ++numberOfEnemies;
}

void GameScene::decayYOffsetValue() {
    yOffsetValue *= 0.98f;
    if (fabs(yOffsetValue) < 0.001f) {
        yOffsetValue = 0;
    }
}

bool GameScene::checkForGameOver() {
    CCNode *heroi = this->getChildByTag(666);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    if (
        heroi->getPosition().y > winSize.height //went too far up
        ||
        heroi->getPosition().y < 0 //went too far down
        ) {
        CCLog("GAMEOVER!!!!11111onze!11!");
        return true;
    }
    for (int i = ENEMY_TAG; i < ENEMY_TAG + numberOfEnemies; i++) {
        CCNode *enemy = this->getChildByTag(i);
        if (heroi->boundingBox().intersectsRect(enemy->boundingBox())) {
            CCLog("GAMEOVER!!!!11111onze!11!");
            return true;
        }
    }
    return false;
}

bool GameScene::checkForVictory() {
    CCNode *heroi = this->getChildByTag(666);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    if (heroi->getPosition().x > winSize.width) { // reached the far side of the screen
        CCLog("wiiiiiiiiinnnnnn!!!");
        return true;
    }
    return false;
}

void GameScene::goToGameOverScene() {
    //temporariamente somente reseta e inicia o jogo automaticamente.
    CCDirector::sharedDirector()->replaceScene(GameScene::scene());
}

void GameScene::goToVictoryScene() {
    //ainda faz a mesma coisa que o gameoverscene mas vai melhorar! :)
    CCDirector::sharedDirector()->replaceScene(GameScene::scene());
}
void GameScene::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


bool GameScene::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCLOG("touches began! position: x:%f y:%f", pTouch->getLocation().x, pTouch->getLocation().y);
    if (pTouch->getLocation().y > CCDirector::sharedDirector()->getWinSize().height / 2.0f ) {
        ++yOffsetValue;
    } else {
        --yOffsetValue;
    }
    return true;
}