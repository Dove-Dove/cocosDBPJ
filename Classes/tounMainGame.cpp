#include "tounMainGame.h"
using namespace std;
USING_NS_CC;

cocos2d::Scene* tounMainGame::createScene()
{
    return tounMainGame::create();
}

bool tounMainGame::init()
{
    if(!Scene::init())return false;

    auto backGround = Sprite::create("GameImg/tpBack.png");
    //backGround->setAnchorPoint(Vec2(1, 0));
    backGround->setPosition(Vec2(950, 360));
    backGround->setScale(1.9, 2);
    this -> addChild(backGround );

    mapWall = Sprite::create("GameImg/wall.png");
    mapWall->setPosition(Vec2(950, -10));

    this->addChild(mapWall); mapWall->setOpacity(0);

    
    userSprite = Sprite::create("GameImg/UserIdle01.png");
    userAnimIdle();
    userSprite->setPosition(Vec2(950, 150));
    userSprite->setScale(0.8f);
    this->addChild(userSprite);

    BossSprite = Sprite::create("GameImg/boss/Boss01.png");
    BossSprite->setPosition(Vec2(949, 760));
    
    this->addChild(BossSprite);

    MonsterMove();

    this->schedule(schedule_selector(tounMainGame::enemyFire), 2.0f);  // 2초마다 발사

    createBulletTime = 0;

    //점수 관련 + 점수 텍스트
    {
        ScoreText = Label::createWithSystemFont
        ("현재 점수", "", 28, Size(320, 80), TextHAlignment::LEFT, TextVAlignment::CENTER);
        ScoreText->setAnchorPoint(Vec2(0.0f, 0.5f));
        ScoreText->setPosition(Vec2(130, 540));
        this->addChild(ScoreText);

        pScoreLabel = Label::createWithSystemFont
        (StringUtils::format("%u", allScore),
            "", 28, Size(320, 50), TextHAlignment::LEFT, TextVAlignment::CENTER);
        pScoreLabel->setAnchorPoint(Vec2(0.0f, 0.5f));
        pScoreLabel->setPosition(Vec2(130, 490));
        this->addChild(pScoreLabel);

        hpCountText = Label::createWithSystemFont
        ("남은 목슴", "", 28, Size(320, 50), TextHAlignment::LEFT, TextVAlignment::CENTER);
        hpCountText->setAnchorPoint(Vec2(0.0f, 0.5f));
        hpCountText->setPosition(Vec2(130, 400));
        this->addChild(hpCountText);

        //현재 플레이어가 가지고 있는 목숨 갯수
        hpCount = Label::createWithSystemFont
        (StringUtils::format("%u", lifeCount), "",
            28, Size(300, 50), TextHAlignment::LEFT, TextVAlignment::CENTER);
        hpCount->setAnchorPoint(Vec2(0.0f, 0.5f));
        hpCount->setPosition(Vec2(130, 370));
        this->addChild(hpCount);


        LboomCountText = Label::createWithSystemFont
        ("폭탄", "", 28, Size(320, 50), TextHAlignment::LEFT, TextVAlignment::CENTER);
        LboomCountText->setAnchorPoint(Vec2(0.0f, 0.5f));
        LboomCountText->setPosition(Vec2(130, 300));
        this->addChild(LboomCountText);

        //현재 플레이어가 가지고 있는 폭탄 갯수
        LboomCount = Label::createWithSystemFont
        (StringUtils::format("%u", BoomCount), "", 
            28, Size(300, 50), TextHAlignment::LEFT, TextVAlignment::CENTER);
        LboomCount->setAnchorPoint(Vec2(0.0f, 0.5f));
        LboomCount->setPosition(Vec2(130, 270));
        this->addChild(LboomCount);

        
    }

    //이후 생성
    {
        subObjcreate();
        subObjcreate();

    }

    this->scheduleUpdate();

    return true;
}

