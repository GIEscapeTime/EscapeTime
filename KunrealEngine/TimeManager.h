#pragma once
#include "CommonHeader.h"
#include <chrono>

/// <summary>
/// 시간을 관리하는 클래스
/// 
/// QueryPerformanceCounter를 이용한 방법보다 chrono 라이브러리를 이용한 방법이 권장됨
/// -> QPC는 높은 정밀도를 제공하지만 플랫폼에 따라 사용 방법이 달라 이식성이 떨어짐
/// => chrono에서도 QPC를 사용할 수도 있으니 상위 버전이라 볼 수 있다
/// 
/// 단 무조건 상위호환은 아니고, 구식 프로그램을 다루는 경우에는 QPC가 유리할 수도 있다
/// </summary>

namespace KunrealEngine
{
	class TimeManager
	{
	private:
		TimeManager();
		~TimeManager();

	//싱글톤
	public:
		static TimeManager& GetInstance()
		{
			static TimeManager m_instance;
			return m_instance;
		}

	public:
		void Initialize();
		void Finalize();
		void Update();

	//클라이언트에 넘겨줄 함수들
	public:
		float GetDeltaTime();
		void SetTimeScale(float scale);
		float GetTimeScale();
	private:
		std::chrono::high_resolution_clock::time_point m_startTime;		//타이머의 시작시간
		std::chrono::high_resolution_clock::time_point m_prevTime;		//
		std::chrono::high_resolution_clock::time_point m_currentTime;	//델타타임을 계산하기 위해, 시간 차 계산을 위한 변수들

		double m_deltaTime;			//델타타임
		float m_timeScale;			//타임스케일 혹시 오브젝트나 씬마다 흐르는 시간을 다르게 주고 싶을 수도 있으니									
	};
}

