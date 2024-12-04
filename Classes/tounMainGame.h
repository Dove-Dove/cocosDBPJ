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

	//������Ʈ
	virtual void update(float dt) override;

	//Ű���� �� �Է�
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	CREATE_FUNC(tounMainGame);
private:
	//�⺻ ĳ���� ��������Ʈ 
	cocos2d::Sprite* userSprite;
	cocos2d::Sprite* mapWall;
	cocos2d::Sprite* boomSprite;

	std::vector<cocos2d::Sprite*> vSubObj;

	//������ ��������Ʈ
	cocos2d::Sprite* Item;

	//Ű �Է�
	cocos2d::EventListenerKeyboard* _listener;
	//Ű�� �Է� + ������ ���� 
	int _keyCodeX;
	int _keyCodeY;
	int _keyCodeOrder;
	bool _leftMove ;

	//���� �Ѿ� ���� ----
	
	//���� �Ѿ�
	void Fire();
	//���� �Ѿ� 
	void SubFire(int subx, int suby);
	//���� ������Ʈ On
	void subObjMove(float PlayerX, float PlayerY, int Items);

	//���� �Ѿ��� �߻��ϴ� ������Ʈ (����)
	void subObjcreate();
	//�Ѿ� ����
	void createBullet();
	//�Ѿ� �浹�� �̺�Ʈ
	void collision(float dt);
	//�÷��̾� ����
	void PlayerHit();
	
	//�Ѿ� ����ð�
	float createBulletTime = 0;
	//
	int currentBulletIndex;
	//�������� ������ ������ subFire
	int PlayerSubFireObj = 0;
	//�÷��̾� ���
	int lifeCount = 3;
	bool LifeCounting = true;

	//�÷��̰� ���� ���ϸ� ����
	bool NoneHit = false;
	//���� �ð� 
	float NoneHitTime = 0.0f;


	//���� ������Ʈ Ȯ���ϴ� bool��
	bool subOn = false;

	//���ͷ� �Ѿ��� ���� 
	std::vector<cocos2d::Sprite*> vBullet;
	bool vEnemyHitBullet = false;
	bool WellHit = false;

	//���� 
	int allScore = 0;

	int subFireX = 35;

	//��ź
	int BoomCount = 3;
	bool boomStart = false;
	float boomTime = 4.0f;

	//-------------------

	//---���� �ִϸ��̼�
	void userAnimIdle();
	void userAnimMove();

	//��ź ����
	void usingBoom();

	


	//------------�� ����-------------

	//������ ��������Ʈ
	cocos2d::Sprite* enemySprite;
	std::vector<cocos2d::Sprite*>vEnemy;

	//���� ����(X)��ġ ������
	float SpawnX = 0.0f;
	//���� ���� �ð� ����
	float mTime = 0.0f;
	float spawnTime = 4.0f;

	//���� ��������Ʈ
	cocos2d::Sprite* BossSprite;
	
	//���� ���� Ÿ�̹�
	int monsterCount = 25;
	bool bossAnimStart = true;

	//���� ������ �ϳ��� ���� ��ȹ
	bool BossFireStart = false;
	bool fireType1 = true;
	float BossFireX = 670.0f;


	//���� �ִϸ��̼�
	void BossAnim();
	int bossHp = 10;
	bool BossHit = false;

	//����
	void monsterCreate(float x);

	//���� ���� ����
	void randomSpanw();


	//�� �Ѿ� ����
	void enemyBulletCreate();
	void enemyFire(float dt);
	
	//�� �Ѿ� ����
	void BossFire(float mapX);

	//�Ѿ� �߻� (��)
	float FireMonsterTime = 3.0f;

	//�Ѿ� �߻� (����)
	void BossManyFire();

	//�� �Ѿ� �浹 ����
	void enemyCollision(float dt);
	int currentEnemyBulletIndex;
	
	//���� ������ 
	void MonsterMove();

	// ������ ���� 
	void ItemSpawn(float x, float y);
	void ItemCollder(float dt);

	
	

	std::vector<cocos2d::Sprite*> vEnemyBullet;


	//-------------�ؽ�Ʈ------------
	 
	//���� �ؽ�Ʈ (���� �÷��� ����)
	cocos2d::Label* pScoreLabel;
	//���� �ؽ�Ʈ(���� ���� �뵵)
	cocos2d::Label* ScoreText;
	//���� ���ſ�
	unsigned int mScoreValue;
	
	//���� ��
	cocos2d::Label* hpCountText;
	cocos2d::Label* hpCount;
	//��ź 
	cocos2d::Label* LboomCountText;
	cocos2d::Label* LboomCount;

	//���� ���ſ�
	void UpdateScore(int score);
	// ��� �ؽ�Ʈ ���ſ�
	void UpdateLife();

};