void tounMainGame::onEnter()
{
    Scene::onEnter();

    //이벤트 리스너 등록
    _listener = EventListenerKeyboard::create();

    _listener->onKeyPressed = CC_CALLBACK_2(tounMainGame::onKeyPressed, this);
    _listener->onKeyReleased = CC_CALLBACK_2(tounMainGame::onKeyReleased, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
}

void tounMainGame::onExit()
{
    // 이벤트 리스너 해제
    _eventDispatcher->removeEventListener(_listener);
    tounMainGame::onExit();
}

void tounMainGame::update(float dt)
{
    auto x = userSprite->getPositionX();
    auto y = userSprite->getPositionY();
    createBulletTime += dt;

    if (boomStart || BoomCount != 0)
    {
        boomTime -= dt;
        if (boomTime == 0)
        {
            boomTime = 4.0f;
            boomStart = false;
            _keyCodeOrder ^= 32;
            BoomCount--;
        }
    }
    
   

    bool wasIdle = (_keyCodeX == 0 && _keyCodeY == 0);

    // 움직임 및 애니메이션 함수
    {
        
        if (_keyCodeX == 4)
        {
            userSprite->setPosition(Vec2(x - 2, y));
            if (_keyCodeY == 1)
                userSprite->setPosition(Vec2(x - 1, y + 1));
            else if (_keyCodeY == 2)
                userSprite->setPosition(Vec2(x - 1, y - 1));
        }
        else if (_keyCodeX == 8)
        {
            userSprite->setPosition(Vec2(x + 2, y));
            if (_keyCodeY == 1)
                userSprite->setPosition(Vec2(x + 1, y + 1));
            else if (_keyCodeY == 2)
                userSprite->setPosition(Vec2(x + 1, y - 1));
        }

        else if (_keyCodeY == 1)
            userSprite->setPosition(Vec2(x, y + 2));
        else if (_keyCodeY == 2)
            userSprite->setPosition(Vec2(x, y - 2));

        if (_keyCodeOrder == 16)
        {
            Fire();
        }
        if (_keyCodeOrder == 32)
            boomStart = true;


        //일단 숫자로 표시 (유저가 최대로 갈수 있는거리)
        if (userSprite->getPositionX() < 668)
            userSprite->setPositionX(666);
        else if(userSprite->getPositionX() > 1232)
            userSprite->setPositionX(1230);

        if (userSprite->getPositionY() < 68)
            userSprite->setPositionY(66);
        else if (userSprite->getPositionY() > 655)
            userSprite->setPositionY(653);


            

        bool isIdle = (_keyCodeX == 0 && _keyCodeY == 0);

        // 움직이기 시작하면 이동 애니메이션
        if (wasIdle && !isIdle) {
            userAnimMove();
        }

        // 멈추면 Idle 애니메이션
        if (!wasIdle && isIdle) {
            userAnimIdle();
        }        


        if (PlayerSubFireObj > 0 && PlayerSubFireObj < 3)
        {
            subOn = true;
        }

        if (subOn == true && PlayerSubFireObj >= 2)
        {
            subObjMove(userSprite->getPositionX() + subFireX, userSprite->getPositionY(), 0);
            subObjMove(userSprite->getPositionX() - subFireX, userSprite->getPositionY(), 1);
        }
        else if(subOn == true && PlayerSubFireObj == 1)
        {
            subObjMove(userSprite->getPositionX() + subFireX, userSprite->getPositionY(), 0);
        }
    }

    

    //총알 생성 
    {
        if (vBullet.size() <= subFireX)
                createBullet();

        if (vEnemyBullet.size() <= subFireX)
            enemyBulletCreate();
            
    }
    mTime += dt;

    //스폰 시간 관련(몬스터)
    {
        if (mTime >= spawnTime && monsterCount != 0)
        {
            randomSpanw();
            monsterCreate(SpawnX);
            MonsterMove();
            mTime = 0;
            monsterCount--;
        }

        if (monsterCount == 0 && bossAnimStart && mTime > 8.0f)
        {
            BossAnim();
            bossAnimStart = false;
            mTime = 0;
        }
        else if (monsterCount == 0 && !bossAnimStart)
        {
            BossManyFire();
        }
    }


    {
        for (auto enemys = vEnemy.begin(); enemys != vEnemy.end();)
        {
            auto enemy = *enemys;
            auto pos = enemy->getPosition();

            if (pos.x < 660 || pos.x > 1232)
            {
                // 화면에서 적 제거
                this->removeChild(enemy);
                // 배열에서 적 제거
                enemys = vEnemy.erase(enemys);
            }
            else if (boomStart)
            {
                
                this->removeChild(enemy);
                // 배열에서 적 제거
                enemys = vEnemy.erase(enemys);
            }
            else
            {
                ++enemys;  
            }
        }
    }


  //기타
    {
        //플레이어 무적 시간
        if (NoneHit)
        {
            NoneHitTime += dt;
            if (NoneHitTime >= 4.2f)
            {
                //논 히트 타임 초기화
                NoneHit = false;
                NoneHitTime = 0;
                LifeCounting = true;
            }
        }

        if (BossHit)
        {
            bossHp--;
            if (bossHp <= 0)
            {
                UpdateScore(1000);
                //Director::getInstance()->pause();;
            }
            BossHit = false;
            
        }
    }
   
   
}

void tounMainGame::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
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
        _leftMove = true;
        break;
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        _keyCodeX |= 8;
        _leftMove = false;
        break;
    }

    switch (keyCode)
    {
        case EventKeyboard::KeyCode::KEY_SPACE:
        _keyCodeOrder |= 16;
        break;
        case EventKeyboard::KeyCode::KEY_X:
        {       
            if (BoomCount != 0)
            {
                _keyCodeOrder |= 32;
                usingBoom();
            }           
            
        }
        
    }

    // 이동 상태 업데이트
    if (wasIdle && _keyCodeX != 0 ) 
        userAnimMove();
    

}

