#pragma once
#include "cocos2d.h"

class test2d : cocos2d::Scene
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

	CREATE_FUNC(test2d);
private:
	cocos2d::Sprite* userSprite;

	cocos2d::EventListenerKeyboard* _listener;

	int _keyCodeX;
	int _keyCodeY;
	int _keyCodeOrder;
	bool isMoveing;

	void userAnimIdle();
	void userAnimMove();

};