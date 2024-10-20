#include "GameEngine_pch.h"
#include "SoundManager.h"
#include "ResourceManager.h"
#include "Sound.h"

SoundManager::SoundManager()
	: m_system(nullptr), m_masterVolume(1.f), m_result(FMOD_OK)
{

}

void SoundManager::Play(std::shared_ptr<Sound>& it)
{
	m_result = m_system->playSound(it->m_sound, it->m_channelGroup, false, &(it->m_channel));
	if (m_result != FMOD_OK) { assert(false && L"FMOD playSound �Լ� ����"); }
}

void SoundManager::CreateSoundAndChannelGroup(std::string& filename, bool loop, std::shared_ptr<Sound>& sound)
{
	m_system->createChannelGroup((filename + "_channelGroup").c_str(), &(sound->m_channelGroup));

	if (loop)
	{
		m_result = m_system->createSound((m_soundDir + filename).c_str(), FMOD_LOOP_NORMAL, 0, &(sound->m_sound));
	}
	else
	{
		m_result = m_system->createSound((m_soundDir + filename).c_str(), FMOD_LOOP_OFF, 0, &(sound->m_sound));
	}

	if (m_result != FMOD_OK) { assert(false && L"FMOD ���� ���� ���� - ���� �̸��̳� Ȯ���� ��Ȯ?"); }
}

void SoundManager::PlaySFX(std::string filename, bool loop /*= false*/)
{
	auto it = GetOrCreateSFX(filename, loop);
	Play(it);
	it->m_channelGroup->setVolume(m_masterVolume * it->m_volume);
}

void SoundManager::StopSFX(std::string filename) // �� ���ο� ������� �ش� �̸��� ���� ����� �ٲ�
{
	auto loop = m_loopSFX.find(filename);
	if (loop != m_loopSFX.end())
	{
		loop->second->m_channelGroup->stop();
	}

	auto notloop = m_notLoopSFX.find(filename);
	if (notloop != m_notLoopSFX.end())
	{
		notloop->second->m_channelGroup->stop();
	}
}

void SoundManager::StopAllSFX()
{
	for (auto& pair : m_loopSFX)
	{
		pair.second->m_channelGroup->stop();
	}
	for (auto& pair : m_notLoopSFX)
	{
		pair.second->m_channelGroup->stop();
	}
}

void SoundManager::PlayBGM(std::string filename)
{
	auto it = GetOrCreateBGM(filename);
	it->m_channelGroup->stop();
	Play(it);
	it->m_channelGroup->setVolume(m_masterVolume * it->m_volume);
}

void SoundManager::PauseBGM(std::string filename)
{
	auto bgm = m_BGM.find(filename);
	if (bgm != m_BGM.end())
	{
		m_result = bgm->second->m_channelGroup->setPaused(true);
		if (m_result != FMOD_OK) { assert(false && L"FMOD BGM �Ͻ����� ����"); }
	}
}

void SoundManager::ResumeBGM(std::string filename)
{
	auto bgm = m_BGM.find(filename);
	if (bgm != m_BGM.end())
	{
		m_result = bgm->second->m_channelGroup->setPaused(false);
		if (m_result != FMOD_OK) { assert(false && L"FMOD BGM �Ͻ����� ����"); }
	}
}

void SoundManager::StopAllBGM()
{
	for (auto& pair : m_BGM)
	{
		pair.second->m_channelGroup->stop();
	}
}

void SoundManager::SetMasterVolume(float value)
{
	if (value > 1.f) value = 1.f;
	else if (value < 0.f) value = 0.f;
	m_masterVolume = value;

	UpdateMasterVolumeForAllSounds();
}

void SoundManager::SetBGMVolume(std::string filename, float value)
{
	if (value > 1.f) value = 1.f;
	else if (value < 0.f) value = 0.f;

	auto bgm = m_BGM.find(filename);
	if (bgm != m_BGM.end())
	{
		bgm->second->m_volume = value;
		bgm->second->m_channelGroup->setVolume(m_masterVolume * bgm->second->m_volume);
	}
}