void tounMainGame::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
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
        _leftMove = true;
        break;
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        _keyCodeX ^= 8;
        _leftMove = false;
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

void tounMainGame::Fire()
{
    // 총알 발사 속도 조절
    if (createBulletTime < 0.1f) return;

    // 현재 캐릭터의 x, y 좌표
    auto x = userSprite->getPositionX();
    auto y = userSprite->getPositionY();

    // 배열의 현재 인덱스에 있는 총알을 발사
    auto bullet = vBullet.at(currentBulletIndex);
    bullet->setPosition(x, y);

    // 총알의 이동 액션 정의
    auto forward = MoveTo::create(0.5f, Vec2(x, y + 1000));
    auto seq = Sequence::create(
        Show::create(),
        forward,
        nullptr);
    bullet->runAction(seq);

    // 인덱스 순환 (배열 끝에 도달하면 처음으로 돌아감)
    currentBulletIndex = (currentBulletIndex + 1) % vBullet.size();

    // 서브 총알 발사
    if(PlayerSubFireObj >= 2)
    {
        SubFire(subFireX, 0);   // 오른쪽 서브 총알
        SubFire(-subFireX, 0);  // 왼쪽 서브 총알
    }
    else if (PlayerSubFireObj == 1)
    {
        SubFire(subFireX, 0);
    }

    createBulletTime = 0;

    // 충돌 감지를 위해 스케줄러 실행
    this->schedule(schedule_selector(tounMainGame::collision));
}

void tounMainGame::SubFire(int sub , int subXEnd)
{
    // 현재 캐릭터의 x, y 좌표
    auto x = userSprite->getPositionX() + sub;
    auto y = userSprite->getPositionY() ;

    // 배열의 현재 인덱스에 있는 총알을 발사
    auto bullet = vBullet.at(currentBulletIndex);
    bullet->setPosition(x, y);

    // 총알의 이동 액션 정의
    auto forward = MoveTo::create(0.5f, Vec2(x + subXEnd, y + 1000));
    auto seq = Sequence::create(
        Show::create(),
        forward,
        nullptr);
    bullet->runAction(seq);

    // 인덱스 순환 (배열 끝에 도달하면 처음으로 돌아감)
    currentBulletIndex = (currentBulletIndex + 1) % vBullet.size();

    // 충돌 감지를 위해 스케줄러 실행
    this->schedule(schedule_selector(tounMainGame::collision));
}

void tounMainGame::subObjMove(float PlayerX, float PlayerY, int Items)
{
    vSubObj[Items]->setPosition(Vec2(PlayerX, PlayerY));
}

void tounMainGame::subObjcreate()
{
    auto subObj = Sprite::create("SubFire.png");
    subObj->setPosition(-100, -100);
    subObj->setScale(0.4f);
    //subObj->runAction(Hide::create());
    this->addChild(subObj);

    vSubObj.push_back(subObj);
}

