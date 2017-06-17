#include "customBall.h"
#include"cocos2d.h"
USING_NS_CC;


customBall::customBall()
{
}


customBall::~customBall()
{
}

customBall* customBall::create(const std::string& filename, const int kind)
{
	customBall *_ball = new (std::nothrow) customBall();
	if (_ball && _ball->initWithFile(filename))
	{
		_ball->autorelease();
		_ball->Kind = kind;
		return _ball;
	}
	CC_SAFE_DELETE(_ball);
	return nullptr;
}

