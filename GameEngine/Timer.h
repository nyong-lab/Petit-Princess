#pragma once

/// <summary>
/// Ÿ�̸�
/// ���ӿ����� �ð��� ����Ѵ�
/// �ۼ� : ���¿�
/// ���� : �赿�� - gui���� pause ���� ����
/// </summary>
class Timer
{
public:
	Timer();
	~Timer();

	//���� - ����
	Timer(const Timer& timer) = delete;
	Timer& operator=(const Timer& timer) = delete;

	//�̵� - ����
	Timer(const Timer&& timer) = delete;
	Timer&& operator=(const Timer&& timer) = delete;

	void Initialize();
	void Update();

	double DeltaTime() const;
	double FPS() const;
	double TotalTime() const;

	void Pause();
	void UnPause();

	bool IsPause() const { return m_isPause; }

private:

	double m_secPerFrame; //���� �� ��
	double m_deltaTime;
	double m_totalTime;
	double m_pauseTime;

	__int64 m_preFrame; //���� ����
	__int64 m_curFrame; //���� ����
	__int64 m_prePause; //���� ����
	__int64 m_curPause; //���� ����
	__int64 m_framePerSec; // �ʴ� ����

	bool m_isPause;
};

