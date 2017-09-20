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

	//���ű������֣��������Ļ�����ʵ�ֽӴ�ʱ��ը��Ч������ж�����Ч��
	void playBackgroundMusic();

	//�ƶ�����ͼƬ
	void moveBackground(float dt);

	//����ʹ�ô˺����ж�����Ƿ�hp�ľ����ľ�����Ϸ����
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

    //���¹���Ĳ������
    void updateCreate(float dt);

	//�ڹ�����ɶ����󷢳���Һ
    void updatePoison(float dt);

	//�������ؿ�ʼ�˵��Ĵ����¼�
	void BackClickEventListener(Ref* sender);

	//��Ӽ��̼����¼�
	void addKeyboardListener();

	//����'a'����ʱ������ƣ����¡�d������������ƣ����¡�w������ʱ�����Ծ�����¿ո�ʱ�������ӵ�
	void onKeyPressed(EventKeyboard::KeyCode code, Event* event);

	//�������������¼�
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

	//����ʢ������������������ʱ����Լ���ȥ
	//Vector<Sprite*> traps;
};