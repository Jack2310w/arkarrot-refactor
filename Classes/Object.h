#pragma once
#include "CommonDefines.h"
#include "VictimBase.h"
#include "CSVDataManager.h"

class ObjectComponent : public VictimBase
{
public:
    ObjectInfo objectInfo;
    virtual void die() = 0;
    static ObjectComponent* create(const cocos2d::Value& object);
};

class CompositeObject : public ObjectComponent
{
    cocos2d::Vector<ObjectComponent*> children;
public:
    static CompositeObject* create(cocos2d::Vector<ObjectComponent*>& _children);
    bool init(cocos2d::Vector<ObjectComponent*>& _children);
    void die() override;
    const cocos2d::Vec2& getPosition();
    void addChild(ObjectComponent* child);
};


class Object :public ObjectComponent {
private:
    using _Base = VictimBase;
    bool init();
public:
    static Object* create() {
        Object* pRet = new(std::nothrow) Object(); if (pRet && pRet->init()) {
            pRet->autorelease(); return pRet;
        }
        else {
            delete pRet; pRet = nullptr; return nullptr;
        }
    };


    // ?   VictimBase  ? 
    void die() override;

};