#pragma once 
#include "CommonDefines.h"
#include "VisibleBase.h"

class LevelScene;
class TowerBase;
class BulletBase;

class VictimBase :public VisibleBase {
public:
	using _Base = VisibleBase;
	bool init(const string& plist_path, const string& name, int frame_cnt);
	bool setTaunt(cocos2d::Touch* touch, cocos2d::Event* event);

	virtual bool preHit(int damage);
	virtual bool getHit(int damage);
	void updateLifeBar();
	virtual bool isInRange(TowerBase* tower);
	virtual bool isValid();
	//void subscribeBullet(BulletBase* bullet);

	//virtual void debuff(float duration);

	cocos2d::ProgressTimer* lifeBarFg;
	int maxLP;
	int visibleLP;
	int trueLP;

	virtual void die();
	bool isdead = false;

	cocos2d::Vector<cocos2d::SpriteFrame*> particleFrames;
	//cocos2d::Vector<BulletBase*> subscribedBullets;

	cocos2d::EventListenerTouchOneByOne* touchListener;

	void onExit()override;
};