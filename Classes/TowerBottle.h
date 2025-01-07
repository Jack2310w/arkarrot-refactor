#pragma once
#include "CommonDefines.h"
#include "TowerBase.h"

class TowerBottle :public TowerBase {
private:
    using _Base = TowerBase;

	TowerBottle();
public:

    static TowerBottle* create() {
        TowerBottle* pRet = new(std::nothrow) TowerBottle(); if (pRet && pRet->init()) {
            pRet->autorelease(); return pRet;
        }
        else {
            delete pRet; pRet = nullptr; return nullptr;
        }
    };

	bool init();
	void generateBullet() override;
    bool aim(float dt) override;
};