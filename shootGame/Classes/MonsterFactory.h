#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

class MonsterFactory
{
public:
	
	//获取单例工厂
	static MonsterFactory* getInstance();

	//生成一个怪物，并存储到容器中管理,在游戏界面的右边产生怪物
	Sprite* createMonster();
	
	//让容器中的所有怪物都往角色移动，通过容器管理所有的怪物很方便
	void moveMonster(Vec2 playerPos, float time);
	
	Vector<Sprite*> shootPoison();

	//移除怪物
	void removeMonster(Sprite*);
	
	

	Sprite* createPoison();

	void movePoison(Vec2 playerPos, float time);

	void removePoison(Sprite*);

	//判断玩家子弹是否撞到怪物，如果没有则返回空，有就返回该怪物对象
	Sprite* colliderWithBullets(Rect rect);

	//判断玩家是否撞到怪物，撞到则返回改怪物
	Sprite* colliderWithPlayer(Rect rect);

	
	//初始化怪物帧动画
	void initSpriteFrame();

	//获取所有的怪物
	Vector<Sprite*> getMonster();

	//获取所有的毒液
	Vector<Sprite*> getPoison();
	
	void monsterReset();
private:
	MonsterFactory();
	Vector<Sprite*> monster;
	cocos2d::Vector<SpriteFrame*> monsterDead;
	cocos2d::Vector<SpriteFrame*> monsterMove;
	cocos2d::Vector<SpriteFrame*> monsterShoot;
	static MonsterFactory* factory;
	Vector<Sprite*> poisons;
};

