#pragma once
#include "CommonDefines.h"
#include "BulletBase.h"
#include "VisibleBase.h"
#include "LevelScene.h"

class HitEffect : public cocos2d::Node
{
public:

    virtual void applyEffect(const BulletBase *bullet) const = 0; // 应用命中效果
    bool init();
};

class ParticleEffect : public HitEffect
{
    std::string frameFormat;
    int frameCount;
public:
    void applyEffect(const BulletBase* bullet) const override;
    bool init(const std::string& frameName, int frameCount);
    static ParticleEffect* create(const std::string& frameName, int frameCount);
};

class SlowDownEffect : public HitEffect
{
    float duration;
    float power;
public:
    void applyEffect(const BulletBase* bullet) const;
    static SlowDownEffect* create(float duration, float power);
    bool init(float duration, float power);
};

class RangeDamageEffect : public HitEffect
{
public:
    void applyEffect(const BulletBase* bullet) const override;
    static RangeDamageEffect* create();
};
