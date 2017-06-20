#pragma once
#include "cocos2d.h"
#include"customBall.h"
#include<vector>
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;


class GameScene :public cocos2d::Layer
{
private:
	Label* weightscore;
	int ballTag = 0;
	int _screenWidth, _screenHeight;
	int _Weight;
	Button* return1_button;
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
	const int foodMax = 1200;
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
	//体重更新
	void Weightchange(Node * who, Vec2 position);
	//创造球（尺寸，放置位置，速度设定，球种类）
	void creatBall(float scale, cocos2d::Vect pos, cocos2d::Vect vel,int kind);
	//递增精灵的tag
	void ballTagPlusOne();
	//得到精灵的tag
	int getBallTag();
	//不断产生食物、吐泡
	void createFood(float dt);
	//把玩家作为视觉中心来显示，让地图随玩家移动
	void setViewPointCenter(Vec2 position);
	//确认分裂过了一阵子
	void checkDT(float dt);

	//返回选择界面
	void returnchoice(Node* who, Vec2 position);
	//
	void onEnter();

	Point  scenecenter(Vec2 position);

	//重写update函数
	void update(float dt);
	//void update(float dt) override;
	//接触事件函数
	bool _onContactBegin(const cocos2d::PhysicsContact & contact);
	//设置地图中心
	//void setViewPointCenter(cocos2d::Vec2 position);
	//void mapMove(cocos2d::Vect pos);
	CREATE_FUNC(GameScene);

};

