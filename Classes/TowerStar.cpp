#include "TowerStar.h"
#include "BulletBase.h"
#include "BulletDecorator.h"
#include "LevelScene.h"

USING_NS_CC;

TowerStar::TowerStar()
{
}

bool TowerStar::init()
{
    if (!_Base::init("Star"))return false;
    return true;
}

//void TowerStar::generateBullet()
//{
//    auto bullet = BulletBase::create(target, towerInfo.bullet_speed, towerInfo.attack, towerInfo.plist_path, StringUtils::format("Bullet%s", towerInfo.name.c_str()), towerInfo.bullet_frame_cnt);
//    bullet->setPosition(getPosition());
//    LevelScene::getInstance()->addChild(bullet);
//    Sprite::setSpriteFrame(animationFrames.front());
//    SFX::starFire();
//}
void TowerStar::generateBullet()
{
    BulletBase* plainBullet = BulletBase::create(towerInfo.bullet_speed, towerInfo.attack, towerInfo.plist_path,
        StringUtils::format("Bullet%s", towerInfo.name.c_str()), towerInfo.bullet_frame_cnt, true);
    plainBullet->setPosition(getPosition());
    // add star effect
    HitEffect* starEffect = ParticleEffect::create("BulletStar-particle", 6);
    BulletDecorator* starBullet = BulletDecorator::create(plainBullet, starEffect);
    // add range damage effect
    HitEffect* rangeEffect = RangeDamageEffect::create();
    BulletDecorator* rangeBullet = BulletDecorator::create(starBullet, rangeEffect);
    // add bullet to scene
    rangeBullet->scheduleFlying(target);
    Sprite::setSpriteFrame(animationFrames.front());
    SFX::starFire();
}

