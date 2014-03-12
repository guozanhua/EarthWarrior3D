//
//  ScrollingBackground.cpp
//  Moon3d
//
//  Created by Hao Wu on 2/24/14.
//
//

#include "GameLayer.h"
#include "Player.h"
#include "Fodder.h"
#include "PublicApi.h"
#include "BulletController.h"
#include "consts.h"
#include "Bullet.h"
#include "EnemyManager.h"
#include "Effects.h"
#include "GameEntity.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace std;

bool GameLayer::init()
{
    // variable init
    //_collisionTree = new QuadTree(0, BOUND_RECT);
    
    
    spr = Sprite::create("groundLevel.jpg");
    addChild(spr);
    CCTexture2D::TexParams texRepeat = {GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_REPEAT};
    spr->getTexture()->setTexParameters(texRepeat);
    setRotation3D(Vertex3F(-30.0,0.0f,0.0f));
    spr->setScale(1.4);
    //spr->setFlippedY(true);
    spr->setPosition(0.0f,400.0f);
    
    _player = Player::create();
    
    _streak = MotionStreak::create(0.4, 1, 15, Color3B(82,255,253), "streak.png");
    _player->setTrail(_streak);
    addChild(_streak);
    
    addChild(_player,10);
    
    auto Audio = CocosDenshion::SimpleAudioEngine::getInstance();
    Audio->preloadEffect("boom.mp3");
    Audio->preloadEffect("hit.mp3");
    Audio->preloadEffect("boom2.mp3");

    this->schedule(schedule_selector(GameLayer::createCraft) , 1.0, -1, 0.0);

    BulletController::init(this);
    scheduleUpdate();
    
    return true;
}

void GameLayer::createCraft(float dt)
{
    EnemyManager * aEnemyManager = EnemyManager::sharedEnemyManager();
    if(0 == aEnemyManager->standByEnemyVect.size())
    {
        return;
    }
    int randStandByEnemy= rand()%aEnemyManager->standByEnemyVect.size();

    
    Fodder * enemy = static_cast<Fodder *>(aEnemyManager->standByEnemyVect.at(randStandByEnemy));
   // enemy->setPosition(positionX,900.0f);
    this->addChild(enemy);
    
    aEnemyManager->availabelEnemyVect.pushBack(aEnemyManager->standByEnemyVect.at(randStandByEnemy));

    aEnemyManager->standByEnemyVect.eraseObject(static_cast<AirCraft *>(aEnemyManager->standByEnemyVect.at(randStandByEnemy)),false);

    int idx = aEnemyManager->availabelEnemyVect.size()-1;
    enemy->move(Point(enemy->getPosition3D().x,-visible_size_macro.height*0.5),aEnemyManager->availabelEnemyVect.at(idx));
}

void GameLayer::update(float dt)
{
    xScroll += speed*dt;
    spr->setTextureRect(Rect(0,((int)xScroll)%2048,512,1200));

    BulletController::update(dt);
    //_collisionTree->clear();
    //_streak->setPosition(_player->getPosition()-Point(0,40));
}


