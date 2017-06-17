#include "GameScene.h"
#include<math.h>
USING_NS_CC;
using namespace std;


GameScene::GameScene()
{
}


GameScene::~GameScene()
{
}
cocos2d::Scene* GameScene::createScene()
{
	//创建有物理空间的场景
	cocos2d::Scene* scene = cocos2d::Scene::createWithPhysics();
	//设置Debug模式，你会看到物体的表面被线条包围，主要为了在调试中更容易地观察
	//scene->getPhysicsWorld()->setDebugDrawMask(cocos2d::PhysicsWorld::DEBUGDRAW_ALL);
	GameScene* layer = GameScene::create();
	//把空间保持我们创建的层中，就是上面所说m_world的作用，方便后面设置空间的参数
	layer->setPhyWorld(scene->getPhysicsWorld());
	scene->addChild(layer);
	return scene;
}

bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	//添加地图
	_map = TMXTiledMap::create("_map.tmx");
	_map->setAnchorPoint(Vec2::ZERO);
	_map->setPosition(Vec2::ZERO);
	this->addChild(_map);
	_map->setTag(0);

	//第一个球
	GameScene::creatBall(0.5f, Vect(visibleSize.width / 2, visibleSize.height / 2), Vect(0.f, 0.f), player);
	//食物
	creatBall(0.1f, Vect(visibleSize.width / 2, visibleSize.height / 2), Vect(0.f, 0.f),food);


	//创建一个盒子，用来碰撞
	Sprite* edgeSpace = Sprite::create();
	PhysicsBody* boundBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT);
	boundBody->getShape(0)->setFriction(0.0f);
	boundBody->getShape(0)->setRestitution(0.1f);

	edgeSpace->setPhysicsBody(boundBody);
	edgeSpace->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(edgeSpace);
	edgeSpace->setTag(10);


	//键盘监听，控制分裂
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		log("Key with keycode %d pressed", keyCode);
		if (keyCode == EventKeyboard::KeyCode::KEY_SPACE)
		{
			Vector<Node*> children = this->getChildren();
			for (auto n : children)
			{
				if (((n->getTag() % 10) == player)&&(n->getScale()>0.3f))
				{
					creatBall((n->getScale()) / 1.2599, n->getPosition(), n->getPhysicsBody()->getVelocity() * 2, player);
					creatBall((n->getScale()) / 1.2599, n->getPosition(), n->getPhysicsBody()->getVelocity(), player);
					n->removeFromParentAndCleanup(true);
					_checkDT = false;//触发分裂后短时不能融合
				}
			}
			scheduleOnce(schedule_selector(GameScene::checkDT), 10.f);
		}
		else if (keyCode == EventKeyboard::KeyCode::KEY_W)
		{
			_checkWP = true;
		}
	};
	listener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		log("Key with keycode %d released", keyCode);
		if (keyCode == EventKeyboard::KeyCode::KEY_W)
			_checkWP = false;

	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//触摸监听，控制运动
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = [=](Touch* touch, Event* ev) {
		Vector<Node*> balls = this->getChildren();
		for (auto n : balls)
		{
			if (n->getTag() % 10 == player)
			{
				n->getPhysicsBody()->applyImpulse(((touch->getLocation()) - (n->getPosition())) * 1000);
			}
		}
		return true;
	};
	touchListener->onTouchMoved = [=](Touch* touch, Event* event) {
		Vector<Node*> balls = this->getChildren();
		for (auto n : balls)
		{
			if (n->getTag() % 10 == player)
			{
				n->getPhysicsBody()->applyImpulse(((touch->getLocation()) - (n->getPosition())) * 1000);
			}
		}
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	
	
	//添加接触（碰撞）监听器
	auto contactListener = EventListenerPhysicsContact::create();
	//设置监听器的碰撞开始函数
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::_onContactBegin, this);
	//添加到事件分发器中
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	//添加调度器
	scheduleUpdate();
	//计时器 
	schedule(schedule_selector(GameScene::createFood), 0.1f);

	return true;
}

