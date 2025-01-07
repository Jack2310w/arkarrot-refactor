#pragma once
#include "CommonDefines.h"
#include "BulletBase.h"
#include "BulletHitEffect.h"

class BulletDecorator : public BulletBase
{
public:
    BulletBase *bullet_; // wrapped bullet object
    HitEffect *effect_;  // decorator effect

    //BulletDecorator(BulletBase *bullet, HitEffect *effect)
    //    : bullet_(bullet), effect_(effect) {
    //}
    static BulletDecorator* create(BulletBase* bullet, HitEffect* effect);
    bool init(BulletBase* bullet, HitEffect* effect);

    void hit() override;
    //void update(float dt) override;

    // other methods

    //VictimBase* getTarget();
    //double getSpeed() const override;
    const cocos2d::Vec2& getPosition() const override;
    void setTarget(VictimBase* target) override;
    void setPosition(const cocos2d::Vec2& pos) override;
    void setRotation(float angle) override;
};