//총알 발사 하기전 생성용 함수
void tounMainGame::createBullet()
{
    if (createBulletTime < 0.1f)
        return;

    // 필요한 총알만큼 미리 생성 
    for (int i = 0; i < 1000; ++i) {
        //총알 이미지
        auto bullet = Sprite::create("bullet.png");
        // 처음엔 화면 밖에 위치
        bullet->setPosition(Vec2(-100, -100));  
        //생성후 화면 밖으로 생성
        bullet->runAction(Hide::create());  
        this->addChild(bullet);

        //배열로 넣기
        vBullet.push_back(bullet);
    }

    createBulletTime = 0;

}

//총알 충돌시 나오는 함수
void tounMainGame::collision(float dt)
{
    for (auto bulletIt = vBullet.begin(); bulletIt != vBullet.end();)
    {
        auto bullet = *bulletIt;

        if (bullet->getBoundingBox().intersectsRect(mapWall->getBoundingBox()))
        {
            // 충돌 시 총알을 다시 숨기고 위치를 초기화
            bullet->stopAllActions();
            bullet->setPosition(-100, -100);
            bullet->runAction(Hide::create());
        }
        else if (bullet->getBoundingBox().intersectsRect(BossSprite->getBoundingBox())
            && !bossAnimStart)
        {
            // 보스와 충돌 시 처리
            bullet->stopAllActions();
            bullet->setPosition(-100, -100);
            bullet->runAction(Hide::create());
        }

        else
        {
            for (auto enemyIt = vEnemy.begin(); enemyIt != vEnemy.end();)
            {
                auto enemy = *enemyIt;
                if (bullet->getBoundingBox().intersectsRect(enemy->getBoundingBox()))
                {
                    // 충돌 시 총알을 다시 숨기고 위치를 초기화
                    bullet->stopAllActions();
                    bullet->setPosition(-100, -100);
                    bullet->runAction(Hide::create());
                  
                    ItemSpawn(enemy->getPositionX(), enemy->getPositionY());

                    enemy->stopAllActions();
                    enemy->setPosition(-100, -100);
                    enemy->runAction(Hide::create());

                    
              

                    // 적과 총알을 각각 배열에서 제거
                    enemyIt = vEnemy.erase(enemyIt);  // 적 제거
                    vEnemyHitBullet = true;
                    break;
                }
                else
                {
                    ++enemyIt;  // 충돌하지 않은 경우 다음 적으로 이동
                }
            }

            if (vEnemyHitBullet)
            {
                bulletIt = vBullet.erase(bulletIt);
                vEnemyHitBullet = false;
                UpdateScore(100);
            }
            else
            {
                ++bulletIt;  // 충돌하지 않은 경우 다음 총알로 이동
            }

        }
    }

}

void tounMainGame::PlayerHit()
{
    userSprite->stopAllActions();
    NoneHit = true;
    UpdateLife();
    auto seq = Sequence::create(
        Hide::create(),   
        Place::create(Vec2(950, 150)),
        Blink::create(4.0f, 10),
        Show::create(),
        nullptr
    );
    userSprite->runAction(seq);
    //this->addChild(userSprite);
}

void tounMainGame::userAnimIdle()
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

void tounMainGame::userAnimMove()
{
    
    userSprite->stopAllActions();  // 현재 실행 중인 액션 정지

    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("GameImg/UserAnim.plist");

    Vector<SpriteFrame*> animFrames;
    for (int i = 0; i < 3; i++)
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

    if (!_leftMove)
        userSprite->setFlippedX(true);
    else
        userSprite->setFlippedX(false);
}

void tounMainGame::usingBoom()
{
    BoomCount--;
    LboomCount->setString(StringUtils::format("%u", BoomCount));
}

void tounMainGame::BossAnim()
{
    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("GameImg/boss/Boss.plist");

    Vector<SpriteFrame*> animFrames;
    for (int i = 0; i < 11 ; i++)
    {
        // StringUtils::format()=>지정한 형식으로 문자열 생성
        std::string _frames = StringUtils::format("Boss%02d.png", i + 1);

        // 생성한 문자열을 이용하여 plist 내부 SpriteFrame 정보를 가져온다.
        SpriteFrame* frame = cache->getSpriteFrameByName(_frames);

        // 선별한 SpriteFrame을 삽입
        animFrames.pushBack(frame);
    }

    // 애니메이션 적용
    auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
    auto animate = Animate::create(animation);
    auto rep = Sequence::create(
        MoveTo::create(2.0f, Vec2(949, 560)),
        animate,
        nullptr
    );


    // 기존 스프라이트에 애니메이션 적용
    BossSprite->runAction(rep);
}

