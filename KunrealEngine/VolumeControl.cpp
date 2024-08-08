#include "VolumeControl.h"
#include "SoundManager.h"
#include "InputManager.h"
#include "GameObject.h"
#include "Button.h"
#include <windows.h>

KunrealEngine::SoundManager& smInstance = KunrealEngine::SoundManager::GetInstance();
KunrealEngine::InputManager& ipInstance = KunrealEngine::InputManager::GetInstance();

KunrealEngine::VolumeControl::VolumeControl()
	:m_minCnt(0.1f), m_maxCnt(1.0f), m_currentVolume(1.0f), m_increaseAmount(0.1f), m_transform(nullptr), m_UIMovement(50.f),
	m_isBG(true), m_isSFX(false), m_button(nullptr)
	//m_low(false), m_medium(false), m_high(true)
{

}

//생성자에 문자열로 어떤 사운드를 조절할 컴포넌트인지 정해준다
//어떻게 써야하는지는 내부의 if문 참고
//말도 안되는 문자열로 탈선하면 아무것도 못하게 할거임
KunrealEngine::VolumeControl::VolumeControl(std::string soundType)
	:m_minCnt(0.1f), m_maxCnt(1.0f), m_currentVolume(1.0f), m_increaseAmount(0.1f), m_transform(nullptr), m_UIMovement(50.f),
	m_isBG(false), m_isSFX(false), m_button(nullptr)
	//m_low(false), m_medium(false), m_high(true)
{
// 	if (soundType == "BG" || soundType == "BACKGROUND" || soundType == "BackGround" || soundType == "Background")
// 	{
// 		m_isBG = true;
// 		m_isSFX = false;
// 	}
// 	else if (soundType == "SFX")
// 	{
// 		m_isBG = false;
// 		m_isSFX = true;
// 	}
// 	else
// 	{
// 		//아무것도 안돼
// 		m_isBG = false;
// 		m_isSFX = false;
// 	}
}

KunrealEngine::VolumeControl::~VolumeControl()
{

}

void KunrealEngine::VolumeControl::Initialize()
{
// 	m_transform = GetOwner()->GetComponent<KunrealEngine::Transform>();
// 	m_button = GetOwner()->GetComponent<KunrealEngine::Button>();
}

void KunrealEngine::VolumeControl::Finalize()
{

}

void KunrealEngine::VolumeControl::FixedUpdate()
{

}

void KunrealEngine::VolumeControl::Update()
{
// 	CheckInput();
// 	MoveButton();
// 	UpdateVolume();
}

void KunrealEngine::VolumeControl::LateUpdate()
{

}

void KunrealEngine::VolumeControl::SetBG()
{
// 	m_isBG = true;
// 	m_isSFX = false;
}

void KunrealEngine::VolumeControl::SetSFX()
{
// {
// 	m_isBG = false;
// 	m_isSFX = true;
}

//볼륨 조절 버튼을 움직이게
void KunrealEngine::VolumeControl::MoveButton()
{

// {
// 	if (m_low)
// 	{
// 		m_currentVolume = 0.0f;
// 
// 		if (m_isBG)
// 		{
// 			m_transform->SetUIPosition(610, 380);
// 		}
// 		else if (m_isSFX)
// 		{
// 			m_transform->SetUIPosition(610, 490);
// 		}
// 	}
// 	else if (m_medium)
// 	{
// 		m_currentVolume = 0.5f;
// 
// 		if (m_isBG)
// 		{
// 			m_transform->SetUIPosition(860, 380);
// 		}
// 		else if (m_isSFX)
// 		{
// 			m_transform->SetUIPosition(860, 490);
// 		}
// 	}
// 	else if (m_high)
// 	{
// 		m_currentVolume = 1.0f;
// 
// 		if (m_isBG)
// 		{
// 			m_transform->SetUIPosition(1110, 380);
// 		}
// 		else if (m_isSFX)
// 		{
// 			m_transform->SetUIPosition(1110, 490);
// 		}
// 	}
	
}

void KunrealEngine::VolumeControl::CheckInput()
{
// 	if (GetOwner()->GetActivated())
// 	{
// 		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
// 		{
// 			if (m_low)
// 			{
// 
// 			}
// 			else if (m_medium)
// 			{
// 				m_low = true;
// 				m_medium = false;
// 				m_high = false;
// 			}
// 			else if (m_high)
// 			{
// 				m_low = false;
// 				m_medium = true;
// 				m_high = false;
// 			}
// 		}
// 
// 		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
// 		{
// 			if (m_low)
// 			{
// 				m_low = false;
// 				m_medium = true;
// 				m_high = false;
// 			}
// 			else if (m_medium)
// 			{
// 				m_low = false;
// 				m_medium = false;
// 				m_high = true;
// 			}
// 			else if (m_high)
// 			{
// 
// 			}
// 		}
// 	}
}

void KunrealEngine::VolumeControl::ButtonClicked()
{
// 	if (m_button->OnLeftClick())
// 	{
// 		if (m_low)
// 		{
// 			m_low = false;
// 			m_medium = true;
// 			m_high = false;
// 		}
// 		else if (m_medium)
// 		{
// 			m_low = false;
// 			m_medium = false;
// 			m_high = true;
// 		}
// 		else if (m_high)
// 		{
// 			m_low = false;
// 			m_medium = true;
// 			m_high = false;
// 		}
// 	}
}

//버튼의 포지션이 변경되었을 때 그에 맞게 사운드를 조절
void KunrealEngine::VolumeControl::UpdateVolume()
{
// 	if (m_isBG == true && m_isSFX == false)
// 	{
// 		smInstance.SoundVolumeSetting(KunrealEngine::SoundManager::BACKGROUNDSOUND, m_currentVolume);
// 	}
// 	else if (m_isBG == false && m_isSFX == true)
// 	{
// 		smInstance.SoundVolumeSetting(KunrealEngine::SoundManager::EFFECTSOUND, m_currentVolume);
// 	}
}
