#ifndef _MENUSCENE_H_
#define _MENUSCENE_H_

#include "cocos2d.h"

class GameMenu : public cocos2d::Layer
{
public:
	//静态函数，创建Scene场景
	static cocos2d::Scene*createScene();
	virtual bool init();
    CREATE_FUNC(GameMenu);
};
#endif
