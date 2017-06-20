#include "cocos2d.h"
#include "MenuScene.h"
#include "ChooseScene.h"
#include "ui/CocosGUI.h"
#include"GameScene.h"
//#define userDefault CCUserDefault::sharedUserDefault()

USING_NS_CC;
using namespace ui;

Scene * Choose::createScene()
{
	auto scene = Scene::create();
	auto layer = Choose::create();
	scene->addChild(layer);
	return scene;
}

bool Choose::init()
{
	//调用父类的init方法
	if (!Layer::init())
	{
		return false;
	}

	//获得设备可见视图大小
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//菜单背景
	auto title = Sprite::create("choosebackground.png");
	title->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(title, 0);

	//"皮肤"按钮
	auto choose_button1 = Button::create("haipa.png");
	choose_button1->setPosition(Vec2(visibleSize.width *0.3, visibleSize.height*0.65));
	choose_button1->addTouchEventListener([](Ref*pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::ENDED)
		{
			UserDefault::getInstance()->setIntegerForKey("integer", 1);
			UserDefault::getInstance()->flush();
			auto transition = TransitionShrinkGrow::create(1.0, GameScene::createScene());
			Director::getInstance()->replaceScene(transition);
		}
	});
	this->addChild(choose_button1);
	//2
	auto choose_button2 = Button::create("meng.png");
	choose_button2->setPosition(Vec2(visibleSize.width *0.5, visibleSize.height*0.65));
	choose_button2->addTouchEventListener([](Ref*pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::ENDED)
		{
			UserDefault::getInstance()->setIntegerForKey("integer", 2);
			UserDefault::getInstance()->flush();
			//切换到youxi场景
			auto transition = TransitionShrinkGrow::create(1.0, GameScene::createScene());
			Director::getInstance()->replaceScene(transition);
		}
	});
	this->addChild(choose_button2);
	//3
	auto choose_button3 = Button::create("kaixin.png");
	choose_button3->setPosition(Vec2(visibleSize.width *0.7, visibleSize.height*0.65));
	choose_button3->addTouchEventListener([](Ref*pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::ENDED)
		{
			UserDefault::getInstance()->setIntegerForKey("integer", 3);
			UserDefault::getInstance()->flush();
			//切换到youxi场景
			auto transition = TransitionShrinkGrow::create(1.0, GameScene::createScene());
			Director::getInstance()->replaceScene(transition);
		}
	});
	this->addChild(choose_button3);
	//4
	auto choose_button4 = Button::create("shengqi.png");
	choose_button4->setPosition(Vec2(visibleSize.width *0.3, visibleSize.height*0.35));
	choose_button4->addTouchEventListener([](Ref*pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::ENDED)
		{
			UserDefault::getInstance()->setIntegerForKey("integer", 4);
			UserDefault::getInstance()->flush();
			//切换到youxi场景
			auto transition = TransitionShrinkGrow::create(1.0, GameScene::createScene());
			Director::getInstance()->replaceScene(transition);
		}
	});
	this->addChild(choose_button4);
	//5
	auto choose_button5 = Button::create("mogui.png");
	choose_button5->setPosition(Vec2(visibleSize.width *0.5, visibleSize.height*0.35));
	choose_button5->addTouchEventListener([](Ref*pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::ENDED)
		{
			UserDefault::getInstance()->setIntegerForKey("integer", 5);
			UserDefault::getInstance()->flush();
			//切换到youxi场景
			auto transition = TransitionShrinkGrow::create(1.0, GameScene::createScene());
			Director::getInstance()->replaceScene(transition);
		}
	});
	this->addChild(choose_button5);
	//6
	auto choose_button6 = Button::create("zhongdu.png");
	choose_button6->setPosition(Vec2(visibleSize.width *0.7, visibleSize.height*0.35));
	choose_button6->addTouchEventListener([](Ref*pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::ENDED)
		{
			UserDefault::getInstance()->setIntegerForKey("integer", 6);
			UserDefault::getInstance()->flush();
			//切换到youxi场景
			auto transition = TransitionShrinkGrow::create(1.0, GameScene::createScene());
			Director::getInstance()->replaceScene(transition);
		}
	});
	this->addChild(choose_button6);

	//创建“返回钮点击时调用returnToMenu函数 
	auto return_button = Button::create("return.png");
	return_button->setPosition(Vec2(visibleSize.width *0.9, visibleSize.height*0.1));
	return_button->addTouchEventListener([](Ref* pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::ENDED)
		{
			// 切换到MenuScene场景 
			auto transition = TransitionShrinkGrow::create(0.2, GameMenu::createScene());
			Director::getInstance()->replaceScene(transition);
		}
	});
	this->addChild(return_button);

	return true;
}


	