void SoundManager::SetSFXVolume(std::string filename, float value)
{
	if (value > 1.f) value = 1.f;
	else if (value < 0.f) value = 0.f;

	auto loop = m_loopSFX.find(filename);
	if (loop != m_loopSFX.end())
	{
		loop->second->m_volume = value;
		loop->second->m_channelGroup->setVolume(m_masterVolume * loop->second->m_volume);
	}
	auto notloop = m_notLoopSFX.find(filename);
	if (notloop != m_notLoopSFX.end())
	{
		notloop->second->m_volume = value;
		notloop->second->m_channelGroup->setVolume(m_masterVolume * notloop->second->m_volume);
	}
}

void SoundManager::VolumeUp()
{
	SetMasterVolume(m_masterVolume + 0.1f);
}

void SoundManager::VolumeDown()
{
	SetMasterVolume(m_masterVolume - 0.1f);
}

void SoundManager::StopAllSounds()
{
	for (auto& pair : m_loopSFX)
	{
		pair.second->m_channelGroup->stop();
	}
	for (auto& pair : m_notLoopSFX)
	{
		pair.second->m_channelGroup->stop();
	}
	for (auto& pair : m_BGM)
	{
		pair.second->m_channelGroup->stop();
	}
}

std::shared_ptr<Sound> SoundManager::GetOrCreateSFX(std::string filename, bool isLoop)
{
	// loop ���ο� ���� �ٸ� �ʿ� ����
	// ���� ���ϵ� loop �ִ°Ͱ� ���°��� �Ѵپ��� ���ʿ� ���ҽ��� �����ǰ����� �׷����� ���� �������̶� �����ϰ�..
	if (isLoop)
	{
		auto it = m_loopSFX.find(filename);

		// ����ʿ��� ã�ƺ���
		if (it != m_loopSFX.end())
		{
			return it->second;
		}
		// ����ʿ��� ��ã���� �����
		else
		{
			std::shared_ptr<Sound> newSound = make_shared<Sound>(filename, true);
			CreateSoundAndChannelGroup(filename, isLoop, newSound);
			m_loopSFX.emplace(filename, newSound);
			return m_loopSFX[filename];
		}
	}
	else
	{
		auto it = m_notLoopSFX.find(filename);

		// ����ʿ��� ã�ƺ���
		if (it != m_notLoopSFX.end())
		{
			return it->second;
		}
		// ����ʿ��� ��ã���� �����
		else
		{
			std::shared_ptr<Sound> newSound = make_shared<Sound>(filename, false);
			CreateSoundAndChannelGroup(filename, isLoop, newSound);
			m_notLoopSFX.emplace(filename, newSound);
			return m_notLoopSFX[filename];
		}
	}
}

void SoundManager::FadeInBGM(std::string filename, float offset, float fadetime)
{
	auto it = GetOrCreateBGM(filename);

	// ������� �ƴ϶�� ���
	bool check;
	it->m_channel->isPlaying(&check);
	if (!check) Play(it);

	// �ణ�� ����ó��. ���� ���̵�ƿ� �ǰ� �ִ����̾��ٸ� ���̵�ƿ� ������ ���� �� ��������ġ �״�� ��������
	auto f = std::find(m_fadeOutSounds.begin(), m_fadeOutSounds.end(), it);
	if (f != m_fadeOutSounds.end())
	{
		m_fadeOutSounds.erase(f);
	}
	else
	{
		it->m_fadeVolume = 0.f;
	}

	it->m_fadeSpeed = fadetime;
	it->m_timeWaitForFadeIn = offset;
	it->m_channelGroup->setVolume(m_masterVolume * it->m_volume * it->m_fadeVolume);
	m_fadeInSounds.emplace_back(it);
}

