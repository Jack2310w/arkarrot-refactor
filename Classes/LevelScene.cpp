#include "LevelScene.h"
#include "TowerBase.h"
#include "TowerStar.h"
#include "TowerShit.h"
#include "TowerBottle.h"
#include "ui/UIButton.h"
#include "VictimBase.h"
#include "Carrot.h"
#include "Object.h"
USING_NS_CC;


LevelScene* LevelScene::_single_instance = nullptr;

LevelScene::LevelScene()
{
}

LevelScene* LevelScene::getInstance() {
	if (!_single_instance) {
		_single_instance = new LevelScene();
		_single_instance->autorelease(); // ��ֹ�ڴ�й©
	}
	return _single_instance;
}

void LevelScene::onExitTransitionDidStart()
{
	Scene::onExitTransitionDidStart();

	// �Ƴ������������ӽڵ㣨���ͷţ�
	this->removeAllChildrenWithCleanup(true);
	_single_instance = nullptr;
}

bool LevelScene::init(int lvl)
{
	if (!_Base::init())return false;


	// ���졢�������Ƴ���������������а�ť
	selectBox = cocos2d::Sprite::create("TowerMenu/select_01.png");
	buildButtonBottle = cocos2d::ui::Button::create();
	buildButtonShit = cocos2d::ui::Button::create();
	buildButtonStar = cocos2d::ui::Button::create();
	upgradeButton = cocos2d::ui::Button::create();
	removeButton = cocos2d::ui::Button::create();

	selectBox->setPosition(Vec2(10000, 10000)); //����ʾ��ʱ�����Ų����Ļ����ȥ
	buildButtonBottle->setPosition(Vec2(10000, 10000));
	buildButtonShit->setPosition(Vec2(10000,10000));
	buildButtonStar->setPosition(Vec2(10000,10000));
	upgradeButton->setPosition(Vec2(10000,10000));
	removeButton->setPosition(Vec2(10000,10000));

	addChild(selectBox,1000);
	addChild(buildButtonBottle,1000);
	addChild(buildButtonShit,1000);
	addChild(buildButtonStar,1000);
	addChild(upgradeButton,1000);
	addChild(removeButton,1000);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Vec2 center = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);

	// ������Ϣ
	levelInfo = CSVDataManager::getInstance()->getLevelInfo(lvl);
	regionStates.assign(map_row, std::vector<RegionState>(map_col, EMPTY));
	towerStates.assign(map_row, std::vector<TowerInfo>(map_col, TowerInfo()));
	string levelNmae = StringUtils::format("Level_%d", lvl);
	string levelFolderPath = "Levels/" + levelNmae + "/";

	//����ͼ
	auto path_pic = Sprite::create(levelFolderPath + levelNmae + "_path.png");
	path_pic->setPosition(center - Vec2(0, 80));
	addChild(path_pic, -1);
	auto bg_pic = Sprite::create(levelFolderPath + levelNmae + "_bg.png");
	bg_pic->setPosition(center);
	addChild(bg_pic, -2);

	auto tmxMap = TMXTiledMap::create(levelFolderPath + levelNmae + ".tmx");//��ͼ

	//�ϰ���
	auto objects = tmxMap->getObjectGroup("objects");
	for (const auto& object : objects->getObjects())
	{
		float x = object.asValueMap().at("x").asFloat();
		float y = object.asValueMap().at("y").asFloat();
		string type = object.asValueMap().at("type").asString();
		if (type == "object1x1")
		{
			x += side_length / 2;
			y += side_length / 2;
			regionStates[y / 80][x / 80] = OBJECT;
		}
		else if (type == "object1x2")
		{
			x += side_length;
			y += side_length / 2;
			regionStates[y / 80][x / 80] = OBJECT;
			regionStates[y / 80][x / 80 - 1] = OBJECT;
		}
		else if (type == "object2x1")
		{
			x += side_length / 2;
			y += side_length;
			regionStates[y / 80][x / 80] = OBJECT;
			regionStates[y / 80 - 1][x / 80] = OBJECT;
		}
		else
		{
			x += side_length;
			y += side_length;
			regionStates[y / 80][x / 80] = OBJECT;
			regionStates[y / 80 - 1][x / 80] = OBJECT;
			regionStates[y / 80][x / 80 - 1] = OBJECT;
			regionStates[y / 80 - 1][x / 80 - 1] = OBJECT;
		}
		auto newObject = Object::create(object.asValueMap().at("name").asString());
		newObject->setPosition(x, y);
		addChild(newObject, 1);
	}

	//����
	auto objectCheckpoints = tmxMap->getObjectGroup("checkpoints");
	for (const auto& objectCheckpoint : objectCheckpoints->getObjects())
	{
		float x = objectCheckpoint.asValueMap().at("x").asFloat();
		float y = objectCheckpoint.asValueMap().at("y").asFloat() - 80;
		int idx = objectCheckpoint.asValueMap().at("name").asInt();
		if (idx >= checkPoints.size())checkPoints.resize(idx + 1);
		checkPoints[idx] = Vec2(x + side_length / 2, y + side_length / 2);
	}
	for (int i = 1, n = checkPoints.size(); i < n; ++i)
	{
		if (checkPoints[i].x == checkPoints[i - 1].x)
		{
			for (int j = std::min(checkPoints[i].y, checkPoints[i - 1].y) / 80, jt = std::max(checkPoints[i].y, checkPoints[i - 1].y) / 80; j <= jt; ++j)
			{
				regionStates[j][checkPoints[i].x / 80] = UNAVAILABLE;
			}
		}
		else
		{
			for (int j = std::min(checkPoints[i].x, checkPoints[i - 1].x) / 80, jt = std::max(checkPoints[i].x, checkPoints[i - 1].x) / 80; j <= jt; ++j)
			{
				regionStates[checkPoints[i].y / 80][j] = UNAVAILABLE;
			}
		}
	}

	//������
	auto spawnPoint = Sprite::create("Objects/spawnpoint.png");
	spawnPoint->setPosition(checkPoints[0] + Vec2(0, 36));
	addChild(spawnPoint);

	//��Ǯ
	money = levelInfo.money;
	moneyLabel = cocos2d::Label::createWithTTF("0", "numFont/Pacifico-Regular.ttf", 30);
	moneyLabel->setAnchorPoint(cocos2d::Vec2(0, 1));  // ����ê��Ϊ���Ͻ�
	moneyLabel->setPosition(cocos2d::Vec2(100, cocos2d::Director::getInstance()->getVisibleSize().height));
	this->addChild(moneyLabel, 1000);  // ������ʾ�㼶��ȷ��������Ԫ���Ϸ�
	schedule(CC_SCHEDULE_SELECTOR(LevelScene::updateMoneyLabel), 0.1f);//ˢ��

	//����ʱ����
	currentWave = 0;
	scheduleOnce(
		[&](float dt) {
			gotoNextWave();
		}, 1.0f, "start next wave");
	tauntedTarget = nullptr;

	//������ʾ
	waveLabel = cocos2d::Label::createWithTTF("wave: 1", "numFont/Pacifico-Regular.ttf", 30);
	waveLabel->setAnchorPoint(cocos2d::Vec2(0, 1));  // ����ê��Ϊ���Ͻ�
	waveLabel->setPosition(cocos2d::Vec2(400, cocos2d::Director::getInstance()->getVisibleSize().height));
	this->addChild(waveLabel, 1000);
	schedule(CC_CALLBACK_1(LevelScene::updateWaveLabel, this), 0.2f, "waveSchedule");//ˢ��

	//����
	banner=Sprite::create("LevelSceneButton/banner.png");
	banner->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height - banner->getContentSize().height / 2));
	addChild(banner, 30);
	for (int i = 0; i < 12; i++)
		regionStates[7][i] = UNAVAILABLE;

	//�ܲ�
	carrot = Carrot::create();
	carrot->setPosition(checkPoints.back() + Vec2(15, 40));
	addChild(carrot);

	//�����ͽ��������
	touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(LevelScene::tryBuildorUpgrade, this);
	_eventDispatcher->addEventListenerWithFixedPriority(touchListener, 10);
}

