#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <string>
#include "network/HttpClient.h"
#include "Player.h"
#include "MonsterFactory.h"


USING_NS_CC;
using namespace std;
using namespace cocos2d::ui;
using namespace cocos2d::network;

class GameScene : public cocos2d::Layer
{
public:

	static cocos2d::Scene* createScene();

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);

	void preloadMusic();

	//播放背景音乐（有条件的话可以实现接触时爆炸音效，玩家中毒的音效）
	void playBackgroundMusic();

	//移动背景图片
	void moveBackground(float dt);

	//不断使用此函数判断玩家是否hp耗尽，耗尽则游戏结束
	void isGameOver(float dt);

	//create a hp item on the top
	void createHp();

	//create the background
	void createMap();

	//update the scene to create monster, move monster and 
	//judge if the monster hit or shoot the player and 
	//count the score that create by the player
	//
	void updateScene(float dt);

	//basic hp control event. when player was attacked by monster or hitted by monster, decrease the HP
	void hpMenuCallback(Ref* f);

    //更新怪物的产生情况
    void updateCreate(float dt);

	//在怪物完成动作后发出毒液
    void updatePoison(float dt);

	//触发返回开始菜单的触摸事件
	void BackClickEventListener(Ref* sender);

	//添加键盘监听事件
	void addKeyboardListener();

	//按下'a'按键时玩家左移，按下‘d’按键玩家右移，按下‘w’按键时玩家跳跃，按下空格时玩家射击子弹
	void onKeyPressed(EventKeyboard::KeyCode code, Event* event);

	//按键结束后处理事件
	void onKeyReleased(EventKeyboard::KeyCode code, Event* event);
	
	
private:
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	cocos2d::ProgressTimer* HP;
	cocos2d::Label* scoreCounter;
	Player* player;
	MonsterFactory* factory;

	char movekey;
	bool ismove;

	bool shootFlag;
	Sprite* bgsprite1;
	Sprite* bgsprite2;

	//用来盛放陷阱的容器，如果够时间可以加上去
	//Vector<Sprite*> traps;
};