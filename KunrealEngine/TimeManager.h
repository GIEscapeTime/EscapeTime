#pragma once
#include "CommonHeader.h"
#include <chrono>

/// <summary>
/// �ð��� �����ϴ� Ŭ����
/// 
/// QueryPerformanceCounter�� �̿��� ������� chrono ���̺귯���� �̿��� ����� �����
/// -> QPC�� ���� ���е��� ���������� �÷����� ���� ��� ����� �޶� �̽ļ��� ������
/// => chrono������ QPC�� ����� ���� ������ ���� �����̶� �� �� �ִ�
/// 
/// �� ������ ����ȣȯ�� �ƴϰ�, ���� ���α׷��� �ٷ�� ��쿡�� QPC�� ������ ���� �ִ�
/// </summary>

namespace KunrealEngine
{
	class TimeManager
	{
	private:
		TimeManager();
		~TimeManager();

	//�̱���
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

	//Ŭ���̾�Ʈ�� �Ѱ��� �Լ���
	public:
		float GetDeltaTime();
		void SetTimeScale(float scale);
		float GetTimeScale();
	private:
		std::chrono::high_resolution_clock::time_point m_startTime;		//Ÿ�̸��� ���۽ð�
		std::chrono::high_resolution_clock::time_point m_prevTime;		//
		std::chrono::high_resolution_clock::time_point m_currentTime;	//��ŸŸ���� ����ϱ� ����, �ð� �� ����� ���� ������

		double m_deltaTime;			//��ŸŸ��
		float m_timeScale;			//Ÿ�ӽ����� Ȥ�� ������Ʈ�� ������ �帣�� �ð��� �ٸ��� �ְ� ���� ���� ������									
	};
}