void tounMainGame::monsterCreate(float x)
{
    auto enemy = Sprite::create("GameImg/monster1/mon1.png");
    enemy->setPosition(x, 700);

    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("GameImg/monster1/mon.plist");

    Vector<SpriteFrame*> animFrames;
    for (int i = 0; i < 4; i++)
    {
        // StringUtils::format()=>지정한 형식으로 문자열 생성
        std::string _frames = StringUtils::format("mon%d.png", i + 1);

        // 생성한 문자열을 이용하여 plist 내부 SpriteFrame 정보를 가져온다.
        SpriteFrame* frame = cache->getSpriteFrameByName(_frames);

        // 선별한 SpriteFrame을 삽입
        animFrames.pushBack(frame);
    }



    auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
    auto animate = Animate::create(animation);


    auto rep = RepeatForever::create(animate);

    // 기존 스프라이트에 애니메이션 적용
    enemy->runAction(rep);

    vEnemy.push_back(enemy);
    this->addChild(enemy);
}

void tounMainGame::randomSpanw()
{
    //몬스터 스폰 위치를 렌덤으로 정하는 함수
    float randX = random(666, 1232);
    SpawnX = randX;

}

void tounMainGame::enemyBulletCreate()
{
    // 필요한 총알만큼 미리 생성 
    for (int i = 0; i < 1000; ++i) {
        //총알 이미지
        auto bullet = Sprite::create("EnemyBullet.png");
        // 처음엔 화면 밖에 위치
        bullet->setPosition(Vec2(-100, -100));
        //생성후 화면 밖으로 생성
        bullet->runAction(Hide::create());
        this->addChild(bullet);

        //배열로 넣기
        vEnemyBullet.push_back(bullet);
    }
}

void tounMainGame::enemyFire(float dt)
{
    // 적의 총알이 없다면 생성
    if (vEnemyBullet.empty())
        enemyBulletCreate();  

    for (auto enemy : vEnemy)
    {
        // 배열의 현재 인덱스에 있는 총알을 발사
        auto bullet = vEnemyBullet.at(currentEnemyBulletIndex);

        // 적의 x, y 좌표
        float x = enemy->getPositionX();
        float y = enemy->getPositionY();

        bullet->setPosition(x, y);

        // 총알의 이동 액션 정의
        auto forward = MoveTo::create(4.0f, Vec2(x, y - 800));
        auto seq = Sequence::create(
            Show::create(),
            forward,
            nullptr);
        bullet->runAction(seq);

        // 인덱스 순환 (배열 끝에 도달하면 처음으로 돌아감)
        currentEnemyBulletIndex = (currentEnemyBulletIndex + 1) % vEnemyBullet.size();

        // 충돌 감지를 위해 스케줄러 실행
        this->schedule(schedule_selector(tounMainGame::enemyCollision));
    }


}

void tounMainGame::BossFire(float mapX)
{
    // 적의 총알이 없다면 생성
    if (vEnemyBullet.empty())
        enemyBulletCreate();

    // 배열의 현재 인덱스에 있는 총알을 발사
    auto bullet = vEnemyBullet.at(currentEnemyBulletIndex);

    // 맵 기준 x, y 좌표 (700은 스폰 위치)
    float x = mapX;
    float y = 700.0f;

    bullet->setPosition(x, y);

    // 총알의 이동 액션 정의
    auto forward = MoveTo::create(5.0f, Vec2(x , y - 800));
    auto seq = Sequence::create(
        Show::create(),
        forward,
        nullptr);
    bullet->runAction(seq);

    // 인덱스 순환 (배열 끝에 도달하면 처음으로 돌아감)
    currentEnemyBulletIndex = (currentEnemyBulletIndex + 1) % vEnemyBullet.size();

    // 충돌 감지를 위해 스케줄러 실행
    this->schedule(schedule_selector(tounMainGame::enemyCollision));
    
}

