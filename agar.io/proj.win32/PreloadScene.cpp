#include "PreloadScene.h"  
static SGPlayMusic *playMusic = NULL;

SGPlayMusic::SGPlayMusic()
{
	m_nMusicVolume = 0.5;
	m_nEffectVolume = 0.5f;

}
SGPlayMusic::~SGPlayMusic()
{
	//п╤ть  
	SimpleAudioEngine::sharedEngine()->end();
}

SGPlayMusic* SGPlayMusic::sharePlayMusic()
{
	if (NULL == playMusic)
	{
		playMusic = new SGPlayMusic();
	}

	return playMusic;
}

void SGPlayMusic::playerMusicByType(const char* musicName)
{
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("music.wav");
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("music.wav", true);
}

void SGPlayMusic::setVolume(float volume)
{
	m_nMusicVolume = volume;
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(m_nMusicVolume);
}

void SGPlayMusic::stopMusic()
{
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}

void SGPlayMusic::rewindMusic()
{
	SimpleAudioEngine::sharedEngine()->rewindBackgroundMusic();
}

void SGPlayMusic::pauseMusic()
{
	SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

void SGPlayMusic::playEffectByType(bool isLoop, const char *effectName)
{
	SimpleAudioEngine::sharedEngine()->preloadEffect("373.wav");
	SimpleAudioEngine::sharedEngine()->playEffect(effectName, isLoop);
}

void SGPlayMusic::stopEffect()
{
	SimpleAudioEngine::sharedEngine()->stopAllEffects();
}

void SGPlayMusic::setEffectVolume(float volume)
{
	m_nEffectVolume = volume;
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(m_nEffectVolume);
}

void SGPlayMusic::stopEffect(unsigned int effectId)
{
	SimpleAudioEngine::sharedEngine()->stopEffect(effectId);
}

void SGPlayMusic::rewindEffect(unsigned int effectId)
{
	SimpleAudioEngine::sharedEngine()->resumeEffect(effectId);
}
