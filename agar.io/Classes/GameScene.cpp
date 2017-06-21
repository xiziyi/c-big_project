#include "GameScene.h"
#include "MenuScene.h"
#include "AudioControlScene.h"
#include"SimpleAudioEngine.h"
#include<math.h>
#include "ui/CocosGUI.h"
#include "string"
#include"LossLayer.h"
#include"VectorLayer.h"
USING_NS_CC;
using namespace ui;
using namespace std;
using namespace CocosDenshion;

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
	layer->setTag(0);
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
	_screenWidth = visibleSize.width;
	_screenHeight = visibleSize.height;

	//添加地图
	_map = TMXTiledMap::create("_map.tmx");
	_map->setAnchorPoint(Vec2::ZERO);
	_map->setPosition(Vec2::ZERO);
	this->addChild(_map);
	_map->setTag(0);

	//是否播放背景音乐
	float musicpercent = UserDefault::getInstance()->getFloatForKey("musictPercent");
	if (musicpercent > 0.5)
	{
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music.wav", true);
	}
	else
	{
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	}

	//第一个球
	GameScene::creatBall(0.5f, Vect(_map->getMapSize().width*_map->getTileSize().width/2, _map->getMapSize().height*_map->getTileSize().height / 2), Vect(0.f, 0.f), player);
	//ai生成
	int y = rand() % (int)(_map->getMapSize().height*_map->getTileSize().height);//随机选择位置生成
	int x = rand() % (int)(_map->getMapSize().width*_map->getTileSize().width);
	x = MIN(x, _map->getMapSize().width*_map->getTileSize().width - 200);//但不会在地图边缘出生
	y = MIN(y, _map->getMapSize().height*_map->getTileSize().height - 200);
	x = MAX(x, 200);
	y = MAX(y, 200);
	GameScene::creatBall(0.75f, Vect(x, y), Vect(0.f, 0.f), aiball);
	//食物
	creatBall(0.1f, Vect(visibleSize.width / 2, visibleSize.height / 2), Vect(0.f, 0.f),food);
	//病毒
	creatBall(0.3f, Vect(visibleSize.width / 2, visibleSize.height / 2), Vect(0.f, 0.f), virus);
	_ai1 = 300;
	//创建标签
	_Weight = 100;
	char a[8];
	//数字转字符串
	char score[20] = "score:";
	sprintf(a, "%d", _Weight);
	strcat(score, a);
	weightscore = Label::createWithSystemFont(score, "Arial", 32);
	weightscore->setPosition(_map->getMapSize().width*_map->getTileSize().width / 2 - 300, _map->getMapSize().height*_map->getTileSize().height / 2 + 185);
	this->addChild(weightscore);


	//创建返回
	return1_button = Button::create("return.png");
	return1_button->setPosition(Vec2(_map->getMapSize().width*_map->getTileSize().width / 2+350, _map->getMapSize().height*_map->getTileSize().height / 2+185));
	return1_button->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::ENDED)
		{
			// 切换到MenuScene场景 
			auto transition = TransitionShrinkGrow::create(0.2, GameMenu::createScene());
			Director::getInstance()->replaceScene(transition);
		}
	});
	this->addChild(return1_button);

	//创建一个盒子，用来碰撞
	Sprite* edgeSpace = Sprite::create();
	PhysicsBody* boundBody = PhysicsBody::createEdgeBox(Size(_map->getMapSize().width*_map->getTileSize().width, _map->getMapSize().height*_map->getTileSize().height), PHYSICSBODY_MATERIAL_DEFAULT);
	boundBody->getShape(0)->setFriction(0.0f);
	boundBody->getShape(0)->setRestitution(0.1f);

	edgeSpace->setPhysicsBody(boundBody);
	edgeSpace->setPosition(Point(_map->getMapSize().width*_map->getTileSize().width / 2, _map->getMapSize().height*_map->getTileSize().height / 2));
	this->addChild(edgeSpace);
	edgeSpace->setTag(10);


	//键盘监听，控制分裂
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
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
					//播放分裂音效
					float effectpercent = UserDefault::getInstance()->getFloatForKey("effectPercent");
					if (effectpercent > 0.5)
					{
						SimpleAudioEngine::sharedEngine()->playEffect("effect1.wav", false);
					}
					else
					{
						SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
					}
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
				n->getPhysicsBody()->applyImpulse(((this->convertToNodeSpace(touch->getLocation())) - (n->getPosition())) * 1000);
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
				n->getPhysicsBody()->applyImpulse(((this->convertToNodeSpace(touch->getLocation())) - (n->getPosition())) * 1000);
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
	//计时器 ，定时吐泡泡，刷新食物，刷新病毒
	schedule(schedule_selector(GameScene::createFood), 0.1f);
	this->schedule(schedule_selector(GameScene::update1), 1 / 60);
	this->schedule(schedule_selector(GameScene::update), 1 / 60);

	return true;
}


