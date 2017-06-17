#pragma once
#include "cocos2d.h"
#include"customBall.h"
#include<vector>
class GameScene :public cocos2d::Layer
{
private:
	int ballTag = 0;
public:
	GameScene();
	~GameScene();

    //物理世界
	cocos2d::PhysicsWorld* m_world;
	//地图
	cocos2d::TMXTiledMap* _map;
	//当前食物数量
	int foodCount = 0;
	//最大食物数量
	const int foodMax = 60;
	//不可见的神秘力量
	cocos2d::Sprite* master;

	//精灵种类的标识符
	const int player = 1;
	const int food = 2;
	const int mass = 3;
	//const int player2 = 2;
	/* And so on......*/
	int aaa = 0;
	bool _checkDT = true;
	//确认W键是否按下
	bool _checkWP = false;

	//创建物理世界
	void setPhyWorld(cocos2d::PhysicsWorld* world) { m_world = world; };
	//创建场景
	static cocos2d::Scene* createScene();
	//初始化
	virtual bool init();
	//创造球（尺寸，放置位置，速度设定，球种类）
	void creatBall(float scale, cocos2d::Vect pos, cocos2d::Vect vel,int kind);
	//递增精灵的tag
	void ballTagPlusOne();
	//得到精灵的tag
	int getBallTag();
	//不断产生食物、吐泡
	void createFood(float dt);
	//确认分裂过了一阵子
	void checkDT(float dt);
	//
	void onEnter();
	//重写update函数
	//void update(float dt) override;
	//接触事件函数
	bool _onContactBegin(const cocos2d::PhysicsContact & contact);
	//设置地图中心
	//void setViewPointCenter(cocos2d::Vec2 position);
	//void mapMove(cocos2d::Vect pos);
	CREATE_FUNC(GameScene);

};

