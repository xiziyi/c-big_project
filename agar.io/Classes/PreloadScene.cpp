#include "PreloadScene.h"
using namespace CocosDenshion;
USING_NS_CC;
Scene* PreLoad::createScene() {
	// 创建一个场景对象，该对象将会由自动释放池管理内存的释放 
	auto scene = Scene::create();
	// 创建层对象，该对象将会由自动释放池管理内存的释放
		auto layer = PreLoad::create();
	// 将 PreLoad层作为子节点添加到场景
		scene->addChild(layer);
	// 返回场景对象
		return scene;
}
bool PreLoad::init() {
	// 调用父类的init方法 
	if (!Layer::init()) {
		return false;
	}
	// 获得设备可见视图大小 
	Size visibleSize = Director::getInstance()->getVisibleSize();
	// 创建一个进度条 
	auto barSprite = Sprite::create("progressbar.png"); 
	// 初始化一个ProgressTimer进度条对象 
	_progress = ProgressTimer::create(barSprite); 
	// setPercentage:0.0f,表示并未加栽任何资源，表现在屏幕上就是什么也看不见 
	_progress->setPercentage(0); 
	// 由于图片大小关系，把 scale设置成0.5,即缩小一半 
	_progress->setScale(0.5f); 
	// 设置进度条动画的起始位置，默认在图片的中点 
	// 如果想要显示从左到右的一个动画效果，必须改成（0,y) 
	_progress->setMidpoint(Vec2(0, 0.5f)); 
	// barChangeRate表示是否改变水平或者垂直方向的比例，设置成1表示改变，0表示不改变 
	_progress->setBarChangeRate(Vec2(1, 0.f)); 
	// kCCProgressTimerTypeBar 表示为条形进度条 
	_progress->setType(ProgressTimer::Type::BAR); 
	// 设置进度条位置在屏幕中心 
	_progress->setPosition(visibleSize.width/2,visibleSize.height/2); 
	// 将进度条添加为当前层的子节点 
	this->addChild(_progress);
	return true;
}
void PreLoad::onEnterTransitionDidFinish() { 
	// 调用父类的 onEnterTransitionDidFinish 方法 
	Layer::onEnterTransitionDidFinish(); 
	/* 注意：cocos2d-x 3.0之后，Array和 Dictionary都被废弃了，使用Vector和 map替代 
	cocos2d-x 3.0新增数据类型Value,可以存储各种基本数据类型 
	ValueVector 就是保存 Value 的 Vector 集合：typedef std::vector<Value> ValueVector; 
	ValueMap 就是保存 Value 的 Map 集合：typedef std : : unordered_map<std ::string, Value> ValueMap;
	*/
	// 加载preloadResources.plist配置文件，读取文件中的游戏资源名称列表，返回一个ValueMap对象 
	ValueMap map = FileUtils::getInstance()->getValueMapFromFile("preloadResources.plist");
	// 通过key值取出每种不同类型资源的ValueVector数组 
	ValueVector spriteSheets = map.at("SpriteSheets").asValueVector();
	ValueVector effects = map.at("Sounds").asValueVector(); 
	ValueVector musics = map.at("Musics").asValueVector(); 
	// 多个ValueVector数组的size相加得到需要加栽的资源总数量
	_sourceCount = spriteSheets.size() + effects.size() + musics.size();
	// 设置进度条更新次数 = 100 / 需要加栽的资源数量
	_progressInterval = 100 / _sourceCount;
	// 依次加栽资源
	loadMusic(musics);
	loadEffect(effects);
	loadSpriteSheets(spriteSheets);
	
// 加载背景音乐
void loadMusic(ValueVector musicFiles) {
	for (unsigned int i = 0; i != musicFiles.size(); ++i) {
		Value v = musicFiles.at(i);
		SimpleAudioEngine::getInstance()->preloadBackgroundMusic(v.asString().c_str());
		progressUpdate();		
	}
}

// 加载声音
void loadEffect(ValueVector effectFiles) {
	for (unsigned int i = 0; i != effectFiles.size();++i) { 
		Value v = effectFiles.at(i); 
		SimpleAudioEngine::getInstance()->preloadEffect(v.asString().c_str()); 
		progressUpdate (); 
	} 
}

// 加载精灵表单 
void loadSpriteSheets(ValueVector spriteSheets) {
	/*该函数会加栽与plist文件名称相同但后缀为.png的纹理图片， 
	把该plist的所有spriteFrame信息读取出来，在之后的代码中可以通过 
	Sprite : : createWithSpriteFrameName(const std : : strings spriteFrameName)函数获取对应的 精灵顿本例中 airfightSheet.plist 对应 airfightSheet.png 
	*/ 
	for (unsigned int i = 0; i != spriteSheets.size(); ++i) {
		Value v = spriteSheets.at(i);
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(v.asString().c_str());
		progressUpdate();
	}
}

// 进度条更新函数 
void PreLoad::progressUpdate(){
	// 每次调用该函数说明加载一个资源，自减更新资源总数
	if (--_sourceCount) {
		// 设置进度条百分比
		_progress->setPercentage(100 - (_progressInterval * _sourceCount));
	}
	else {
		// ProgressFromTo动作以渐进的方式显示图片 
		// 第一个参数表示持续0.5秒，第二个参数表示进度条百分比开始，第三个参数表示一直到100 
		auto pft = ProgressFromTo::create(0.5f, _progress->getPercentage(), 100);
		//定义了一个CallFunc动作 
		auto callFunc = CallFunc::create([=] {
			// 当加载资源完成，延迟2 秒跳转到游戏主场景 
			auto delay = DelayTime::create(2);
			auto callFunc = CallFunc::create([] {
				Director::getInstance()->replaceScene(Game::createScene());
			})；
				auto action = Sequence:icreate(delay, callFunc, NULL);
			this->runAction(action);
		});
		auto action = Sequence::create(pft, callFunc, NULL);
		// 进度条执行动作
		_progress->runAction(action);
	}
}
