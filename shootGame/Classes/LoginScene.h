#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "ui/CocosGUI.h"
#include "network/HttpClient.h"
#include <string>
#include <vector>
#include "cocos2d.h"
#include "Player.h"
#include "Player.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "SimpleAudioEngine.h"
#include <string>
#include "GameScene.h"
#define database UserDefault::getInstance()
#pragma execution_character_set("utf-8")

USING_NS_CC;
using std::vector;
using std::string;
using namespace cocos2d::ui;
using namespace cocos2d::network;
using namespace rapidjson;
using namespace CocosDenshion;



class LoginScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    //为登录界面初始化背景图片并添加音乐
    virtual bool init();

    
	
	// implement the "static create()" method manually
	CREATE_FUNC(LoginScene);
	void loginClick(Ref* pSender);
	void onLoginCompleted(HttpClient* sender, HttpResponse* res);
	void preloadMusic();
	void playBgm();
private:
	float visibleHeight;
	float visibleWidth;
	TextField * userName;
};

#endif // __HELLOWORLD_SCENE_H__
