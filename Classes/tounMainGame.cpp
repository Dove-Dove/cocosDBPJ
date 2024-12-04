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

    this->schedule(schedule_selector(tounMainGame::enemyFire), 2.0f);  // 2�ʸ��� �߻�

    createBulletTime = 0;

    //���� ���� + ���� �ؽ�Ʈ
    {
        ScoreText = Label::createWithSystemFont
        ("���� ����", "", 28, Size(320, 80), TextHAlignment::LEFT, TextVAlignment::CENTER);
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
        ("���� ��", "", 28, Size(320, 50), TextHAlignment::LEFT, TextVAlignment::CENTER);
        hpCountText->setAnchorPoint(Vec2(0.0f, 0.5f));
        hpCountText->setPosition(Vec2(130, 400));
        this->addChild(hpCountText);

        //���� �÷��̾ ������ �ִ� ��� ����
        hpCount = Label::createWithSystemFont
        (StringUtils::format("%u", lifeCount), "",
            28, Size(300, 50), TextHAlignment::LEFT, TextVAlignment::CENTER);
        hpCount->setAnchorPoint(Vec2(0.0f, 0.5f));
        hpCount->setPosition(Vec2(130, 370));
        this->addChild(hpCount);


        LboomCountText = Label::createWithSystemFont
        ("��ź", "", 28, Size(320, 50), TextHAlignment::LEFT, TextVAlignment::CENTER);
        LboomCountText->setAnchorPoint(Vec2(0.0f, 0.5f));
        LboomCountText->setPosition(Vec2(130, 300));
        this->addChild(LboomCountText);

        //���� �÷��̾ ������ �ִ� ��ź ����
        LboomCount = Label::createWithSystemFont
        (StringUtils::format("%u", BoomCount), "", 
            28, Size(300, 50), TextHAlignment::LEFT, TextVAlignment::CENTER);
        LboomCount->setAnchorPoint(Vec2(0.0f, 0.5f));
        LboomCount->setPosition(Vec2(130, 270));
        this->addChild(LboomCount);

        
    }

    //���� ����
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

    //�̺�Ʈ ������ ���
    _listener = EventListenerKeyboard::create();

    _listener->onKeyPressed = CC_CALLBACK_2(tounMainGame::onKeyPressed, this);
    _listener->onKeyReleased = CC_CALLBACK_2(tounMainGame::onKeyReleased, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
}

