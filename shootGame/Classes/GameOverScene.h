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

	//初始化game over的场景，添加背景和game over的游戏音乐
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GameOverScene);

	//点击submit按钮触发该事件，能够使所填写的分数上传到服务器中
	void submitClickEventListener(Ref* sender);

	//用于提交函数后request请求结束后的数据处理的回调函数
	void submitOnHttpRequestCompleted(HttpClient *sender, HttpResponse* response);

	//点击rank按钮触发该事件能够获取服务器中的rank排名显示到游戏界面
	void rankClickEventListener(Ref* sender);

	//排名函数
	void rankOnHttpRequestCompleted(HttpClient *sender, HttpResponse* response);

	//通过该函数将所获的数据转换成格式化的排名形式
	string convertRequestData(string data);

	// 重新开始游戏
	void restartClickEventListener(Ref* Sender);

private:

	float visibleHeight;
	float visibleWidth;
	TextField * score_field;
	TextField * rank_field;
	Label* gameOver;//显示game over的字样

};

#endif // !BACKGROUND_H