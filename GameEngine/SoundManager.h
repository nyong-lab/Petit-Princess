#pragma once
#include "ISingleton.h"
#include "fmod.hpp"

/// <summary>
/// 2024.01.03
/// 소리 리소스관리를 담당할 사운드 매니저 - 싱글톤
/// 작성 : 유승운
/// 
/// 2024.02.06 [수정] 박지원
/// - 일단 별도의 맵을 가진 사운드매니저 구현
/// </summary>

class Sound;
class ResourceManager;

class SoundManager : public ISingleton<SoundManager>
{
	friend class ISingleton<SoundManager>;
public:
	void Initialize(std::shared_ptr<ResourceManager> resourceManager);
	void Update(double dt);
	void Finalize();

	void PlaySFX(std::string filename, bool loop = false);	// 효과음 재생, 룹 여부 설정가능 (ex. PlaySFX("swish.wav", false);)
	void StopSFX(std::string filename);						// 특정 효과음 끄기
	void StopAllSFX();										// 모든 효과음 끄기

	void PlayBGM(std::string filename);		// 배경음 재생 (ex. PlayBGM("seaway.mp3");)
	void PauseBGM(std::string filename);	// 해당 배경음 일시정지
	void ResumeBGM(std::string filename);	// 해당 배경음 재개
	void StopAllBGM();						// 모든 배경음 끄기

	void StopAllSounds();					// 모든 소리 끄기

	// 볼륨
	void SetMasterVolume(float value);						// 전체볼륨 셋
	void SetBGMVolume(std::string filename, float value);	// 개별 BGM 볼륨셋
	void SetSFXVolume(std::string filename, float value);	// 개별 SFX 볼륨셋
	
	// 볼륨테스트
	void VolumeUp();		// 전체볼륨 10프로 올리기
	void VolumeDown();		// 전체볼륨 10프로 내리기

	// 페이드 - 브금에만 적용가능합니다
	void FadeInBGM(std::string filename, float offset, float fadetime);	// offset 시간동안 기다리고, fadetime 시간동안 볼륨 페이드 인 (초단위)
	void FadeOutBGM(std::string filename, float fadetime);				// fadetime 시간동안 볼륨 페이드 아웃 (초단위)

	// 주먹구구식으로 추가했다ㅜㅜ 일반브금<->추격브금 전환에만 사용!!
	void ResumeFadeInBGM(std::string filename, float offset, float fadetime);
	void CancelFadeoutPlayBGM(std::string filename);

private:
	SoundManager();

	void Play(std::shared_ptr<Sound>&);
	void CreateSoundAndChannelGroup(std::string& filename, bool loop, std::shared_ptr<Sound>& sound);
	void UpdateMasterVolumeForAllSounds();

	void FadeInUpdate(float dt);
	void FadeOutUpdate(float dt);

	// 사운드 관리: SFX는 loop 여부로 나눈다
	std::unordered_map<std::string, std::shared_ptr<Sound>> m_loopSFX;
	std::unordered_map<std::string, std::shared_ptr<Sound>> m_notLoopSFX;
	std::shared_ptr<Sound> GetOrCreateSFX(std::string filename, bool isLoop);

	// BGM은 무조건 loop
	std::unordered_map<std::string, std::shared_ptr<Sound>> m_BGM;
	std::shared_ptr<Sound> GetOrCreateBGM(std::string filename);

	// 시스템과 채널들
	FMOD::System* m_system;

	// 에러 체크
	FMOD_RESULT m_result;

	// 볼륨
	float m_masterVolume;

	// 사운드 파일 위치
	std::string m_soundDir = "..\\Resource\\Sound\\";

	// 페이드
	std::vector<std::shared_ptr<Sound>> m_fadeInSounds;
	std::vector<std::shared_ptr<Sound>> m_fadeOutSounds;
};