void SoundManager::FadeOutBGM(std::string filename, float fadetime)
{
	auto bgm = m_BGM.find(filename);
	if (bgm != m_BGM.end())
	{
		// �ణ�� ����ó��. ���� ���̵��� �ǰ� �ִ����̾��ٸ� ���̵��� ������ ���� �� ��������ġ �״�� ��������
		auto f = std::find(m_fadeInSounds.begin(), m_fadeInSounds.end(), bgm->second);
		if (f != m_fadeInSounds.end())
		{
			m_fadeInSounds.erase(f);
		}
		else
		{
			bgm->second->m_fadeVolume = 1.f;
		}

		bgm->second->m_fadeSpeed = fadetime;
		bgm->second->m_channelGroup->setVolume(m_masterVolume * bgm->second->m_volume * bgm->second->m_fadeVolume);
		m_fadeOutSounds.emplace_back(bgm->second);
	}
}

void SoundManager::ResumeFadeInBGM(std::string filename, float offset, float fadetime)
{
	auto bgm = m_BGM.find(filename);
	if (bgm != m_BGM.end())
	{
		// ����ó����
		// ���� ���̵��� ���̶�� �н�
		auto m = std::find(m_fadeInSounds.begin(), m_fadeInSounds.end(), bgm->second);
		if (m != m_fadeInSounds.end()) return;
		
		// ���� ���̵�ƿ� �ǰ� �ִ����̾��ٸ� ���̵�ƿ� ������ ���� �� ��������ġ �״�� ��������
		auto f = std::find(m_fadeOutSounds.begin(), m_fadeOutSounds.end(), bgm->second);
		if (f != m_fadeOutSounds.end()) { m_fadeOutSounds.erase(f); }
		else { bgm->second->m_fadeVolume = 0.f; }

		bool t;
		bgm->second->m_channelGroup->getPaused(&t);
		if (!t) return;
		else
		{
			bgm->second->m_channelGroup->setPaused(false);
			bgm->second->m_fadeSpeed = fadetime;
			bgm->second->m_timeWaitForFadeIn = offset;
			bgm->second->m_channelGroup->setVolume(m_masterVolume * bgm->second->m_volume * bgm->second->m_fadeVolume);
			m_fadeInSounds.emplace_back(bgm->second);
		}
	}
}

void SoundManager::CancelFadeoutPlayBGM(std::string filename)
{
	//auto it = GetOrCreateBGM(filename);
	//it->m_channelGroup->stop();
	//Play(it);
	//it->m_channelGroup->setVolume(m_masterVolume * it->m_volume);

	auto it = GetOrCreateBGM(filename);

	auto f = std::find(m_fadeOutSounds.begin(), m_fadeOutSounds.end(), it);
	if (f != m_fadeOutSounds.end())
	{
		m_fadeOutSounds.erase(f);
	}

	it->m_channelGroup->setPaused(false);
	it->m_channelGroup->stop();
	Play(it);
	it->m_channelGroup->setVolume(m_masterVolume * it->m_volume);
}

void SoundManager::UpdateMasterVolumeForAllSounds()
{
	for (auto& pair : m_loopSFX)
	{
		pair.second->m_channelGroup->setVolume(m_masterVolume * pair.second->m_volume);
	}
	for (auto& pair : m_notLoopSFX)
	{
		pair.second->m_channelGroup->setVolume(m_masterVolume * pair.second->m_volume);
	}
	for (auto& pair : m_BGM)
	{
		pair.second->m_channelGroup->setVolume(m_masterVolume * pair.second->m_volume);
	}
}

