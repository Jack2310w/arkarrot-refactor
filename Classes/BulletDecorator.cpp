#include "BulletDecorator.h"

USING_NS_CC;

BulletDecorator* BulletDecorator::create(BulletBase* bullet, HitEffect* effect)
{
    BulletDecorator* pRet = new(std::nothrow) BulletDecorator();
    if (pRet && pRet->init(bullet, effect))
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

bool BulletDecorator::init(BulletBase* bullet, HitEffect* effect)
{
    if (!Node::init()) {
        return false;
    }
    bullet_ = bullet;
    effect_ = effect;
    speed = bullet->getSpeed();
    addChild(bullet_);
    addChild(effect_);
    return true;
}

void BulletDecorator::hit()
{
    if (isScheduled("flying")) {
        unschedule("flying");
    }
    effect_->applyEffect(bullet_); // apply the effect
    bullet_->hit();
}

void BulletDecorator::setTarget(VictimBase* target)
{
    this->target = target;
    bullet_->setTarget(target);
}

//void BulletDecorator::update(float dt)
//{
//    Vec2 targetPos = target->getPosition(), myPos = getPosition();
//    setRotation(90 - CC_RADIANS_TO_DEGREES((targetPos - myPos).getAngle()));
//    if (!target->isValid() || targetPos.distance(myPos) <= 10) {
//        hit();
//        return;
//    }
//    Vec2 direction = (targetPos - myPos).getNormalized();
//    setPosition(myPos + dt * speed * direction);
//    //auto target = getTarget();
//
//    //// ... some existing code omitted here ..
//
//    //auto& targetPos = target->getPosition();
//    //auto& myPos = getPosition();
//    //if (targetPos.distance(myPos) <= 10)
//    //{
//    //    hit();
//    //    return;
//    //}
//
//    //Vec2 direction = (targetPos - myPos).getNormalized();
//    //setRotation(90 - CC_RADIANS_TO_DEGREES((targetPos - myPos).getAngle()));
//    //setPosition(myPos + dt * getSpeed() * direction);
//
//    //this->_position = getPosition();
//}

// other methods

//VictimBase* BulletDecorator::getTarget()
//{
//    return bullet_->getTarget();
//}

//double BulletDecorator::getSpeed() const
//{
//    return bullet_->getSpeed();
//}

const Vec2& BulletDecorator::getPosition() const
{
    return bullet_->getPosition();
}

void BulletDecorator::setPosition(const Vec2& pos)
{
    bullet_->setPosition(pos);
}

void BulletDecorator::setRotation(float angle)
{
    bullet_->setRotation(angle);
}