#include "RockComponent.h"

GameClient::RockComponent::RockComponent()
	: m_boolTouch(false), m_mapX(0), m_mapY(0), m_heightPoint(0.f)
{

}

GameClient::RockComponent::~RockComponent()
{

}

void GameClient::RockComponent::Initialize()
{

}

void GameClient::RockComponent::Finalize()
{

}

void GameClient::RockComponent::FixedUpdate()
{

}

void GameClient::RockComponent::Update()
{
	//1.055f - ((float)m_mapX) * 0.195f, 0.05f+m_heightPoint, 1.1f - ((float)m_mapY) * 0.195f
	this->GetOwner()->GetComponent <KunrealEngine::Transform>()->SetPosition(1.055f - ((float)m_mapX) * 0.195f, 0.005f+m_heightPoint, 1.1f - ((float)m_mapY) * 0.195f);
}

void GameClient::RockComponent::LateUpdate()
{

}

void GameClient::RockComponent::SetPosition(int posX, int posY)
{
	// 3스테이지는 다르게 받을 예정
	m_mapX = posX;
	m_mapY = posY;
	m_heightPoint = 0;
}

bool GameClient::RockComponent::ComparePosition(int posX, int posY)
{
	if (posX == m_mapX && posY == m_mapY)
	{
		return true;
	}
	return false;
}

void GameClient::RockComponent::AtWater(int posX, int posY)
{
	SetPosition(posX, posY);
	m_heightPoint = -0.25f;
}
