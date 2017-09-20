#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;

class MonsterFactory
{
public:
	
	//��ȡ��������
	static MonsterFactory* getInstance();

	//����һ��������洢�������й���,����Ϸ������ұ߲�������
	Sprite* createMonster();
	
	//�������е����й��ﶼ����ɫ�ƶ���ͨ�������������еĹ���ܷ���
	void moveMonster(Vec2 playerPos, float time);
	
	Vector<Sprite*> shootPoison();

	//�Ƴ�����
	void removeMonster(Sprite*);
	
	

	Sprite* createPoison();

	void movePoison(Vec2 playerPos, float time);

	void removePoison(Sprite*);

	//�ж�����ӵ��Ƿ�ײ��������û���򷵻ؿգ��оͷ��ظù������
	Sprite* colliderWithBullets(Rect rect);

	//�ж�����Ƿ�ײ�����ײ���򷵻ظĹ���
	Sprite* colliderWithPlayer(Rect rect);

	
	//��ʼ������֡����
	void initSpriteFrame();

	//��ȡ���еĹ���
	Vector<Sprite*> getMonster();

	//��ȡ���еĶ�Һ
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

