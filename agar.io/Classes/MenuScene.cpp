#include"cocos2d.h"
#include "MenuScene.h"
#include"AudioControlScene.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;

Scene * GameMenu::createScene()
{
	auto scene = Scene::create();
    auto layer = GameMenu::create();
	scene->addChild(layer);
	return scene;
}
bool GameMenu::init()
{
	//调用父类的init方法
	if (!Layer::init()) 
	{
		return false;
	}
	
	//获得设备可见视图大小
	Size visibleSize = Director::getInstance()->getVisibleSize();
	
	//菜单背景
    auto title = Sprite::create("menu_background.png");
	title->setPosition(Vec2(visibleSize.width/2 , visibleSize.height/2 ));
	this->addChild(title,0);
	
	//"开始游戏"按钮
	auto start_button = Button::create("Begin.png");
	start_button->setPosition(Vec2(visibleSize.width / 2, visibleSize.height*0.6));
	/*start_button->addTouchEventListener([](Ref*pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::ENDED) 
		{
			//切换到preload场景
			auto transition = TransitionShrinkGrow::create(1.0, preload::createScene());
			Director::getInstance()->replaceScene(transition);
		}
	});*/ 
	this->addChild(start_button);
	
	//"游戏设置"按钮
	auto set_button = Button::create("Setting.png");
	set_button->setPosition(Vec2(visibleSize. width / 2, visibleSize.height*0.45));
	set_button->addTouchEventListener([](Ref*pSender, Widget::TouchEventType type)
		{
			if (type == Widget::TouchEventType::ENDED)
			{
				//切换到AudioControl场景
				auto transition = TransitionShrinkGrow::create(0.2, AudioControl::createScene());
				Director::getInstance()->replaceScene(transition);
			}
		});
	this->addChild(set_button);
	
	//"退出游戏"按钮
	auto close_button = Button::create("Exit.png");
	close_button->setPosition(Vec2(visibleSize.width / 2, visibleSize.height*0.3));
	close_button->addTouchEventListener([](Ref*pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::ENDED)
		{
          #if (CC_TARGET_PLATFORM ==CC_PLATFORM_WP8) || (CC_TARGET_PLATFROM ==CC_PLATFORM_WINRT)
			MessageBox("You pressed the cliose button.Window Store Apps do not implement a close button.", "Alert");
			return;
          #endif
			Director::getInstance()->end();

		}
	});
	this->addChild(close_button);
	return true;
}


