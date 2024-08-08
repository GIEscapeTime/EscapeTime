#include <windows.h>

#include "GameTimer.h"

GameClient::GameTimer::GameTimer()
	: m_secondsPerCount(0.f), m_deltaTime(0.f), m_baseTime(0),
	m_pausedTime(0), m_prevTime(0), m_stopTime(0), m_currtime(0), m_stopped(false)
{
	long long countsPerSec = 0;

	// win api 의 시스템 타이머의 주파수 측정하고 값을 저장
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);

	m_secondsPerCount = 1.f / (double)countsPerSec;
}

float GameClient::GameTimer::TotalTime() const
{
	if (m_stopped)
	{
		return (float)(((m_stopTime - m_pausedTime) - m_baseTime) * m_secondsPerCount);
	}
	else
	{
		return(float)(((m_currtime - m_pausedTime) - m_baseTime) * m_secondsPerCount);
	}
}

float GameClient::GameTimer::DeltaTime() const
{
	return (float)m_deltaTime;
}

void GameClient::GameTimer::Reset()
{
	long long currTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	m_baseTime = currTime;
	m_prevTime = currTime;
	m_stopTime = 0;
	m_stopTime = false;

}

void GameClient::GameTimer::Start()
{
	long long startTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if (m_stopped)
	{
		m_pausedTime += (startTime - m_stopTime);

		m_prevTime = startTime;
		m_stopTime = 0;
		m_stopped = false;
	}
}

void GameClient::GameTimer::Stop()
{
	if (!m_stopped)
	{
		long long currTime = 0;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		m_stopTime = currTime;
		m_stopped = true;
	}
}

void GameClient::GameTimer::Tick()
{
	if (m_stopped)
	{
		m_deltaTime = 0.f;
		return;
	}

	long long currTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	m_currtime = currTime;
	m_deltaTime = (m_currtime - m_prevTime) * m_secondsPerCount;
	m_prevTime = m_currtime;

	// 23.07.04 이런 경우가 있을 수 있나?
	if (m_deltaTime < 0.f)
	{
		m_deltaTime = 0.f;
	}

}
