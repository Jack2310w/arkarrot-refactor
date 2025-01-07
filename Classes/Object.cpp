#include "Object.h"
#include "LevelScene.h"

USING_NS_CC;

bool Object::init()
{
    //objectInfo = CSVDataManager::getInstance()->getObjectInfo(name);
    //if (!_Base::init(objectInfo.plist_path, name, 1))return false;
    //maxLP = trueLP = visibleLP = objectInfo.lifepoint;
    objectInfo.reward = 0;
    return true;
}

void Object::die()
{
    if (isdead)return;
    isdead = true;
    if (getPosition().x > 10000)return;
    auto lvlscene = LevelScene::getInstance();
    int x = getPosition().x / 80, y = getPosition().y / 80;
    lvlscene->regionStates[y][x] = LevelScene::EMPTY;
    lvlscene->addMonsy(objectInfo.reward);
    //setPosition(10000, 10000);
}

CompositeObject* CompositeObject::create(Vector<ObjectComponent*>& _children)
{
    CompositeObject* pRet = new(std::nothrow) CompositeObject(); if (pRet && pRet->init(_children)) {
        pRet->autorelease(); return pRet;
    }
    else {
        delete pRet; pRet = nullptr; return nullptr;
    }
}

bool CompositeObject::init(Vector<ObjectComponent*>& _children)
{
    children = _children;
    setPosition(children.at(0)->getPosition());
    return true;
}

void CompositeObject::die()
{
    isdead = true;
    for (auto child = children.begin(); child < children.end(); child++) {
        (*child)->die();
    }
}

void CompositeObject::addChild(ObjectComponent* child)
{
    children.pushBack(child);
}

const Vec2& CompositeObject::getPosition()
{
    return children.at(0)->getPosition();
}

ObjectComponent* ObjectComponent::create(const Value& object)
{
    float x = object.asValueMap().at("x").asFloat();
    float y = object.asValueMap().at("y").asFloat();
    string type = object.asValueMap().at("type").asString();
    string name = object.asValueMap().at("name").asString();
    LevelScene* scene = LevelScene::getInstance();
    int height = type[6] - '0';
    int width = type[8] - '0';
    Vector<ObjectComponent*> components;
    //      ?? ?    
    x += side_length / 2 * width;
    y += side_length / 2 * height;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            Object* object = Object::create();
            object->setPosition(x - side_length / 2 * i, y - side_length / 2 * j);
            scene->regionStates[y / 80][x / 80] = scene->OBJECT;
            components.pushBack(object);
        }
    }
    ObjectComponent* res = nullptr;
    if (components.size() == 1) {
        res = components.at(0);
    }
    else {
        res = CompositeObject::create(components);
    }
    res->objectInfo = CSVDataManager::getInstance()->getObjectInfo(name);
    res->init(res->objectInfo.plist_path, name, 1);
    res->maxLP = res->trueLP = res->visibleLP = res->objectInfo.lifepoint;
    return res;
}