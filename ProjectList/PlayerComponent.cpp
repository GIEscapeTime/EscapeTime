#include "PlayerComponent.h"
#include "SoundManager.h"

GameClient::PlayerComponent::PlayerComponent()
	: m_rope(false), m_live(true), m_glove(false), m_ropeSurport(0), m_useRopeSurport(0), m_Sheild(false)
	, m_getGlove(false), m_getSheild(false)
{

}

GameClient::PlayerComponent::~PlayerComponent()
{

}

void GameClient::PlayerComponent::Initialize()
{
	m_useRopeSurport = 0;
	m_rope = false;
	m_live = true;
	m_glove = false;
	m_ropeSurport = 0;
	m_Sheild = false;
	m_getGlove = false;
	m_getSheild = false;

}

void GameClient::PlayerComponent::Finalize()
{

}

void GameClient::PlayerComponent::FixedUpdate()
{

}

void GameClient::PlayerComponent::LateUpdate()
{

}

void GameClient::PlayerComponent::PlayerRelive()
{
	m_Sheild = false;
	m_glove = false;
	m_ropeSurport = 0;
	//m_live = true;
}

int GameClient::PlayerComponent::UseRopeSurport()
{
	m_ropeSurport--;	
	m_useRopeSurport++;
	KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Player_Rope_Supporter.wav");

	return m_useRopeSurport;
}

void GameClient::PlayerComponent::UseRope()
{

}

// 
// void GameClient::PlayerComponent::PlayerDie()
// {
// 	m_live = false;
// }

void GameClient::PlayerComponent::GetSheild()
{
	KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Item_Get.wav");
	m_getSheild = true;
}

void GameClient::PlayerComponent::GetRope()
{
	KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Item_Get.wav");
	m_rope = true;
}

void GameClient::PlayerComponent::GetGlove()
{
	KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Item_Get.wav");
	m_getGlove = true;
}

bool GameClient::PlayerComponent::GetRopeSurport()
{
	if (m_ropeSurport < 4)
	{
		KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Item_Get.wav");
		m_ropeSurport++;
		return true;
	}
	return false;
}

void GameClient::PlayerComponent::SetRopeSurport(KunrealEngine::GameObject* ropeSurport)
{
	m_vRopeSurport.push_back(ropeSurport);
}

void GameClient::PlayerComponent::Update()
{

}
