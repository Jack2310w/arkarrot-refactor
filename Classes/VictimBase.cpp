#include "VictimBase.h"
#include "LevelScene.h"
#include "BulletBase.h"
#include "TowerBase.h"

USING_NS_CC;

bool VictimBase::preHit(int damage)
{
    if (!this)return true;
    trueLP = std::max(0, trueLP - damage);
    return trueLP == 0;
}

bool VictimBase::getHit(int damage)
{
    visibleLP = std::max(0, visibleLP - damage);
    if (visibleLP == 0) {
        auto particle = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("air01.png"));
        particle->setPosition(getPosition());
        LevelScene::getInstance()->addChild(particle, 500);
        particle->runAction(Sequence::createWithTwoActions(Animate::create(Animation::createWithSpriteFrames(particleFrames, 0.1)), Hide::create()));
        die();
        setPosition(10000, 10000);
        runAction(Hide::create());
    }
    updateLifeBar();
    return visibleLP == 0;
}

void VictimBase::updateLifeBar()
{
    float percentage = (float)visibleLP / maxLP * 100;
    lifeBarFg->setPercentage(100 - percentage);
}

//void VictimBase::debuff(float duration)
//{
//}

void VictimBase::onExit()
{
    Node::onExit();
    _eventDispatcher->removeEventListener(touchListener);
}

bool VictimBase::init(const string& plist_path, const string& name, int frame_cnt)
{
    if (!_Base::init(plist_path, name, frame_cnt))return false;
    touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(VictimBase::setTaunt, this);
    touchListener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithFixedPriority(touchListener, 1);
    // 创建血条
    auto lifeBarBg = Sprite::create("Items/items/MonsterHP01.png");
    lifeBarBg->setPosition(Vec2(getContentSize()) / 2 + Vec2(0, 40));
    this->addChild(lifeBarBg);

    lifeBarFg = ProgressTimer::create(Sprite::create("Items/items/MonsterHP02.png"));
    lifeBarFg->setType(ProgressTimer::Type::BAR);
    lifeBarFg->setMidpoint(Vec2(1, 0.5));
    lifeBarFg->setBarChangeRate(Vec2(1, 0));
    lifeBarFg->setPosition(Vec2(getContentSize()) / 2 + Vec2(0, 40));
    lifeBarFg->setPercentage(0);  // 设置血条初始百分比
    this->addChild(lifeBarFg);

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Items/Items02-hd.plist");
    particleFrames.clear();
    for (int i = 1; i <= 5; ++i)
        particleFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("air0%d.png", i)));

    return true;
}

bool VictimBase::setTaunt(Touch* touch, Event* event)
{
    auto touchLocation = touch->getLocation();
    // 检查是否点击在精灵区域内
    if (getBoundingBox().containsPoint(touchLocation))
    {
        LevelScene::getInstance()->setTauntedTarget(this);
        return true;
    }
    return false;
}

void VictimBase::die()
{
    isdead = true;
    //for (auto bullet = subscribedBullets.begin(); bullet < subscribedBullets.end(); ++bullet) {
    //    (*bullet)->setTarget(NullVictim::getInstance());
    //}
}

//void VictimBase::subscribeBullet(BulletBase* bullet)
//{
//    subscribedBullets.pushBack(bullet);
//}

bool VictimBase::isInRange(TowerBase* tower)
{
    return trueLP > 0 && getPosition().distance(tower->getPosition()) < tower->getRange();
}

bool VictimBase::isValid()
{
    return !isdead;
}