// ���ڸ��� Label ����ʾ
void LevelScene::updateMoneyLabel(float delta)
{
	// ����Label��ʾ�Ľ������
	if (moneyLabel)
	{
		moneyLabel->setString(std::to_string(money));
	}
}

void LevelScene::updateWaveLabel(float delta)
{
	// ����Label��ʾ�Ľ������
	if (waveLabel)
	{
		waveLabel->setString("wave: " + std::to_string(std::min(currentWave + 1, levelInfo.wave_cnt)) + "/" + std::to_string(levelInfo.wave_cnt));
	}
	if (currentWave == levelInfo.wave_cnt&&monsters.empty())
	{
		unschedule("waveSchedule");
		AllWavesEnd();
	}
}

void LevelScene::AllWavesEnd()
{
	if (gameOver)return;
	gameOver = 1;
	auto accomplished = ui::Button::create("Accomplished/Win.png");
	Director::getInstance()->pause();
	// ��ȡ��Ļ��С��ԭ��
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Vec2 center = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	accomplished->setPosition(center);

	addChild(accomplished, 10000);

	_eventDispatcher->removeEventListener(this->touchListener);

	auto home = ui::Button::create("Accomplished/home.png");
	home->addClickEventListener([=](Ref* sender) {
		Director::getInstance()->resume();
		Director::getInstance()->popScene();
		});
	home->setPosition(center - Vec2(0, 200));
	addChild(home,10001);

}

