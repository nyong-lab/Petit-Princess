#pragma once

/// <summary>
/// 타이머
/// 게임엔진의 시간을 담당한다
/// 작성 : 유승운
/// 수정 : 김동일 - gui에서 pause 관리 가능
/// </summary>
class Timer
{
public:
	Timer();
	~Timer();

	//복사 - 금지
	Timer(const Timer& timer) = delete;
	Timer& operator=(const Timer& timer) = delete;

	//이동 - 금지
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

	double m_secPerFrame; //갯수 당 초
	double m_deltaTime;
	double m_totalTime;
	double m_pauseTime;

	__int64 m_preFrame; //이전 갯수
	__int64 m_curFrame; //현재 갯수
	__int64 m_prePause; //현재 갯수
	__int64 m_curPause; //현재 갯수
	__int64 m_framePerSec; // 초당 갯수

	bool m_isPause;
};

