#include "cocos2d.h"
#include "SimpleAudioEngine.h"
//预加载类：完成预加载背景音乐、音效、精灵表单等准备工作
class PreLoad : public cocos2d::Layer {
public:
	/**
	*静态函数，创建Scene场景 
	* 注意：在 Cocos2d - x中没有Cocos2d - iPhone中 的 “id” ， 建议返回类实例的指针 
	*/
	static cocos2d::Scene* createScene();
	/**
	* init函数，完成初始化操作。 
	* 注意：init函数在Cocos2d - x中返回bool值，而不是返回Cocos2d - iPhone中 的 “id”
	*/
	virtual bool init ();
	// 当Node节点进入“舞台”时调用 
	virtual void onEnterTransitionDidFinish(); 
	// 加载背景音乐 
	void loadMusic(ValueVector musicFiles);
	// 加载游戏音效
	void loadEffect(ValueVector effectFiles);
	// 加载精灵表单 
	void loadSpriteSheets(ValueVector spriteSheets); 
	// 更新游戏进度条 void progressUpdate(); 
	void progressUpdate();
	/** 
	*使用CREATE_FUNC宏创建当前类的对象，返回的对象将会由自动释放池管理内存的释放
	*/
	CREATE_FUNC(PreLoad);
protected: 
	// 用来保存游戏需要加载的资源总数 
	int _sourceCount; 
	//进度条
	ProgressTimer* _progress; 
	// 进度条更新的次数 
	float _progressInterval; 
};