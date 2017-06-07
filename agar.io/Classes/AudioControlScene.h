#include"cocos2d.h"

class AudioControl : public cocos2d::Layer
{
public:
	//静态函数，创建Scene场景
	static cocos2d::Scene* createScene();
	//init函数，完成初始化操作
	virtual bool init();
	CREATE_FUNC(GameSet);
};