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

	// 1���� 1ä�� 1ä�α׷��� �δ� ������ �������..
	FMOD::Sound* m_sound;
	FMOD::Channel* m_channel;
	FMOD::ChannelGroup* m_channelGroup;

	// ���̵� ����
	float m_fadeVolume;			// ���̵�� ���� ����ġ
	float m_fadeSpeed;			// ���̵� �ӵ�
	float m_timeWaitForFadeIn;	// ���̵��α��� ��ٸ� �ð�
};