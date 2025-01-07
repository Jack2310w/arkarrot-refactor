#pragma once
#include "CommonDefines.h"
#include "TowerBase.h"

class LevelScene;

class TowerStar :public TowerBase {
private:
    using _Base = TowerBase;

    TowerStar();
public:

    static TowerStar* create() {
        TowerStar* pRet = new(std::nothrow) TowerStar(); if (pRet && pRet->init()) {
            pRet->autorelease(); return pRet;
        }
        else {
            delete pRet; pRet = nullptr; return nullptr;
        }
    };

    bool init();

    void generateBullet() override;
};