void GameScene::creatBall(float scale, Vect pos, Vect vel,int kind)
{

	if (kind == player)
	{
		auto ball = customBall::create("mengB.png", kind);
		//精灵尺寸
		ball->setScale(scale);
		//精灵位置
		ball->setPosition(pos);
		//球半径
		ball->rad = ball->getContentSize().width / 2;
		//创建物体，并且物体的形状为圆形，第一参数为半径，第二个参数为物体材质
		//第三个参数为边的厚度,就是在Debug模式下看到的物体外面线条的厚度，默认为0
		PhysicsBody* ballBody = PhysicsBody::createCircle(ball->rad, PHYSICSBODY_MATERIAL_DEFAULT);
		//是否设置物体为静态
		//ballBody->setDynamic(true);
		//设置物体的恢复力（弹性）
		ballBody->getShape(0)->setRestitution(0.1f);
		//设置物体的摩擦力（只有碰撞时才有用）
		ballBody->getShape(0)->setFriction(0.0f);
		//设置物体密度
		ballBody->getShape(0)->setDensity(1.0f);
		//设置质量
		//ballBodyOne->getShape(0)->setMass(5000);
		//设置物体是否受重力系数影响
		ballBody->setGravityEnable(false);

		//设置物体的冲力
		Vect force = Vect(0.0f, 0.0f);
		ballBody->applyImpulse(force);
		//设置速度
		ballBody->setVelocity(vel);
		ballBody->setVelocityLimit(20000.f / ball->rad);
		//把物体添加到精灵中
		ball->setPhysicsBody(ballBody);
		//设置标志
		ball->setTag(getBallTag()*10+kind);
		//设置分类掩码
		ballBody->setCategoryBitmask(0x0001);
		//设置碰撞掩码
		ballBody->setCollisionBitmask(0x0001);
		//设置接触测试掩码
		ballBody->setContactTestBitmask(0x0001);

		this->addChild(ball);
		//球的tag加一
		ballTagPlusOne();
		
	}
	else if (kind == food)
	{
		Size visibleSize = Director::getInstance()->getVisibleSize();
		for (;foodCount != foodMax;)
		{
			auto ball = customBall::create("food.png", kind);
			ball->setScale(scale);
			int y = rand() % (int)(visibleSize.height);
			int x = rand() % (int)(visibleSize.width);
			ball->setPosition(x, y);
			ball->rad = ball->getContentSize().width / 2;
			PhysicsBody* ballBody = PhysicsBody::createCircle(ball->rad, PHYSICSBODY_MATERIAL_DEFAULT);
			//ballBody->setDynamic(true);
			ballBody->getShape(0)->setRestitution(0.1f);
			ballBody->getShape(0)->setFriction(0.0f);
			ballBody->getShape(0)->setDensity(1.0f);
			//ballBodyOne->getShape(0)->setMass(5000);
			ballBody->setGravityEnable(false);

			//Vect force = Vect(0.0f, 0.0f);
			//ballBody->applyImpulse(force);
			//ballBody->setVelocity(vel);
			ballBody->setVelocityLimit(1.f / ball->rad);
			ball->setPhysicsBody(ballBody);
			ball->setTag(getBallTag()*10+kind);
			ballBody->setCategoryBitmask(0x0001);
			ballBody->setCollisionBitmask(0x0001);
			ballBody->setContactTestBitmask(0x0001);

			this->addChild(ball);
			ballTagPlusOne();
			++foodCount;
		}
	}
	else if (kind == mass)
	{
		log("mass is made");
		auto ball = customBall::create("mass.png", kind);
		ball->setScale(scale);
		ball->setPosition(pos);
		ball->rad = ball->getContentSize().width / 2;
		PhysicsBody* ballBody = PhysicsBody::createCircle(ball->rad, PHYSICSBODY_MATERIAL_DEFAULT);
		//ballBody->setDynamic(true);
		ballBody->getShape(0)->setRestitution(0.1f);
		ballBody->getShape(0)->setFriction(0.0f);
		//设置线性阻尼使其停止
		ballBody->setLinearDamping(8.f);
		ballBody->getShape(0)->setDensity(1.0f);
		//ballBodyOne->getShape(0)->setMass(5000);
		ballBody->setGravityEnable(false);

		//Vect force = Vect(0.0f, 0.0f);
		//ballBody->applyImpulse(force);
		ballBody->setVelocity(vel);
		//ballBody->setVelocityLimit(20000.f / ball->rad);
		ball->setPhysicsBody(ballBody);
		ball->setTag(getBallTag() * 10 + kind);
		ballBody->setCategoryBitmask(0x0001);
		ballBody->setCollisionBitmask(0x0001);
		ballBody->setContactTestBitmask(0x0001);

		this->addChild(ball);
		ballTagPlusOne();
	}
}