void SoundManager::FadeInUpdate(float dt)
{
	// ����ġ 0���� �����ؼ� 1�� �Ǹ� �����°� ���̵� ��
	// ���̵��� ���� ���尡 �ϳ��� ������
	if (!m_fadeInSounds.empty())
	{
		for (auto it = m_fadeInSounds.begin(); it != m_fadeInSounds.end();)
		{
			auto& i = *it;

			// ��ٸ���
			if (i->m_timeWaitForFadeIn > 0.f)
			{
				i->m_timeWaitForFadeIn -= dt;
				++it;
			}
			else
			{
				// ���̵� ���� ����ġ�� 1.f�� �ѱ����� ���̵� ���Ϳ��� ���ܽ�Ŵ
				if (i->m_fadeVolume + dt / (i->m_fadeSpeed) > 1.f)
				{
					it = m_fadeInSounds.erase(it);
					i->m_channelGroup->setVolume(m_masterVolume * i->m_volume);
				}
				// 1�� �ѱ� ���̶�� ����ġ�� ��� ������Ų��
				else
				{
					i->m_fadeVolume += dt / (i->m_fadeSpeed);
					i->m_channelGroup->setVolume(m_masterVolume * i->m_volume * i->m_fadeVolume);
					++it;
				}
			}
		}
	}
}

void SoundManager::FadeOutUpdate(float dt)
{
	// ����ġ 1���� �����ؼ� 0�� �Ǹ� �����°� ���̵� �ƿ�
	// ���̵�ƿ� ���� ���尡 �ϳ��� ������
	if (!m_fadeOutSounds.empty())
	{
		for (auto it = m_fadeOutSounds.begin(); it != m_fadeOutSounds.end();)
		{
			auto& i = *it;

			// ���̵� ���� ����ġ�� 0.f���� �۾����� ���� ���̵� ���Ϳ��� ���ܽ�Ŵ
			if (i->m_fadeVolume - dt / (i->m_fadeSpeed) < 0.f)
			{
				i->m_channelGroup->setPaused(true); // �����鼭 �Ͻ�����
				it = m_fadeOutSounds.erase(it);
				i->m_channelGroup->setVolume(m_masterVolume * i->m_volume);
			}
			// 0���� �۾����� ���̶�� ����ġ�� ��� ���ҽ�Ų��
			else
			{
				i->m_fadeVolume -= dt / (i->m_fadeSpeed);
				i->m_channelGroup->setVolume(m_masterVolume * i->m_volume * i->m_fadeVolume);
				++it;
			}
		}
	}
}

std::shared_ptr<Sound> SoundManager::GetOrCreateBGM(std::string filename)
{
	auto it = m_BGM.find(filename);

	// ����ʿ��� ã�ƺ���
	if (it != m_BGM.end())
	{
		return it->second;
	}
	// ����ʿ��� ��ã���� �����
	else
	{
		std::shared_ptr<Sound> newSound = make_shared<Sound>(filename, true);
		CreateSoundAndChannelGroup(filename, true, newSound);
		m_BGM.emplace(filename, newSound);
		return m_BGM[filename];
	}
}

void SoundManager::Initialize(std::shared_ptr<ResourceManager> resourceManager)
{
	// �ý��� ����
	m_result = FMOD::System_Create(&m_system);
	if (m_result != FMOD_OK) { assert(false && L"FMOD �ý��� ���� ����"); }

	// �ý��� �ʱ�ȭ
	m_result = m_system->init(32, FMOD_INIT_NORMAL, 0);
	if (m_result != FMOD_OK) { assert(false && L"FMOD �ý��� �ʱ�ȭ ����"); }
}

void SoundManager::Update(double dt)
{
	// �ý��� ������Ʈ
	m_result = m_system->update();
	if (m_result != FMOD_OK) { assert(false && L"FMOD ������Ʈ ����"); }

	FadeInUpdate(dt);
	FadeOutUpdate(dt);
}

void SoundManager::Finalize()
{
	// �ý��� ����
	if (m_system)
	{
		m_result = m_system->close();
		if (m_result != FMOD_OK) { assert(false && L"FMOD �ý��� �ݱ� ����"); }

		m_result = m_system->release();
		if (m_result != FMOD_OK) { assert(false && L"FMOD �ý��� ���� ����"); }
	}
}