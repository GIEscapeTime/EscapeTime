#include "ComponentHeaders.h"
#include "GameObject.h"
#include "Camera.h"
#include "InputManager.h"
#include <windows.h>

KunrealEngine::ObjectCamera::ObjectCamera()
	:m_camera(nullptr), m_transform(nullptr)
{

}

KunrealEngine::ObjectCamera::~ObjectCamera()
{

}

void KunrealEngine::ObjectCamera::Initialize()
{
	//부모 오브젝트가 가진 트랜스폼 컴포넌트를 가져옴
	m_transform = GetOwner()->GetComponent<KunrealEngine::Transform>();
	
	m_camera = new Camera();
	m_camera->LookAt(DirectX::XMFLOAT3(-1.f, 2.45f, 1.f), DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT3(0.f, 1.f, 0.f));
	m_camera->SetPosition(-0.73f, 2.3f, 0.73f);
	
}

void KunrealEngine::ObjectCamera::Finalize()
{

}

void KunrealEngine::ObjectCamera::FixedUpdate()
{

}

void KunrealEngine::ObjectCamera::Update()
{
	// 카메라 이동함수
	//if (GetAsyncKeyState('W') & 0x8000) m_camera->Walk(10.0f * 0.001f);
	//if (GetAsyncKeyState('S') & 0x8000) m_camera->Walk(-10.0f * 0.001f);
	//if (GetAsyncKeyState('A') & 0x8000) m_camera->Strafe(-10.0f * 0.001f);
	//if (GetAsyncKeyState('D') & 0x8000) m_camera->Strafe(10.0f * 0.001f);
	//if (GetAsyncKeyState('Q') & 0x8000) m_camera->WorldUpDown(-10.0f * 0.001f);
	//if (GetAsyncKeyState('E') & 0x8000) m_camera->WorldUpDown(10.0f * 0.001f);
	//
	//if (GetAsyncKeyState(VK_UP) & 0x8000) m_camera->Pitch(-10.f * 0.001f);
	//if (GetAsyncKeyState(VK_DOWN) & 0x8000) m_camera->Pitch(10.f * 0.001f);
	//if (GetAsyncKeyState(VK_LEFT) & 0x8000) m_camera->RotateY(-10.f * 0.001f);
	//if (GetAsyncKeyState(VK_RIGHT) & 0x8000) m_camera->RotateY(10.1f * 0.001f);

// 	if(InputManager::GetInstance().OnceKeyInput(W_KEYCODE))
// 	{
// 		m_camera->SetPosition(m_camera->GetPosition().x + 0.1f, m_camera->GetPosition().y, m_camera->GetPosition().z);
// 	}
// 	if (InputManager::GetInstance().OnceKeyInput(S_KEYCODE))
// 	{
// 		m_camera->SetPosition(m_camera->GetPosition().x - 0.1f, m_camera->GetPosition().y, m_camera->GetPosition().z);
// 	}
// 	if (InputManager::GetInstance().OnceKeyInput(A_KEYCODE))
// 	{
// 		m_camera->SetPosition(m_camera->GetPosition().x, m_camera->GetPosition().y + 0.1f, m_camera->GetPosition().z);
// 	}
// 	if (InputManager::GetInstance().OnceKeyInput(D_KEYCODE))
// 	{
// 		m_camera->SetPosition(m_camera->GetPosition().x, m_camera->GetPosition().y - 0.1f, m_camera->GetPosition().z);
// 	}
// 	if (InputManager::GetInstance().OnceKeyInput(Q_KEYCODE))
// 	{
// 		m_camera->SetPosition(m_camera->GetPosition().x + 0.5f, m_camera->GetPosition().y - 0.5f, m_camera->GetPosition().z + 0.5f);
// 	}
// 	if (InputManager::GetInstance().OnceKeyInput(E_KEYCODE))
// 	{
// 		m_camera->SetPosition(m_camera->GetPosition().x - 0.5f, m_camera->GetPosition().y + 0.5f, m_camera->GetPosition().z - 0.5f);
// 	}

	//if (GetAsyncKeyState('W') & 0x8000)
	//{
	//	m_camera->m_Position.x + 0.1f;
	//}
	//if (GetAsyncKeyState('S') & 0x8000)
	//{
	//	m_camera->m_Position.x - 0.1f;
	//}
	//if (GetAsyncKeyState('A') & 0x8000) m_camera->m_Position.y + 0.1f;
	//if (GetAsyncKeyState('D') & 0x8000) m_camera->m_Position.y + 0.1f;
	//if (GetAsyncKeyState('Q') & 0x8000) m_camera->m_Position.z + 0.1f;
	//if (GetAsyncKeyState('E') & 0x8000) m_camera->m_Position.z + 0.1f;
}

void KunrealEngine::ObjectCamera::LateUpdate()
{
	m_camera->UpdateViewMatrix();
}

KunrealEngine::Camera* KunrealEngine::ObjectCamera::GetCamera()
{
	return m_camera;
}
