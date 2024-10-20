#include "GameEngine_pch.h"
#include "Sound.h"

Sound::Sound(std::string name, bool loop)
	: m_fileName(name), m_isLoop(loop), m_volume(1.f),
	m_sound(nullptr), m_channel(nullptr), m_channelGroup(nullptr),
	m_fadeVolume(1.f), m_fadeSpeed(6.f), m_timeWaitForFadeIn(0.f)
{

}

Sound::~Sound()
{
	if (m_sound) m_sound->release();
	if (m_channelGroup) m_channelGroup->release();
}


//void Sound::UpdateVolume()
//{
//	m_result = m_channel->setVolume(m_masterVolume * m_volume);
//	if (m_result != FMOD_OK) { assert(false && L"FMOD 볼륨설정 실패"); }
//}