void tounMainGame::BossManyFire()
{
    if (mTime >= 4.0f)
    {
        BossFireStart = true;
        mTime = 0.0f;
    }    
    if (!BossFireStart)
        return;

    if (fireType1)
    {
        if (mTime >= 0.5f)
        {
            if (BossFireX >= 670 && BossFireX < 1232)
            {
                BossFire(BossFireX);
                BossFireX += 85;
            }
            else
            {
                fireType1 = false;
                BossFireX = 700;
                mTime = 0;
            }
        }
    }
    else 
    {
        if (mTime >= 0.5f)
        {
            if (BossFireX >= 700 && BossFireX < 1232)
            {
                BossFire(BossFireX);
                BossFireX += 85;
            }
            else
            {
                fireType1 = true;
                BossFireX = 670;
                mTime = 0;
            }
        }

    }

}

void tounMainGame::enemyCollision(float dt)
{

    for (auto bulletIt = vEnemyBullet.begin(); bulletIt != vEnemyBullet.end();)
    {
        auto bullet = *bulletIt;
        //맵 밖으로 나갔을대 관련
        if (bullet->getBoundingBox().intersectsRect(mapWall->getBoundingBox()))
        {
            // 충돌 시 적 총알을 다시 숨기고 위치를 초기화
            bullet->stopAllActions();
            bullet->setPosition(-100, -100);
            bullet->runAction(Hide::create());
        }
        //적 총알이 유저에게 맞았을때 관련
        else if (bullet->getBoundingBox().intersectsRect(userSprite->getBoundingBox()))
        {
            // 적 총알이 플레이어와 충돌했을 때
            bullet->stopAllActions();
            bullet->setPosition(-100, -100);
            bullet->runAction(Hide::create());
            if(!NoneHit)
                PlayerHit();
        }
        else
        {
            ++bulletIt;
        }
    }

    for (auto enemys = vEnemy.begin(); enemys != vEnemy.end();)
    {
        //적이 플레이어의 총알을 맞았을때 관련
        auto enemy = *enemys;
        if (enemy->getBoundingBox().intersectsRect(mapWall->getBoundingBox()))
        {
            enemy->stopAllActions();
            enemy->setPosition(-100, -100);
            enemy->runAction(Hide::create());

        }
        else
        {
            ++enemys;
        }

    }
}

void tounMainGame::UpdateScore(int score)
{
    mScoreValue += score;
    pScoreLabel->setString(StringUtils::format("%u", mScoreValue));
}

void tounMainGame::UpdateLife()
{
    //목숨이 0이 되면 게임이 정지 됨
    if (lifeCount  <= 0)
    {
        Director::getInstance()->pause();
    }
    if (!LifeCounting)
        return;
    lifeCount--;
    hpCount->setString(StringUtils::format("%u", lifeCount));
    LifeCounting = false;
}

void tounMainGame::MonsterMove()
{
    
    for (auto enemy : vEnemy)
    {
        float x = enemy->getPositionX();
        float y = 40.0f;

        auto moveEnd = MoveTo::create(10.0f,Vec2(x, y));
        // 이동 완료 후 멈추게 하는 CallFunc
        auto stopAction = CallFunc::create([enemy]() {
            enemy->stopAllActions();  // 모든 액션을 정지하여 멈추기
            });

        // BezierBy 액션 뒤에 멈춤 액션을 연결하는 시퀀스 생성
        auto sequence = Sequence::create(
            moveEnd,
            stopAction, 
            nullptr);


        // 스프라이트에 액션 적용
        enemy->runAction(sequence);

    }
}

void tounMainGame::ItemSpawn(float x, float y)
{
    Item = Sprite::create("GameImg/Item.png");
    Item->setVisible(true);

    Item->setPosition(x, y);

    // 부모 노드에 추가
    this->addChild(Item);

    // 아이템의 이동 후 애니메이션 
    auto forward = MoveTo::create(4.0f, Vec2(x, y - 700));
    auto seq = Sequence::create(
        Show::create(),
        forward,
        nullptr);
    Item->runAction(seq);


   this->schedule(schedule_selector(tounMainGame::ItemCollder));
}

void tounMainGame::ItemCollder(float dt)
{
    //플레이어와 충돌시
    if (Item->getBoundingBox().intersectsRect(userSprite->getBoundingBox()))
    {
        Item->stopAllActions();
        Item->setPosition(-100, -100);
        Item->runAction(Hide::create());
        PlayerSubFireObj ++;
    }
    // 맵 끝에 도달할 경우 
    else if (Item->getBoundingBox().intersectsRect(mapWall->getBoundingBox()))
    {
        Item->stopAllActions();
        Item->setPosition(-100, -100);
        Item->runAction(Hide::create());
    }
    
}




