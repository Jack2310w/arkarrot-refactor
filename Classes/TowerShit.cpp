#include "TowerShit.h"
#include "LevelScene.h"
#include "BulletDecorator.h"
#include "BulletHitEffect.h"

USING_NS_CC;

TowerShit::TowerShit()
{
}

bool TowerShit::init()
{
    if (!_Base::init("Shit"))return false;
    return true;
}

//void TowerShit::generateBullet()
//{
//    auto bullet = BulletShit::create(target, towerInfo.bullet_speed, towerInfo.attack, towerInfo.plist_path, StringUtils::format("Bullet%s", towerInfo.name.c_str()), towerInfo.bullet_frame_cnt);
//    bullet->setPosition(getPosition());
//    LevelScene::getInstance()->addChild(bullet);
//    Sprite::setSpriteFrame(animationFrames.front());
//    SFX::shitFire();
//}

void TowerShit::generateBullet()
{
    BulletBase* plainBullet = BulletBase::create(towerInfo.bullet_speed, towerInfo.attack, towerInfo.plist_path,
        StringUtils::format("Bullet%s", towerInfo.name.c_str()), towerInfo.bullet_frame_cnt, true);
    plainBullet->setPosition(getPosition());
    HitEffect* slowDownEffect = SlowDownEffect::create(5.0f, 0.5f);
    BulletDecorator* slowDownBullet = BulletDecorator::create(plainBullet, slowDownEffect);
    slowDownBullet->scheduleFlying(target);
    Sprite::setSpriteFrame(animationFrames.front());
    SFX::shitFire();
}
