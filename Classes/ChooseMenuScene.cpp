#include "ChooseMenuScene.h"
#include "ui/CocosGUI.h"
#include"StartMenuScene.h"

USING_NS_CC;

cocos2d::Scene* ChooseMenuScene::createScene() {
    return ChooseMenuScene::create();
}

bool ChooseMenuScene::init() {
    if (!Scene::init()) {
        return false;
    }

    // ��ʼ��ͼƬ·��������
    imagePaths = { "Themes/scene/stages_theme1-hd/ss_map01.png",
                   "Themes/scene/stages_theme1-hd/ss_map02.png",
                   "Themes/scene/stages_theme1-hd/ss_map03.png" }; // �滻Ϊʵ�ʵ�ͼƬ·��
    currentImageIndex = 0;

    // ��������ʼ��ͼƬ����
    imageSprite = cocos2d::Sprite::create("Themes/scene/stages_bg-hd/ss_bg.png");
    imageSprite->setPosition(cocos2d::Vec2(480, 320)); // ��������λ��
    this->addChild(imageSprite); // ��������ӵ�����

    secondImageSprite = cocos2d::Sprite::create(imagePaths[currentImageIndex]);
    secondImageSprite->setPosition(cocos2d::Vec2(480, 320)); // ����λ��.
    this->addChild(secondImageSprite);

    // ͼһ��ť
    //auto buttonMap1 = cocos2d::ui::Button::create("Themes/scene/stages_theme1-hd/ss_map01.png");
    //buttonMap1->setPosition(cocos2d::Vec2(480, 320)); // ���ð�ťλ��
    //buttonMap1->addClickEventListener(CC_CALLBACK_1(ChooseMenuScene::buttonMap1Clicked, this));
    //this->addChild(buttonMap1);

    //// ͼ����ť
    //auto buttonMap2 = cocos2d::ui::Button::create("Themes/scene/stages_theme1-hd/ss_map02.png");
    //buttonMap2->setPosition(cocos2d::Vec2(480, 100)); // ���ð�ťλ��
    //buttonMap2->addClickEventListener(CC_CALLBACK_1(ChooseMenuScene::buttonMap2Clicked, this));
    //this->addChild(buttonMap2);

    //// ͼ����ť
    //auto buttonMap3 = cocos2d::ui::Button::create("Themes/scene/stages_theme1-hd/ss_map03.png");
    //buttonMap3->setPosition(cocos2d::Vec2(480, 100)); // ���ð�ťλ��
    //buttonMap3->addClickEventListener(CC_CALLBACK_1(ChooseMenuScene::buttonMap3Clicked, this));
    //this->addChild(buttonMap3);

    // �������˵���ť
    auto buttonMainMenu = cocos2d::ui::Button::create("Themes/scene/stages_bg-hd/ss_back_normal.png", "Themes/scene/stages_bg-hd/ss_back_pressed.png", "Themes/scene/stages_bg-hd/ss_back_normal.png");
    buttonMainMenu->setPosition(cocos2d::Vec2(20, 620)); // ��ť��λ��
    buttonMainMenu->addClickEventListener(CC_CALLBACK_1(ChooseMenuScene::buttonMainMenuClicked, this));
    this->addChild(buttonMainMenu);

    // ��һ�ŵ�ͼ��ť
    auto buttonLastMap = cocos2d::ui::Button::create("Themes/scene/recommend-hd/recommend_left_normal.png", "Themes/scene/recommend-hd/recommend_left_pressed.png", "Themes/scene/recommend-hd/recommend_left_normal.png");
    buttonLastMap->setPosition(cocos2d::Vec2(20, 300)); // ��ť��λ��
    buttonLastMap->addClickEventListener(CC_CALLBACK_1(ChooseMenuScene::buttonLastMapClicked, this));
    this->addChild(buttonLastMap);

    // ��һ�ŵ�ͼ��ť
    auto buttonNextMap = cocos2d::ui::Button::create("Themes/scene/recommend-hd/recommend_right_normal.png", "Themes/scene/recommend-hd/recommend_right_pressed.png", "Themes/scene/recommend-hd/recommend_right_normal.png");
    buttonNextMap->setPosition(cocos2d::Vec2(940, 300)); // ��ť��λ��
    buttonNextMap->addClickEventListener(CC_CALLBACK_1(ChooseMenuScene::buttonNextMapClicked, this));
    this->addChild(buttonNextMap);



    return true;
}

void ChooseMenuScene::changeImage(cocos2d::Ref* sender) {
    currentImageIndex++;
    if (currentImageIndex >= imagePaths.size()) {
        currentImageIndex = 0;
    }

    // ��ȡ��ͼƬ��·�������¾�����ʾ��ͼƬ
    std::string nextImagePath = imagePaths[currentImageIndex];
    imageSprite->setTexture(nextImagePath);
}

// ��һ����ť�Ļص�����
void ChooseMenuScene::buttonMap1Clicked(cocos2d::Ref* sender) {
    // �����һ����ť�ĵ���¼�
}

// �ڶ�����ť�Ļص�����
void ChooseMenuScene::buttonMap2Clicked(cocos2d::Ref* sender) {
    // ����ڶ�����ť�ĵ���¼�
}

void ChooseMenuScene::buttonMap3Clicked(cocos2d::Ref* sender) {
    // ����ڶ�����ť�ĵ���¼�
}

void ChooseMenuScene::buttonMainMenuClicked(cocos2d::Ref* sender) {
    auto scene = StartMenuScene::create();  // ���� StartMenuScene ��ʵ��
    cocos2d::Director::getInstance()->replaceScene(cocos2d::TransitionFade::create(0.5, scene));  // �л�����
}

void ChooseMenuScene::buttonLastMapClicked(cocos2d::Ref* sender) {
    if (currentImageIndex > 0) {
        currentImageIndex--;
    }
    else {
        currentImageIndex = imagePaths.size() - 1;  // ��ת�����һ��ͼƬ
    }

    // ��ȡ��ͼƬ��·�������µڶ���ͼƬ�������ʾ��ͼƬ
    std::string previousImagePath = imagePaths[currentImageIndex];
    secondImageSprite->setTexture(previousImagePath);
}

void ChooseMenuScene::buttonNextMapClicked(cocos2d::Ref* sender) {
    if (currentImageIndex < imagePaths.size() - 1) {
        currentImageIndex++;
    }
    else {
        currentImageIndex = 0;  // ��ת����һ��ͼƬ
    }

    // ��ȡ��ͼƬ��·�������µڶ���ͼƬ�������ʾ��ͼƬ
    std::string nextImagePath = imagePaths[currentImageIndex];
    secondImageSprite->setTexture(nextImagePath);
}