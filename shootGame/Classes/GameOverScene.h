#ifndef BACKGROUND_H
#define BACKGROUND_H
#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <string>
#include "network/HttpClient.h"
#include "Player.h"

using namespace cocos2d::ui;
using namespace cocos2d::network;
using std::string;

class GameOverScene :public cocos2d::Layer
{
public:

	void preloadMusic();
	void playBgm();
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	//��ʼ��game over�ĳ�������ӱ�����game over����Ϸ����
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GameOverScene);

	//���submit��ť�������¼����ܹ�ʹ����д�ķ����ϴ�����������
	void submitClickEventListener(Ref* sender);

	//�����ύ������request�������������ݴ���Ļص�����
	void submitOnHttpRequestCompleted(HttpClient *sender, HttpResponse* response);

	//���rank��ť�������¼��ܹ���ȡ�������е�rank������ʾ����Ϸ����
	void rankClickEventListener(Ref* sender);

	//��������
	void rankOnHttpRequestCompleted(HttpClient *sender, HttpResponse* response);

	//ͨ���ú��������������ת���ɸ�ʽ����������ʽ
	string convertRequestData(string data);

	// ���¿�ʼ��Ϸ
	void restartClickEventListener(Ref* Sender);

private:

	float visibleHeight;
	float visibleWidth;
	TextField * score_field;
	TextField * rank_field;
	Label* gameOver;//��ʾgame over������

};

#endif // !BACKGROUND_H