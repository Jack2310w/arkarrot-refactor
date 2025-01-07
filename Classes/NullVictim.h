#pragma once
#include "VictimBase.h"

class TowerBase;

class NullVictim : public VictimBase { //Refactored with Null Object Pattern, Singleton Pattern
public:
    static NullVictim* getInstance();
    bool isInRange(TowerBase* tower) override;
    //bool isAim() override;
    //void debuff(float duration) override;
    bool preHit(int damage) override;
    bool getHit(int damage) override;
    bool isValid() override; 
    void die() override;
    bool init(const string& name, std::vector<cocos2d::Vec2>& checkpoints);
    void move(float dt);
    float distance = 0;

private:
    NullVictim();
    static NullVictim* instance;
};

