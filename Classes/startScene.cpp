#include "startScene.h"
#include "tounMainGame.h"

USING_NS_CC;

cocos2d::Scene* startScene::createScene()
{
    return startScene::create();
}

bool startScene::init()
{
    if(!Scene::init())return false;

    MenuItemFont::setFontSize(48);
    
    auto startButton = MenuItemFont::create("start", CC_CALLBACK_1(startScene::nextScene, this));
    auto ExitButton = MenuItemFont::create("Exit", CC_CALLBACK_1(startScene::Exit, this));

    startButton->setPosition(480,150);
    ExitButton->setPosition(480, 50);

    auto pMenu = Menu::create(startButton, ExitButton, nullptr);
    pMenu->setPosition(Vec2::ZERO);
    this->addChild(pMenu);

    return true;
}

void startScene::nextScene(Ref* pSender)
{
    auto scene = tounMainGame::createScene();
    _director->pushScene(scene);
}

void startScene::Exit(Ref* pSender)
{
    Director::getInstance()->end();
}
