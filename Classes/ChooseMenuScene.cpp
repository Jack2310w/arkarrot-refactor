#include "ChooseMenuScene.h"
#include "ui/CocosGUI.h"
#include "StartMenuScene.h"
#include "LevelScene.h"

USING_NS_CC;

bool ChooseMenuScene::init() {
    if (!Scene::init()) return false;
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Scenes/choosemenuscene/stages.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Scenes/choosemenuscene/stages_bg.plist");

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 center = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);

    currentStageIndex = 1;

    //1-indexռλ
    stageSprites.push_back(Sprite::create());
    //��ֹ3�صĻ�Ҫ�޸�����
    for (int i = 1; i <= 3; ++i) {
        auto stageSprite = Sprite::createWithSpriteFrameName(StringUtils::format("ss_map_%d.png", i));
        if (i == 1)stageSprite->setPosition(center);
        else stageSprite->setPosition(center + Vec2(visibleSize.width, 0));
        this->addChild(stageSprite, 1);
        stageSprites.push_back(stageSprite);
    }

    //��ӱ���
    auto background = Sprite::createWithSpriteFrameName("ss_bg.png");
    background->setPosition(center);
    this->addChild(background);

    // �������˵���ť
    auto buttonMainMenu = ui::Button::create("Scenes/choosemenuscene/buttons/ss_back_normal.png", "Scenes/choosemenuscene/buttons/ss_back_pressed.png");
    buttonMainMenu->setPosition(cocos2d::Vec2(20, 620)); 
    buttonMainMenu->addClickEventListener(CC_CALLBACK_1(ChooseMenuScene::buttonMainMenuClicked, this));
    this->addChild(buttonMainMenu, 10);

    // ��һ�ŵ�ͼ��ť
    auto buttonLastMap = ui::Button::create("Scenes/choosemenuscene/buttons/left_normal.png", "Scenes/choosemenuscene/buttons/left_pressed.png");
    buttonLastMap->setPosition(cocos2d::Vec2(20, 300)); 
    buttonLastMap->addClickEventListener(CC_CALLBACK_1(ChooseMenuScene::buttonLastMapClicked, this));
    this->addChild(buttonLastMap, 10);

    // ��һ�ŵ�ͼ��ť
    auto buttonNextMap = ui::Button::create("Scenes/choosemenuscene/buttons/right_normal.png", "Scenes/choosemenuscene/buttons/right_pressed.png");
    buttonNextMap->setPosition(cocos2d::Vec2(940, 300));
    buttonNextMap->addClickEventListener(CC_CALLBACK_1(ChooseMenuScene::buttonNextMapClicked, this));
    this->addChild(buttonNextMap, 10);

    //��ʼ��ť
    auto buttonStart = ui::Button::create("Scenes/choosemenuscene/buttons/ss_play_normal.png", "Scenes/choosemenuscene/buttons/ss_play_pressed.png");
    buttonStart->setPosition(center - cocos2d::Vec2(0, 200));
    buttonStart->addClickEventListener(CC_CALLBACK_1(ChooseMenuScene::buttonStartClicked, this));
    this->addChild(buttonStart, 10);

    return true;
}

void ChooseMenuScene::buttonMainMenuClicked(cocos2d::Ref* sender) {
    auto scene = StartMenuScene::create();  // ���� StartMenuScene ��ʵ��
    cocos2d::Director::getInstance()->popScene();  // �л�����
}

void ChooseMenuScene::buttonStartClicked(cocos2d::Ref* sender){
    auto scene = LevelScene::getInstance();
    scene->init(currentStageIndex);
    cocos2d::Director::getInstance()->pushScene(cocos2d::TransitionFade::create(0.5, scene));
}

void ChooseMenuScene::buttonLastMapClicked(cocos2d::Ref* sender) {
    if (currentStageIndex == 1)return;
    //�������ð�ťһ��
    auto button = static_cast<cocos2d::ui::Button*>(sender);
    button->setTouchEnabled(false);
    this->scheduleOnce([=](float dt) {
        button->setTouchEnabled(true);
        }, 1.0f, "enableButtonTag");

    stageSprites[currentStageIndex - 1]->runAction(MoveBy::create(1, Vec2(Director::getInstance()->getVisibleSize().width, 0)));
    stageSprites[currentStageIndex]->runAction(MoveBy::create(1, Vec2(Director::getInstance()->getVisibleSize().width, 0)));
    --currentStageIndex;
}

void ChooseMenuScene::buttonNextMapClicked(cocos2d::Ref* sender) {
    //�����ֹ��������Ҫ��
    if (currentStageIndex == 3)return;
    //�������ð�ťһ��
    auto button = static_cast<cocos2d::ui::Button*>(sender);
    button->setTouchEnabled(false);
    this->scheduleOnce([=](float dt) {
        button->setTouchEnabled(true);
        }, 0.5f, "enableButtonTag");

    stageSprites[currentStageIndex + 1]->runAction(MoveBy::create(0.5, Vec2(-Director::getInstance()->getVisibleSize().width, 0)));
    stageSprites[currentStageIndex]->runAction(MoveBy::create(0.5, Vec2(-Director::getInstance()->getVisibleSize().width, 0)));
    ++currentStageIndex;
}