#include "TowerBottle.h"
#include "LevelScene.h"
#include "BulletDecorator.h"
//#include "ParticleEffect.h"
//#include "RangeDamageEffect.h"
USING_NS_CC;

TowerBottle::TowerBottle()
{
}

bool TowerBottle::init()
{
    if (!_Base::init("Bottle"))return false;
    return true;
}

//void TowerBottle::generateBullet()
//{
//    target->preHit(towerInfo.attack);
//    auto bullet = BulletBottle::create(target, towerInfo.bullet_speed, towerInfo.attack, towerInfo.plist_path, StringUtils::format("Bullet%s", towerInfo.name.c_str()), towerInfo.bullet_frame_cnt);
//    bullet->setPosition(getPosition() + 30 * Vec2::forAngle(CC_DEGREES_TO_RADIANS(getPointingAngle())));
//    bullet->setRotation(getRotation());
//    LevelScene::getInstance()->addChild(bullet, 20);
//    Sprite::setSpriteFrame(animationFrames.front());
//    SFX::bottleFire();
//}

void TowerBottle::generateBullet()
{
    // select target
    target->preHit(towerInfo.attack);

    // create simplest bullet without any special effects
    BulletBase* plainBullet = BulletBase::create(towerInfo.bullet_speed, towerInfo.attack, towerInfo.plist_path,
        StringUtils::format("Bullet%s", towerInfo.name.c_str()), towerInfo.bullet_frame_cnt, false);
    plainBullet->setPosition(getPosition() + 30 * Vec2::forAngle(CC_DEGREES_TO_RADIANS(getPointingAngle())));
    plainBullet->setRotation(getRotation());

    // add particle effect decorator
    HitEffect* particleEffect = ParticleEffect::create("BulletBottle-particle", 2);
    BulletDecorator* bulletWithParticle = BulletDecorator::create(plainBullet, particleEffect);

    // add bullet to scene
    bulletWithParticle->scheduleFlying(target);
    
    // play fire animation and sound effect
    Sprite::setSpriteFrame(animationFrames.front());
    SFX::bottleFire();
}

bool TowerBottle::aim(float dt)
{
    if (!target->isValid()) {
        return false;
    }
    Vec2 targetPos = target->getPosition(), myPos = getPosition();
    float targetAngle = int(CC_RADIANS_TO_DEGREES((targetPos - myPos).getAngle()) + 360) % 360, pointingAngle = getPointingAngle();
    if (abs(targetAngle - pointingAngle) < towerInfo.rotate_speed * dt) {
        setPointingAngle(targetAngle);
        return true;
    }
    else if ((targetAngle < pointingAngle && pointingAngle - targetAngle < 180) || (targetAngle > pointingAngle && targetAngle - pointingAngle > 180)) {
        setPointingAngle(pointingAngle - towerInfo.rotate_speed * dt);
    }
    else {
        setPointingAngle(pointingAngle + towerInfo.rotate_speed * dt);
    }
    return false;
}