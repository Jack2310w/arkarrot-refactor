#include "TowerBase.h"
#include "LevelScene.h"

USING_NS_CC;

TowerBase::TowerBase()
{
}

bool TowerBase::init(const string& towerName)
{
	towerInfo = CSVDataManager::getInstance()->getTowerInfo(StringUtils::format("%s1", towerName.c_str()));
	if (!_Base::init(towerInfo.plist_path, towerInfo.name, towerInfo.frame_cnt)) {
		return false;
	}
	target = NullVictim::getInstance();
	level = 1;
	schedule(CC_CALLBACK_1(TowerBase::update, this), 1.0f / 30, "tower_update");
	return true;
}

void TowerBase::levelup()
{
	towerInfo = CSVDataManager::getInstance()->getTowerInfo(StringUtils::format("%s%d", towerInfo.name.substr(0, towerInfo.name.length() - 1).c_str(), ++level));
	loadAnimationFramesFromPlist(towerInfo.plist_path, towerInfo.name, towerInfo.frame_cnt);
}

const VictimBase* TowerBase::getTarget() const
{
	return target;
}

void TowerBase::setTarget(VictimBase* target)
{
	this->target = target;
}

bool TowerBase::aim(float dt)
{
	return target->isValid();
}

void TowerBase::fire()
{
	timeToFire = towerInfo.cooldown;
	auto fireSequence = Sequence::create(Animate::create(Animation::createWithSpriteFrames(animationFrames, 0.02f)), CallFunc::create(CC_CALLBACK_0(TowerBase::generateBullet, this)), nullptr);
	this->runAction(fireSequence);
}

void TowerBase::update(float dt)
{
	auto taunted = LevelScene::getInstance()->getTauntedTarget();
	if (taunted->isInRange(this)) {
		target = taunted;
	}
	else {
		if (!target->isInRange(this))
			target = NullVictim::getInstance();
		for (auto monster : LevelScene::getInstance()->getMonsters()) {
			if (monster->isInRange(this) && (!target->isValid() ||
				static_cast<Monster*>(target)->distance < monster->distance))
				target = monster;
		}
	}
	timeToFire = std::max(0.0f, timeToFire - dt);
	if (aim(dt) && timeToFire == 0) fire();
}

float TowerBase::getPointingAngle() const
{
	return (int(90 - getRotation()) % 360 + 360) % 360;
}

void TowerBase::setPointingAngle(float angle)
{
	setRotation(90 - angle);
}

int TowerBase::getRange() const
{
	return towerInfo.range;
}