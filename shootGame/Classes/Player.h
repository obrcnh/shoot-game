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
	
	//ͨ���ú�������ȡһ�����ڵ����ʵ�����������򴴽�һ����
	//�ú�������һ����������ʵ��
	static Player* getInstance();

	//��request��������õ����ݱ�Ϊ�ַ�������
	string toString(vector<char> *buffer);

	//��request�����е�head�����е�gameSessionId��ȡ����
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
	
	//����������flag�������ƶ��ķ��򣬡�a�����������ƶ���'d'���������ƶ�
	void move(char flag);
	
	//�ú���ʵ�����������һ�ξ���������µĹ���
	void jump();
	
	//����һ��bullet�����ͬʱ���������ȥ
	Sprite* shoot();

	Sprite* getSelf();
	
	//
	bool colliderWithPoison(Vec2 temp);
	
	void playerReset();
private:
	//�и��ʼ��player���ƶ�,����������Ķ���
	void initSpriteFrame();
	Player();
	static Player* player;
	Sprite* self;//�����ɫ

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
