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
    //이벤트 리스너 등록
    _listener = EventListenerKeyboard::create();

    _listener->onKeyPressed = CC_CALLBACK_2(test2d::onKeyPressed, this);
    _listener->onKeyReleased = CC_CALLBACK_2(test2d::onKeyReleased, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
}

void test2d::onExit()
{
    // 이벤트 리스너 해제
    _eventDispatcher->removeEventListener(_listener);
    test2d::onExit();
}

void test2d::update(float dt)
{
    auto x = userSprite->getPositionX();
    auto y = userSprite->getPositionY();

    bool wasIdle = (_keyCodeX == 0 && _keyCodeY == 0);

    //대각선으로 이동할때 문제가 생김(속도가 빠름(버니합)) --추후 수정
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

        // 움직이기 시작하면 이동 애니메이션
        if (wasIdle && !isIdle) {
            userAnimMove();
        }

        // 멈추면 Idle 애니메이션
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
    userSprite->stopAllActions();  // 현재 실행 중인 액션 정지

    // 외부 프로그램을 사용하여 만든 plist 파일 불러오기
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("GameImg/UserAnim.plist");

    // cocos2d::Vector
    Vector<SpriteFrame*> animFrames;
    for (int i = 0; i < 4; i++)
    {
        // StringUtils::format()=>지정한 형식으로 문자열 생성
        std::string _frames = StringUtils::format("UserIdle%02d.png", i + 1);

        // 생성한 문자열을 이용하여 plist 내부 SpriteFrame 정보를 가져온다.
        SpriteFrame* frame = cache->getSpriteFrameByName(_frames);

        // 선별한 SpriteFrame을 삽입
        animFrames.pushBack(frame);
    }

    // 애니메이션 적용
    auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
    auto animate = Animate::create(animation);
    auto rep = RepeatForever::create(animate);

    // 기존 스프라이트에 애니메이션 적용
    userSprite->runAction(rep);
}

void test2d::userAnimMove()
{
    userSprite->stopAllActions();  // 현재 실행 중인 액션 정지

    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("GameImg/UserAnim.plist");

    Vector<SpriteFrame*> animFrames;
    for (int i = 0; i < 7; i++)
    {
        // StringUtils::format()=>지정한 형식으로 문자열 생성
        std::string _frames = StringUtils::format("UserMove%02d.png", i + 1);

        // 생성한 문자열을 이용하여 plist 내부 SpriteFrame 정보를 가져온다.
        SpriteFrame* frame = cache->getSpriteFrameByName(_frames);

        // 선별한 SpriteFrame을 삽입
        animFrames.pushBack(frame);
    }

    // 애니메이션 적용
    auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
    auto animate = Animate::create(animation);
    auto rep = RepeatForever::create(animate);

    // 기존 스프라이트에 애니메이션 적용
    userSprite->runAction(rep);
}

