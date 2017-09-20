#ifndef PLAYER_h
#define PLAYER_h

#pragma once
#include "cocos2d.h"
#include <regex>
#include <string>
#include <vector>

USING_NS_CC;
using std::vector;
using std::string;
using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;

class Player: public cocos2d::Sprite
{
public:
	
	//通过该函数来获取一个存在的玩家实例若不存在则创建一个。
	//该函数返回一个共享的玩家实例
	static Player* getInstance();

	//将request请求所获得的数据变为字符串类型
	string toString(vector<char> *buffer);

	//将request请求中的head部分中的gameSessionId提取出来
	string getSessionIdFromHeader(string header);

	string getGameSessionId();

	long getScore();
	
	Vector<Sprite*> getBullets();
	
	int getHp();
	
	void setHp(int temp);
	
	void setGameSessionId(string temp);
	
	void setScore(int temp);
	
	void setBullets(Vector<Sprite*>);

	void removeBullet(Sprite* sp);
	
	//根据所给的flag来决定移动的方向，‘a’代表向左移动，'d'代表向右移动
	void move(char flag);
	
	//该函数实现让玩家跳起一段距离后又落下的功能
	void jump();
	
	//产生一个bullet对象的同时把它给射出去
	Sprite* shoot();

	Sprite* getSelf();
	
	//
	bool colliderWithPoison(Vec2 temp);
	
	void playerReset();
private:
	//切割并初始化player的移动,射击和死亡的动画
	void initSpriteFrame();
	Player();
	static Player* player;
	Sprite* self;//人物角色

	string gameSessionId;
	int score;
	int hp;
  char movekey;
	cocos2d::Vector<SpriteFrame*> playerMove;
	cocos2d::Vector<SpriteFrame*> playerShoot;
	cocos2d::Vector<SpriteFrame*> playerDead;
	cocos2d::Vector<Sprite*> bullets;
};


#endif // !PLAYER_h
