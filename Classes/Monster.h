#pragma once
#include "CommonDefines.h"
#include "VictimBase.h"
#include "CSVDataManager.h"

class Monster;

class MonsterState : public cocos2d::Node{
public:
    bool init();
    virtual void setContext(Monster* monster) = 0;
    virtual void update(float dt) = 0;
    virtual void removeState();
};

class MonsterStateNormal :public MonsterState {
private:
    Monster* monster;
public:
    void setContext(Monster* monster)override;
    void update(float dt)override;
    CREATE_FUNC(MonsterStateNormal)
};

class MonsterStateFrozen :public MonsterState {
private:
    Monster* monster;
    float duration;
    cocos2d::Sprite* particle;
public:
    //MonsterStateFrozen(float duration);
    bool init(float duration);
    void setContext(Monster* monster)override;
    void update(float dt)override;
    void removeState() override;
    static MonsterStateFrozen* create(float duration);
};


class MonsterStateSlowed :public MonsterState {
private:
    Monster* monster;
    float duration;
    float rate;
    cocos2d::Sprite* particle;
    cocos2d::Vector<cocos2d::SpriteFrame*> frames;
public:
    bool init(float duration, float power);
    //~MonsterStateSlowed() {
    //    particle->removeFromParentAndCleanup(true);
    //}
    void removeState() override;
    void setContext(Monster* monster)override;
    void update(float dt)override;
    static MonsterStateSlowed* create(float duration, float power);
};

class Monster :public VictimBase {
public:
    using _Base = VictimBase;
    static Monster* create(const string& name, std::vector<cocos2d::Vec2>& checkpoints) {
        Monster* pRet = new(std::nothrow) Monster(); if (pRet && pRet->init(name, checkpoints)) {
            pRet->autorelease(); return pRet;
        }
        else {
            delete pRet; pRet = nullptr; return nullptr;
        }
    };
    //void debuff(float duration)override;
    float distance;
private:
    friend class MonsterStateNormal;
    friend class MonsterStateSlowed;
    friend class MonsterStateFrozen;

    // refactored with state
    MonsterState* state;
public:
    void changeState(MonsterState* state);
    void update(float dt);

    // end of refactoring
private:
    bool init(const string& name, std::vector<cocos2d::Vec2>& checkpoints);

    void move(float dt);

    MonsterInfo monsterInfo;
    int currentSpeed;
    std::vector<cocos2d::Vec2> checkpoints;
    int currentCheckpoint;

    // Í¨¹ý VictimBase ¼Ì³Ð
    void die() override;
};