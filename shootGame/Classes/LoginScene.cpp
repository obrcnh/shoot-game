#include "LoginScene.h"


Scene* LoginScene::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = LoginScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool LoginScene::init() {
	// 1. super init first
	if (!Layer::init()) {
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	visibleHeight = visibleSize.height;
	visibleWidth = visibleSize.width;


	//��ӱ���ͼƬ
	auto bgSprite = Sprite::create("begin_pic/beginning_bg.jpg");
	bgSprite->setPosition(visibleSize / 2);
	bgSprite->setScale(visibleSize.width / bgSprite->getContentSize().width, visibleSize.height / bgSprite->getContentSize().height);
	this->addChild(bgSprite, 0);

	// ��ӱ�������
	preloadMusic();
	playBgm();

	// �����Ϸlogo
	auto signal = Sprite::create("begin_pic/signal.png");
	signal->setPosition(visibleWidth / 2 + 250, visibleHeight - 100);
	this->addChild(signal);

	// �û�ͷ��
	auto user = Sprite::create("begin_pic/user.png");
	user->setPosition(visibleWidth / 2 - 220, visibleHeight / 2 - 150);
	this->addChild(user);

	// �û��������
	auto box = Sprite::create("begin_pic/box.png");
	box->setPosition(visibleWidth / 2, visibleHeight / 2 - 150);
	this->addChild(box);

	userName = TextField::create("Player Name", "fonts/STXINWEI.TTF", 40);
	userName->setPosition(Vec2(visibleWidth / 2, visibleHeight / 2 - 150));
	userName->setMaxLengthEnabled(true);
	userName->setMaxLength(20);
	userName->setTextColor(Color4B(0, 0, 0, 255));
	this->addChild(userName, 1);

	// ��¼��ť
	auto loginLabel = Label::createWithTTF("L o g i n", "fonts/Marker Felt.ttf", 48);
	loginLabel->setColor(Color3B(187, 255, 255));
	auto loginBtn = MenuItemLabel::create(loginLabel, CC_CALLBACK_1(LoginScene::loginClick, this));
	Menu* login = Menu::create(loginBtn, NULL);
	login->setPosition(visibleWidth / 2 - 15, visibleHeight / 2 - 250);
	this->addChild(login);

	// ��ӿ�����Ա��Ϣ
	auto infoLabel = Label::createWithTTF("������Ա����: \n������ ���� ���Ϻ� ����ά", "fonts/simkai.ttf", 20);
	infoLabel->setColor(Color3B(187, 255, 255));
	infoLabel->setPosition(visibleWidth - infoLabel->getContentSize().width / 2, visibleHeight / 2 - 290);
	this->addChild(infoLabel);

	return true;
}

// �����¼��ť
void LoginScene::loginClick(Ref * pSender) {
	if (userName->getString() != "") {
		HttpRequest* req = new HttpRequest();
		req->setRequestType(HttpRequest::Type::POST);
		req->setUrl("localhost:8080/login");
		string data = "username=" + userName->getString();
		req->setRequestData(data.c_str(), data.size());
		req->setResponseCallback(CC_CALLBACK_2(LoginScene::onLoginCompleted, this));
		HttpClient::getInstance()->send(req);
		req->release();
	}
}

// ��¼��Ӧ
void LoginScene::onLoginCompleted(HttpClient * sender, HttpResponse * res) {
	if (res == nullptr) return;

	// ��¼�ɹ�, ��ȡResponseHeader,��ȡgameSessionId,�������û���,�´ο�ֱ�ӵ�¼
	// ��ת����Ϸ����

	if (res->isSucceed()) {
		string resHeader(res->getResponseHeader()->begin(), res->getResponseHeader()->end());
		Player::getInstance()->setGameSessionId(Player::getInstance()->getSessionIdFromHeader(resHeader));
		database->setStringForKey("username", userName->getString());
		//��ת����һ��ҳ��
		
		Director::getInstance()->replaceScene(TransitionCrossFade::create(0.3f, GameScene::createScene()));
	}
	else {
		log("Login failed: %s", res->getErrorBuffer());
	}
}

void LoginScene::preloadMusic()
{
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("begin_music/beginningbgm.mp3");
}

void LoginScene::playBgm()
{
	SimpleAudioEngine::getInstance()->playBackgroundMusic("begin_music/beginningbgm.mp3", true);
}