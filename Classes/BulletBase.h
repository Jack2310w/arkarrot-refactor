#pragma once
#include "CommonDefines.h"
#include "VisibleBase.h"
#include "VictimBase.h"

class BulletBase :public VisibleBase {
protected:
    using _Base = VisibleBase;
    int damage;
    double speed;
    VictimBase* target;

public:
    static BulletBase* create(int speed, int damage, const string& plist_path, const string& name,
        int frame_cnt, bool rotate);

    virtual bool init(int speed, int damage, const string& plist_path, const string& name,
        int frame_cnt, bool rotate);
    /*
    * @brief 回调函数，处理飞行，击中判定
    */
    virtual void update(float dt);
    /*
    * @brief 处理击中特效，目标血量扣除，自销毁
    */
    virtual void hit();
    void scheduleFlying(VictimBase* target);

    virtual void setTarget(VictimBase* target);
    VictimBase* getTarget() const;
    virtual double getSpeed() const;
    virtual double getDamage() const;
};