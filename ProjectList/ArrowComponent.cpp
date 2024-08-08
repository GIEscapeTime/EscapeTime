#include "ArrowComponent.h"

GameClient::ArrowComponent::ArrowComponent()
	: m_mapX(0), m_mapY(0), m_moveX(0), m_moveY(0), m_currentStageNumber(0)
{

}

GameClient::ArrowComponent::~ArrowComponent()
{

}

void GameClient::ArrowComponent::Initialize()
{
	
}

void GameClient::ArrowComponent::Finalize()
{

}

void GameClient::ArrowComponent::FixedUpdate()
{

}

void GameClient::ArrowComponent::Update()
{
	//0.8f - ((float)m_mapX) * 0.15, 0.05f, 0.85f - ((float)m_mapY) * 0.15f
	this->GetOwner()->GetComponent <KunrealEngine::Transform>()->SetPosition(1.055f - ((float)m_mapX) * 0.195f, 0.1f, 1.1f - ((float)m_mapY) * 0.195f);
}

void GameClient::ArrowComponent::LateUpdate()
{

}

void GameClient::ArrowComponent::SettingArrow(int moveX, int moveY, int& stageNumber)
{
	m_moveX = moveX;
	m_moveY = moveY;
	m_currentStageNumber = stageNumber;
}

void GameClient::ArrowComponent::SetPosition(int posX, int posY)
{
	m_mapX = posX;
	m_mapY = posY;
}

void GameClient::ArrowComponent::MoveForward()
{
	switch (m_currentStageNumber)
	{
		case 1:
			if (m_mapX == 0 || m_mapX == 12 || m_mapY == 0 || m_mapY == 12)
			{
				m_mapX += m_moveX;
				m_mapY += m_moveY;
			}
			else
			{
				m_mapX += m_moveX;
				m_mapX += m_moveX;
				m_mapY += m_moveY;
				m_mapY += m_moveY;
			}
			break;
		case 2:
			if (m_mapX == 0 || m_mapX == 12 || m_mapY == 0 || m_mapY == 12)
			{
				m_mapX += m_moveX;
				m_mapY += m_moveY;
			}
			else
			{
				m_mapX += m_moveX;
				m_mapX += m_moveX;
				m_mapY += m_moveY;
				m_mapY += m_moveY;
			}
			break;
		case 3:
			if (m_mapX == 0 || m_mapX == 17 || m_mapY == 0 || m_mapY == 17)
			{
				m_mapX += m_moveX;
				m_mapY += m_moveY;
			}
			else
			{
				m_mapX += m_moveX;
				m_mapX += m_moveX;
				m_mapY += m_moveY;
				m_mapY += m_moveY;
			}
			break;
		default:
			break;
	}
	
}

int GameClient::ArrowComponent::ReturnPositionX()
{
	return m_mapX;
}

int GameClient::ArrowComponent::ReturnPositionY()
{
	return m_mapY;
}
