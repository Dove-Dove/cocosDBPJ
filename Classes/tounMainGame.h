#pragma once
#include "cocos2d.h"
#include <vector>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#pragma execution_character_set("utf-8")
#endif


class tounMainGame : cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	void onEnter();
	void onExit();

	//업데이트
	virtual void update(float dt) override;

	//키보드 값 입력
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	CREATE_FUNC(tounMainGame);
private:
	//기본 캐릭터 스프라이트 
	cocos2d::Sprite* userSprite;
	cocos2d::Sprite* mapWall;
	cocos2d::Sprite* boomSprite;

	std::vector<cocos2d::Sprite*> vSubObj;

	//아이템 스프라이트
	cocos2d::Sprite* Item;

	//키 입력
	cocos2d::EventListenerKeyboard* _listener;
	//키값 입력 + 움직임 관련 
	int _keyCodeX;
	int _keyCodeY;
	int _keyCodeOrder;
	bool _leftMove ;

	//유저 총알 관련 ----
	
	//메인 총알
	void Fire();
	//서브 총알 
	void SubFire(int subx, int suby);
	//서브 오브젝트 On
	void subObjMove(float PlayerX, float PlayerY, int Items);

	//서브 총알을 발사하는 오브젝트 (생성)
	void subObjcreate();
	//총알 생성
	void createBullet();
	//총알 충돌시 이벤트
	void collision(float dt);
	//플레이어 격추
	void PlayerHit();
	
	//총알 생산시간
	float createBulletTime = 0;
	//
	int currentBulletIndex;
	//아이템을 먹으면 나오는 subFire
	int PlayerSubFireObj = 0;
	//플레이어 목숨
	int lifeCount = 3;
	bool LifeCounting = true;

	//플레이가 격추 당하면 무적
	bool NoneHit = false;
	//무적 시간 
	float NoneHitTime = 0.0f;


	//서브 오브젝트 확인하는 bool값
	bool subOn = false;

	//백터로 총알을 관리 
	std::vector<cocos2d::Sprite*> vBullet;
	bool vEnemyHitBullet = false;
	bool WellHit = false;

	//점수 
	int allScore = 0;

	int subFireX = 35;

	//폭탄
	int BoomCount = 3;
	bool boomStart = false;
	float boomTime = 4.0f;

	//-------------------

	//---유저 애니메이션
	void userAnimIdle();
	void userAnimMove();

	//폭탄 관련
	void usingBoom();

	


	//------------적 관련-------------

	//적관련 스프라이트
	cocos2d::Sprite* enemySprite;
	std::vector<cocos2d::Sprite*>vEnemy;

	//몬스터 스폰(X)위치 지정용
	float SpawnX = 0.0f;
	//몬스터 스폰 시간 관련
	float mTime = 0.0f;
	float spawnTime = 4.0f;

	//보스 스프라이트
	cocos2d::Sprite* BossSprite;
	
	//보스 등장 타이밍
	int monsterCount = 25;
	bool bossAnimStart = true;

	//보스 패턴을 하나만 만들 계획
	bool BossFireStart = false;
	bool fireType1 = true;
	float BossFireX = 670.0f;


	//보스 애니메이션
	void BossAnim();
	int bossHp = 10;
	bool BossHit = false;

	//몬스터
	void monsterCreate(float x);

	//몬스터 스폰 관련
	void randomSpanw();


	//적 총알 관련
	void enemyBulletCreate();
	void enemyFire(float dt);
	
	//적 총알 관련
	void BossFire(float mapX);

	//총알 발사 (적)
	float FireMonsterTime = 3.0f;

	//총알 발사 (보스)
	void BossManyFire();

	//적 총알 충돌 관련
	void enemyCollision(float dt);
	int currentEnemyBulletIndex;
	
	//몬스터 움직임 
	void MonsterMove();

	// 아이템 관련 
	void ItemSpawn(float x, float y);
	void ItemCollder(float dt);

	
	

	std::vector<cocos2d::Sprite*> vEnemyBullet;


	//-------------텍스트------------
	 
	//점수 텍스트 (게임 플레이 점수)
	cocos2d::Label* pScoreLabel;
	//점수 텍스트(글자 띄우는 용도)
	cocos2d::Label* ScoreText;
	//점수 갱신용
	unsigned int mScoreValue;
	
	//남은 목슴
	cocos2d::Label* hpCountText;
	cocos2d::Label* hpCount;
	//폭탄 
	cocos2d::Label* LboomCountText;
	cocos2d::Label* LboomCount;

	//점수 갱신용
	void UpdateScore(int score);
	// 목숨 텍스트 갱신용
	void UpdateLife();

};