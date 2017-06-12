#pragma once
#include "cocos2d.h"
USING_NS_CC;


class Game : public cocos2d::Layer
{
private:
	int _screenWidth, _screenHeight;
	int _count;
	TMXTiledMap* _map;
	Sprite* player;
public:
	static cocos2d::Scene* createScene();

	//获得动画函数
	//Animate* getAnimateByName(std::string animName, float delay, int animNum);
	//将屏幕OpenGL坐标转化为TileMap坐标
	Vec2 tileCoordForPosition(Vec2 position);
	//计算玩家移动时间函数
	float getPlayerMoveTime(Vec2 startPos, Vec2 endPos);
	//玩家移动函数
	void playerMover(Vec2 position);
	//把玩家作为视觉中心来显示，让地图随玩家移动
	void setViewPointCenter(Vec2 position);
	//随机创造食物
	void createfood();
	//计时器中转函数
	void foodmagic(float dt);

	virtual bool init();

	//void update(float delta);


	//a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(Game);
};
