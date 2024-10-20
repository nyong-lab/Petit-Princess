#pragma once
#include "ISingleton.h"
#include "fmod.hpp"

/// <summary>
/// 2024.01.03
/// �Ҹ� ���ҽ������� ����� ���� �Ŵ��� - �̱���
/// �ۼ� : ���¿�
/// 
/// 2024.02.06 [����] ������
/// - �ϴ� ������ ���� ���� ����Ŵ��� ����
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

	void PlaySFX(std::string filename, bool loop = false);	// ȿ���� ���, �� ���� �������� (ex. PlaySFX("swish.wav", false);)
	void StopSFX(std::string filename);						// Ư�� ȿ���� ����
	void StopAllSFX();										// ��� ȿ���� ����

	void PlayBGM(std::string filename);		// ����� ��� (ex. PlayBGM("seaway.mp3");)
	void PauseBGM(std::string filename);	// �ش� ����� �Ͻ�����
	void ResumeBGM(std::string filename);	// �ش� ����� �簳
	void StopAllBGM();						// ��� ����� ����

	void StopAllSounds();					// ��� �Ҹ� ����

	// ����
	void SetMasterVolume(float value);						// ��ü���� ��
	void SetBGMVolume(std::string filename, float value);	// ���� BGM ������
	void SetSFXVolume(std::string filename, float value);	// ���� SFX ������
	
	// �����׽�Ʈ
	void VolumeUp();		// ��ü���� 10���� �ø���
	void VolumeDown();		// ��ü���� 10���� ������

	// ���̵� - ��ݿ��� ���밡���մϴ�
	void FadeInBGM(std::string filename, float offset, float fadetime);	// offset �ð����� ��ٸ���, fadetime �ð����� ���� ���̵� �� (�ʴ���)
	void FadeOutBGM(std::string filename, float fadetime);				// fadetime �ð����� ���� ���̵� �ƿ� (�ʴ���)

	// �ָԱ��������� �߰��ߴ٤̤� �Ϲݺ��<->�߰ݺ�� ��ȯ���� ���!!
	void ResumeFadeInBGM(std::string filename, float offset, float fadetime);
	void CancelFadeoutPlayBGM(std::string filename);

private:
	SoundManager();

	void Play(std::shared_ptr<Sound>&);
	void CreateSoundAndChannelGroup(std::string& filename, bool loop, std::shared_ptr<Sound>& sound);
	void UpdateMasterVolumeForAllSounds();

	void FadeInUpdate(float dt);
	void FadeOutUpdate(float dt);

	// ���� ����: SFX�� loop ���η� ������
	std::unordered_map<std::string, std::shared_ptr<Sound>> m_loopSFX;
	std::unordered_map<std::string, std::shared_ptr<Sound>> m_notLoopSFX;
	std::shared_ptr<Sound> GetOrCreateSFX(std::string filename, bool isLoop);

	// BGM�� ������ loop
	std::unordered_map<std::string, std::shared_ptr<Sound>> m_BGM;
	std::shared_ptr<Sound> GetOrCreateBGM(std::string filename);

	// �ý��۰� ä�ε�
	FMOD::System* m_system;

	// ���� üũ
	FMOD_RESULT m_result;

	// ����
	float m_masterVolume;

	// ���� ���� ��ġ
	std::string m_soundDir = "..\\Resource\\Sound\\";

	// ���̵�
	std::vector<std::shared_ptr<Sound>> m_fadeInSounds;
	std::vector<std::shared_ptr<Sound>> m_fadeOutSounds;
};