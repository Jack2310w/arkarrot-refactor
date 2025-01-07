#include "BulletHitEffect.h"
#include "Monster.h"

USING_NS_CC;

bool HitEffect::init()
{
    if (!Node::init()) {
        return false;
    }
    return true;
}

ParticleEffect* ParticleEffect::create(const string& frameName, int frameCount)
{
    ParticleEffect* pRet = new(std::nothrow) ParticleEffect();
    if (pRet && pRet->init(frameName, frameCount))
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

bool ParticleEffect::init(const string& frameName, int frameCount)
{
    if (!HitEffect::init()) {
        return false;
    }
    frameFormat = frameName + "_%d.png";
    this->frameCount = frameCount;
    return true;
}

void ParticleEffect::applyEffect(const BulletBase* bullet) const
{
    // 确保 bullet 不是 nullptr
    if (!bullet)
    {
        return;
    }

    // 创建粒子精灵
    auto particle = Sprite::createWithSpriteFrameName(StringUtils::format(frameFormat.c_str(), 1));
    Vector<SpriteFrame*> frames;
    for (int i = 1; i <= frameCount; ++i) {
        frames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format(frameFormat.c_str(), i)));
    }
    auto animation = Animation::createWithSpriteFrames(frames, 0.1f);
    auto animate = Animate::create(animation);
    auto sequence = Sequence::create(animate, Hide::create(), nullptr);

    Vec2 pos = bullet->getPosition();
    // 设置粒子位置为子弹位置
    particle->setPosition(pos);


    LevelScene::getInstance()->addChild(particle, 500);
    particle->runAction(Sequence::create(Animate::create(Animation::createWithSpriteFrames(frames, 0.1f)), Hide::create(), nullptr));
}

SlowDownEffect* SlowDownEffect::create(float duration, float power)
{
    SlowDownEffect* pRet = new(std::nothrow) SlowDownEffect();
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

bool SlowDownEffect::init(float duration, float power)
{
    if (!HitEffect::init()) {
        return false;
    }
    this->duration = duration;
    this->power = power;
    return true;
}

void SlowDownEffect::applyEffect(const BulletBase* bullet) const {
    // 确保 bullet 不是 nullptr
    if (!bullet)
    {
        return;
    }

    auto enemy = bullet->getTarget();
    auto target = dynamic_cast<Monster*>(enemy);
    if (target)
    {
        target->changeState(MonsterStateSlowed::create(duration, power));
    }
}

RangeDamageEffect* RangeDamageEffect::create()
{
    RangeDamageEffect* pRet = new(std::nothrow) RangeDamageEffect();
    if (pRet && pRet->init())
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

void RangeDamageEffect::applyEffect(const BulletBase* bullet) const
{
    // 确保 bullet 不是 nullptr
    if (!bullet)
    {
        return;
    }

    // 造成范围伤害
    auto target = bullet->getTarget();
    if (target)
    {
        const cocos2d::Vector<Monster*>& enemies = LevelScene::getInstance()->getMonsters();
        for (auto enemy : enemies)
        {
            if (enemy->getPosition().distance(target->getPosition()) < 100)
            {
                enemy->getHit(bullet->getDamage() / 2);
            }
        }
    }
}
