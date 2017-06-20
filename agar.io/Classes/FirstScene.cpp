#include "FirstScene.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "MenuScene.h"
USING_NS_CC;
using namespace ui;

Scene * EnterScene::createScene()
{
	auto scene = Scene::create();
	auto layer = EnterScene::create();
	scene->addChild(layer);
	return scene;
}

bool EnterScene::init()
{
	//调用父类的init方法
	if (!Layer::init())
	{
		return false;
	}

	//获得设备可见视图大小
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//菜单背景
	auto title = Sprite::create("background1.png");
	title->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(title, 0);

	auto transition = TransitionFade::create(12.0, GameMenu::createScene());
	Director::getInstance()->replaceScene(transition);
	this->addChild(transition,0);

	return true;
}