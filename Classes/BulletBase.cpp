#include "CommonDefines.h"
#include "BulletBase.h"
#include "LevelScene.h"
#include "NullVictim.h"

USING_NS_CC;

BulletBase* BulletBase::create(int speed, int damage, const string& plist_path, const string& name,
    int frame_cnt, bool rotate)
{
    BulletBase* pRet = new(std::nothrow) BulletBase();
    if (pRet && pRet->init(speed, damage, plist_path, name, frame_cnt, rotate))
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

bool BulletBase::init(int speed, int damage, const string& plist_path, const string& name,
    int frame_cnt, bool rotate) {
    this->speed = speed;
    this->damage = damage;
    if (!_Base::init(plist_path, name, frame_cnt)) {
        return false;
    }
    if (rotate) {
        runAction(RepeatForever::create(RotateBy::create(1, 360)));
    }
    return true;
}

void BulletBase::update(float dt)
{
    Vec2 targetPos = target->getPosition(), myPos = getPosition();
    setRotation(90 - CC_RADIANS_TO_DEGREES((targetPos - myPos).getAngle()));
    if (!target->isValid() || targetPos.distance(myPos) <= 10) {
        hit();
        return;
    }
    Vec2 direction = (targetPos - myPos).getNormalized();
    setPosition(myPos + dt * speed * direction);
}

void BulletBase::hit()
{
    target->getHit(damage);
    runAction(Hide::create());
    setPosition(10000, 10000);
    removeFromParentAndCleanup(true);
}

void BulletBase::scheduleFlying(VictimBase* target)
{
    //target->subscribeBullet(this);
    if (!target->isValid()) {
        return;
    }
    setTarget(target);
    this->schedule(CC_CALLBACK_1(BulletBase::update, this), 0, "flying");
    this->runAction(RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(animationFrames, 0.1f))));
    LevelScene::getInstance()->addChild(this, 200);
}

void BulletBase::setTarget(VictimBase* target)
{
    this->target = target;
}

VictimBase* BulletBase::getTarget() const
{
    return target;
}

double BulletBase::getSpeed() const
{
    return speed;
}

double BulletBase::getDamage() const
{
    return damage;
}