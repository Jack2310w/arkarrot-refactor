#include "Monster.h"
#include "LevelScene.h"
#include "Carrot.h"
#include "SoundFx.h"
USING_NS_CC;

//void Monster::debuff(float duration)
//{
//    for (auto child : this->getChildren())
//    {
//        if (child->getPosition().distance(Vec2(getContentSize())) == 0)child->runAction(Hide::create());
//    }
//    bool alreadyDebuffed = currentSpeed < monsterInfo.speed;
//    if (alreadyDebuffed)
//    {
//        unschedule("debuffed");
//    }
//    currentSpeed = monsterInfo.speed * 0.5;
//    scheduleOnce([&](float dt)->bool {
//        currentSpeed = monsterInfo.speed;
//        return true;
//        }, duration, "debuffed");
//}

bool Monster::init(const string& name, std::vector<cocos2d::Vec2>& checkpoints)
{
    monsterInfo = CSVDataManager::getInstance()->getMonsterInfo(name);
    maxLP = visibleLP = trueLP = monsterInfo.lifepoint;
    currentSpeed = monsterInfo.speed;
    changeState(MonsterStateNormal::create());
    this->checkpoints = checkpoints;
    currentCheckpoint = 1;
    distance = 0;
    schedule(CC_CALLBACK_1(Monster::update, this), 1.0f / 30, "monster_moving");
    if (!_Base::init(monsterInfo.plist_path, monsterInfo.name, monsterInfo.frame_cnt))return false;
    runAction(RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(animationFrames, 0.2))));
    return true;
}

void Monster::move(float distance)
{
    Vec2 myPos = getPosition();
    Vec2 direction = (checkpoints[currentCheckpoint] - myPos).getNormalized();
    setFlippedX(direction.x < 0);
	this->distance += distance;
    setPosition(myPos + direction * distance);
    if (myPos.distance(checkpoints[currentCheckpoint]) < 5) {
        if (++currentCheckpoint >= checkpoints.size()) {
            LevelScene::getInstance()->eraseMonster(this);
            unschedule("monster_moving");
            runAction(Hide::create());
            setPosition(10000, 10000);
            LevelScene::getInstance()->getCarrot()->getHurt(monsterInfo.damage);
        }
    }
}

void Monster::die()
{
    SFX::monsterDie();
    unschedule("monster_moving");
    VictimBase::die();
    scheduleOnce(
        [&](float dt) {
            if (state) {
                state->removeState();
            }
            LevelScene::getInstance()->eraseMonster(this);
        }, 2.0f, "delay erase");
    LevelScene::getInstance()->addMonsy(monsterInfo.reward);
    //setPosition(10000, 10000);
}

void Monster::update(float dt) {
	this->state->update(dt);
}

void Monster::changeState(MonsterState* state) {
    if (this->state) {
        this->state->removeState();
    }
    addChild(state);
	this->state = state;
	this->state->setContext(this);
}

// 与MonsterState有关的实现

bool MonsterState::init()
{
    if (!Node::init()) {
        return false;
    }
    return true;
}

void MonsterState::removeState()
{
    removeFromParentAndCleanup(true);
}

void MonsterStateNormal::setContext(Monster* monster) {
	this->monster = monster;
}

void MonsterStateNormal::update(float dt) {
	monster->move(monster->monsterInfo.speed * dt);
}

//inline MonsterStateSlowed::MonsterStateSlowed(float duration, float power) :duration(duration), rate(power) {
//	particle = Sprite::createWithSpriteFrameName("BulletShit-shitted-1.png");
//	frames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("BulletShit-shitted-1.png"));
//	frames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("BulletShit-shitted-2.png"));
//}

MonsterStateSlowed* MonsterStateSlowed::create(float duration, float power)
{
    MonsterStateSlowed* pRet = new(std::nothrow) MonsterStateSlowed();
    if (pRet && pRet->init(duration, power))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool MonsterStateSlowed::init(float duration, float power)
{
    if (!MonsterState::init()) {
        return false;
    }
    this->duration = duration;
    rate = power;
    particle = Sprite::createWithSpriteFrameName("BulletShit-shitted-1.png");
    frames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("BulletShit-shitted-1.png"));
    frames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("BulletShit-shitted-2.png"));
    return true;
}

void MonsterStateSlowed::setContext(Monster* monster) {
	this->monster = monster;
	//particle->setPosition(monster->getPosition());
    particle->setPosition(Vec2(monster->getContentSize()) / 2);
	monster->addChild(particle, 500);
	particle->runAction(RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(frames, 0.1))));
}

void MonsterStateSlowed::update(float dt) {
	monster->move(dt * monster->monsterInfo.speed * rate);
	duration -= dt;
	if (duration <= 0) {
		monster->changeState(MonsterStateNormal::create());
	}
}

void MonsterStateSlowed::removeState()
{
    particle->removeFromParentAndCleanup(true);
    removeFromParentAndCleanup(true);
}

//MonsterStateFrozen::MonsterStateFrozen(float duration) :duration(duration) {
//	particle = Sprite::createWithSpriteFrameName("Ice.png");
//}

void MonsterStateFrozen::setContext(Monster* monster) {
	this->monster = monster;
	particle->setPosition(monster->getPosition());
	monster->addChild(particle, 500);
}

void MonsterStateFrozen::update(float dt) {
	duration -= dt;
	if (duration <= 0) {
		//particle->removeFromParentAndCleanup(true);
		monster->changeState(MonsterStateNormal::create());
	}
}

MonsterStateFrozen* MonsterStateFrozen::create(float duration)
{
    MonsterStateFrozen* pRet = new(std::nothrow) MonsterStateFrozen();
    if (pRet && pRet->init(duration))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool MonsterStateFrozen::init(float duration)
{
    if (!MonsterState::init()) {
        return false;
    }
    this->duration = duration;
    particle = Sprite::createWithSpriteFrameName("Ice.png");
    return true;
}

void MonsterStateFrozen::removeState()
{
    particle->removeFromParentAndCleanup(true);
    removeFromParentAndCleanup(true);
}