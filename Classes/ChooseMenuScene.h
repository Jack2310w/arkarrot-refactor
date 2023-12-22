#pragma once
#include "cocos2d.h"
#include <vector>
#include<string>

class ChooseMenuScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;

    // ��ť����¼�������
    void changeImage(cocos2d::Ref* sender);
    void buttonMap1Clicked(cocos2d::Ref* sender);
    void buttonMap2Clicked(cocos2d::Ref* sender);
    void buttonMap3Clicked(cocos2d::Ref* sender);
    void buttonLastMapClicked(cocos2d::Ref* sender);
    void buttonNextMapClicked(cocos2d::Ref* sender);
    void buttonMainMenuClicked(cocos2d::Ref* sender);

    // implement the "static create()" method manually
    CREATE_FUNC(ChooseMenuScene);

private:
    cocos2d::Sprite* imageSprite; // ������ʾͼƬ�ľ���
    cocos2d::Sprite* secondImageSprite; // ������ʾ�ڶ���ͼƬ�ľ���
    int currentImageIndex; // ��ǰͼƬ����
    std::vector<std::string> imagePaths; // ͼƬ·������

};