#pragma once
#include "cocos2d.h"

class test2d : cocos2d::Scene
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