#pragma once
#include "CommonDefines.h"
#include "TowerBase.h"

class LevelScene;

class TowerShit :public TowerBase {
private:
    using _Base = TowerBase;

    TowerShit();
public:

    static TowerShit* create() {
        TowerShit* pRet = new(std::nothrow) TowerShit(); if (pRet && pRet->init()) {
            pRet->autorelease(); return pRet;
        }
        else {
            delete pRet; pRet = nullptr; return nullptr;
        }
    };

    bool init();

    void generateBullet() override;
};