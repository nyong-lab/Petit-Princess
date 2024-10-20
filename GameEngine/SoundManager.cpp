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
	if (m_result != FMOD_OK) { assert(false && L"FMOD playSound 함수 실패"); }
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

	if (m_result != FMOD_OK) { assert(false && L"FMOD 사운드 생성 실패 - 사운드 이름이나 확장자 정확?"); }
}

void SoundManager::PlaySFX(std::string filename, bool loop /*= false*/)
{
	auto it = GetOrCreateSFX(filename, loop);
	Play(it);
	it->m_channelGroup->setVolume(m_masterVolume * it->m_volume);
}

void SoundManager::StopSFX(std::string filename) // 룹 여부에 관계없이 해당 이름을 가진 사운드는 다끔
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
		if (m_result != FMOD_OK) { assert(false && L"FMOD BGM 일시정지 실패"); }
	}
}

void SoundManager::ResumeBGM(std::string filename)
{
	auto bgm = m_BGM.find(filename);
	if (bgm != m_BGM.end())
	{
		m_result = bgm->second->m_channelGroup->setPaused(false);
		if (m_result != FMOD_OK) { assert(false && L"FMOD BGM 일시정지 실패"); }
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
	// loop 여부에 따라 다른 맵에 들어간다
	// 같은 파일도 loop 있는것과 없는것을 둘다쓰면 양쪽에 리소스가 생성되겠지만 그럴일은 많이 없을것이라 가정하고..
	if (isLoop)
	{
		auto it = m_loopSFX.find(filename);

		// 사운드맵에서 찾아보기
		if (it != m_loopSFX.end())
		{
			return it->second;
		}
		// 사운드맵에서 못찾으면 만들기
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

		// 사운드맵에서 찾아보기
		if (it != m_notLoopSFX.end())
		{
			return it->second;
		}
		// 사운드맵에서 못찾으면 만들기
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

	// 재생중이 아니라면 재생
	bool check;
	it->m_channel->isPlaying(&check);
	if (!check) Play(it);

	// 약간의 예외처리. 아직 페이드아웃 되고 있는중이었다면 페이드아웃 열에서 빼고 그 볼륨가중치 그대로 가져가기
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
		// 약간의 예외처리. 아직 페이드인 되고 있는중이었다면 페이드인 열에서 빼고 그 볼륨가중치 그대로 가져가기
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
		// 예외처리들
		// 아직 페이드인 중이라면 패스
		auto m = std::find(m_fadeInSounds.begin(), m_fadeInSounds.end(), bgm->second);
		if (m != m_fadeInSounds.end()) return;
		
		// 아직 페이드아웃 되고 있는중이었다면 페이드아웃 열에서 빼고 그 볼륨가중치 그대로 가져가기
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
	// 가중치 0부터 시작해서 1이 되면 끝나는게 페이드 인
	// 페이드인 중인 사운드가 하나라도 있으면
	if (!m_fadeInSounds.empty())
	{
		for (auto it = m_fadeInSounds.begin(); it != m_fadeInSounds.end();)
		{
			auto& i = *it;

			// 기다리기
			if (i->m_timeWaitForFadeIn > 0.f)
			{
				i->m_timeWaitForFadeIn -= dt;
				++it;
			}
			else
			{
				// 페이드 볼륨 가중치가 1.f을 넘기전에 페이드 벡터에서 제외시킴
				if (i->m_fadeVolume + dt / (i->m_fadeSpeed) > 1.f)
				{
					it = m_fadeInSounds.erase(it);
					i->m_channelGroup->setVolume(m_masterVolume * i->m_volume);
				}
				// 1을 넘기 전이라면 가중치를 계속 증가시킨다
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
	// 가중치 1부터 시작해서 0이 되면 끝나는게 페이드 아웃
	// 페이드아웃 중인 사운드가 하나라도 있으면
	if (!m_fadeOutSounds.empty())
	{
		for (auto it = m_fadeOutSounds.begin(); it != m_fadeOutSounds.end();)
		{
			auto& i = *it;

			// 페이드 볼륨 가중치가 0.f보다 작아지기 전에 페이드 벡터에서 제외시킴
			if (i->m_fadeVolume - dt / (i->m_fadeSpeed) < 0.f)
			{
				i->m_channelGroup->setPaused(true); // 나가면서 일시정지
				it = m_fadeOutSounds.erase(it);
				i->m_channelGroup->setVolume(m_masterVolume * i->m_volume);
			}
			// 0보다 작아지기 전이라면 가중치를 계속 감소시킨다
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

	// 사운드맵에서 찾아보기
	if (it != m_BGM.end())
	{
		return it->second;
	}
	// 사운드맵에서 못찾으면 만들기
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
	// 시스템 생성
	m_result = FMOD::System_Create(&m_system);
	if (m_result != FMOD_OK) { assert(false && L"FMOD 시스템 생성 실패"); }

	// 시스템 초기화
	m_result = m_system->init(32, FMOD_INIT_NORMAL, 0);
	if (m_result != FMOD_OK) { assert(false && L"FMOD 시스템 초기화 실패"); }
}

void SoundManager::Update(double dt)
{
	// 시스템 업데이트
	m_result = m_system->update();
	if (m_result != FMOD_OK) { assert(false && L"FMOD 업데이트 실패"); }

	FadeInUpdate(dt);
	FadeOutUpdate(dt);
}

void SoundManager::Finalize()
{
	// 시스템 해제
	if (m_system)
	{
		m_result = m_system->close();
		if (m_result != FMOD_OK) { assert(false && L"FMOD 시스템 닫기 실패"); }

		m_result = m_system->release();
		if (m_result != FMOD_OK) { assert(false && L"FMOD 시스템 해제 실패"); }
	}
}