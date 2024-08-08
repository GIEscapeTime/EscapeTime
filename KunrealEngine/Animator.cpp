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

	//이후 여기에 애니메이션이 출력될 때 
}

void KunrealEngine::Animator::LateUpdate()
{

}