void GameScene::Weightchange(Node* who, Vec2 position)
{
	who->removeFromParentAndCleanup(true);
	char a[8];
	char score[20] = "score:";
	//数字转字符串
	sprintf(a, "%d", _Weight);
	strcat(score, a);
	weightscore = Label::createWithSystemFont(score, "Arial", 32);
	weightscore->setPosition(scenecenter(position).x - 300, scenecenter(position).y + 185);
	this->addChild(weightscore);
}


void GameScene::creatBall(float scale, Vect pos, Vect vel,int kind)
{

	if (kind == player)
	{
		auto ball = customBall::create("meng1.png", kind);
		int i = UserDefault::getInstance()->getIntegerForKey("integer");
		if (i == 1)
		{
             ball = customBall::create("haipa1.png", kind);
		}
		else if (i == 2)
		{
			 ball = customBall::create("meng1.png", kind);
		}
		else if (i == 3)
		{
			 ball = customBall::create("kaixin1.png", kind);
		}
		else if (i == 4)
		{
			 ball = customBall::create("shengqi1.png", kind);
		}
		else if (i ==5)
		{
			 ball = customBall::create("mogui1.png", kind);
		}
		else if (i == 6)
		{
			 ball = customBall::create("zhongdu1.png", kind);
		}

		
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
		ballBody->getShape(0)->setDensity(1.f);
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
		ball->setPosition(pos);
		
	}
	else if (kind == food)
	{
		Size visibleSize = Director::getInstance()->getVisibleSize();
		for (;foodCount != foodMax;)
		{
			auto ball = customBall::create("food.png", kind);
			ball->setScale(scale);
			int y = rand() % (int)( _map->getMapSize().height*_map->getTileSize().height);
			int x = rand() % (int)(_map->getMapSize().width*_map->getTileSize().width );
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
	else if (kind == virus)
	{
		Size visibleSize = Director::getInstance()->getVisibleSize();
		for (;virusCount != virusMax;)
		{
			auto ball = customBall::create("virus.png", kind);
			ball->setScale(scale);
			int y = rand() % (int)(_map->getMapSize().height*_map->getTileSize().height);
			int x = rand() % (int)(_map->getMapSize().width*_map->getTileSize().width);
			ball->setPosition(x, y);
			ball->rad = ball->getContentSize().width / 2;
			PhysicsBody* ballBody = PhysicsBody::createCircle(ball->rad, PHYSICSBODY_MATERIAL_DEFAULT);
			ballBody->setDynamic(false);
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
			ball->setTag(getBallTag() * 10 + kind);
			ballBody->setCategoryBitmask(0x0001);
			ballBody->setCollisionBitmask(0x0001);
			ballBody->setContactTestBitmask(0x0001);

			this->addChild(ball);
			ballTagPlusOne();
			++virusCount;
		}
	}
	else if (kind == aiball)
	{
		auto ai_ball = customBall::create("ai.png", kind);
		ai_ball->setScale(scale);
		ai_ball->setPosition(pos);
		//球半径
		ai_ball->rad = ai_ball->getContentSize().width / 2;
		//创建物体，并且物体的形状为圆形，第一参数为半径，第二个参数为物体材质
		//第三个参数为边的厚度,就是在Debug模式下看到的物体外面线条的厚度，默认为0
		PhysicsBody* ballBody = PhysicsBody::createCircle(ai_ball->rad, PHYSICSBODY_MATERIAL_DEFAULT);
		//是否设置物体为静态
		//ballBody->setDynamic(true);
		//设置物体的恢复力（弹性）
		ballBody->getShape(0)->setRestitution(0.1f);
		//设置物体的摩擦力（只有碰撞时才有用）
		ballBody->getShape(0)->setFriction(0.0f);
		//设置物体密度
		ballBody->getShape(0)->setDensity(1.f);
		//设置质量
		//ballBodyOne->getShape(0)->setMass(5000);
		//设置物体是否受重力系数影响
		ballBody->setGravityEnable(false);

		//设置物体的冲力
		Vect force = Vect(0.0f, 0.0f);
		ballBody->applyImpulse(force);
		//设置速度
		ballBody->setVelocity(vel);
		ballBody->setVelocityLimit(20000.f / ai_ball->rad);
		//把物体添加到精灵中
		ai_ball->setPhysicsBody(ballBody);
		//设置标志
		ai_ball->setTag(getBallTag() * 10 + kind);
		//设置分类掩码
		ballBody->setCategoryBitmask(0x0001);
		//设置碰撞掩码
		ballBody->setCollisionBitmask(0x0001);
		//设置接触测试掩码
		ballBody->setContactTestBitmask(0x0001);

		this->addChild(ai_ball);
		//球的tag加一
		ballTagPlusOne();


	}
}

void GameScene::aiaction(Node* who)
{
	Vector<Node*> children = this->getChildren();
	int index = 0;
	for (auto n : children)
	{
		if (n->getTag() % 10 == player)
		{

			if ((who->getPosition().x >= (_map->getMapSize().width*_map->getTileSize().width - 100) || who->getPosition().x <= 100) && (who->getPosition().y >= (_map->getMapSize().height*_map->getTileSize().height - 100) || who->getPosition().y <= 100))
			{
				who->getPhysicsBody()->applyImpulse((Point(_map->getMapSize().width*_map->getTileSize().width / 2, 300) - (who->getPosition())) * 2000);
			}
			else if (who->getPosition().x >= (_map->getMapSize().width*_map->getTileSize().width - 100) || who->getPosition().x <= 100)
			{
				if (_Weight >= _ai1)
					who->getPhysicsBody()->applyImpulse((Point(0, who->getPosition().y) - Point(0, n->getPosition().y)) * 2000);
				else who->getPhysicsBody()->applyImpulse((Point(0, n->getPosition().y) - Point(0, who->getPosition().y)) * 2000);
			}
			else if (who->getPosition().y >= (_map->getMapSize().height*_map->getTileSize().height - 100) || who->getPosition().y <= 100)
			{
				if (_Weight >= _ai1)
					who->getPhysicsBody()->applyImpulse((Point(who->getPosition().x, 0) - Point(n->getPosition().x, 0)) * 2000);
				else  who->getPhysicsBody()->applyImpulse((Point(n->getPosition().x, 0) - Point(who->getPosition().x, 0)) * 2000);
			}
			else
			{
				if (_Weight >= _ai1)
					who->getPhysicsBody()->applyImpulse(((who->getPosition()) - (n->getPosition())) * 2000);
				else who->getPhysicsBody()->applyImpulse(((n->getPosition()) - (who->getPosition())) * 2000);
			}
			++index;
			if (index != 0)break;
		}
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
	creatBall(0.1f, Vect(_map->getMapSize().width*_map->getTileSize().width / 2, _map->getMapSize().height*_map->getTileSize().height / 2), Vect(0.f, 0.f), food);
	creatBall(0.3f, Vect(_map->getMapSize().width*_map->getTileSize().width / 2, _map->getMapSize().height*_map->getTileSize().height / 2), Vect(0.f, 0.f), virus);
	if (_checkWP)//产生mass
	{
		Vector<Node*> children = this->getChildren();
		for (auto n : children)
		{
			if (((n->getTag() % 10) == player) && (n->getScale() > 0.3f))
			{				
				creatBall(0.15f, n->getPosition(), n->getPhysicsBody()->getVelocity()* 15, mass);
				//播放吐泡泡音效
				float effectpercent = UserDefault::getInstance()->getFloatForKey("effectPercent");
				if (effectpercent > 0.5)
				{
					SimpleAudioEngine::sharedEngine()->playEffect("effect2.wav", false);
				}
				else
				{
					SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
				}
				float newScale = pow(((n->getScale()*n->getScale()*n->getScale())-0.003375f), 0.333333f);
				creatBall(newScale, n->getPosition(), n->getPhysicsBody()->getVelocity(), player);
				_Weight -= 15;
				n->removeFromParentAndCleanup(true);
			}
		}
	}

}

void GameScene::checkDT(float dt)
{
	_checkDT = true;
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



//地图中心寻找函数
Point GameScene::scenecenter(Vec2 position)
{
	int x = MAX(position.x, _screenWidth / 2);
	int y = MAX(position.y, _screenHeight / 2);

	x = MIN(x, (_map->getMapSize().width*_map->getTileSize().width - _screenWidth / 2));
	y = MIN(y, (_map->getMapSize().height*_map->getTileSize().height - _screenHeight / 2));
	return Point(x, y);
}


void GameScene::returnchoice(Node* who, Vec2 position)
{
	who->setPosition(Vec2(scenecenter(position)).x+350, Vec2(scenecenter(position)).y+185);
}

void GameScene::setViewPointCenter(Vec2 position)
{
	
	Point actualPosition =scenecenter(position);

	Point centerOfView = Point(_screenWidth / 2, _screenHeight / 2);
	Point viewPoint = centerOfView - actualPosition;
	//Point try1 = Point(x + 10, y + 10);
	this->setPosition(viewPoint);
	//this->runAction(MoveTo::create(1.0f, viewPoint));
	//this->runAction(MoveTo::create(0.01f, Point(3000,3000));
}


void GameScene::update(float dt)
{
	Vector<Node*> children = this->getChildren();
	int index = 0;
	int index1 = 0;
	int index2 = 0;
	for (auto n : children)
	{
		if (n->getTag() %10 == player)
		{
			setViewPointCenter(n->getPosition());
			Weightchange(weightscore, n->getPosition());
			returnchoice(return1_button, n->getPosition());
			++index;
			if (index != 0)break;
		}
	}
	for (auto n : children)
	{
		if (n->getTag() % 10 == player)
		{
			++index1;
		}
		if (n->getTag() % 10 == aiball)
		{
			++index2;
		}
	}
	if (index1 == 0)
	{
		Layer* _layer=LossLayer::create();
		_layer->setTag(1);
		this->getParent()->addChild(_layer);
	}
	else if (index2 == 0)
	{
		Layer* _layer = VectorLayer::create();
		_layer->setTag(1);
		this->getParent()->addChild(_layer);
	}
}

void GameScene::update1(float dt)
{
	Vector<Node*> children = this->getChildren();
	for (auto n : children)
	{
		if (n->getTag() % 10 == aiball)
		{
			this->aiaction(n);
		}
	}
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
			if ((tagA % 10 == 1) && (tagB % 10 == 1) && _checkDT)//玩家和玩家
			{
				float newScale = pow((spriteA->getScale()*spriteA->getScale()*spriteA->getScale()) + (spriteB->getScale()*spriteB->getScale()*spriteB->getScale()), 0.333333f);
				spriteA->setScale(newScale);
				spriteB->removeFromParentAndCleanup(true);
				unschedule(schedule_selector(GameScene::checkDT));
			}
			else if ((tagA % 10 == 1) && (tagB % 10 == 2))//玩家和食物
			{
				float newScale = pow((spriteA->getScale()*spriteA->getScale()*spriteA->getScale()) + (spriteB->getScale()*spriteB->getScale()*spriteB->getScale()), 0.333333f);
				spriteA->setScale(newScale);
				spriteB->removeFromParentAndCleanup(true);
				_Weight += 10;
				--foodCount;
			}
			else if ((tagA % 10 == 2) && (tagB % 10 == 1))//食物和玩家
			{
				float newScale = pow((spriteA->getScale()*spriteA->getScale()*spriteA->getScale()) + (spriteB->getScale()*spriteB->getScale()*spriteB->getScale()), 0.333333f);
				spriteB->setScale(newScale);
				spriteA->removeFromParentAndCleanup(true);
				_Weight += 10;
				--foodCount;
			}
			else if ((tagA % 10 == 1) && (tagB % 10 == 3) && (!_checkWP))//玩家和mass
			{
				float newScale = pow((spriteA->getScale()*spriteA->getScale()*spriteA->getScale()) + (spriteB->getScale()*spriteB->getScale()*spriteB->getScale()), 0.333333);
				spriteA->setScale(newScale);
				spriteB->removeFromParentAndCleanup(true);
				_Weight += 15;
			}
			else if ((tagA % 10 == 3) && (tagB % 10 == 1) && (!_checkWP))//mass和玩家
			{
				float newScale = pow((spriteA->getScale()*spriteA->getScale()*spriteA->getScale()) + (spriteB->getScale()*spriteB->getScale()*spriteB->getScale()), 0.333333);
				spriteB->setScale(newScale);
				spriteA->removeFromParentAndCleanup(true);
				_Weight += 15;
			}
			else if ((tagA % 10 == 1) && (tagB % 10 == 5) && (spriteA->getScale()>0.2))//玩家和virus
			{
				float newScale = spriteA->getScale() / 2;
				Vect pos = this->convertToWorldSpace(spriteA->getPosition());
				creatBall(newScale, pos, Vect(0.f, 100.f), player);
				creatBall(newScale, pos, Vect(0.f, -100.f), player);
				creatBall(newScale, pos, Vect(100.f, 0.f), player);
				creatBall(newScale, pos, Vect(100.f, -100.f), player);
				creatBall(newScale, pos, Vect(-100.f, 0.f), player);
				creatBall(newScale, pos, Vect(-100.f, 100.f), player);
				creatBall(newScale, pos, Vect(-100.f, -100.f), player);
				creatBall(newScale, pos, Vect(100.f, 100.f), player);
				spriteA->removeFromParentAndCleanup(true);
				spriteB->removeFromParentAndCleanup(true);
				_checkDT = false;//中毒分裂后短时不能融合
				scheduleOnce(schedule_selector(GameScene::checkDT), 10.f);
			}
			else if ((tagA % 10 == 5) && (tagB % 10 == 1) && (spriteB->getScale()>0.2))//virus和玩家
			{
				float newScale = spriteB->getScale() / 2;
				Vect pos = this->convertToWorldSpace(spriteA->getPosition());
				creatBall(newScale, pos, Vect(0.f, 100.f), player);
				creatBall(newScale, pos, Vect(0.f, -100.f), player);
				creatBall(newScale, pos, Vect(100.f, 0.f), player);
				creatBall(newScale, pos, Vect(100.f, -100.f), player);
				creatBall(newScale, pos, Vect(-100.f, 0.f), player);
				creatBall(newScale, pos, Vect(-100.f, 100.f), player);
				creatBall(newScale, pos, Vect(-100.f, -100.f), player);
				creatBall(newScale, pos, Vect(100.f, 100.f), player);
				spriteA->removeFromParentAndCleanup(true);
				spriteB->removeFromParentAndCleanup(true);
				_checkDT = false;
				scheduleOnce(schedule_selector(GameScene::checkDT), 10.f);
			}
			else if ((tagA % 10 == 1) && (tagB % 10 == 4))
			{
				float newScale = pow((spriteA->getScale()*spriteA->getScale()*spriteA->getScale()) + (spriteB->getScale()*spriteB->getScale()*spriteB->getScale()), 0.333333f);
				if (spriteA->getScale() > spriteB->getScale())//玩家吃掉ai
				{
					_Weight += spriteB->getScale() *spriteB->getScale() *spriteB->getScale() * 1000;
					spriteA->setScale(newScale);
					spriteB->removeFromParentAndCleanup(true);

				}
				else if (spriteA->getScale() < spriteB->getScale())//ai吃掉玩家
				{
					_ai1 += spriteA->getScale() *spriteA->getScale() *spriteA->getScale() * 1000;
					_Weight -= spriteA->getScale() *spriteA->getScale() *spriteA->getScale() * 1000;
					spriteB->setScale(newScale);
					spriteA->removeFromParentAndCleanup(true);

				}

			}
			else if ((tagA % 10 == 4) && (tagB % 10 == 1))
			{
				float newScale = pow((spriteA->getScale()*spriteA->getScale()*spriteA->getScale()) + (spriteB->getScale()*spriteB->getScale()*spriteB->getScale()), 0.333333f);
				if (spriteA->getScale() < spriteB->getScale())//玩家吃掉ai
				{
					_Weight += spriteA->getScale() *spriteA->getScale() *spriteA->getScale() * 1000;
					spriteB->setScale(newScale);
					spriteA->removeFromParentAndCleanup(true);

				}
				else if (spriteA->getScale() > spriteB->getScale())//ai吃掉玩家
				{
					_ai1 += spriteB->getScale() *spriteB->getScale() *spriteB->getScale() * 1000;
					_Weight -= spriteB->getScale() *spriteB->getScale() *spriteB->getScale() * 1000;
					spriteA->setScale(newScale);
					spriteB->removeFromParentAndCleanup(true);
				}
			}
			else if ((tagA % 10 == 4) && (tagB % 10 == 2))//ai 吃食物
			{
				float newScale = pow((spriteA->getScale()*spriteA->getScale()*spriteA->getScale()) + (spriteB->getScale()*spriteB->getScale()*spriteB->getScale()), 0.333333f);
				spriteA->setScale(newScale);
				spriteB->removeFromParentAndCleanup(true);
				_ai1 += 15;
				log("%d", _ai1);
				--foodCount;
			}
			else if ((tagA % 10 == 2) && (tagB % 10 == 4))//ai吃食物
			{
				float newScale = pow((spriteA->getScale()*spriteA->getScale()*spriteA->getScale()) + (spriteB->getScale()*spriteB->getScale()*spriteB->getScale()), 0.333333f);
				spriteB->setScale(newScale);
				spriteA->removeFromParentAndCleanup(true);
				--foodCount;
				_ai1 += 15;
			}
		}
		++aaa;
	}
	//log("%d and %d", tagA, tagB);
	return true;
}