void LevelScene::lose()
{
	if (gameOver)return;
	gameOver = 1;
	auto fail = ui::Button::create("Accomplished/fail.png");
	Director::getInstance()->pause();
	// ��ȡ��Ļ��С��ԭ��
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Vec2 center = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	fail->setPosition(center + Vec2(28, 28));

	addChild(fail, 10000);

	_eventDispatcher->removeEventListener(this->touchListener);

	auto home = ui::Button::create("Accomplished/home.png");
	home->addClickEventListener([=](Ref* sender) {
		Director::getInstance()->resume();
		Director::getInstance()->popScene();
		});
	home->setPosition(center - Vec2(0, 200));
	addChild(home, 10001);
}

const std::vector<cocos2d::Vec2>& LevelScene::getCheckPoints() const
{
	return checkPoints;
}

const cocos2d::Vector<Monster*>& LevelScene::getMonsters() const
{
	return monsters;
}

void LevelScene::eraseMonster(Monster* mons)
{
	monsters.eraseObject(mons);
}

const cocos2d::Vector<TowerBase*>& LevelScene::getTowers() const
{
	return towers;
}

int LevelScene::getMoney() const
{
	return money;
}

bool LevelScene::consumeMoney(int cost)
{
	if (money < cost)return false;
	money -= cost;
	return true;
}

void LevelScene::addMonsy(int reward)
{
	money += reward;
}

int LevelScene::getCurrentWave() const
{
	return currentWave;
}

void LevelScene::gotoNextWave()
{
	if (currentWave >= levelInfo.waves.size())return;
	schedule(
		[&](float dt) {
			static int cnt = 0;
			if (++cnt > levelInfo.waves[currentWave].second)
			{
				unschedule("spawn wave");
				scheduleOnce(
					[&](float dt) {
						gotoNextWave();
					}, 5.0f, "start next wave");
				cnt = 0;
				++currentWave;
				return;
			}
			auto monster = Monster::create(levelInfo.waves[currentWave].first, checkPoints);
			monster->setPosition(checkPoints[0]);
			monsters.pushBack(monster);
			addChild(monster, 2);
		}, 0.5f, "spawn wave");
}

Carrot* LevelScene::getCarrot() const
{
	return carrot;
}

void LevelScene::menuClear()
{
	selectBox->setPosition(Vec2(10000, 10000));
	buildButtonBottle->setPosition(Vec2(10000, 10000));
	buildButtonShit->setPosition(Vec2(10000, 10000));
	buildButtonStar->setPosition(Vec2(10000, 10000));
	upgradeButton->setPosition(Vec2(10000, 10000));
	removeButton->setPosition(Vec2(10000, 10000));
}

void LevelScene::buildMenu(cocos2d::Vec2 pos)
{
	CCLOG("%d\n", getMoney());
	if (hasMenu == 1)
	{
		SFX::towerDeselect();
		menuClear();
		hasMenu = 0;
		return;
	}
	int x = pos.x / 80, y = pos.y / 80;
	Vec2 girdPos = Vec2(x * 80 + 40, y * 80 + 40);
	Vec2 offSet = Vec2(0, 0);
	SFX::towerSelect();
	selectBox->setPosition(girdPos);
	int money = getMoney();
	bool canBuild[3] = { money >= 100,money >= 120,money >= 160 };
	string filenameBottle = "Towers/Bottle/Bottle-menu_0" + std::to_string(1 + canBuild[0]) + ".png";
	string filenameShit = "Towers/Shit/Shit0" + std::to_string(canBuild[1]) + ".png";
	string filenameStar = "Towers/Star/Star0" + std::to_string(canBuild[2]) + ".png";
	hasMenu = 1;

	if (x == 0)offSet = Vec2(80, 0);
	if (x == 11)offSet += Vec2(-80, 0);
	if (y == 7)offSet += Vec2(0, -160);


	buildButtonBottle->loadTextures(filenameBottle, filenameBottle);
	buildButtonBottle->setPosition(Vec2(-80, 80) + girdPos+offSet);
	
	buildButtonShit->loadTextures(filenameShit, filenameShit);
	buildButtonShit->setPosition(Vec2(0, 80)+girdPos+offSet);
	
	buildButtonStar->loadTextures(filenameStar, filenameStar);
	buildButtonStar->setPosition(Vec2(80, 80)+girdPos+offSet);

	buildButtonBottle->addClickEventListener([=](cocos2d::Ref* sender) {
		if (canBuild[0])
		{
			auto bottle = TowerBottle::create();
			bottle->setPosition(girdPos);
			addChild(bottle);
			towerStates[y][x] = TowerInfo(0, 1, bottle);
			CCLOG("%d", consumeMoney(100));
			SFX::towerBulid();
			regionStates[y][x] = TOWER;
		}
		menuClear();
		hasMenu = 0;
	});	
	
	buildButtonShit->addClickEventListener([=](cocos2d::Ref* sender) {
		if (canBuild[1])
		{
			//  TODO  ShitTower replace bottle
			auto shit = TowerShit::create();
			shit->setPosition(girdPos);
			addChild(shit);
			towerStates[y][x] = TowerInfo(1, 1, shit);
			CCLOG("%d", consumeMoney(120));
			SFX::towerBulid();
			regionStates[y][x] = TOWER;
		}
		menuClear();
		hasMenu = 0;
	});

	buildButtonStar->addClickEventListener([=](cocos2d::Ref* sender) {
		if (canBuild[2])
		{
			auto star = TowerStar::create();
			star->setPosition(girdPos);
			addChild(star);
			towerStates[y][x] = TowerInfo(2, 1, star);
			CCLOG("%d", consumeMoney(160));
			regionStates[y][x] = TOWER;
		}
		menuClear();
		hasMenu = 0;
		});

}

