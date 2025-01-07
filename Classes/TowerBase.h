#pragma once
#include "CSVDataManager.h"
#include "CommonDefines.h"
#include "VisibleBase.h"
#include "VictimBase.h"
#include "NullVictim.h"
#include "Monster.h"
#include "SoundFx.h"

class LevelScene;
/*
* @brief 所有塔的基类
*/
class TowerBase :public VisibleBase {
private:
	using _Base = VisibleBase;
protected:
	//不许创建
	TowerBase();

	//关于基本信息和初始化
protected:
	TowerInfo towerInfo;
	virtual bool init(const string& towerName);

	//关于升级
private:
	int level;
public:
	void levelup();

	//关于目标
protected:
	VictimBase* target;
public:
	const VictimBase* getTarget()const;
	void setTarget(VictimBase* target);
	int getRange() const; 

	//关于瞄准和开火，自动行为，不可调用
private:
	float timeToFire;
	virtual bool aim(float dt);
	virtual void fire();
	/*
	* @brief 生成子弹，不同类型的子弹伤害逻辑不同
	*/
	virtual void generateBullet() = 0;

	//scheduler
private:
	//主循环
	void update(float dt)override;

	//辅助函数，素材方向与cocos2d默认方向不同，需要转换
protected:
	float getPointingAngle()const;
	void setPointingAngle(float angle);
};