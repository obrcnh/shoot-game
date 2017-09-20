#include "GameOverScene.h"
#include "Player.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "SimpleAudioEngine.h"
#include <regex>
#include <string>
#include <iostream>
#include <sstream>
#include "GameScene.h"
#define database UserDefault::getInstance()
#pragma execution_character_set("utf-8")

using std::regex;
using std::match_results;
using std::regex_match;
using std::cmatch;
using namespace std;
using namespace rapidjson;
using namespace rapidjson;
using namespace CocosDenshion;
USING_NS_CC;

Scene* GameOverScene::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameOverScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool GameOverScene::init()
{
	// 1. super init first
	if (!Layer::init()) {
		return false;
	}

	// 预加载背景音乐
	preloadMusic();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	visibleHeight = visibleSize.height;
	visibleWidth = visibleSize.width;

	//添加背景图片
	auto bgSprite = Sprite::create("end_pic/ending_bg.jpg");
	bgSprite->setPosition(visibleSize / 2);
	bgSprite->setScale(visibleSize.width / bgSprite->getContentSize().width, visibleSize.height / bgSprite->getContentSize().height);
	this->addChild(bgSprite, 0);

	// 提交分数按钮
	auto submitLabel = Label::createWithTTF("S u b m i t", "fonts/Marker Felt.ttf", 48);
	submitLabel->setColor(Color3B(187, 255, 255));
	auto submitBtn = MenuItemLabel::create(submitLabel, CC_CALLBACK_1(GameOverScene::submitClickEventListener, this));
	Menu* submit = Menu::create(submitBtn, NULL);
	submit->setPosition(visibleWidth / 2 - 300, 50);
	this->addChild(submit);

	// 查看排名按钮
	auto rankLabel = Label::createWithTTF("R a n k", "fonts/Marker Felt.ttf", 48);
	rankLabel->setColor(Color3B(187, 255, 255));
	auto rankBtn = MenuItemLabel::create(rankLabel, CC_CALLBACK_1(GameOverScene::rankClickEventListener, this));
	Menu* rank = Menu::create(rankBtn, NULL);
	rank->setPosition(visibleWidth / 2, 50);
	this->addChild(rank);

	// 重新开始按钮
	auto restartLabel = Label::createWithTTF("R e s t a r t", "fonts/Marker Felt.ttf", 48);
	restartLabel->setColor(Color3B(187, 255, 255));
	auto restartBtn = MenuItemLabel::create(restartLabel, CC_CALLBACK_1(GameOverScene::restartClickEventListener, this));
	Menu* restart = Menu::create(restartBtn, NULL);
	restart->setPosition(visibleWidth / 2 + 300, 50);
	this->addChild(restart);

	stringstream ss;
	ss << Player::getInstance()->getScore();
	string s = ss.str();
	auto infoLabel = Label::createWithTTF("final score:  " + s, "fonts/Marker Felt.ttf", 30);
	infoLabel->setColor(Color3B(187, 255, 255));
	infoLabel->setPosition(visibleWidth / 2, visibleHeight - 100);
	this->addChild(infoLabel);

	rank_field = TextField::create("", "Arial", 30);
	rank_field->setColor(Color3B(187, 255, 255));
	rank_field->setPosition(Size(visibleWidth / 2, visibleHeight / 2));
	this->addChild(rank_field, 2);

	
	
	// 添加背景音乐
	playBgm();

	return true;
}

void GameOverScene::rankClickEventListener(Ref* sender)
{
	HttpRequest* request = new HttpRequest();
	request->setRequestType(HttpRequest::Type::GET);
	request->setUrl("http://localhost:8080/rank?top=10");
	request->setResponseCallback(CC_CALLBACK_2(GameOverScene::rankOnHttpRequestCompleted, this));

	request->setTag("Get rank");

	vector<string> headers;
	headers.push_back("cookie: GAMESESSIONID=" + Player::getInstance()->getGameSessionId());
	request->setHeaders(headers);

	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

string GameOverScene::convertRequestData(string data)
{
	string result;
	string finalResult;

	bool flag = false;
	for (auto i = data.size() - 1; i >= 0; i--)
	{
		if (data[i] == '"')
		{
			if (flag == true) {
				flag = false;
				break;
			}
			else {
				flag = true;
			}
			i--;
		}
		if (flag == true)
		{
			if (data[i] == '|')
			{
				result.push_back('\n');
			}
			else {
				result.push_back(data[i]);
			}

		}
	}

	reverse(result.begin(), result.end());
	return result;
}


void GameOverScene::rankOnHttpRequestCompleted(HttpClient *sender, HttpResponse* response)
{
	if (!response)
	{
		return;
	}
	if (!response->isSucceed())
	{
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}

	std::vector<char> *buffer = response->getResponseHeader();
	auto showData = Player::getInstance()->toString(buffer);
	auto showData1 = Player::getInstance()->toString(response->getResponseData());

	rank_field->setText(convertRequestData(showData1));
}

void GameOverScene::submitClickEventListener(Ref* sender)
{
	string mscore = (String::createWithFormat("%ld", Player::getInstance()->getScore()))->_string;

	HttpRequest* request = new HttpRequest();
	request->setRequestType(HttpRequest::Type::POST);
	request->setUrl("http://localhost:8080/submit");
	request->setResponseCallback(CC_CALLBACK_2(GameOverScene::submitOnHttpRequestCompleted, this));

	string useScore = mscore;
	string tempScore = "score=";
	tempScore = tempScore + useScore;
	const char* postData = tempScore.c_str();
	request->setRequestData(postData, strlen(postData));
	request->setTag("Post score");

	vector<string> headers;
	headers.push_back("cookie: GAMESESSIONID=" + Player::getInstance()->getGameSessionId());
	request->setHeaders(headers);

	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

void GameOverScene::submitOnHttpRequestCompleted(HttpClient *sender, HttpResponse* response)
{
	if (!response)
	{
		return;
	}
	if (!response->isSucceed())
	{
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}
}

void GameOverScene::restartClickEventListener(Ref* pSender)
{
	Player::getInstance()->playerReset();
	MonsterFactory::getInstance()->monsterReset();
	Director::getInstance()->replaceScene(TransitionCrossFade::create(0.3f, GameScene::createScene()));  //跳到开始界面

}

void GameOverScene::preloadMusic()
{
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("end_music/bgm.wav");
}

void GameOverScene::playBgm()
{
	SimpleAudioEngine::getInstance()->playBackgroundMusic("end_music/bgm.wav", true);
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume() + 20.0f);
}