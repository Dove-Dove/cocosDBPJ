#include "test2d.h"

USING_NS_CC;

cocos2d::Scene* test2d::createScene()
{
    return test2d::create();
}

bool test2d::init()
{
    if (!Scene::init())return false;

    userSprite = Sprite::create("GameImg/UserIdle01.png");
    userAnimMove();
    userSprite->setPosition(Vec2(640, 600));

    this->addChild(userSprite);


    this->scheduleUpdate();

    return true;
}

void test2d::onEnter()
{
    Scene::onEnter();
    //�̺�Ʈ ������ ���
    _listener = EventListenerKeyboard::create();

    _listener->onKeyPressed = CC_CALLBACK_2(test2d::onKeyPressed, this);
    _listener->onKeyReleased = CC_CALLBACK_2(test2d::onKeyReleased, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
}

void test2d::onExit()
{
    // �̺�Ʈ ������ ����
    _eventDispatcher->removeEventListener(_listener);
    test2d::onExit();
}

void test2d::update(float dt)
{
    auto x = userSprite->getPositionX();
    auto y = userSprite->getPositionY();

    bool wasIdle = (_keyCodeX == 0 && _keyCodeY == 0);

    //�밢������ �̵��Ҷ� ������ ����(�ӵ��� ����(������)) --���� ����
    {

        if (_keyCodeX == 4)
        {
            userSprite->setPosition(Vec2(x - 2, y));
            if (_keyCodeY == 1)
                userSprite->setPosition(Vec2(x - 2, y + 2));
            else if (_keyCodeY == 2)
                userSprite->setPosition(Vec2(x - 2, y - 2));
        }
        else if (_keyCodeX == 8)
        {
            userSprite->setPosition(Vec2(x + 2, y));
            if (_keyCodeY == 1)
                userSprite->setPosition(Vec2(x + 2, y + 2));
            else if (_keyCodeY == 2)
                userSprite->setPosition(Vec2(x + 2, y - 2));
        }

        else if (_keyCodeY == 1)
            userSprite->setPosition(Vec2(x, y + 2));
        else if (_keyCodeY == 2)
            userSprite->setPosition(Vec2(x, y - 2));

        bool isIdle = (_keyCodeX == 0 && _keyCodeY == 0);

        // �����̱� �����ϸ� �̵� �ִϸ��̼�
        if (wasIdle && !isIdle) {
            userAnimMove();
        }

        // ���߸� Idle �ִϸ��̼�
        if (!wasIdle && isIdle) {
            userAnimIdle();
        }
    }
}

void test2d::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    bool wasIdle = (_keyCodeX == 0 && _keyCodeY == 0);

    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
        _keyCodeY |= 1; 
        break;
    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        _keyCodeY |= 2; 
        break;
    }
    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        _keyCodeX |= 4;
        break;
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        _keyCodeX |= 8;
        break;
    }

    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_SPACE:
        _keyCodeOrder |= 16;
    }

    if (wasIdle && (_keyCodeX != 0 || _keyCodeY != 0)) {
        userAnimMove();
    }
}

void test2d::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    switch (keyCode)
    {

    case EventKeyboard::KeyCode::KEY_UP_ARROW:
        _keyCodeY ^= 1;
        break;
    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        _keyCodeY ^= 2;
        break;

    }
    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        _keyCodeX ^= 4;
        break;
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        _keyCodeX ^= 8;
        break;
    }

    switch (keyCode)
    {
    case EventKeyboard::KeyCode::KEY_SPACE:
        _keyCodeOrder ^= 16;
    }

    if (_keyCodeX == 0 && _keyCodeY == 0) {
        userAnimIdle();
    }
}

void test2d::userAnimIdle()
{
    userSprite->stopAllActions();  // ���� ���� ���� �׼� ����

    // �ܺ� ���α׷��� ����Ͽ� ���� plist ���� �ҷ�����
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("GameImg/UserAnim.plist");

    // cocos2d::Vector
    Vector<SpriteFrame*> animFrames;
    for (int i = 0; i < 4; i++)
    {
        // StringUtils::format()=>������ �������� ���ڿ� ����
        std::string _frames = StringUtils::format("UserIdle%02d.png", i + 1);

        // ������ ���ڿ��� �̿��Ͽ� plist ���� SpriteFrame ������ �����´�.
        SpriteFrame* frame = cache->getSpriteFrameByName(_frames);

        // ������ SpriteFrame�� ����
        animFrames.pushBack(frame);
    }

    // �ִϸ��̼� ����
    auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
    auto animate = Animate::create(animation);
    auto rep = RepeatForever::create(animate);

    // ���� ��������Ʈ�� �ִϸ��̼� ����
    userSprite->runAction(rep);
}

void test2d::userAnimMove()
{
    userSprite->stopAllActions();  // ���� ���� ���� �׼� ����

    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("GameImg/UserAnim.plist");

    Vector<SpriteFrame*> animFrames;
    for (int i = 0; i < 7; i++)
    {
        // StringUtils::format()=>������ �������� ���ڿ� ����
        std::string _frames = StringUtils::format("UserMove%02d.png", i + 1);

        // ������ ���ڿ��� �̿��Ͽ� plist ���� SpriteFrame ������ �����´�.
        SpriteFrame* frame = cache->getSpriteFrameByName(_frames);

        // ������ SpriteFrame�� ����
        animFrames.pushBack(frame);
    }

    // �ִϸ��̼� ����
    auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
    auto animate = Animate::create(animation);
    auto rep = RepeatForever::create(animate);

    // ���� ��������Ʈ�� �ִϸ��̼� ����
    userSprite->runAction(rep);
}

