#pragma once

/// <summary>
/// 23.07.21 �����
/// </summary>
namespace GameClient
{
	class GameTimer
	{
	public:
		GameTimer();

		float TotalTime()const;			    // ���� �ð� ��� �� return
		float DeltaTime()const;			    // ��Ÿ Ÿ�� return

		void Reset();					    // �ʱ�ȭ
		void Start();					    // �Ͻ� ���� �� ����
		void Stop();					    // �Ͻ� ����
		void Tick();					    // �� �������� ���

	private:
		double m_secondsPerCount;			// �ð��� ���ϸ� second �� ���´�
		double m_deltaTime;					// �� ������ ������ �ð�

		long long m_baseTime;				// Ÿ�̸� ������ ������ �ð�
		long long m_stopTime;				// Ÿ�̸Ӱ� ���� ������ �ð�
		long long m_prevTime;				// ���� ������ ���� ������ �ð�
		long long m_currtime;				// Ÿ�̸��� ���� ������ �ð�
		long long m_pausedTime;				// ������ �����ǰ� ����� �ð�

		bool m_stopped;

	};


}
