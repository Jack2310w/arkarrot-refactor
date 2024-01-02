#pragma once
#include "cocos2d.h"
#include <vector>
#include<string>

class ChooseMenuScene : public cocos2d::Scene {
private:
    virtual bool init() override;
    /*
    * @brief ת����һ�صĻص�����
    */
    void buttonLastMapClicked(cocos2d::Ref* sender);
    /*
    * @brief ת����һ�صĻص�����
    */
    void buttonNextMapClicked(cocos2d::Ref* sender);
    /*
    * @brief �ص����˵��Ļص�����
    */
    void buttonMainMenuClicked(cocos2d::Ref* sender);
    /*
    * @brief ��ʼ��Ϸ�Ļص�����
    * @todo ��ȫ��Ϸ����
    */
    void buttonStartClicked(cocos2d::Ref* sender);
    //��ǰչʾ�Ĺؿ����
    int currentStageIndex;
    std::vector<cocos2d::Sprite*> stageSprites;
public:
    CREATE_FUNC(ChooseMenuScene);
};