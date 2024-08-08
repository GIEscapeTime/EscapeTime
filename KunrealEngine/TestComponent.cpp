#include "TestComponent.h"
#include "ComponentHeaders.h"
#include "GameObject.h"
#include <windows.h>
#include "InputManager.h"

KunrealEngine::InputManager& imManager = KunrealEngine::InputManager::GetInstance();

KunrealEngine::TestComponent::TestComponent()
	:m_transform(nullptr)
{

}

KunrealEngine::TestComponent::~TestComponent()
{

}

void KunrealEngine::TestComponent::Initialize()
{
	m_transform = GetOwner()->GetComponent<KunrealEngine::Transform>();
}

void KunrealEngine::TestComponent::Finalize()
{

}

void KunrealEngine::TestComponent::FixedUpdate()
{

}

void KunrealEngine::TestComponent::Update()
{
	//if (GetAsyncKeyState('W') & 0x8000) m_transform->Translate(0.1f, 0.f, 0.f);
	//if (GetAsyncKeyState('W') & 0x8000) m_transform->SetPosition(0.1f, 0.f, 0.f);
	if (imManager.OnceKeyInput('W') && m_transform->GetPosition().x < 0.1)
	{
		m_transform->SetRotation(0, 90, 0);
		m_transform->Translate(0.1f, 0.f, 0.f);
	}

	if (imManager.OnceKeyInput('S') && m_transform->GetPosition().x > -0.1)
	{
		m_transform->SetRotation(0, -90, 0);
		m_transform->Translate(-0.1f, 0.f, 0.f);
	}

	if (imManager.OnceKeyInput('A') && m_transform->GetPosition().z < 0.1)
	{
		m_transform->SetRotation(0, 0, 0);
		m_transform->Translate(0.f, 0.f, 0.1f);
	}

	if (imManager.OnceKeyInput('D') && m_transform->GetPosition().z > -0.1)
	{
		m_transform->SetRotation(0, 180, 0);
		m_transform->Translate(0.f, 0.f, -0.1f);
	}
	//if (GetAsyncKeyState('S') & 0x8000) m_transform->SetPosition();
	//if (GetAsyncKeyState('A') & 0x8000) m_transform->SetPosition();
	//if (GetAsyncKeyState('D') & 0x8000) m_transform->SetPosition();
}

void KunrealEngine::TestComponent::LateUpdate()
{

}
