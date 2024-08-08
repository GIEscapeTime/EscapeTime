#pragma once

/// <summary>
/// 23.07.21 주희용
/// </summary>
namespace GameClient
{
	class GameTimer
	{
	public:
		GameTimer();

		float TotalTime()const;			    // 총합 시간 계산 후 return
		float DeltaTime()const;			    // 델타 타임 return

		void Reset();					    // 초기화
		void Start();					    // 일시 정지 후 시작
		void Stop();					    // 일시 정지
		void Tick();					    // 한 프레임을 기록

	private:
		double m_secondsPerCount;			// 시간에 곱하면 second 가 나온다
		double m_deltaTime;					// 한 프레임 동안의 시간

		long long m_baseTime;				// 타이머 시작한 순간의 시간
		long long m_stopTime;				// 타이머가 멈춘 순간의 시간
		long long m_prevTime;				// 이전 프레임 측정 순간의 시간
		long long m_currtime;				// 타이머의 현재 순간의 시간
		long long m_pausedTime;				// 이전에 정지되고 경과된 시간

		bool m_stopped;

	};


}