void GameScene::ballTagPlusOne()
{
	++ballTag;
}

int GameScene::getBallTag()
{
	return ballTag;
}

void GameScene::createFood(float dt)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	creatBall(0.1f, Vect(visibleSize.width / 2, visibleSize.height / 2), Vect(0.f, 0.f), food);
	log("foodOk");
	if (_checkWP)//产生mass
	{
		log("mass ok");
		Vector<Node*> children = this->getChildren();
		for (auto n : children)
		{
			if (((n->getTag() % 10) == player) && (n->getScale() > 0.3f))
			{
				creatBall(0.15f, n->getPosition(), n->getPhysicsBody()->getVelocity()* 15, mass);
				float newScale = pow(((n->getScale()*n->getScale()*n->getScale())-0.003375f), 0.333333f);
				creatBall(newScale, n->getPosition(), n->getPhysicsBody()->getVelocity(), player);
				n->removeFromParentAndCleanup(true);
			}
		}
	}

}

void GameScene::checkDT(float dt)
{
	_checkDT = true;
	log("divideOk");
}

void GameScene::onEnter()
{
	Layer::onEnter();
	//添加监听器
	auto contactListener = EventListenerPhysicsContact::create();
	//设置监听器的碰撞开始函数
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::_onContactBegin, this);
	//添加到事件分发器中
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}


/*void GameScene::update(float dt)
{
	Vector<Node*> balls = this->getChildren();
	for (auto n : balls)
	{
		PhysicsBody *_ballbody = n->getPhysicsBody();
		_ballbody->setVelocity(eventMouse->getLocation());
	}
}*/

bool GameScene::_onContactBegin(const PhysicsContact& contact)
{
	auto spriteA =contact.getShapeA()->getBody()->getNode();
	auto spriteB =contact.getShapeB()->getBody()->getNode();
	if ((spriteA != nullptr) && (spriteB != nullptr))
	{
		if (aaa % 2 == 1)
		{
			int tagA = spriteA->getTag();
			int tagB = spriteB->getTag();
			if ((tagA % 10 == 1) && (tagB % 10 == 1) && _checkDT)
			{
				float newScale = pow((spriteA->getScale()*spriteA->getScale()*spriteA->getScale()) + (spriteB->getScale()*spriteB->getScale()*spriteB->getScale()), 0.333333f);
				spriteA->setScale(newScale);
				spriteB->removeFromParentAndCleanup(true);
				unschedule(schedule_selector(GameScene::checkDT));
			}
			else if ((tagA % 10 == 1) && (tagB % 10 == 2))
			{
				float newScale = pow((spriteA->getScale()*spriteA->getScale()*spriteA->getScale()) + (spriteB->getScale()*spriteB->getScale()*spriteB->getScale()), 0.333333f);
				spriteA->setScale(newScale);
				spriteB->removeFromParentAndCleanup(true);
				--foodCount;
			}
			else if ((tagA % 10 == 2) && (tagB % 10 == 1))
			{
				float newScale = pow((spriteA->getScale()*spriteA->getScale()*spriteA->getScale()) + (spriteB->getScale()*spriteB->getScale()*spriteB->getScale()), 0.333333f);
				spriteB->setScale(newScale);
				spriteA->removeFromParentAndCleanup(true);
				--foodCount;
			}
			else if ((tagA % 10 == 1) && (tagB % 10 == 3) && (!_checkWP))
			{
				log("mass gone");
				float newScale = pow((spriteA->getScale()*spriteA->getScale()*spriteA->getScale()) + (spriteB->getScale()*spriteB->getScale()*spriteB->getScale()), 0.333333);
				spriteA->setScale(newScale);
				spriteB->removeFromParentAndCleanup(true);
			}
			else if ((tagA % 10 == 3) && (tagB % 10 == 1) && (!_checkWP))
			{
				log("mass gone");
				float newScale = pow((spriteA->getScale()*spriteA->getScale()*spriteA->getScale()) + (spriteB->getScale()*spriteB->getScale()*spriteB->getScale()), 0.333333);
				spriteB->setScale(newScale);
				spriteA->removeFromParentAndCleanup(true);
			}
		}
		++aaa;
	}
	//log("%d and %d", tagA, tagB);
	return true;
}


