#include "GameEngine_pch.h"
#include "Timer.h"
#include <windows.h> //QureyPerformanceCounte ���� �ʿ�


//������
Timer::Timer() :
	m_secPerFrame(0), m_deltaTime(0), m_pauseTime(0), m_totalTime(0),
	m_preFrame(0), m_curFrame(0), m_prePause(0), m_curPause(0), m_framePerSec(0),
	m_isPause(false)
{

}

//�Ҹ���
Timer::~Timer()
{

}

//Ÿ�̸� �ʱ�ȭ
void Timer::Initialize()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_preFrame);
	QueryPerformanceCounter((LARGE_INTEGER*)&m_curFrame);

	QueryPerformanceFrequency((LARGE_INTEGER*)&m_framePerSec);

	//������ ��
	m_secPerFrame = 1.0 / static_cast<double>(m_framePerSec);
}

//�ð��� ����� �����ؾ��Ѵ�
void Timer::Update()
{
	if (m_isPause)
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&m_curPause);
		double delta = m_curPause - m_prePause;
		m_pauseTime += (delta * m_secPerFrame);
		m_prePause = m_curPause;
		return;
	}

	QueryPerformanceCounter((LARGE_INTEGER*)&m_curFrame);

	m_deltaTime = static_cast<double>(m_curFrame - m_preFrame) * m_secPerFrame;
	m_totalTime += m_deltaTime;

	static double time = 0;
	time += m_deltaTime;

	static int count = 0;

	if (time > 1)
	{
		time  -= 1;

		m_framePerSec = count;
		count = 0;
	}

	count++;

	m_preFrame = m_curFrame;


}

//(���� ���� - ���� ����) * ������ �� = delta time
double Timer::DeltaTime() const
{
	return m_deltaTime;
}

double Timer::FPS() const
{
	return static_cast<double>(m_framePerSec);
}

double Timer::TotalTime() const
{
	if (m_isPause)
	{
		return m_totalTime;
	}

	return m_totalTime - m_pauseTime;
}

void Timer::Pause()
{
	m_isPause = true;
	m_prePause = m_curFrame;
}

void Timer::UnPause()
{
	m_isPause = false;

	m_curFrame = m_curPause;
	m_preFrame = m_prePause;

	m_pauseTime = 0;
}
