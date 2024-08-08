#include "TimeManager.h"

KunrealEngine::TimeManager::TimeManager()
	:m_startTime(std::chrono::high_resolution_clock::now()), m_prevTime(std::chrono::high_resolution_clock::now()), m_deltaTime(0.0), m_timeScale(1.0f)
{

}

KunrealEngine::TimeManager::~TimeManager()
{

}

void KunrealEngine::TimeManager::Initialize()
{

}

void KunrealEngine::TimeManager::Finalize()
{

}

//���������� ��ŸŸ�� �� �ʿ��� �κе��� ������Ʈ
void KunrealEngine::TimeManager::Update()
{
	///������Ʈ�� ���ư��鼭 currentTime�� ��� ������Ʈ
	///prevTime�� ���� ������Ʈ�� �ð��� ���̸� ����ؼ� ��ŸŸ�� ����
	///�׸��� prevTime�� currentTime�� ������Ʈ
	m_currentTime = std::chrono::high_resolution_clock::now();
	m_deltaTime = std::chrono::duration<double>(m_currentTime - m_prevTime).count() * m_timeScale;
	m_prevTime = m_currentTime;
}

//��ŸŸ���� �������ִ� �Լ�
float KunrealEngine::TimeManager::GetDeltaTime()
{
	return m_deltaTime;
}

//Ÿ�ӽ������� �����ϴ� �Լ�	0���ϴ� �Ұ���
void KunrealEngine::TimeManager::SetTimeScale(float scale)
{
	if (scale <= 0)
	{
		return;
	}

	m_timeScale = scale;
}

//�� �Լ��� ���� ������Ʈ�� ���� �ٸ� Ÿ�ӽ������� ������ �� �� ����
float KunrealEngine::TimeManager::GetTimeScale()
{
	return m_timeScale;
}
