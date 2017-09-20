#include "Player.h"
#include "MonsterFactory.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;


Player* Player::player = NULL;


Player::Player()
{
	self = Sprite::create("play_pic/player/image1432.png");
	initSpriteFrame();
	gameSessionId = "";
	score = 0;
	hp = 100;
}


Player * Player::getInstance()
{
	if (player == NULL)
	{
		player = new Player();
	}
	return player;
}


string Player::toString(vector<char> *buffer) {
	string rst;
	for (char ch : *buffer) {
		rst.push_back(ch);
	}
	return rst;
}

string Player::getSessionIdFromHeader(string head) {
	regex nlp("\\r\\n");
	string header = regex_replace(head, nlp, " ");
	regex pattern(".*GAMESESSIONID=(.*) Content-Type.*");
	//match_results<std::string::const_iterator> result;
	cmatch result;
	bool valid = regex_match(header.c_str(), result, pattern);

	if (!valid) {
		return "";
	}
	return result[1];
}

string Player::getGameSessionId()
{
	return gameSessionId;
}

long Player::getScore()
{
	return score;
}

Vector<Sprite*> Player::getBullets()
{
	return bullets;
}

int Player::getHp()
{
	return hp;
}

void Player::setHp(int temp)
{
	if (temp >= 0 && temp <= 100) {
		hp = temp;
	}
	else if (temp < 0) {
		hp = 0;
	}
	else {
		hp = 100;
	}
}

void Player::setGameSessionId(string temp)
{
	gameSessionId = temp;
}

void Player::setScore(int temp)
{
	score = temp;
}

void Player::setBullets(Vector<Sprite*> temp)
{
	for (auto i = temp.begin(); i != temp.end(); i++)
	{
		bullets.pushBack((*i));
	}
}

void Player::move(char flag)
{
	auto position = self->getPosition();
	auto positionX = position.x, positionY = position.y;
	FlipX* flip;
	if (flag == 'a') {
		positionX -= 5;
		flip = FlipX::create(false);
	}
	else if (flag == 'd') {
		positionX += 5;
		flip = FlipX::create(true);
	}
	auto location = convertToNodeSpace(Vec2(positionX, positionY));
	auto animation = Animation::createWithSpriteFrames(playerMove, 0.1f);
	auto animate = Animate::create(animation);
	auto moveTo = MoveTo::create(0.5, location);
	auto mySpawn = Spawn::createWithTwoActions(moveTo, animate);
	auto seq = Sequence::create(flip, mySpawn, nullptr);
	self->runAction(seq);
}

void Player::jump()
{
	auto position = self->getPosition();
	auto positionX = position.x, positionY = position.y;
	JumpBy* jump = CCJumpBy::create(1.0f, ccp(0, 0), 100, 1);
	self->runAction(jump);
}

Sprite* Player::shoot()
{
	auto position = self->getPosition();
	auto bullet = Sprite::create("play_pic/bullet.png");
	bullet->setAnchorPoint(Vec2(0.5, 0.5));
	bullets.pushBack(bullet);
	bullet->setPosition(position);
	
	/*判断玩家是向左还是向右射击*/
	if (self->isFlipX()) {
		bullet->runAction(Sequence::create(MoveBy::create(1.0f, Vec2(Director::getInstance()->getVisibleSize().width, 0)),
			CallFunc::create([bullet, this]() {
			bullet->removeFromParentAndCleanup(true);
			this->bullets.eraseObject(bullet);
		}), nullptr));
	}
	else {
		bullet->runAction(Sequence::create(MoveBy::create(1.0f, Vec2(-Director::getInstance()->getVisibleSize().width, 0)),
			CallFunc::create([bullet, this]() {
			bullet->removeFromParentAndCleanup(true);
			this->bullets.eraseObject(bullet);
		}), nullptr));
	}


	SimpleAudioEngine::getInstance()->playEffect("play_music/shoot_bullets.wav", false);
	return bullet;
}

Sprite * Player::getSelf()
{
	return self;
}

void Player::removeBullet(Sprite* sp)
{
	sp->removeFromParentAndCleanup(true);
	bullets.eraseObject(sp);

}

bool Player::colliderWithPoison(Vec2 temp)
{
	float flag = self->getPosition().getDistance(temp);
	if (flag < 25)
	{
		return true;
	}
	return false;
}

void Player::playerReset()
{
	player->removeFromParentAndCleanup(true);
	player = new Player();
}



void Player::initSpriteFrame()
{
	char str[100] = { 0 };
	for (int i = 0; i < 5; i++)
	{
		sprintf(str, "playing/pic/player/image%d.png", 1432 + 2 * i);
		SpriteFrame* frame = SpriteFrame::create(str, Rect(0, 0, 77, 59));
		playerMove.pushBack(frame);
	}

	char str2[100] = { 0 };
	for (int i = 0; i < 6; i++)
	{
		sprintf(str2, "playing/pic/player/image%d.png", 1684 + 2 * i);
		SpriteFrame *frame = SpriteFrame::create(str, Rect(0, 0, 77, 59));
		playerShoot.pushBack(frame);
	}

	char str3[100] = { 0 };
	for (int i = 0; i < 11; i++)
	{
		sprintf(str3, "playing/pic/player/image%d.png", 1763 + 2 * i);
		SpriteFrame *frame = SpriteFrame::create(str, Rect(0, 0, 77, 59));
		playerDead.pushBack(frame);
	}
}
