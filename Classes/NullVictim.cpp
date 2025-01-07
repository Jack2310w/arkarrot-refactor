#include "NullVictim.h"
#include "TowerBase.h"

NullVictim::NullVictim() {}

NullVictim* NullVictim::instance = nullptr;

NullVictim* NullVictim::getInstance()
{
	if (!instance) {
		instance = new NullVictim();
	}
	return instance;
}

bool NullVictim::isInRange(TowerBase* tower)
{
	return false;
}

void NullVictim::die()
{
}

bool NullVictim::preHit(int damage)
{
	return true;
}

bool NullVictim::getHit(int damage)
{
	return true;
}

bool NullVictim::init(const string& name, std::vector<cocos2d::Vec2>& checkpoints)
{
	return false;
}

void NullVictim::move(float dt){}

bool NullVictim::isValid()
{
	return false;
}