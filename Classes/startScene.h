#pragma once
#include "cocos2d.h"

class startScene : cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	CREATE_FUNC(startScene);
private:
	void nextScene(Ref* pSender);
	void Exit(Ref* pSender);

};