void tounMainGame::onExit()
{
    // �̺�Ʈ ������ ����
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

    // ������ �� �ִϸ��̼� �Լ�
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


        //�ϴ� ���ڷ� ǥ�� (������ �ִ�� ���� �ִ°Ÿ�)
        if (userSprite->getPositionX() < 668)
            userSprite->setPositionX(666);
        else if(userSprite->getPositionX() > 1232)
            userSprite->setPositionX(1230);

        if (userSprite->getPositionY() < 68)
            userSprite->setPositionY(66);
        else if (userSprite->getPositionY() > 655)
            userSprite->setPositionY(653);


            

        bool isIdle = (_keyCodeX == 0 && _keyCodeY == 0);

        // �����̱� �����ϸ� �̵� �ִϸ��̼�
        if (wasIdle && !isIdle) {
            userAnimMove();
        }

        // ���߸� Idle �ִϸ��̼�
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

    

    //�Ѿ� ���� 
    {
        if (vBullet.size() <= subFireX)
                createBullet();

        if (vEnemyBullet.size() <= subFireX)
            enemyBulletCreate();
            
    }
    mTime += dt;

    //���� �ð� ����(����)
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
                // ȭ�鿡�� �� ����
                this->removeChild(enemy);
                // �迭���� �� ����
                enemys = vEnemy.erase(enemys);
            }
            else if (boomStart)
            {
                
                this->removeChild(enemy);
                // �迭���� �� ����
                enemys = vEnemy.erase(enemys);
            }
            else
            {
                ++enemys;  
            }
        }
    }


  //��Ÿ
    {
        //�÷��̾� ���� �ð�
        if (NoneHit)
        {
            NoneHitTime += dt;
            if (NoneHitTime >= 4.2f)
            {
                //�� ��Ʈ Ÿ�� �ʱ�ȭ
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

    // �̵� ���� ������Ʈ
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
    // �Ѿ� �߻� �ӵ� ����
    if (createBulletTime < 0.1f) return;

    // ���� ĳ������ x, y ��ǥ
    auto x = userSprite->getPositionX();
    auto y = userSprite->getPositionY();

    // �迭�� ���� �ε����� �ִ� �Ѿ��� �߻�
    auto bullet = vBullet.at(currentBulletIndex);
    bullet->setPosition(x, y);

    // �Ѿ��� �̵� �׼� ����
    auto forward = MoveTo::create(0.5f, Vec2(x, y + 1000));
    auto seq = Sequence::create(
        Show::create(),
        forward,
        nullptr);
    bullet->runAction(seq);

    // �ε��� ��ȯ (�迭 ���� �����ϸ� ó������ ���ư�)
    currentBulletIndex = (currentBulletIndex + 1) % vBullet.size();

    // ���� �Ѿ� �߻�
    if(PlayerSubFireObj >= 2)
    {
        SubFire(subFireX, 0);   // ������ ���� �Ѿ�
        SubFire(-subFireX, 0);  // ���� ���� �Ѿ�
    }
    else if (PlayerSubFireObj == 1)
    {
        SubFire(subFireX, 0);
    }

    createBulletTime = 0;

    // �浹 ������ ���� �����ٷ� ����
    this->schedule(schedule_selector(tounMainGame::collision));
}

void tounMainGame::SubFire(int sub , int subXEnd)
{
    // ���� ĳ������ x, y ��ǥ
    auto x = userSprite->getPositionX() + sub;
    auto y = userSprite->getPositionY() ;

    // �迭�� ���� �ε����� �ִ� �Ѿ��� �߻�
    auto bullet = vBullet.at(currentBulletIndex);
    bullet->setPosition(x, y);

    // �Ѿ��� �̵� �׼� ����
    auto forward = MoveTo::create(0.5f, Vec2(x + subXEnd, y + 1000));
    auto seq = Sequence::create(
        Show::create(),
        forward,
        nullptr);
    bullet->runAction(seq);

    // �ε��� ��ȯ (�迭 ���� �����ϸ� ó������ ���ư�)
    currentBulletIndex = (currentBulletIndex + 1) % vBullet.size();

    // �浹 ������ ���� �����ٷ� ����
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

//�Ѿ� �߻� �ϱ��� ������ �Լ�
void tounMainGame::createBullet()
{
    if (createBulletTime < 0.1f)
        return;

    // �ʿ��� �Ѿ˸�ŭ �̸� ���� 
    for (int i = 0; i < 1000; ++i) {
        //�Ѿ� �̹���
        auto bullet = Sprite::create("bullet.png");
        // ó���� ȭ�� �ۿ� ��ġ
        bullet->setPosition(Vec2(-100, -100));  
        //������ ȭ�� ������ ����
        bullet->runAction(Hide::create());  
        this->addChild(bullet);

        //�迭�� �ֱ�
        vBullet.push_back(bullet);
    }

    createBulletTime = 0;

}

//�Ѿ� �浹�� ������ �Լ�
void tounMainGame::collision(float dt)
{
    for (auto bulletIt = vBullet.begin(); bulletIt != vBullet.end();)
    {
        auto bullet = *bulletIt;

        if (bullet->getBoundingBox().intersectsRect(mapWall->getBoundingBox()))
        {
            // �浹 �� �Ѿ��� �ٽ� ����� ��ġ�� �ʱ�ȭ
            bullet->stopAllActions();
            bullet->setPosition(-100, -100);
            bullet->runAction(Hide::create());
        }
        else if (bullet->getBoundingBox().intersectsRect(BossSprite->getBoundingBox())
            && !bossAnimStart)
        {
            // ������ �浹 �� ó��
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
                    // �浹 �� �Ѿ��� �ٽ� ����� ��ġ�� �ʱ�ȭ
                    bullet->stopAllActions();
                    bullet->setPosition(-100, -100);
                    bullet->runAction(Hide::create());
                  
                    ItemSpawn(enemy->getPositionX(), enemy->getPositionY());

                    enemy->stopAllActions();
                    enemy->setPosition(-100, -100);
                    enemy->runAction(Hide::create());

                    
              

                    // ���� �Ѿ��� ���� �迭���� ����
                    enemyIt = vEnemy.erase(enemyIt);  // �� ����
                    vEnemyHitBullet = true;
                    break;
                }
                else
                {
                    ++enemyIt;  // �浹���� ���� ��� ���� ������ �̵�
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
                ++bulletIt;  // �浹���� ���� ��� ���� �Ѿ˷� �̵�
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

void tounMainGame::userAnimMove()
{
    
    userSprite->stopAllActions();  // ���� ���� ���� �׼� ����

    auto cache = SpriteFrameCache::getInstance();
    cache->addSpriteFramesWithFile("GameImg/UserAnim.plist");

    Vector<SpriteFrame*> animFrames;
    for (int i = 0; i < 3; i++)
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
        // StringUtils::format()=>������ �������� ���ڿ� ����
        std::string _frames = StringUtils::format("Boss%02d.png", i + 1);

        // ������ ���ڿ��� �̿��Ͽ� plist ���� SpriteFrame ������ �����´�.
        SpriteFrame* frame = cache->getSpriteFrameByName(_frames);

        // ������ SpriteFrame�� ����
        animFrames.pushBack(frame);
    }

    // �ִϸ��̼� ����
    auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
    auto animate = Animate::create(animation);
    auto rep = Sequence::create(
        MoveTo::create(2.0f, Vec2(949, 560)),
        animate,
        nullptr
    );


    // ���� ��������Ʈ�� �ִϸ��̼� ����
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
        // StringUtils::format()=>������ �������� ���ڿ� ����
        std::string _frames = StringUtils::format("mon%d.png", i + 1);

        // ������ ���ڿ��� �̿��Ͽ� plist ���� SpriteFrame ������ �����´�.
        SpriteFrame* frame = cache->getSpriteFrameByName(_frames);

        // ������ SpriteFrame�� ����
        animFrames.pushBack(frame);
    }



    auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
    auto animate = Animate::create(animation);


    auto rep = RepeatForever::create(animate);

    // ���� ��������Ʈ�� �ִϸ��̼� ����
    enemy->runAction(rep);

    vEnemy.push_back(enemy);
    this->addChild(enemy);
}

void tounMainGame::randomSpanw()
{
    //���� ���� ��ġ�� �������� ���ϴ� �Լ�
    float randX = random(666, 1232);
    SpawnX = randX;

}

void tounMainGame::enemyBulletCreate()
{
    // �ʿ��� �Ѿ˸�ŭ �̸� ���� 
    for (int i = 0; i < 1000; ++i) {
        //�Ѿ� �̹���
        auto bullet = Sprite::create("EnemyBullet.png");
        // ó���� ȭ�� �ۿ� ��ġ
        bullet->setPosition(Vec2(-100, -100));
        //������ ȭ�� ������ ����
        bullet->runAction(Hide::create());
        this->addChild(bullet);

        //�迭�� �ֱ�
        vEnemyBullet.push_back(bullet);
    }
}

void tounMainGame::enemyFire(float dt)
{
    // ���� �Ѿ��� ���ٸ� ����
    if (vEnemyBullet.empty())
        enemyBulletCreate();  

    for (auto enemy : vEnemy)
    {
        // �迭�� ���� �ε����� �ִ� �Ѿ��� �߻�
        auto bullet = vEnemyBullet.at(currentEnemyBulletIndex);

        // ���� x, y ��ǥ
        float x = enemy->getPositionX();
        float y = enemy->getPositionY();

        bullet->setPosition(x, y);

        // �Ѿ��� �̵� �׼� ����
        auto forward = MoveTo::create(4.0f, Vec2(x, y - 800));
        auto seq = Sequence::create(
            Show::create(),
            forward,
            nullptr);
        bullet->runAction(seq);

        // �ε��� ��ȯ (�迭 ���� �����ϸ� ó������ ���ư�)
        currentEnemyBulletIndex = (currentEnemyBulletIndex + 1) % vEnemyBullet.size();

        // �浹 ������ ���� �����ٷ� ����
        this->schedule(schedule_selector(tounMainGame::enemyCollision));
    }


}

void tounMainGame::BossFire(float mapX)
{
    // ���� �Ѿ��� ���ٸ� ����
    if (vEnemyBullet.empty())
        enemyBulletCreate();

    // �迭�� ���� �ε����� �ִ� �Ѿ��� �߻�
    auto bullet = vEnemyBullet.at(currentEnemyBulletIndex);

    // �� ���� x, y ��ǥ (700�� ���� ��ġ)
    float x = mapX;
    float y = 700.0f;

    bullet->setPosition(x, y);

    // �Ѿ��� �̵� �׼� ����
    auto forward = MoveTo::create(5.0f, Vec2(x , y - 800));
    auto seq = Sequence::create(
        Show::create(),
        forward,
        nullptr);
    bullet->runAction(seq);

    // �ε��� ��ȯ (�迭 ���� �����ϸ� ó������ ���ư�)
    currentEnemyBulletIndex = (currentEnemyBulletIndex + 1) % vEnemyBullet.size();

    // �浹 ������ ���� �����ٷ� ����
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
        //�� ������ �������� ����
        if (bullet->getBoundingBox().intersectsRect(mapWall->getBoundingBox()))
        {
            // �浹 �� �� �Ѿ��� �ٽ� ����� ��ġ�� �ʱ�ȭ
            bullet->stopAllActions();
            bullet->setPosition(-100, -100);
            bullet->runAction(Hide::create());
        }
        //�� �Ѿ��� �������� �¾����� ����
        else if (bullet->getBoundingBox().intersectsRect(userSprite->getBoundingBox()))
        {
            // �� �Ѿ��� �÷��̾�� �浹���� ��
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
        //���� �÷��̾��� �Ѿ��� �¾����� ����
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
    //����� 0�� �Ǹ� ������ ���� ��
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
        // �̵� �Ϸ� �� ���߰� �ϴ� CallFunc
        auto stopAction = CallFunc::create([enemy]() {
            enemy->stopAllActions();  // ��� �׼��� �����Ͽ� ���߱�
            });

        // BezierBy �׼� �ڿ� ���� �׼��� �����ϴ� ������ ����
        auto sequence = Sequence::create(
            moveEnd,
            stopAction, 
            nullptr);


        // ��������Ʈ�� �׼� ����
        enemy->runAction(sequence);

    }
}

void tounMainGame::ItemSpawn(float x, float y)
{
    Item = Sprite::create("GameImg/Item.png");
    Item->setVisible(true);

    Item->setPosition(x, y);

    // �θ� ��忡 �߰�
    this->addChild(Item);

    // �������� �̵� �� �ִϸ��̼� 
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
    //�÷��̾�� �浹��
    if (Item->getBoundingBox().intersectsRect(userSprite->getBoundingBox()))
    {
        Item->stopAllActions();
        Item->setPosition(-100, -100);
        Item->runAction(Hide::create());
        PlayerSubFireObj ++;
    }
    // �� ���� ������ ��� 
    else if (Item->getBoundingBox().intersectsRect(mapWall->getBoundingBox()))
    {
        Item->stopAllActions();
        Item->setPosition(-100, -100);
        Item->runAction(Hide::create());
    }
    
}




