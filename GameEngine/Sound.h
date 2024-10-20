#pragma once
#include "Resource.h"
#include "fmod.hpp"

class Sound
{
public:
	Sound(std::string name, bool loop);
	~Sound();

public:
	std::string m_fileName;
	bool m_isLoop;
	float m_volume;

	// 1사운드 1채널 1채널그룹을 두는 구조로 만들었다..
	FMOD::Sound* m_sound;
	FMOD::Channel* m_channel;
	FMOD::ChannelGroup* m_channelGroup;

	// 페이드 관련
	float m_fadeVolume;			// 페이드시 볼륨 가중치
	float m_fadeSpeed;			// 페이드 속도
	float m_timeWaitForFadeIn;	// 페이드인까지 기다릴 시간
};