void LevelScene::upgradeMenu(cocos2d::Vec2 pos)
{
	CCLOG("%d\n", getMoney());
	if (hasMenu == 1)
	{
		SFX::towerDeselect();
		menuClear();
		hasMenu = 0;
		return;
	}
	int x = pos.x / 80, y = pos.y / 80;
	Vec2 girdPos = Vec2(x * 80 + 40, y * 80 + 40);
	Vec2 offSet = Vec2(0, 0);
	SFX::towerSelect();
	selectBox->setPosition(girdPos);
	int money = getMoney();

	TowerInfo& towerHere = towerStates[y][x];

	int upgradeCost[3][4] = { { 0,180,260,999999 } ,
							  { 0,220,260,999999 } ,
							  { 0,220,260,999999 } };
	string filename;
	if (upgradeCost[towerHere._what][towerHere._level] == 999999)
		filename = "TowerMenu/upgrade_0_CN.png";
	else if (money >= upgradeCost[towerHere._what][towerHere._level])
		filename = "TowerMenu/upgrade_" + std::to_string(upgradeCost[towerHere._what][towerHere._level]) + ".png";
	else
		filename = "TowerMenu/upgrade_-" + std::to_string(upgradeCost[towerHere._what][towerHere._level]) + ".png";
	upgradeButton->loadTextures(filename, filename);

	hasMenu = 1;

	if (x == 11)offSet += Vec2(-80, 0);
	if (y == 7)offSet += Vec2(0, -160);


	upgradeButton->setPosition(Vec2(0, 80) + girdPos + offSet);
	removeButton->loadTextures("TowerMenu/sell_80.png", "TowerMenu/sell_80.png");
	removeButton->setPosition(Vec2(80, 80) + girdPos + offSet);


	upgradeButton->addClickEventListener([=, &towerHere](cocos2d::Ref* sender) {
		if (money >= upgradeCost[towerHere._what][towerHere._level])
		{
			towerHere._ptrTower->levelup();
			CCLOG("%d", consumeMoney(upgradeCost[towerHere._what][towerHere._level]));
			SFX::towerUpdata();
			towerHere._level++;
		}
		menuClear();
		hasMenu = 0;
		});

	removeButton->addClickEventListener([=, &towerHere](cocos2d::Ref* sender) {
		towerHere._ptrTower->removeFromParentAndCleanup(true);
		CCLOG("%d", consumeMoney(-80));
		towerHere._level = 0; towerHere._ptrTower = nullptr; towerHere._what = -1;
		regionStates[y][x] = EMPTY;
		SFX::towerSell();
		menuClear();
		hasMenu = 0;
		});

}

bool LevelScene::tryBuildorUpgrade(cocos2d::Touch* touch, cocos2d::Event* event)
{
	auto location = touch->getLocation();
	int x = location.x / 80, y = location.y / 80;
	auto regionState = regionStates[y][x];
	if (regionState == EMPTY) {
		buildMenu(location);
	}
	else if (regionState == TOWER) {
		upgradeMenu(location);
	}
	return true;
}

VictimBase* LevelScene::getTauntedTarget() const
{
	return tauntedTarget;
}

void LevelScene::setTauntedTarget(VictimBase* target)
{
	tauntedTarget = target;
}