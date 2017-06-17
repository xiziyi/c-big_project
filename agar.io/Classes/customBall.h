#pragma once
#include"cocos2d.h"
class customBall :public cocos2d::Sprite
{
public:
	customBall();
	~customBall();

	//球的种类
	int Kind;
	//球的半径
	float rad;

	//重写create
	static customBall* create(const std::string& filename, const int kind);


};

