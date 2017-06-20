#ifndef __Game__SGPlayMusic__  
#define __Game__SGPlayMusic__  

#include <iostream>  
#include "SimpleAudioEngine.h"  
#include "cocos2d.h"  
using namespace cocos2d;
using namespace CocosDenshion;

class SGPlayMusic
{
private:
	float m_nMusicVolume;
	float m_nEffectVolume;

public:
	SGPlayMusic();
	~SGPlayMusic();
	static SGPlayMusic *sharePlayMusic();

	//播放音乐  
	void playerMusicByType(const char* musicName);
	//停止音乐  
	void stopMusic();
	//改变音乐音量  
	void setVolume(float volume);
	//暂停背景音乐  
	void pauseMusic();
	//重新播放背景音乐  
	void rewindMusic();

	//播放音效  
	void playEffectByType(bool isLoop, const char *effectName);
	//停止音效  
	void stopEffect();
	//改变音效音量  
	void setEffectVolume(float volume);
	//停止某个音效  
	void stopEffect(unsigned int effectId);
	//重新播放音效  
	void rewindEffect(unsigned int effectId);

};
#endif 
