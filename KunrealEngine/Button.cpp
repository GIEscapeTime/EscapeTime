#include "InputManager.h"
#include "ComponentHeaders.h"
#include "GameObject.h"
#include "Animator.h"

KunrealEngine::InputManager& ipInstance = KunrealEngine::InputManager::GetInstance();

KunrealEngine::Button::Button()
	:m_width(100), m_height(100), m_transform(nullptr),
	m_top(0), m_bottom(0), m_left(0), m_right(0)
{

}

KunrealEngine::Button::~Button()
{

}

void KunrealEngine::Button::Initialize()
{
	m_transform = GetOwner()->GetComponent<KunrealEngine::Transform>();
}

void KunrealEngine::Button::Finalize()
{

}

void KunrealEngine::Button::FixedUpdate()
{

}

void KunrealEngine::Button::Update()
{
	OnLeftClick();
	OnRightClick();
}

void KunrealEngine::Button::LateUpdate()
{

}

//��Ŭ��
bool KunrealEngine::Button::OnLeftClick()
{
	if (ipInstance.MouseLeftInput()
		&& ipInstance.ReturnMouseState().x >= m_left
		&& ipInstance.ReturnMouseState().x <= m_right
		&& ipInstance.ReturnMouseState().y <= m_top
		&& ipInstance.ReturnMouseState().y >= m_bottom
		)
	{
		return true;
	}
// 	else if (ipInstance.MouseLeftInput())
// 	{
// 		int a = 10;
// 		return false;
// 	}
	else
	{
		return false;
	}
}

//��Ŭ�� ���� ��
bool KunrealEngine::Button::OnLeftClickUp()
{
	return false;
}

//��Ŭ�� Ȧ��
bool KunrealEngine::Button::OnLeftHold()
{
	return false;
}

//��Ŭ��
bool KunrealEngine::Button::OnRightClick()
{
	if (ipInstance.MouseRightInput()
		&& ipInstance.ReturnMouseState().x >= m_left
		&& ipInstance.ReturnMouseState().x <= m_right
		&& ipInstance.ReturnMouseState().y <= m_top
		&& ipInstance.ReturnMouseState().y >= m_bottom
		)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//��Ŭ�� ���� ��
bool KunrealEngine::Button::OnRightClickUp()
{
	return false;
}

//��Ŭ�� Ȧ��
bool KunrealEngine::Button::OnRightHold()
{
	return false;
}

//��ư ũ�� ����
//���� ũ�Ⱑ �ƴ϶� ���õǴ� ����
void KunrealEngine::Button::SetSize(float width, float height)
{
	m_width = width;
	m_height = height;

	m_left = m_transform->GetUIPosition().x - (width / 2);
	m_right = m_transform->GetUIPosition().x + (width / 2);
	m_top = m_transform->GetUIPosition().y + (height / 2);
	m_bottom = m_transform->GetUIPosition().y - (height / 2);
}
