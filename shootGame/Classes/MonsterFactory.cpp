#include "MonsterFactory.h"
#include "SimpleAudioEngine.h"

MonsterFactory* MonsterFactory::factory = NULL;

using namespace CocosDenshion;

MonsterFactory* MonsterFactory::getInstance() {
	if (factory == NULL) {
		factory = new MonsterFactory();
	}
	return factory;
}

Sprite * MonsterFactory::createMonster()
{
	Sprite* mons = Sprite::create("play_pic/monster.png");
	monster.pushBack(mons);
	float x = Director::getInstance()->getVisibleSize().width;
	float y = random<float>(100.0, Director::getInstance()->getVisibleSize().height - 100);
	mons->setPosition(x, y);
	return mons;
}

void MonsterFactory::moveMonster(Vec2 playerPos, float time)
{
	for (int i = 0; i < monster.size(); i++) {
		Vec2 position = monster.at(i)->getPosition();
		Vec2 direction = playerPos - position;
		direction.normalize();
		monster.at(i)->runAction(MoveBy::create(time, direction * 30));
		auto animation1 = Animation::createWithSpriteFrames(monsterMove, 0.1f);
		auto animate1 = Animate::create(animation1);
		auto animation2 = Animation::createWithSpriteFrames(monsterShoot, 0.1f);
		auto animate2 = Animate::create(animation2);
		Sequence* seq = Sequence::create(animate1, animate2, NULL);
		monster.at(i)->runAction(seq);
	}
}

Vector<Sprite*> MonsterFactory::shootPoison()
{
	Vector<Sprite*> result;
	for (int i = 0; i < factory->getMonster().size(); i++) {
		auto poison = factory->createPoison();
		auto position = factory->getMonster().at(i)->getPosition();
		position.x = position.x - 10;
		position.y = position.y + 10;
		poison->setPosition(position);

		result.pushBack(poison);
		
		poison->runAction(Sequence::create(MoveBy::create(3.0f, Vec2(-Director::getInstance()->getVisibleSize().width, 0)), CallFunc::create([poison, this]() {
			poison->removeFromParentAndCleanup(true);
			this->factory->getPoison().eraseObject(poison);
		}), nullptr));
	}

	return result;

}

void MonsterFactory::removeMonster(Sprite * sp)
{
	auto animation = Animation::createWithSpriteFrames(monsterDead, 0.1f);
	auto animate = Animate::create(animation);
	Sequence* seq = Sequence::create(animate, CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, sp)), NULL);
	sp->runAction(seq);
	monster.eraseObject(sp);
}

Sprite * MonsterFactory::createPoison()
{
	ccBlendFunc cbl = { GL_SRC_ALPHA, GL_ONE };
	Sprite* sp = Sprite::create("play_pic/poison2.png");
	sp->setAnchorPoint(Vec2(0.5, 0.5));
	sp->setScale(0.5);
	sp->setBlendFunc(cbl);//È¥³ý¶¾ÒºÍ¼Æ¬µÄºÚÉ«±³¾°
	poisons.pushBack(sp);
	return sp;
}

void MonsterFactory::movePoison(Vec2 playerPos, float time)
{

}

void MonsterFactory::removePoison(Sprite * sp)
{
	sp->removeFromParentAndCleanup(true);
	poisons.eraseObject(sp);
	
}

Sprite* MonsterFactory::colliderWithBullets(Rect rect)
{
	Vector<Sprite*>::iterator it;
	for (it = monster.begin(); it != monster.end(); it++) {
    if (rect.intersectsRect((*it)->getBoundingBox())) {
      SimpleAudioEngine::getInstance()->playEffect("play_music/explosion.wav", false);
      return *it;
    }
	}
	return nullptr;
}

Sprite * MonsterFactory::colliderWithPlayer(Rect rect)
{
  Vector<Sprite*>::iterator it;
  for (it = monster.begin(); it != monster.end(); it++) {
    Vec2 location = (*it)->getPosition();
    if (rect.intersectsRect((*it)->getBoundingBox())) {
      return *it;
    }
  }
  return nullptr;
}

void MonsterFactory::initSpriteFrame()
{
	
	monsterDead.reserve(14);
	for (int i = 0; i < 14; i++) {
		char enemyPath[50];
		sprintf(enemyPath, "play_pic/monster1dead/image%d.png", 865 + 2 * i);
		auto frame = SpriteFrame::create(enemyPath, Rect(0, 0, 114, 125));
		monsterDead.pushBack(frame);
	}
	monsterMove.reserve(7);
	for (int i = 0; i < 7; i++) {
		char enemyPath[50];
		sprintf(enemyPath, "play_pic/monster1move/image%d.png", 761 + 2 * i);
		auto frame = SpriteFrame::create(enemyPath, Rect(0, 0, 100, 120));
		monsterMove.pushBack(frame);
	}
	monsterShoot.reserve(14);
	for (int i = 0; i < 14; i++) {
		char enemyPath[50];
		sprintf(enemyPath, "play_pic/monster1shoot/image%d.png", 777 + 2 * i);
		auto frame = SpriteFrame::create(enemyPath, Rect(0, 0, 107, 115));
		monsterShoot.pushBack(frame);
	}
}

MonsterFactory::MonsterFactory()
{
	initSpriteFrame();
}

Vector<Sprite*> MonsterFactory::getMonster() {
	return monster;
}

Vector<Sprite*> MonsterFactory::getPoison() {
	return poisons;
}

void MonsterFactory::monsterReset()
{
	factory = new MonsterFactory();
}
