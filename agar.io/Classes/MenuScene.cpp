#include "cocos2d.h"
#include "MenuScene.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;
cocos2d::Scene * GameMenu::createScene()
{
	return nullptr;
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

	//"开始游戏"按钮
	auto start_button = Button::create("button.png");
	start_button->setScale(2);
	start_button->setTitleText("开始游戏");
	start_button->setTitleFontName("微软雅黑");
	start_button->setTitleFontSize(16);
	start_button->setPosition(Vec2(visibleSize.width / 2, visibleSize.height*0.7));
	start_button->addTouchEventListener([](Ref*pSender, Widget::TouchEventType type)
	{
		if (type == Widget::TouchEventType::ENDED) 
		{
			//切换到preload场景
			auto transition = TransitionSlideInL::create(2.0, PreLoad::createScene());
			Director::getInstance()->replaceScene(transition);
		}
	});
	this->addChild(start_button);
	//"游戏设置"按钮
	auto set_button = Button::create("button.png");
	set_button->setScale(2);
	set_button->setTitleText("游戏设置");
	set_button->setTitleFontName("微软雅黑");
	set_button->setTitleFontSize(16);
	set_button->setPosition(Vec2(visibleSize. width / 2, visibleSize.height*0.55));
	set_button->addTouchEventListener([](Ref*pSender, Widget::TouchEventType type)
		{
			if (type == Widget::TouchEventType::ENDED)
			{
				//切换到AudioControl场景
				auto transition = TransitionSlideInL::create(2.0, AudioControlScene::createScene());
				Director::getInstance()->replaceScene(transition);
			}
		});
	this->addChild(set_button);
	//"退出游戏"按钮
	auto close_button = Button::create("button.png");
	close_button->setScale(2);
	close_button->setTitleText("游戏设置");
	close_button->setTitleFontName("微软雅黑");
	close_button->setTitleFontSize(16);
	close_button->setPosition(Vec2(visibleSize.width / 2, visibleSize.height*0.4));
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

GameMenu::CREATE_FUNC(GameMenu)
{
}
