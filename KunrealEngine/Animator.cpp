#include "Animator.h"
#include "TimeManager.h"

KunrealEngine::Animator::Animator()
{

}

KunrealEngine::Animator::~Animator()
{

}

void KunrealEngine::Animator::Initialize()
{

}

void KunrealEngine::Animator::Finalize()
{

}

void KunrealEngine::Animator::FixedUpdate()
{

}

void KunrealEngine::Animator::Update()
{
	m_updateTime += KunrealEngine::TimeManager::GetInstance().GetDeltaTime();

	//���� ���⿡ �ִϸ��̼��� ��µ� �� 
}

void KunrealEngine::Animator::LateUpdate()
{

}
