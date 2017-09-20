#pragma once

#include <iostream>
#include "GameScene.h"
#include "GameOverScene.h"
#include "LoginScene.h"
#include "SimpleAudioEngine.h"
#include <algorithm>
using namespace CocosDenshion;
using namespace std;


Scene* GameScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	
	// 'layer' is an autorelease object
	auto layer = GameScene::create();

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
	if (!Layer::init())
	{
		return false;
	}
	preloadMusic();
	ismove = false;

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	factory = MonsterFactory::getInstance();
	player = Player::getInstance();
	
	//create map
	createMap();

	//create a piece of Hp item
	createHp();

	addKeyboardListener();

	//update the sence;
	schedule(schedule_selector(GameScene::updateScene), 0.05f, kRepeatForever, 0);
	schedule(schedule_selector(GameScene::updateCreate), 4.0f, kRepeatForever, 0);
	schedule(schedule_selector(GameScene::updatePoison), 0.01f, kRepeatForever, 0);
	schedule(schedule_selector(GameScene::moveBackground), 0.01f, kRepeatForever, 0);
	schedule(schedule_selector(GameScene::isGameOver), 1.0f, kRepeatForever, 0);
	
	playBackgroundMusic();
	return true;
}

void GameScene::addKeyboardListener() {
	// Todo
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

void GameScene::preloadMusic()
{
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("play_music/bg.mp3");
	SimpleAudioEngine::getInstance()->preloadEffect("play_music/shoot_bullets.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("play_music/explore.wav");
}

void GameScene::playBackgroundMusic()
{
	SimpleAudioEngine::getInstance()->playBackgroundMusic("play_music/bg.mp3", true);
}

void GameScene::moveBackground(float dt)
{
	int X1 = bgsprite1->getPositionX();
	int X2 = bgsprite2->getPositionX();
	int speed = 1;
	X1 -= speed;
	X2 -= speed;
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	if (X1 <= -size.width / 2 + 1)
	{
		X2 = size.width / 2;
		X1 = size.width / 2 + size.width;
	}
	if (X2 <= -size.width / 2 + 1)
	{
		X1 = size.width / 2;
		X2 = size.width / 2 + size.width;
	}
	bgsprite1->setPositionX(X1);
	bgsprite2->setPositionX(X2);
}

void GameScene::isGameOver(float dt)
{
	if (player->getHp() <= 0) {
		unschedule(schedule_selector(GameScene::moveBackground));
		unschedule(schedule_selector(GameScene::isGameOver));
		unschedule(schedule_selector(GameScene::updateScene));
		unschedule(schedule_selector(GameScene::updateCreate));
		unschedule(schedule_selector(GameScene::updatePoison));
		Director::getInstance()->replaceScene(TransitionCrossFade::create(0.3f, GameOverScene::createScene()));
	}
}

void GameScene::createHp()
{
	Sprite* sp0 = Sprite::create("play_pic/hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
	Sprite* sp = Sprite::create("play_pic/hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(610, 362, 4, 16)));

	//使用HP条设置progressBar
	HP = ProgressTimer::create(sp);
	HP->setScaleX(90);
	HP->setAnchorPoint(Vec2(0, 0));
	HP->setType(ProgressTimerType::BAR);
	HP->setBarChangeRate(Point(1, 0));
	HP->setMidpoint(Point(0, 1));
	HP->setPercentage(100);
	HP->setPosition(Vec2(origin.x + 14 * HP->getContentSize().width, origin.y + visibleSize.height - 2 * HP->getContentSize().height));
	addChild(HP, 1);
	sp0->setAnchorPoint(Vec2(0, 0));
	sp0->setPosition(Vec2(origin.x + HP->getContentSize().width, origin.y + visibleSize.height - sp0->getContentSize().height));
	addChild(sp0, 0);
}

void GameScene::createMap()
{
	bgsprite1 = CCSprite::create("play_pic/bg1/stage1-1.jpg");
	bgsprite2 = CCSprite::create("play_pic/bg1/stage1-1.jpg");

	bgsprite1->setScale(visibleSize.width / bgsprite1->getContentSize().width,
		visibleSize.height / bgsprite1->getContentSize().height);
	bgsprite2->setScale(visibleSize.width / bgsprite2->getContentSize().width,
		visibleSize.height / bgsprite2->getContentSize().height);

	CCSize size = CCDirector::sharedDirector()->getWinSize();
	bgsprite1->setPosition(ccp(size.width / 2, size.height / 2));
	bgsprite2->setPosition(ccp(size.width + size.width / 2, size.height / 2));
	bgsprite2->setFlipX(true);

	this->addChild(bgsprite1, 0);
	this->addChild(bgsprite2, 0);

	player->getSelf()->setPosition(Vec2(0, visibleSize.height / 5));
	this->addChild(player->getSelf(), 2);
	player->move('d');

	TTFConfig labelConfig;
	labelConfig.fontFilePath = "fonts/arial.ttf";
	labelConfig.fontSize = 36;
	labelConfig.glyphs = GlyphCollection::DYNAMIC;
	labelConfig.outlineSize = 0;
	labelConfig.customGlyphs = nullptr;
	labelConfig.distanceFieldEnabled = false;
	scoreCounter = Label::createWithTTF(labelConfig, "0");
	scoreCounter->setColor(Color3B(0, 0, 0));
	scoreCounter->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + scoreCounter->getContentSize().height + 550));
	addChild(scoreCounter);
}

void GameScene::updateScene(float dt)  //dt = 0.01
{
  /*怪物移动*/
  if (ismove) {
    player->move(movekey);
  }
  
  /*判断怪物是否与子弹相撞*/
  auto all = player->getBullets();
  for (auto i = 0; i < all.size(); i++)
  {
    Sprite* collision = factory->colliderWithBullets(all.at(i)->getBoundingBox());
    if (collision != NULL) {
		all.at(i)->stopAllActions();
      factory->removeMonster(collision);
      player->setScore(player->getScore() + 10);
      int n = player->getScore();
      std::stringstream ss;
      std::string str;
      ss << n;
      ss >> str;
      scoreCounter->setString(str);
	  player->removeBullet(all.at(i));
    }
    
  }

  //检测玩家是否与毒液碰撞
  bool ishit = false;
  auto poisons = factory->getPoison();
  for (auto it = poisons.begin(); it != poisons.end(); )
  {
    ishit = false;
    if (player->getSelf()->getBoundingBox().intersectsRect((*it)->getBoundingBox()))
    {
      /*
       *hp变化
       */
	  hpMenuCallback(this);
      ishit = true;
	  factory->removePoison(*it);
      
      break;
    }
    if (!ishit) ++it;
  }

  //判断玩家是否和怪物相撞
  Sprite* collision = factory->colliderWithPlayer(player->getSelf()->getBoundingBox());
  if (collision != NULL) {
    factory->removeMonster(collision);

    /*
    *hp变化
    */
	hpMenuCallback(this);
  }
}

void GameScene::updateCreate(float dt)   //dt = 4
{
  // 产生怪物
  auto monster = factory->createMonster();
  addChild(monster, 2);
  Vec2 location = player->getSelf()->getPosition();
  factory->moveMonster(location, 2.0f);
  shootFlag = true;
}

void GameScene::updatePoison(float dt)
{
	//怪物产生毒液
	auto monsters = factory->getMonster();
	if (monsters.size() == 0 || shootFlag == false)
	{
		return;
	}
	auto i = 0;
	for (; i < monsters.size() - 1; i++)
	{
	}
	Sprite* temp = monsters.at(i);
	if (temp->numberOfRunningActions() == 0)
	{
	auto poisons = factory->shootPoison();
	for (auto i = 0; i < poisons.size(); i++)
	{
		addChild(poisons.at(i), 2);
	}
	shootFlag = false;
	}
}
void GameScene::hpMenuCallback(Ref * f)
{
	float percentage = HP->getPercentage();
	if (percentage >= 10) {
		HP->runAction(CCProgressTo::create(1.5f, HP->getPercentage() - 10));
		player->setHp(player->getHp() - 10);
	}
	else {
		HP->runAction(CCProgressTo::create(1.5f, 0));
		player->setHp(0);
	}
}

void GameScene::BackClickEventListener(Ref * sender)
{
	auto scene = LoginScene::createScene();
	Director::getInstance()->replaceScene((Scene*)scene);
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode code, Event * event)
{
	switch (code) {
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_A:
	case EventKeyboard::KeyCode::KEY_A:
    movekey = 'a';
    ismove = true;
		player->move('a');
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_D:
	case EventKeyboard::KeyCode::KEY_D:
    movekey = 'd';
    ismove = true;
		player->move('d');
    
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
	case EventKeyboard::KeyCode::KEY_CAPITAL_W:
	case EventKeyboard::KeyCode::KEY_W:
		player->jump();
		break;
	case EventKeyboard::KeyCode::KEY_SPACE:
		addChild(player->shoot(), 2);
		break;
	}
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode code, Event * event)
{
  ismove = false;
	return;
}