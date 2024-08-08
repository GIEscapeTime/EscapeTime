#include <windows.h>

#include "RockComponent.h"
#include "PlayerComponent.h"
#include "ArrowComponent.h"
#include "MapMatrix.h"
#include "MonsterAi.h" 
#include "SoundManager.h"
#include "Light.h"



GameClient::MapMatrix::MapMatrix(std::string sceneName)
	: m_currentMapX(10), m_currentMapY(1), m_currentMapNumber(1), m_playerLife(true), m_sceneName(sceneName), m_turnChanger(true), m_mapChange(false)
	, m_ropeSurport_1(false), m_ropeSurport_2(false), m_ropeSurport_3(false), m_playerView(4)
{

}

GameClient::MapMatrix::~MapMatrix()
{
	// 	delete[] m_mapXYStatus1;
	// 	delete[] m_mapXYStatus2;
	// 	delete[] m_bossMapXYStatus;
}

void GameClient::MapMatrix::Initialize()
{
	// 맵 외벽 생성
	for (int i = 0; i < 12; i++)
	{
		m_mapXYStatus1[0][i] = MapStatus::WALL;
		m_mapXYStatus1[11][i] = MapStatus::WALL;
		m_mapXYStatus1[i][0] = MapStatus::WALL;
		m_mapXYStatus1[i][11] = MapStatus::WALL;

		m_mapXYStatus2[0][i] = MapStatus::WALL;
		m_mapXYStatus2[11][i] = MapStatus::WALL;
		m_mapXYStatus2[i][0] = MapStatus::WALL;
		m_mapXYStatus2[i][11] = MapStatus::WALL;
	}
	for (int i = 0; i < 17; i++)
	{
		m_mapXYStatus3[0][i] = MapStatus::WALL;
		m_mapXYStatus3[16][i] = MapStatus::WALL;
		m_mapXYStatus3[i][0] = MapStatus::WALL;
		m_mapXYStatus3[i][16] = MapStatus::WALL;
	}

	// 맵 생성
	CreateMapBase();
	SetMapBase();

	// 여러 오브젝트 생성
	CreateObjectInMap();

	// 맵배치
	MapReset();
	ResetXYObjectInMap();
	SetXYObjectInMap();
	SetEquipmentPosition();
	SetArrowPosition();
}

void GameClient::MapMatrix::ChangeMap(int mapNumber)
{
	if (m_mapChange)
	{
		if (m_currentMapNumber <= 2 && m_currentMapNumber >= 0)
		{
			m_currentMapNumber += mapNumber;
			MapReset();
			ResetXYObjectInMap();
			SetXYObjectInMap();
			SetEquipmentPosition();
			SetArrowPosition();
			SetMapBase();
			m_vPlayerBodyX.clear();
			m_vPlayerBodyY.clear();

			if (m_currentMapNumber == 2)
			{
				KunrealEngine::Light::GetInstance().DeleteLight();
				KunrealEngine::Light::GetInstance().AddLight(1.f, 0.12f, 1.f, 0.35f);
				KunrealEngine::Light::GetInstance().AddLight(1.f, 0.12f, -1.0f, 0.35f);
				KunrealEngine::Light::GetInstance().AddLight(-1.f, 0.12f, -1.f, 0.35f);
				KunrealEngine::Light::GetInstance().AddLight(-0.49f, 0.12f, -1.5f, 0.19f, 1.f, 1.f, 0.6f);
			}

			// 플레이어 위치 조정
			m_currentMapX = 10;
			m_currentMapY = 1;
			if (m_currentMapNumber == 3) 
			{
				m_currentMapX = 15;
				KunrealEngine::GetScene("GAMESCENE")->GetGameObject("CameraController")->GetComponent<KunrealEngine::CameraController>()->CheckStage3();
				KunrealEngine::Light::GetInstance().DeleteLight();
				KunrealEngine::Light::GetInstance().AddLight(1.f, 0.12f, 1.f, 0.35f);
				KunrealEngine::Light::GetInstance().AddLight(1.f, 0.12f, -0.5f, 0.35f);
				KunrealEngine::Light::GetInstance().AddLight(1.1f, 0.12f, -2.1f, 0.35f);
				KunrealEngine::Light::GetInstance().AddLight(0.0f, 0.12f, -2.0f, 0.35f);
				KunrealEngine::Light::GetInstance().AddLight(-1.0f, 0.12f, -2.0f, 0.35f);
				KunrealEngine::Light::GetInstance().AddLight(-2.0f, 0.12f, -2.0f, 0.35f);
				KunrealEngine::Light::GetInstance().AddLight(-0.50f, 0.12f, -2.5f, 0.19f, 1.f, 1.f, 0.6f);

			}
			
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->PlayerRelive();
		}
		else if (m_currentMapNumber == 3)
		{
			KunrealEngine::ChangeScene("EndingScene");
		}
		else
		{
			MessageBox(0, L"정상적이지 않은 맵 변환", L"MapMatrix.cpp", 0);
		}
	}

	if (m_currentMapNumber == 1)
	{
		KunrealEngine::Light::GetInstance().DeleteLight();
		KunrealEngine::Light::GetInstance().AddLight(1.f, 0.12f, 1.f, 0.35f);
		KunrealEngine::Light::GetInstance().AddLight(1.f, 0.12f, -0.1f, 0.35f);
		KunrealEngine::Light::GetInstance().AddLight(1.f, 0.12f, -1.f, 0.35f);
		KunrealEngine::Light::GetInstance().AddLight(-0.05f, 0.12f, -1.f, 0.35f);
		KunrealEngine::Light::GetInstance().AddLight(-1.f, 0.12f, -1.f, 0.35f);
		KunrealEngine::Light::GetInstance().AddLight(-0.49f, 0.12f, -1.5f, 0.19f, 1.f, 1.f, 0.8f);
	}

	m_mapChange = false;

}

void GameClient::MapMatrix::CurrentMapXCrease(int crease)
{
	m_currentMapX = m_currentMapX + crease;
	if (crease > 0)
	{
		m_playerView = 6;
	}
	else if (crease < 0)
	{
		m_playerView = 4;
	}
}

void GameClient::MapMatrix::CurrentMapYCrease(int crease)
{
	m_currentMapY = m_currentMapY + crease;
	if (crease > 0)
	{
		m_playerView = 8;
	}
	else if (crease < 0)
	{
		m_playerView = 2;
	}
}

void GameClient::MapMatrix::UpdatePlayerPosition()
{
	////0.8f - 7.f * 0.15f, 0.05f, 0.85f - 1.f * 0.15f
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - ((float)m_currentMapX) * 0.195f, 0.05f, 1.09f - ((float)m_currentMapY) * 0.195f);
}

bool GameClient::MapMatrix::ObjectCheck(int creaseX, int creaseY)
{
	MapStatus frontMapstatus = NORMAL;

	// 맵 정보 불러오기
	switch (m_currentMapNumber)
	{
		case 1:
			if (m_mapXYStatus1[m_currentMapX + creaseX][m_currentMapY + creaseY] != NORMAL)
			{
				frontMapstatus = m_mapXYStatus1[m_currentMapX + creaseX][m_currentMapY + creaseY];
			}
			break;
		case 2:
			if (m_mapXYStatus2[m_currentMapX + creaseX][m_currentMapY + creaseY] != NORMAL)
			{
				frontMapstatus = m_mapXYStatus2[m_currentMapX + creaseX][m_currentMapY + creaseY];
			}
			break;
		case 3:
			if (m_mapXYStatus3[m_currentMapX + creaseX][m_currentMapY + creaseY] != NORMAL)
			{
				frontMapstatus = m_mapXYStatus3[m_currentMapX + creaseX][m_currentMapY + creaseY];
			}
			break;
		default:
			return  false;
			break;
	}

	// 맵 타일 체크
	switch (frontMapstatus)
	{
		case GameClient::NORMAL:
			return true;
			break;
		case GameClient::WATER:
			m_playerLife = false;
			KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Player_Die_Water.wav");

			return true;
			break;
		case GameClient::ARROW:
			return false;
			break;
		case GameClient::TRAP:
			//m_playerLife = false;
			return true;
			break;
		case GameClient::TRAPOFF:
			m_playerLife = false;
			return true;
			break;
		case GameClient::ROPESURPORTITEM:
			GetRopeSurport(creaseX, creaseY);
			return true;
			break;
		case GameClient::Equipment:
			GetEquipment(creaseX, creaseY);
			return true;
			break;
		case GameClient::EXIT:
			KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Stage_Clear.wav");

			m_mapChange = true;
			return true;
			break;
		case GameClient::MONSTER:
			m_playerLife = false;
			return true;
			break;
		case GameClient::ROCK:
			return PushRock(creaseX, creaseY);
			break;
		case GameClient::WALL:
			return false;
			break;
		default:
			return true;
			break;
	}
	return true;
}

void GameClient::MapMatrix::PlayerDieCheck()
{
	if (!m_playerLife)
	{
		KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Player_Die.wav");

		// 맵배치
		MapReset();
		ResetXYObjectInMap();
		SetXYObjectInMap();
		SetEquipmentPosition();
		SetArrowPosition();

		// 플레이어 초기화
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<KunrealEngine::Transform>()->SetRotation(0, 0, 0);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->PlayerRelive();
		m_vPlayerBodyX.push_back(m_currentMapX);
		m_vPlayerBodyY.push_back(m_currentMapY);
		SetPlayerBody();
		m_currentMapX = 10;
		m_currentMapY = 1;
		if (m_currentMapNumber == 3) { m_currentMapX = 15; }
		m_playerLife = true;
	}
}

void GameClient::MapMatrix::CreateObjectInMap()
{
	// 1스테이지 바위 생성
	{
		CreateRockObject("Rock1_1");
		m_vRock1.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_1"));

		CreateRockObject("Rock1_2");
		m_vRock1.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_2"));

		CreateRockObject("Rock1_3");
		m_vRock1.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_3"));

		CreateRockObject("Rock1_4");
		m_vRock1.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_4"));

		CreateRockObject("Rock1_5");
		m_vRock1.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_5"));

		CreateRockObject("Rock1_6");
		m_vRock1.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_6"));

		CreateRockObject("Rock1_7");
		m_vRock1.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_7"));

		CreateRockObject("Rock1_8");
		m_vRock1.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_8"));

		CreateRockObject("Rock1_9");
		m_vRock1.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_9"));

		CreateRockObject("Rock1_10");
		m_vRock1.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_10"));

		CreateRockObject("Rock1_11");
		m_vRock1.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_11"));

		CreateRockObject("Rock1_12");
		m_vRock1.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_12"));

		CreateRockObject("Rock1_13");
		m_vRock1.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_13"));

		CreateRockObject("Rock1_14");
		m_vRock1.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_14"));

		CreateRockObject("Rock1_15");
		m_vRock1.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_15"));

		CreateRockObject("Rock1_16");
		m_vRock1.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_16"));

		CreateRockObject("Rock1_17");
		m_vRock1.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_17"));

		CreateRockObject("Rock1_18");
		m_vRock1.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_18"));

		CreateRockObject("Rock1_19");
		m_vRock1.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_19"));

		CreateRockObject("Rock1_20");
		m_vRock1.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_20"));

		CreateRockObject("Rock1_21");
		m_vRock1.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_21"));

		CreateRockObject("Rock1_22");
		m_vRock1.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_22"));
	}

	// 2스테이지 바위 생성
	{
		CreateRockObject("Rock2_1");
		m_vRock2.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock2_1"));

		CreateRockObject("Rock2_2");
		m_vRock2.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock2_2"));

		CreateRockObject("Rock2_3");
		m_vRock2.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock2_3"));

		CreateRockObject("Rock2_4");
		m_vRock2.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock2_4"));

		CreateRockObject("Rock2_5");
		m_vRock2.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock2_5"));

		CreateRockObject("Rock2_6");
		m_vRock2.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock2_6"));

		CreateRockObject("Rock2_7");
		m_vRock2.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock2_7"));

		CreateRockObject("Rock2_8");
		m_vRock2.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock2_8"));

		CreateRockObject("Rock2_9");
		m_vRock2.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock2_9"));

		CreateRockObject("Rock2_10");
		m_vRock2.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock2_10"));

		CreateRockObject("Rock2_11");
		m_vRock2.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock2_11"));
	}

	// 3스테이지 바위 생성
	{
		CreateRockObject("Rock3_1");
		m_vRock3.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_1"));

		CreateRockObject("Rock3_2");
		m_vRock3.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_2"));

		CreateRockObject("Rock3_3");
		m_vRock3.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_3"));

		CreateRockObject("Rock3_4");
		m_vRock3.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_4"));

		CreateRockObject("Rock3_5");
		m_vRock3.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_5"));

		CreateRockObject("Rock3_6");
		m_vRock3.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_6"));

		CreateRockObject("Rock3_7");
		m_vRock3.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_7"));

		CreateRockObject("Rock3_8");
		m_vRock3.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_8"));

		CreateRockObject("Rock3_9");
		m_vRock3.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_9"));

		CreateRockObject("Rock3_10");
		m_vRock3.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_10"));

		CreateRockObject("Rock3_11");
		m_vRock3.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_11"));

		CreateRockObject("Rock3_12");
		m_vRock3.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_12"));

		CreateRockObject("Rock3_13");
		m_vRock3.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_13"));

		CreateRockObject("Rock3_14");
		m_vRock3.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_14"));

		CreateRockObject("Rock3_15");
		m_vRock3.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_15"));

		CreateRockObject("Rock3_16");
		m_vRock3.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_16"));

		CreateRockObject("Rock3_17");
		m_vRock3.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_17"));

		CreateRockObject("Rock3_18");
		m_vRock3.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_18"));

		CreateRockObject("Rock3_19");
		m_vRock3.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_19"));
	}

	// 아이템 생성
	CreateEquipmentItem();

	// 트랩 오브젝트 생성
	CreateTrapObject();

	// 화살 오브젝트 생성
	CreateArrowObject();

	// 시체 생성
	CreatePlayerBody();

	// 몬스터 생성
	CreateMonsterObject();
}

void GameClient::MapMatrix::CreateTrapObject()
{
	KunrealEngine::GetScene(m_sceneName)->CreateObject("Trap2_1");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap2_1")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap2_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap2_1")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap2_1")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Thorn_Trap_Thorn");
	m_vTrap.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap2_1"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("Trap2_2");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap2_2")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap2_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap2_2")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap2_2")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Thorn_Trap_Thorn");
	m_vTrap.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap2_2"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("Trap2_3");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap2_3")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap2_3")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap2_3")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap2_3")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Thorn_Trap_Thorn");
	m_vTrap.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap2_3"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("Trap3_1");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_1")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_1")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_1")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Thorn_Trap_Thorn");
	m_vTrap.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_1"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("Trap3_2");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_2")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_2")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_2")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Thorn_Trap_Thorn");
	m_vTrap.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_2"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("Trap3_3");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_3")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_3")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_3")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_3")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Thorn_Trap_Thorn");
	m_vTrap.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_3"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("Trap3_4");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_4")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_4")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_4")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_4")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Thorn_Trap_Thorn");
	m_vTrap.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_4"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("Trap3_5");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_5")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_5")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_5")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_5")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Thorn_Trap_Thorn");
	m_vTrap.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_5"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("Trap3_6");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_6")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_6")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_6")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_6")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Thorn_Trap_Thorn");
	m_vTrap.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_6"));


	KunrealEngine::GetScene(m_sceneName)->CreateObject("UnTrap2_1");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap2_1")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap2_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap2_1")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap2_1")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Thorn_Trap");
	m_vUnTrap.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap2_1"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("UnTrap2_2");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap2_2")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap2_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap2_2")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap2_2")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Thorn_Trap");
	m_vUnTrap.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap2_2"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("UnTrap2_3");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap2_3")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap2_3")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap2_3")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap2_3")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Thorn_Trap");
	m_vUnTrap.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap2_3"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("UnTrap3_1");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_1")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_1")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_1")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Thorn_Trap");
	m_vUnTrap.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_1"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("UnTrap3_2");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_2")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_2")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_2")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Thorn_Trap");
	m_vUnTrap.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_2"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("UnTrap3_3");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_3")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_3")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_3")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_3")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Thorn_Trap");
	m_vUnTrap.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_3"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("UnTrap3_4");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_4")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_4")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_4")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_4")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Thorn_Trap");
	m_vUnTrap.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_4"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("UnTrap3_5");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_5")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_5")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_5")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_5")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Thorn_Trap");
	m_vUnTrap.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_5"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("UnTrap3_6");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_6")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_6")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_6")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_6")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Thorn_Trap");
	m_vUnTrap.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_6"));



}

void GameClient::MapMatrix::CreateArrowObject()
{
	KunrealEngine::GetScene(m_sceneName)->CreateObject("Arrow_XPlus");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XPlus")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XPlus")->GetComponent<KunrealEngine::Transform>()->SetRotation(0.f, 270.f, 0.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XPlus")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XPlus")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XPlus")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Arrow_Trap_Projectile");
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XPlus")->AddComponent<ArrowComponent>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XPlus")->GetComponent<ArrowComponent>()->SettingArrow(1, 0, m_currentMapNumber);

	KunrealEngine::GetScene(m_sceneName)->CreateObject("Arrow_XMinus");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XMinus")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XMinus")->GetComponent<KunrealEngine::Transform>()->SetRotation(0.f, 90.f, 0.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XMinus")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XMinus")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XMinus")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Arrow_Trap_Projectile");
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XMinus")->AddComponent<ArrowComponent>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XMinus")->GetComponent<ArrowComponent>()->SettingArrow(-1, 0, m_currentMapNumber);

	KunrealEngine::GetScene(m_sceneName)->CreateObject("Arrow_YPlus");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YPlus")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YPlus")->GetComponent<KunrealEngine::Transform>()->SetRotation(0.f, 180.f, 0.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YPlus")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YPlus")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YPlus")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Arrow_Trap_Projectile");
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YPlus")->AddComponent<ArrowComponent>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YPlus")->GetComponent<ArrowComponent>()->SettingArrow(0, 1, m_currentMapNumber);

	KunrealEngine::GetScene(m_sceneName)->CreateObject("Arrow_YMinus");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YMinus")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YMinus")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YMinus")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YMinus")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Arrow_Trap_Projectile");
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YMinus")->AddComponent<ArrowComponent>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YMinus")->GetComponent<ArrowComponent>()->SettingArrow(0, -1, m_currentMapNumber);
}

void GameClient::MapMatrix::SetXYObjectInMap()
{
	switch (m_currentMapNumber)
	{
		case 1:
			// 1스테이지 바위 초기 위치 셋팅
		{
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_1")->GetComponent<GameClient::RockComponent>()->SetPosition(1, 5);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_2")->GetComponent<GameClient::RockComponent>()->SetPosition(1, 7);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_3")->GetComponent<GameClient::RockComponent>()->SetPosition(1, 9);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_4")->GetComponent<GameClient::RockComponent>()->SetPosition(1, 10);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_5")->GetComponent<GameClient::RockComponent>()->SetPosition(2, 1);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_6")->GetComponent<GameClient::RockComponent>()->SetPosition(2, 2);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_7")->GetComponent<GameClient::RockComponent>()->SetPosition(2, 3);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_8")->GetComponent<GameClient::RockComponent>()->SetPosition(3, 3);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_9")->GetComponent<GameClient::RockComponent>()->SetPosition(3, 5);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_10")->GetComponent<GameClient::RockComponent>()->SetPosition(3, 6);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_11")->GetComponent<GameClient::RockComponent>()->SetPosition(3, 8);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_12")->GetComponent<GameClient::RockComponent>()->SetPosition(3, 9);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_13")->GetComponent<GameClient::RockComponent>()->SetPosition(4, 2);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_14")->GetComponent<GameClient::RockComponent>()->SetPosition(4, 3);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_15")->GetComponent<GameClient::RockComponent>()->SetPosition(4, 8);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_16")->GetComponent<GameClient::RockComponent>()->SetPosition(5, 10);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_17")->GetComponent<GameClient::RockComponent>()->SetPosition(6, 9);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_18")->GetComponent<GameClient::RockComponent>()->SetPosition(6, 10);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_19")->GetComponent<GameClient::RockComponent>()->SetPosition(7, 3);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_20")->GetComponent<GameClient::RockComponent>()->SetPosition(7, 9);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_21")->GetComponent<GameClient::RockComponent>()->SetPosition(8, 9);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock1_22")->GetComponent<GameClient::RockComponent>()->SetPosition(9, 4);

			for (auto rock : m_vRock1)
			{
				rock->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			}
		}


		break;
		case 2:
			// 2스테이지 바위 초기 위치 셋팅
		{
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock2_1")->GetComponent<GameClient::RockComponent>()->SetPosition(2, 5);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock2_2")->GetComponent<GameClient::RockComponent>()->SetPosition(3, 1);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock2_3")->GetComponent<GameClient::RockComponent>()->SetPosition(3, 7);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock2_4")->GetComponent<GameClient::RockComponent>()->SetPosition(3, 9);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock2_5")->GetComponent<GameClient::RockComponent>()->SetPosition(4, 8);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock2_6")->GetComponent<GameClient::RockComponent>()->SetPosition(4, 9);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock2_7")->GetComponent<GameClient::RockComponent>()->SetPosition(6, 3);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock2_8")->GetComponent<GameClient::RockComponent>()->SetPosition(6, 5);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock2_9")->GetComponent<GameClient::RockComponent>()->SetPosition(10, 2);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock2_10")->GetComponent<GameClient::RockComponent>()->SetPosition(10, 4);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock2_11")->GetComponent<GameClient::RockComponent>()->SetPosition(10, 8);

			for (auto rock : m_vRock2)
			{
				rock->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			}
		}

		// 트랩 위치 셋팅. 0.8f - ((float)m_mapX) * 0.15f, 0.05f, 0.85f - ((float)m_mapY) * 0.15f
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap2_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 3.f * 0.195f, 0.005f, 1.09f - 2.f * 0.195f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap2_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 4.f * 0.195f, 0.005f, 1.09f - 10.f * 0.195f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap2_3")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 7.f * 0.195f, 0.005f, 1.09f - 2.f * 0.195f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap2_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 3.f * 0.195f, 0.005f, 1.09f - 2.f * 0.195f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap2_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 4.f * 0.195f, 0.005f, 1.09f - 10.f * 0.195f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap2_3")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 7.f * 0.195f, 0.005f, 1.09f - 2.f * 0.195f);



		break;
		case 3:
			// 3스테이지 바위 초기 위치 셋팅
		{
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_1")->GetComponent<GameClient::RockComponent>()->SetPosition(1, 11);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_2")->GetComponent<GameClient::RockComponent>()->SetPosition(1, 13);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_3")->GetComponent<GameClient::RockComponent>()->SetPosition(3, 1);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_4")->GetComponent<GameClient::RockComponent>()->SetPosition(4, 3);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_5")->GetComponent<GameClient::RockComponent>()->SetPosition(4, 9);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_6")->GetComponent<GameClient::RockComponent>()->SetPosition(5, 11);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_7")->GetComponent<GameClient::RockComponent>()->SetPosition(6, 4);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_8")->GetComponent<GameClient::RockComponent>()->SetPosition(9, 1);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_9")->GetComponent<GameClient::RockComponent>()->SetPosition(9, 9);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_10")->GetComponent<GameClient::RockComponent>()->SetPosition(9, 10);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_11")->GetComponent<GameClient::RockComponent>()->SetPosition(10, 3);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_12")->GetComponent<GameClient::RockComponent>()->SetPosition(10, 10);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_13")->GetComponent<GameClient::RockComponent>()->SetPosition(10, 14);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_14")->GetComponent<GameClient::RockComponent>()->SetPosition(11, 11);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_15")->GetComponent<GameClient::RockComponent>()->SetPosition(12, 2);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_16")->GetComponent<GameClient::RockComponent>()->SetPosition(12, 13);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_17")->GetComponent<GameClient::RockComponent>()->SetPosition(12, 14);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_18")->GetComponent<GameClient::RockComponent>()->SetPosition(14, 9);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rock3_19")->GetComponent<GameClient::RockComponent>()->SetPosition(15, 10);

			for (auto rock : m_vRock3)
			{
				rock->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			}
		}

		// 트랩 위치 셋팅. 0.8f - ((float)m_mapX) * 0.15f, 0.05f, 0.85f - ((float)m_mapY) * 0.15f
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 2.f * 0.195f, 0.005f, 1.09f - 6.f * 0.195f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 3.f * 0.195f, 0.005f, 1.09f - 9.f * 0.195f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_3")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 4.f * 0.195f, 0.005f, 1.09f - 2.f * 0.195f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_4")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 5.f * 0.195f, 0.005f, 1.09f - 4.f * 0.195f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_5")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 7.f * 0.195f, 0.005f, 1.09f - 4.f * 0.195f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_6")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 12.f * 0.195f, 0.005f, 1.09f - 7.f * 0.195f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 2.f * 0.195f, 0.005f, 1.09f - 6.f * 0.195f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 3.f * 0.195f, 0.005f, 1.09f - 9.f * 0.195f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_3")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 4.f * 0.195f, 0.005f, 1.09f - 2.f * 0.195f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_4")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 5.f * 0.195f, 0.005f, 1.09f - 4.f * 0.195f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_5")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 7.f * 0.195f, 0.005f, 1.09f - 4.f * 0.195f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_6")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 12.f * 0.195f, 0.005f, 1.09f - 7.f * 0.195f);



		break;
		default:
			break;
	}
}

void GameClient::MapMatrix::ResetXYObjectInMap()
{
	for (auto x : m_vRock1)
	{
		x->GetComponent<RockComponent>()->SetPosition(100, 100);
		x->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();

	}

	for (auto x : m_vRock2)
	{
		x->GetComponent<RockComponent>()->SetPosition(100, 100);
		x->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
	}

	for (auto x : m_vRock3)
	{
		x->GetComponent<RockComponent>()->SetPosition(100, 100);
		x->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
	}

	for (auto x : m_vTrap)
	{
		x->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	}

	for (auto x : m_vUnTrap)
	{
		x->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	}

	for (auto x : m_vPlayerBody)
	{
		x->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		x->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
	}

	SetMonsterObject();
}

void GameClient::MapMatrix::DistroyObjectInMap()
{
	for (auto objectIter : m_vRock1)
	{
		objectIter;
	}
}

void GameClient::MapMatrix::MapReset()
{
	// 맵 초기화
	{
		for (int i = 1; i < 11; i++)
		{
			for (int j = 1; j < 11; j++)
			{
				m_mapXYStatus1[i][j] = MapStatus::NORMAL;
				m_mapXYStatus2[i][j] = MapStatus::NORMAL;
			}
		}
		for (int i = 1; i < 16; i++)
		{
			for (int j = 1; j < 16; j++)
			{
				m_mapXYStatus3[i][j] = MapStatus::NORMAL;
			}
		}
	}

	// 스테이지1 맵배치
	{
		TileChange(1, 1, 5, ROCK);
		TileChange(1, 1, 7, ROCK);
		TileChange(1, 1, 9, ROCK);
		TileChange(1, 1, 10, ROCK);
		TileChange(1, 2, 1, ROCK);
		TileChange(1, 2, 2, ROCK);
		TileChange(1, 2, 3, ROCK);
		TileChange(1, 3, 3, ROCK);
		TileChange(1, 3, 5, ROCK);
		TileChange(1, 3, 6, ROCK);
		TileChange(1, 3, 8, ROCK);
		TileChange(1, 3, 9, ROCK);
		TileChange(1, 4, 2, ROCK);
		TileChange(1, 4, 3, ROCK);
		TileChange(1, 4, 8, ROCK);
		TileChange(1, 5, 10, ROCK);
		TileChange(1, 6, 9, ROCK);
		TileChange(1, 6, 10, ROCK);
		TileChange(1, 7, 3, ROCK);
		TileChange(1, 7, 9, ROCK);
		TileChange(1, 8, 9, ROCK);
		TileChange(1, 9, 4, ROCK);

		TileChange(1, 4, 4, WATER);
		TileChange(1, 4, 5, WATER);
		TileChange(1, 4, 6, WATER);
		TileChange(1, 5, 5, WATER);
		TileChange(1, 5, 6, WATER);
		TileChange(1, 5, 7, WATER);
		TileChange(1, 6, 4, WATER);
		TileChange(1, 6, 5, WATER);
		TileChange(1, 6, 6, WATER);
		TileChange(1, 6, 7, WATER);
		TileChange(1, 7, 4, WATER);
		TileChange(1, 7, 5, WATER);
		TileChange(1, 7, 6, WATER);
		TileChange(1, 7, 7, WATER);
		TileChange(1, 8, 4, WATER);
		TileChange(1, 8, 5, WATER);
		TileChange(1, 8, 6, WATER);
		TileChange(1, 8, 7, WATER);
		TileChange(1, 8, 8, WATER);
		TileChange(1, 9, 5, WATER);
		TileChange(1, 9, 6, WATER);
		TileChange(1, 9, 7, WATER);
		TileChange(1, 9, 8, WATER);
		TileChange(1, 10, 4, WATER);
		TileChange(1, 10, 5, WATER);
		TileChange(1, 10, 6, WATER);
		TileChange(1, 10, 7, WATER);

		if (!KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_getGlove)
		{
			TileChange(1, 7, 1, Equipment);
		}
		if (!KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_getSheild)
		{
			TileChange(1, 7, 2, Equipment);
		}

		TileChange(1, 2, 10, ARROW);
		TileChange(1, 8, 10, EXIT);
	}

	// 스테이지2 맵배치
	{
		TileChange(2, 2, 5, ROCK);
		TileChange(2, 3, 1, ROCK);
		TileChange(2, 3, 7, ROCK);
		TileChange(2, 3, 9, ROCK);
		TileChange(2, 4, 8, ROCK);
		TileChange(2, 4, 9, ROCK);
		TileChange(2, 6, 3, ROCK);
		TileChange(2, 6, 5, ROCK);
		TileChange(2, 10, 2, ROCK);
		TileChange(2, 10, 4, ROCK);
		TileChange(2, 10, 8, ROCK);

		TileChange(2, 2, 3, WATER);
		TileChange(2, 2, 4, WATER);
		TileChange(2, 3, 4, WATER);
		TileChange(2, 3, 5, WATER);
		TileChange(2, 4, 5, WATER);
		TileChange(2, 4, 6, WATER);
		TileChange(2, 5, 5, WATER);
		TileChange(2, 5, 6, WATER);
		TileChange(2, 5, 7, WATER);
		TileChange(2, 6, 6, WATER);
		TileChange(2, 6, 7, WATER);
		TileChange(2, 7, 5, WATER);
		TileChange(2, 7, 6, WATER);
		TileChange(2, 7, 7, WATER);
		TileChange(2, 8, 5, WATER);
		TileChange(2, 8, 6, WATER);
		TileChange(2, 9, 4, WATER);
		TileChange(2, 9, 5, WATER);
		TileChange(2, 9, 6, WATER);
		TileChange(2, 10, 5, WATER);
		TileChange(2, 10, 6, WATER);

		TileChange(2, 3, 2, TRAP);
		TileChange(2, 4, 10, TRAP);
		TileChange(2, 7, 2, TRAP);

		TileChange(2, 7, 8, MONSTER);

		TileChange(2, 4, 3, ROPESURPORTITEM);

		if (!KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_rope)
		{
			TileChange(2, 8, 3, Equipment);
		}

		TileChange(2, 1, 2, ARROW);
		TileChange(2, 1, 10, ARROW);
		TileChange(2, 8, 10, EXIT);
	}

	// 스테이지3 맵배치
	{
		TileChange(3, 1, 11, ROCK);
		TileChange(3, 1, 13, ROCK);
		TileChange(3, 3, 1, ROCK);
		TileChange(3, 4, 3, ROCK);
		TileChange(3, 4, 9, ROCK);
		TileChange(3, 5, 11, ROCK);
		TileChange(3, 6, 4, ROCK);
		TileChange(3, 9, 1, ROCK);
		TileChange(3, 9, 9, ROCK);
		TileChange(3, 9, 10, ROCK);
		TileChange(3, 10, 3, ROCK);
		TileChange(3, 10, 10, ROCK);
		TileChange(3, 10, 14, ROCK);
		TileChange(3, 11, 11, ROCK);
		TileChange(3, 12, 2, ROCK);
		TileChange(3, 12, 13, ROCK);
		TileChange(3, 12, 14, ROCK);
		TileChange(3, 14, 9, ROCK);
		TileChange(3, 15, 10, ROCK);

		TileChange(3, 1, 3, WATER);
		TileChange(3, 1, 4, WATER);
		TileChange(3, 2, 4, WATER);
		TileChange(3, 2, 11, WATER);
		TileChange(3, 2, 12, WATER);
		TileChange(3, 3, 6, WATER);
		TileChange(3, 3, 12, WATER);
		TileChange(3, 3, 13, WATER);
		TileChange(3, 4, 5, WATER);
		TileChange(3, 4, 6, WATER);
		TileChange(3, 4, 7, WATER);
		TileChange(3, 4, 11, WATER);
		TileChange(3, 4, 12, WATER);
		TileChange(3, 4, 13, WATER);
		TileChange(3, 5, 5, WATER);
		TileChange(3, 5, 6, WATER);
		TileChange(3, 5, 7, WATER);
		TileChange(3, 5, 8, WATER);
		TileChange(3, 5, 12, WATER);
		TileChange(3, 6, 5, WATER);
		TileChange(3, 6, 7, WATER);
		TileChange(3, 6, 8, WATER);
		TileChange(3, 6, 9, WATER);
		TileChange(3, 7, 8, WATER);
		TileChange(3, 7, 9, WATER);
		TileChange(3, 7, 10, WATER);
		TileChange(3, 8, 7, WATER);
		TileChange(3, 8, 8, WATER);
		TileChange(3, 8, 9, WATER);
		TileChange(3, 8, 10, WATER);
		TileChange(3, 9, 7, WATER);
		TileChange(3, 9, 8, WATER);
		TileChange(3, 11, 3, WATER);
		TileChange(3, 11, 4, WATER);
		TileChange(3, 11, 12, WATER);
		TileChange(3, 12, 3, WATER);
		TileChange(3, 12, 4, WATER);
		TileChange(3, 12, 5, WATER);
		TileChange(3, 12, 11, WATER);
		TileChange(3, 12, 12, WATER);
		TileChange(3, 13, 4, WATER);
		TileChange(3, 13, 5, WATER);
		TileChange(3, 13, 6, WATER);
		TileChange(3, 13, 10, WATER);
		TileChange(3, 13, 11, WATER);
		TileChange(3, 13, 12, WATER);
		TileChange(3, 13, 13, WATER);
		TileChange(3, 14, 5, WATER);
		TileChange(3, 14, 6, WATER);
		TileChange(3, 14, 11, WATER);
		TileChange(3, 14, 12, WATER);
		TileChange(3, 14, 13, WATER);
		TileChange(3, 15, 4, WATER);
		TileChange(3, 15, 5, WATER);
		TileChange(3, 15, 6, WATER);
		TileChange(3, 15, 11, WATER);
		TileChange(3, 15, 12, WATER);
		TileChange(3, 15, 13, WATER);

		TileChange(3, 2, 6, TRAP);
		TileChange(3, 3, 9, TRAP);
		TileChange(3, 4, 2, TRAP);
		TileChange(3, 5, 4, TRAP);
		TileChange(3, 7, 4, TRAP);
		TileChange(3, 12, 7, TRAP);

		TileChange(3, 6, 13, MONSTER);
		TileChange(3, 9, 3, MONSTER);
		TileChange(3, 10, 13, MONSTER);

		TileChange(3, 3, 8, ROPESURPORTITEM);
		TileChange(3, 10, 1, ROPESURPORTITEM);
		TileChange(3, 11, 6, ROPESURPORTITEM);

		TileChange(3, 1, 5, ARROW);
		TileChange(3, 2, 15, ARROW);
		TileChange(3, 6, 1, ARROW);
		TileChange(3, 15, 7, ARROW);

		TileChange(3, 7, 15, EXIT);
		TileChange(3, 8, 15, EXIT);
		TileChange(3, 9, 15, EXIT);
	}

	m_ropeSurport_1 = false;
	m_ropeSurport_2 = false;
	m_ropeSurport_3 = false;

	m_turnChanger = true;
}

void GameClient::MapMatrix::TurnTrapONOFF()
{
	if (m_turnChanger)
	{
		TrapChange(2, 3, 2, TRAP);
		TrapChange(2, 4, 10, TRAP);
		TrapChange(2, 7, 2, TRAP);
		TrapChange(3, 2, 6, TRAP);
		TrapChange(3, 3, 9, TRAP);
		TrapChange(3, 4, 2, TRAP);
		TrapChange(3, 5, 4, TRAP);
		TrapChange(3, 7, 4, TRAP);
		TrapChange(3, 12, 7, TRAP);

		// 		for (auto x : m_vTrap)
		// 		{			
		// 			x->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Pot_2");
		// 		}

		switch (m_currentMapNumber)
		{
			case 2:
				// 트랩 위치 셋팅. 0.8f - ((float)m_mapX) * 0.15f, 0.05f, 0.85f - ((float)m_mapY) * 0.15f
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap2_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 3.f * 0.195f, 0.005f, 1.09f - 2.f * 0.195f);
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap2_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 4.f * 0.195f, 0.005f, 1.09f - 10.f * 0.195f);
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap2_3")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 7.f * 0.195f, 0.005f, 1.09f - 2.f * 0.195f);

				// 트랩 위치 셋팅. 0.8f - ((float)m_mapX) * 0.15f, 0.05f, 0.85f - ((float)m_mapY) * 0.15f
				//KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap2_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
				//KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap2_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
				//KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap2_3")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);

				break;
			case 3:
				// 트랩 위치 셋팅. 0.8f - ((float)m_mapX) * 0.15f, 0.05f, 0.85f - ((float)m_mapY) * 0.15f
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 2.f * 0.195f, 0.005f, 1.09f - 6.f * 0.195f);
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 3.f * 0.195f, 0.005f, 1.09f - 9.f * 0.195f);
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_3")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 4.f * 0.195f, 0.005f, 1.09f - 2.f * 0.195f);
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_4")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 5.f * 0.195f, 0.005f, 1.09f - 4.f * 0.195f);
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_5")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 7.f * 0.195f, 0.005f, 1.09f - 4.f * 0.195f);
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_6")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 12.f * 0.195f, 0.005f, 1.09f - 7.f * 0.195f);

				// 트랩 위치 셋팅. 0.8f - ((float)m_mapX) * 0.15f, 0.05f, 0.85f - ((float)m_mapY) * 0.15f
				//KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
				//KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
				//KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_3")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
				//KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_4")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
				//KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_5")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
				//KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_6")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);

				break;
			default:
				break;
		}


	}
	else
	{
		TrapChange(2, 3, 2, TRAPOFF);
		TrapChange(2, 4, 10, TRAPOFF);
		TrapChange(2, 7, 2, TRAPOFF);
		TrapChange(3, 2, 6, TRAPOFF);
		TrapChange(3, 3, 9, TRAPOFF);
		TrapChange(3, 4, 2, TRAPOFF);
		TrapChange(3, 5, 4, TRAPOFF);
		TrapChange(3, 7, 4, TRAPOFF);
		TrapChange(3, 12, 7, TRAPOFF);

		// 		for (auto x : m_vTrap)
		// 		{
		// 			x->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Pot_1");
		// 		}

		switch (m_currentMapNumber)
		{
			case 2:
				// 트랩 위치 셋팅. 0.8f - ((float)m_mapX) * 0.15f, 0.05f, 0.85f - ((float)m_mapY) * 0.15f
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap2_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.005f, 100.f);
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap2_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.005f, 100.f);
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap2_3")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.005f, 100.f);

				// 트랩 위치 셋팅. 0.8f - ((float)m_mapX) * 0.15f, 0.05f, 0.85f - ((float)m_mapY) * 0.15f
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap2_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 3.f * 0.195f, 0.005f, 1.09f - 2.f * 0.195f);
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap2_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 4.f * 0.195f, 0.005f, 1.09f - 10.f * 0.195f);
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap2_3")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 7.f * 0.195f, 0.005f, 1.09f - 2.f * 0.195f);

				break;
			case 3:
				// 트랩 위치 셋팅. 0.8f - ((float)m_mapX) * 0.15f, 0.05f, 0.85f - ((float)m_mapY) * 0.15f
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.005f, 100.f);
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.005f, 100.f);
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_3")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.005f, 100.f);
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_4")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.005f, 100.f);
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_5")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.005f, 100.f);
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Trap3_6")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.005f, 100.f);

				// 트랩 위치 셋팅. 0.8f - ((float)m_mapX) * 0.15f, 0.05f, 0.85f - ((float)m_mapY) * 0.15f
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 2.f * 0.195f, 0.005f, 1.09f - 6.f * 0.195f);
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 3.f * 0.195f, 0.005f, 1.09f - 9.f * 0.195f);
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_3")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 4.f * 0.195f, 0.005f, 1.09f - 2.f * 0.195f);
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_4")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 5.f * 0.195f, 0.005f, 1.09f - 4.f * 0.195f);
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_5")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 7.f * 0.195f, 0.005f, 1.09f - 4.f * 0.195f);
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("UnTrap3_6")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 12.f * 0.195f, 0.005f, 1.09f - 7.f * 0.195f);

				break;
			default:
				break;
		}


	}
}

void GameClient::MapMatrix::TurnChange()
{
	m_turnChanger = !m_turnChanger;

	// 화살
	ArrowAction();
	ArrowMove();

	// 몬스터
	MonsterTouch();
	MonsterMove();
}

int GameClient::MapMatrix::ReturnCurrentX()
{
	return m_currentMapX;
}

int GameClient::MapMatrix::ReturnCurrentY()
{
	return m_currentMapY;
}

void GameClient::MapMatrix::UseRopeSurport()
{
	if (KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_ropeSurport > 0)
	{
		switch (m_currentMapNumber)
		{
			case 2:
				if (m_mapXYStatus2[m_currentMapX][m_currentMapY] == NORMAL)
				{
					m_mapXYStatus2[m_currentMapX][m_currentMapY] = ROPESURPORTBUILD;
					KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->UseRopeSurport();
					KunrealEngine::GetScene(m_sceneName)->GetGameObject("UsingRopeSurport_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - ((float)(m_currentMapX)) * 0.195f, 0.05f, 1.09f - ((float)(m_currentMapY)) * 0.195f);

				}
				break;
			case 3:
				if (m_mapXYStatus3[m_currentMapX][m_currentMapY] == NORMAL)
				{
					m_mapXYStatus3[m_currentMapX][m_currentMapY] = ROPESURPORTBUILD;
					switch (KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->UseRopeSurport())
					{
						case 1:
							KunrealEngine::GetScene(m_sceneName)->GetGameObject("UsingRopeSurport_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - ((float)(m_currentMapX)) * 0.195f, 0.05f, 1.09f - ((float)(m_currentMapY)) * 0.195f);
							break;
						case 2:
							KunrealEngine::GetScene(m_sceneName)->GetGameObject("UsingRopeSurport_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - ((float)(m_currentMapX)) * 0.195f, 0.05f, 1.09f - ((float)(m_currentMapY)) * 0.195f);
							break;
						case 3:
							KunrealEngine::GetScene(m_sceneName)->GetGameObject("UsingRopeSurport_3")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - ((float)(m_currentMapX)) * 0.195f, 0.05f, 1.09f - ((float)(m_currentMapY)) * 0.195f);
							break;
						case 4:
							KunrealEngine::GetScene(m_sceneName)->GetGameObject("UsingRopeSurport_4")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - ((float)(m_currentMapX)) * 0.195f, 0.05f, 1.09f - ((float)(m_currentMapY)) * 0.195f);
							break;
						default:
							break;
					}
				}
				break;
			default:
				break;
		}
		TurnChange();
	}

}

void GameClient::MapMatrix::UseRope()
{
	if (KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_rope)
	{
		switch (m_playerView)
		{
			// 오른쪽
			case 6:
				RopeCheck(1, 0);
				break;

				// 왼쪽
			case 4:
				RopeCheck(-1, 0);
				break;

				// 위
			case 8:
				RopeCheck(0, 1);
				break;

				// 아래
			case 2:
				RopeCheck(0, -1);
				break;

			default:
				break;
		}
	}


}

void GameClient::MapMatrix::ReStart()
{
	// 플레이어 초기화
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<KunrealEngine::Transform>()->SetRotation(0, 0, 0);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->PlayerRelive();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_getGlove = false;
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_getSheild = false;
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_rope = false;
	SetPlayerBody();
	m_currentMapX = 10;
	m_currentMapY = 1;
	m_playerLife = true;
	m_vPlayerBodyX.clear();
	m_vPlayerBodyY.clear();

	// 맵배치
	m_currentMapNumber = 1;
	MapReset();
	ResetXYObjectInMap();
	SetXYObjectInMap();
	SetEquipmentPosition();
	SetArrowPosition();
	SetMapBase();
}

GameClient::MapStatus GameClient::MapMatrix::MonsterFrontTileCheck(GameClient::MonsterTileIndex monsterIndex, int creaseX, int creaseY)
{
	int monsterPositionX = monsterIndex.nowX;
	int monsterPositionY = monsterIndex.nowY;


	switch (m_currentMapNumber)
	{
		case 1:
			return ROCK;
			break;
		case 2:
			return m_mapXYStatus2[monsterPositionX + creaseX][monsterPositionY + creaseY];
			break;
		case 3:
			return m_mapXYStatus3[monsterPositionX + creaseX][monsterPositionY + creaseY];
			break;
		default:
			return WALL;
			break;
	}
}

void GameClient::MapMatrix::TileChange(int mapNumber, int tileX, int tiley, MapStatus status)
{
	switch (mapNumber)
	{
		case 1:
			m_mapXYStatus1[tileX][tiley] = status;
			break;
		case 2:
			m_mapXYStatus2[tileX][tiley] = status;
			break;
		case 3:
			m_mapXYStatus3[tileX][tiley] = status;
			break;
		default:
			MessageBox(0, L"정상적이지 않은 타일 생성", L"MapMatrix.cpp", 0);
			break;
	}
}

void GameClient::MapMatrix::TrapChange(int stageNumber, int tileX, int tiley, MapStatus status)
{
	switch (stageNumber)
	{
		case 1:
		{}
		break;
		case 2:
			if (m_mapXYStatus2[tileX][tiley] != ROCK)
			{
				TileChange(stageNumber, tileX, tiley, status);
			}
			break;
		case 3:
			if (m_mapXYStatus3[tileX][tiley] != ROCK)
			{
				TileChange(stageNumber, tileX, tiley, status);
			}
			break;
		default:
			break;
	}
}

void GameClient::MapMatrix::CreateRockObject(std::string rockName)
{
	KunrealEngine::GetScene(m_sceneName)->CreateObject(rockName);
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->AddComponent<GameClient::RockComponent>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Rock");
}

void GameClient::MapMatrix::CreateEquipmentItem()
{
	KunrealEngine::GetScene(m_sceneName)->CreateObject("Glove");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Glove")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Glove")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Glove")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Glove")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Glove");

	KunrealEngine::GetScene(m_sceneName)->CreateObject("Sheild");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Sheild")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Sheild")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Sheild")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Sheild")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Shield");

	KunrealEngine::GetScene(m_sceneName)->CreateObject("Rope");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rope")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rope")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rope")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rope")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Rope");

	KunrealEngine::GetScene(m_sceneName)->CreateObject("RopeSurport_1");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("RopeSurport_1")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("RopeSurport_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("RopeSurport_1")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("RopeSurport_1")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Rope_Connect");

	KunrealEngine::GetScene(m_sceneName)->CreateObject("RopeSurport_2");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("RopeSurport_2")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("RopeSurport_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("RopeSurport_2")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("RopeSurport_2")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Rope_Connect");

	KunrealEngine::GetScene(m_sceneName)->CreateObject("RopeSurport_3");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("RopeSurport_3")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("RopeSurport_3")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("RopeSurport_3")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("RopeSurport_3")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Rope_Connect");

	KunrealEngine::GetScene(m_sceneName)->CreateObject("RopeSurport_4");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("RopeSurport_4")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("RopeSurport_4")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("RopeSurport_4")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("RopeSurport_4")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Rope_Connect");

	KunrealEngine::GetScene(m_sceneName)->CreateObject("UsingRopeSurport_1");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UsingRopeSurport_1")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UsingRopeSurport_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UsingRopeSurport_1")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UsingRopeSurport_1")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Rope_Connector");
	
	KunrealEngine::GetScene(m_sceneName)->CreateObject("UsingRopeSurport_2");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UsingRopeSurport_1")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UsingRopeSurport_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UsingRopeSurport_1")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UsingRopeSurport_1")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Rope_Connector");
	
	KunrealEngine::GetScene(m_sceneName)->CreateObject("UsingRopeSurport_3");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UsingRopeSurport_1")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UsingRopeSurport_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UsingRopeSurport_1")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UsingRopeSurport_1")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Rope_Connector");
	
	KunrealEngine::GetScene(m_sceneName)->CreateObject("UsingRopeSurport_4");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UsingRopeSurport_1")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UsingRopeSurport_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UsingRopeSurport_1")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UsingRopeSurport_1")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Rope_Connector");
	
}

void GameClient::MapMatrix::SetEquipmentPosition()
{
	//1.055f - ((float)(m_vPlayerBodyX[i])) * 0.195f, 0.05f, 1.09f - ((float)(m_vPlayerBodyY[i] - 1)) * 0.195f
	if (m_currentMapNumber == 1 && !KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_getGlove)
	{
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Glove")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 7.f * 0.195f, 0.05f, 1.09f - 1.f * 0.195f);
	}
	else
	{
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Glove")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	}

	if (m_currentMapNumber == 1 && !KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_getSheild)
	{
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Sheild")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 7.f * 0.195f, 0.05f, 1.09f - 2.f * 0.195f);
	}
	else
	{
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Sheild")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	}

	if (m_currentMapNumber == 2 && !KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_rope)
	{
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rope")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 8.f * 0.195f, 0.05f, 1.09f - 3.f * 0.195f);
	}
	else
	{
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rope")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	}

	if (m_currentMapNumber == 2)
	{
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("RopeSurport_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 4.f * 0.195f, 0.05f, 1.09f - 3.f * 0.195f);
	}
	else
	{
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("RopeSurport_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	}

	if (m_currentMapNumber == 3)
	{
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("RopeSurport_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 3.f * 0.195f, 0.05f, 1.09f - 8.f * 0.195f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("RopeSurport_3")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 10.f * 0.195f, 0.05f, 1.09f - 1.f * 0.195f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("RopeSurport_4")->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - 11.f * 0.195f, 0.05f, 1.09f - 6.f * 0.195f);
	}
	else
	{
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("RopeSurport_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("RopeSurport_3")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("RopeSurport_4")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	}

	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UsingRopeSurport_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UsingRopeSurport_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UsingRopeSurport_3")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("UsingRopeSurport_4")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
}

void GameClient::MapMatrix::SetArrowPosition()
{
	switch (m_currentMapNumber)
	{
		case 1:
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XPlus")->GetComponent<ArrowComponent>()->SetPosition(100, 100);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XMinus")->GetComponent<ArrowComponent>()->SetPosition(100, 100);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YPlus")->GetComponent<ArrowComponent>()->SetPosition(100, 100);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YMinus")->GetComponent<ArrowComponent>()->SetPosition(2, 11);
			break;

		case 2:
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XPlus")->GetComponent<ArrowComponent>()->SetPosition(0, 2);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XMinus")->GetComponent<ArrowComponent>()->SetPosition(100, 100);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YPlus")->GetComponent<ArrowComponent>()->SetPosition(100, 100);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YMinus")->GetComponent<ArrowComponent>()->SetPosition(1, 11);
			break;

		case 3:
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XPlus")->GetComponent<ArrowComponent>()->SetPosition(0, 5);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XMinus")->GetComponent<ArrowComponent>()->SetPosition(17, 7);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YPlus")->GetComponent<ArrowComponent>()->SetPosition(6, 0);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YMinus")->GetComponent<ArrowComponent>()->SetPosition(2, 17);
			break;

		default:
			break;
	}
}

void GameClient::MapMatrix::CreateMapBase()
{
	// 1스테이지
	{
		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase1_Arrow_Trap");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Arrow_Trap")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Arrow_Trap")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Arrow_Trap")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Arrow_Trap")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_01_Arrow_Trap");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase1_Door");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Door")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Door")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Door")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Door")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Door");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase1_Door_Wall");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Door_Wall")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Door_Wall")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Door_Wall")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Door_Wall")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Wall");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase1_Half_Tile");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Half_Tile")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Half_Tile")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Half_Tile")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Half_Tile")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_01_Half_tile");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase1_Pillar");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Pillar")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Pillar")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Pillar")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Pillar")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_01_Pillar");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase1_Stair");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Stair")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Stair")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Stair")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Stair")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_01_Stair");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase1_Statue");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Statue")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Statue")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Statue")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Statue")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_01_Statue");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase1_Tile_1");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Tile_1")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Tile_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Tile_1")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Tile_1")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_01_Tile_1");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase1_Tile_2");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Tile_2")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Tile_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Tile_2")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Tile_2")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_01_Tile_2");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase1_Torch");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Torch")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Torch")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Torch")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Torch")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_01_Torch");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase1_Wall");
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Wall")->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Wall")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Wall")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Wall")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Wall")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_01_Wall");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase1_Water");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Water")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Water")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Water")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Water")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_01_Water");
	}

	// 2스테이지
	{
		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase2_Arrow_Trap");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Arrow_Trap")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Arrow_Trap")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Arrow_Trap")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Arrow_Trap")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_02_Arrow_Trap");


		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase2_Big_Pipe");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase_Big_Pipe")->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Big_Pipe")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Big_Pipe")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Big_Pipe")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Big_Pipe")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_02_Big_Pipe");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase2_Door");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Door")->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Door")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Door")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Door")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Door")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_02_Door");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase2_Half_Tile");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Half_Tile")->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Half_Tile")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Half_Tile")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Half_Tile")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Half_Tile")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_02_Half_Tile");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase2_Iron_Bar");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Iron_Bar")->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Iron_Bar")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Iron_Bar")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Iron_Bar")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Iron_Bar")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_02_Iron_Bar");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase2_Pillar");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pillar")->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pillar")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pillar")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pillar")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pillar")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_02_Pillar");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase2_Pot_1");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pot_1")->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pot_1")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pot_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pot_1")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pot_1")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_02_Pot_1");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase2_Pot_2");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pot_2")->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pot_2")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pot_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pot_2")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pot_2")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_02_Pot_2");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase2_Pot_3");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pot_3")->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pot_3")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pot_3")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pot_3")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pot_3")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_02_Pot_3");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase2_Small_Pipe");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Small_Pipe")->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Small_Pipe")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Small_Pipe")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Small_Pipe")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Small_Pipe")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_02_Small_Pipe");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase2_Stair");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Stair")->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Stair")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Stair")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Stair")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Stair")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_02_Stair");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase2_Statue");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Statue")->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Statue")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Statue")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Statue")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Statue")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_02_Statue");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase2_Tile_1");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Tile_1")->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Tile_1")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Tile_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Tile_1")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Tile_1")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_02_Tile_1");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase2_Tile_2");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pot_1")->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Tile_2")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Tile_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Tile_2")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Tile_2")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_02_Tile_2");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase2_Torch");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Torch")->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Torch")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Torch")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Torch")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Torch")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_02_Torch");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase2_Wall");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Wall")->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Wall")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Wall")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Wall")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Wall")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_02_Wall");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase2_Water");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Water")->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Water")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Water")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Water")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Water")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_02_Water");
	}

	// 3스테이지
	{
		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase3_Arrow_Trap");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Arrow_Trap")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Arrow_Trap")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Arrow_Trap")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Arrow_Trap")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_03_Arrow_Trap");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase3_Big_Pipe");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Big_Pipe")->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Big_Pipe")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Big_Pipe")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Big_Pipe")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Big_Pipe")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_03_Big_Pipe");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase3_Door");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Door")->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Door")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Door")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Door")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Door")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_03_Door");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase3_Half_Tile");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Half_Tile")->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Half_Tile")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Half_Tile")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Half_Tile")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Half_Tile")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_03_Half_Tile");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase3_Iron_Bar");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Iron_Bar")->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Iron_Bar")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Iron_Bar")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Iron_Bar")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Iron_Bar")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_03_Iron_Bar");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase3_Pillar");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pillar")->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pillar")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pillar")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pillar")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pillar")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_03_Pillar");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase3_Pot_1");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pot_1")->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pot_1")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pot_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pot_1")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pot_1")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_03_Pot_1");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase3_Pot_2");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pot_2")->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pot_2")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pot_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pot_2")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pot_2")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_03_Pot_2");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase3_Pot_3");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pot_3")->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pot_3")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pot_3")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pot_3")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pot_3")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_03_Pot_3");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase3_Small_Pipe");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Small_Pipe")->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Small_Pipe")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Small_Pipe")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Small_Pipe")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Small_Pipe")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_03_Small_Pipe");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase3_Stair");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Stair")->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Stair")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Stair")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Stair")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Stair")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_03_Stair");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase3_Statue");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Statue")->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Statue")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Statue")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Statue")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Statue")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_03_Statue");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase3_Tile_1");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Tile_1")->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Tile_1")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Tile_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Tile_1")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Tile_1")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_03_Tile_1");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase3_Tile_2");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pot_1")->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Tile_2")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Tile_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Tile_2")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Tile_2")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_03_Tile_2");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase3_Torch");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Torch")->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Torch")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Torch")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Torch")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Torch")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_03_Torch");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase3_Wall");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Wall")->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Wall")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Wall")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Wall")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Wall")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_03_Wall");

		KunrealEngine::GetScene(m_sceneName)->CreateObject("MapBase3_Water");
		//KunrealEngine::GetScene(m_sceneName)->GetGameObject(MapBase3_Water)->SetLayer(50);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Water")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Water")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Water")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Water")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Level_03_Water");
	}


}

void GameClient::MapMatrix::SetMapBase()
{
	switch (m_currentMapNumber)
	{
		case 1:
			ReSetMapBase();  // 처음에 먼저 다 날려버리기

			// 1스테이지
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Arrow_Trap")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.f, 0.f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Door")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.f, 0.f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Half_Tile")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.f, 0.f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Pillar")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.f, 0.f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Stair")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.f, 0.f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Statue")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.f, 0.f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Tile_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.f, 0.f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Tile_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.f, 0.f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Torch")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.f, 0.f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Wall")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.f, 0.f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Water")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.f, 0.f);

			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Arrow_Trap")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Door")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Half_Tile")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Pillar")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Stair")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Statue")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Tile_1")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Tile_2")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Torch")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Wall")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Water")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();

			// 안보이게
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Arrow_Trap")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Big_Pipe")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Door")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Half_Tile")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Iron_Bar")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pillar")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pot_1")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pot_2")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pot_3")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Small_Pipe")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Stair")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Statue")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Tile_1")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Tile_2")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Torch")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Wall")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Water")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();

			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Arrow_Trap")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Big_Pipe")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Door")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Half_Tile")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Iron_Bar")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pillar")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pot_1")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pot_2")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pot_3")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Small_Pipe")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Stair")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Statue")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Tile_1")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Tile_2")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Torch")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Wall")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Water")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			break;

		case 2:
			ReSetMapBase();

			// 2스테이지
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Arrow_Trap")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.f, 0.f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Big_Pipe")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.f, 0.f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Door")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.f, 0.f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Half_Tile")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.f, 0.f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Iron_Bar")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.f, 0.f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pillar")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.f, 0.f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pot_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.f, 0.f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pot_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.f, 0.f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pot_3")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.f, 0.f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Small_Pipe")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.f, 0.f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Stair")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.f, 0.f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Statue")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.f, 0.f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Tile_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.f, 0.f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Tile_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.f, 0.f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Torch")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.f, 0.f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Wall")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.f, 0.f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Water")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.f, 0.f);

			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Arrow_Trap")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Big_Pipe")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Door")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Half_Tile")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Iron_Bar")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pillar")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pot_1")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pot_2")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pot_3")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Small_Pipe")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Stair")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Statue")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Tile_1")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Tile_2")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Torch")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Wall")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Water")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			break;

			// 안보이게
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Arrow_Trap")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Door")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Half_Tile")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Pillar")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Stair")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Statue")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Tile_1")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Tile_2")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Torch")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Wall")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Water")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();

			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Arrow_Trap")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Big_Pipe")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Door")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Half_Tile")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Iron_Bar")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pillar")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pot_1")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pot_2")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pot_3")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Small_Pipe")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Stair")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Statue")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Tile_1")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Tile_2")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Torch")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Wall")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Water")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();

		case 3:
			ReSetMapBase();

			// 3스테이지
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Arrow_Trap")->GetComponent<KunrealEngine::Transform>()->SetPosition(-0.395f, 0.f, -0.605f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Big_Pipe")->GetComponent<KunrealEngine::Transform>()->SetPosition(-0.395f, 0.f, -0.605f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Door")->GetComponent<KunrealEngine::Transform>()->SetPosition(-0.395f, 0.f, -0.605f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Half_Tile")->GetComponent<KunrealEngine::Transform>()->SetPosition(-0.395f, 0.f, -0.605f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Iron_Bar")->GetComponent<KunrealEngine::Transform>()->SetPosition(-0.395f, 0.f, -0.605f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pillar")->GetComponent<KunrealEngine::Transform>()->SetPosition(-0.395f, 0.f, -0.605f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pot_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(-0.395f, 0.f, -0.605f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pot_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(-0.395f, 0.f, -0.605f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pot_3")->GetComponent<KunrealEngine::Transform>()->SetPosition(-0.395f, 0.f, -0.605f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Small_Pipe")->GetComponent<KunrealEngine::Transform>()->SetPosition(-0.395f, 0.f, -0.605f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Stair")->GetComponent<KunrealEngine::Transform>()->SetPosition(-0.395f, 0.f, -0.605f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Statue")->GetComponent<KunrealEngine::Transform>()->SetPosition(-0.395f, 0.f, -0.605f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Tile_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(-0.395f, 0.f, -0.605f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Tile_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(-0.395f, 0.f, -0.605f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Torch")->GetComponent<KunrealEngine::Transform>()->SetPosition(-0.395f, 0.f, -0.605f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Wall")->GetComponent<KunrealEngine::Transform>()->SetPosition(-0.395f, 0.f, -0.605f);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Water")->GetComponent<KunrealEngine::Transform>()->SetPosition(-0.395f, 0.f, -0.605f);

			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Arrow_Trap")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Big_Pipe")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Door")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Half_Tile")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Iron_Bar")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pillar")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pot_1")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pot_2")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pot_3")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Small_Pipe")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Stair")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Statue")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Tile_1")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Tile_2")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Torch")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Wall")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Water")->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();

			// 안보이게
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Arrow_Trap")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Door")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Half_Tile")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Pillar")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Stair")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Statue")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Tile_1")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Tile_2")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Torch")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Wall")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Water")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();

			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Arrow_Trap")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Big_Pipe")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Door")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Half_Tile")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Iron_Bar")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pillar")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pot_1")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pot_2")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pot_3")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Small_Pipe")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Stair")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Statue")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Tile_1")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Tile_2")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Torch")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Wall")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Water")->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
			break;

		default:
			break;
	}

}

void GameClient::MapMatrix::ReSetMapBase()
{
	// 1스테이지
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Arrow_Trap")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Door")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Half_Tile")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Pillar")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Stair")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Statue")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Tile_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Tile_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Torch")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Wall")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase1_Water")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);

	// 2스테이지
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Arrow_Trap")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Big_Pipe")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Door")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Half_Tile")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Iron_Bar")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pillar")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pot_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pot_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Pot_3")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Small_Pipe")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Stair")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Statue")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Tile_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Tile_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Torch")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Wall")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase2_Water")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);

	// 3스테이지
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Arrow_Trap")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Big_Pipe")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Door")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Half_Tile")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Iron_Bar")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pillar")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pot_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pot_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Pot_3")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Small_Pipe")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Stair")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Statue")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Tile_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Tile_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Torch")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Wall")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("MapBase3_Water")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.f, 100.f);
}

// 죽음의 행군 가급적 펼치지 마세요
void GameClient::MapMatrix::CreatePlayerBody()
{
	KunrealEngine::GetScene(m_sceneName)->CreateObject("PlayerBody_1");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_1")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_1")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_1")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Character_Jade_Dead");
	m_vPlayerBody.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_1"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("PlayerBody_2");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_2")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_2")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_2")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Character_Jade_Dead");
	m_vPlayerBody.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_2"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("PlayerBody_3");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_3")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_3")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_3")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_3")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Character_Jade_Dead");
	m_vPlayerBody.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_3"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("PlayerBody_4");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_4")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_4")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_4")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_4")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Character_Jade_Dead");
	m_vPlayerBody.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_4"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("PlayerBody_5");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_5")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_5")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_5")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_5")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Character_Jade_Dead");
	m_vPlayerBody.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_5"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("PlayerBody_6");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_6")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_6")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_6")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_6")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Character_Jade_Dead");
	m_vPlayerBody.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_6"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("PlayerBody_7");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_7")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_7")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_7")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_7")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Character_Jade_Dead");
	m_vPlayerBody.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_7"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("PlayerBody_8");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_8")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_8")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_8")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_8")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Character_Jade_Dead");
	m_vPlayerBody.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_8"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("PlayerBody_9");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_9")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_9")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_9")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_9")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Character_Jade_Dead");
	m_vPlayerBody.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_9"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("PlayerBody_10");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_10")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_10")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_10")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_10")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Character_Jade_Dead");
	m_vPlayerBody.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_10"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("PlayerBody_11");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_11")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_11")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_11")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_11")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Character_Jade_Dead");
	m_vPlayerBody.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_11"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("PlayerBody_12");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_12")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_12")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_12")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_12")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Character_Jade_Dead");
	m_vPlayerBody.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_12"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("PlayerBody_13");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_13")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_13")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_13")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_13")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Character_Jade_Dead");
	m_vPlayerBody.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_13"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("PlayerBody_14");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_14")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_14")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_14")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_14")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Character_Jade_Dead");
	m_vPlayerBody.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_14"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("PlayerBody_15");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_15")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_15")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_15")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_15")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Character_Jade_Dead");
	m_vPlayerBody.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_15"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("PlayerBody_16");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_16")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_16")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_16")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_16")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Character_Jade_Dead");
	m_vPlayerBody.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_16"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("PlayerBody_17");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_17")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_17")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_17")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_17")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Character_Jade_Dead");
	m_vPlayerBody.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_17"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("PlayerBody_18");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_18")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_18")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_18")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_18")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Character_Jade_Dead");
	m_vPlayerBody.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_18"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("PlayerBody_19");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_19")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_19")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_19")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_19")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Character_Jade_Dead");
	m_vPlayerBody.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_19"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("PlayerBody_20");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_20")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_20")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_20")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_20")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Character_Jade_Dead");
	m_vPlayerBody.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_20"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("PlayerBody_21");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_21")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_21")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_21")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_21")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Character_Jade_Dead");
	m_vPlayerBody.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_21"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("PlayerBody_22");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_22")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_22")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_22")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_22")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Character_Jade_Dead");
	m_vPlayerBody.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_22"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("PlayerBody_23");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_23")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_23")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_23")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_23")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Character_Jade_Dead");
	m_vPlayerBody.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_23"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("PlayerBody_24");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_24")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_24")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_24")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_24")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Character_Jade_Dead");
	m_vPlayerBody.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_24"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("PlayerBody_25");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_25")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_25")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_25")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_25")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Character_Jade_Dead");
	m_vPlayerBody.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_25"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("PlayerBody_26");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_26")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_26")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_26")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_26")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Character_Jade_Dead");
	m_vPlayerBody.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_26"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("PlayerBody_27");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_27")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_27")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_27")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_27")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Character_Jade_Dead");
	m_vPlayerBody.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_27"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("PlayerBody_28");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_28")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_28")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_28")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_28")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Character_Jade_Dead");
	m_vPlayerBody.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_28"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("PlayerBody_29");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_29")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_29")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_29")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_29")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Character_Jade_Dead");
	m_vPlayerBody.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_29"));

	KunrealEngine::GetScene(m_sceneName)->CreateObject("PlayerBody_30");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_30")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_30")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_30")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_30")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Character_Jade_Dead");
	m_vPlayerBody.push_back(KunrealEngine::GetScene(m_sceneName)->GetGameObject("PlayerBody_30"));

	for (int i = 0; i < m_vPlayerBodyX.size(); i++)
	{
		if (i < 30)
		{
			m_vPlayerBody[i]->GetComponent<KunrealEngine::Renderer>()->InactivateComponent();
		}
	}
}

void GameClient::MapMatrix::SetPlayerBody()
{

	for (int i = 0; i < m_vPlayerBodyX.size(); i++)
	{
		if (i < 30)
		{
			// 1.055f - ((float)(m_vPlayerBodyX[i])) * 0.195f, 0.05f, 1.09f - ((float)(m_vPlayerBodyY[i] - 1)) * 0.195f
			// m_vPlayerBody[i]->GetComponent<KunrealEngine::Transform>()->SetPosition(0.75f - (m_vPlayerBodyX[i]) * 0.15f, 0.5f, 0.85f - (m_vPlayerBodyY[i] - 1) * 0.15f);
			m_vPlayerBody[i]->GetComponent<KunrealEngine::Transform>()->SetPosition(1.055f - ((float)(m_vPlayerBodyX[i])) * 0.195f, 0.015f, 1.09f - ((float)(m_vPlayerBodyY[i])) * 0.195f);
			m_vPlayerBody[i]->GetComponent<KunrealEngine::Renderer>()->ActivateComponent();

			switch (m_currentMapNumber)
			{
				case 1:
					if (m_mapXYStatus1[m_vPlayerBodyX[i]][m_vPlayerBodyY[i]] == WATER)
					{
						m_mapXYStatus1[m_vPlayerBodyX[i]][m_vPlayerBodyY[i]] = NORMAL;
					}
					break;
				case 2:
					if (m_mapXYStatus2[m_vPlayerBodyX[i]][m_vPlayerBodyY[i]] == WATER)
					{
						m_mapXYStatus2[m_vPlayerBodyX[i]][m_vPlayerBodyY[i]] = NORMAL;
					}
					break;
				case 3:
					if (m_mapXYStatus3[m_vPlayerBodyX[i]][m_vPlayerBodyY[i]] == WATER)
					{
						m_mapXYStatus3[m_vPlayerBodyX[i]][m_vPlayerBodyY[i]] = NORMAL;
					}
					break;
				default:
					break;
			}
		}
	}
}

void GameClient::MapMatrix::CreateMonsterObject()
{
	KunrealEngine::GetScene(m_sceneName)->CreateObject("Monster_1");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Skeleton");
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->AddComponent<MonsterAi>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->SetMonsterPos(100, 100);


	KunrealEngine::GetScene(m_sceneName)->CreateObject("Monster_2");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Skeleton");
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->AddComponent<MonsterAi>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<MonsterAi>()->SetMonsterPos(100, 100);

	KunrealEngine::GetScene(m_sceneName)->CreateObject("Monster_3");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Skeleton");
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->AddComponent<MonsterAi>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<MonsterAi>()->SetMonsterPos(100, 100);

	KunrealEngine::GetScene(m_sceneName)->CreateObject("Monster_4");
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject(rockName)->SetLayer(50);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	//KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->AddComponent<KunrealEngine::Renderer>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Skeleton");
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->AddComponent<MonsterAi>();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<MonsterAi>()->SetMonsterPos(100, 100);


}

void GameClient::MapMatrix::SetMonsterObject()
{
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->MonsterReset();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<MonsterAi>()->MonsterReset();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<MonsterAi>()->MonsterReset();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<MonsterAi>()->MonsterReset();

	switch (m_currentMapNumber)
	{
		case 1:
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->SetMonsterPos(100, 100);

			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<MonsterAi>()->SetMonsterPos(100, 100);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<MonsterAi>()->SetMonsterPos(100, 100);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<MonsterAi>()->SetMonsterPos(100, 100);
			break;

		case 2:
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->SetMonsterPos(7, 8);

			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<MonsterAi>()->SetMonsterPos(100, 100);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<MonsterAi>()->SetMonsterPos(100, 100);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<MonsterAi>()->SetMonsterPos(100, 100);
			break;

		case 3:
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->SetMonsterPos(100, 100);

			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<MonsterAi>()->SetMonsterPos(9, 3);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<MonsterAi>()->SetMonsterPos(6, 13);
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<MonsterAi>()->SetMonsterPos(10, 13);
			break;
		default:
			break;
	}
}

void GameClient::MapMatrix::ArrowAction()
{
	// 시이이이이이이이바아아아아아아아아아
	int arrowXPX = KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XPlus")->GetComponent<ArrowComponent>()->ReturnPositionX();
	int arrowXPY = KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XPlus")->GetComponent<ArrowComponent>()->ReturnPositionY();
	int arrowXMX = KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XMinus")->GetComponent<ArrowComponent>()->ReturnPositionX();
	int arrowXMY = KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XMinus")->GetComponent<ArrowComponent>()->ReturnPositionY();
	int arrowYPX = KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YPlus")->GetComponent<ArrowComponent>()->ReturnPositionX();
	int arrowYPY = KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YPlus")->GetComponent<ArrowComponent>()->ReturnPositionY();
	int arrowYMX = KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YMinus")->GetComponent<ArrowComponent>()->ReturnPositionX();
	int arrowYMY = KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YMinus")->GetComponent<ArrowComponent>()->ReturnPositionY();


	if (arrowXPY == m_currentMapY)
	{
		if (arrowXPX == m_currentMapX || arrowXPX + 1 == m_currentMapX || arrowXPX + 2 == m_currentMapX)	   // 오른쪽으로 가는 화살이 충돌 했을 때
		{
			if (KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_Sheild &&
				m_playerView == 4)
			{
				// 플레이어의 방패 기믹조건이 맞지 않으면 else로 간다
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Player_Guard.wav");

			}
			else
			{
				switch (m_currentMapNumber)		// 바위가 왼쪽에 없을 때 코로스
				{
					case 2:
						if (m_mapXYStatus2[m_currentMapX - 1][m_currentMapY] != ROCK)
						{
							m_playerLife = false;
						}
						break;
					case 3:
						if (m_mapXYStatus3[m_currentMapX - 1][m_currentMapY] != ROCK)
						{
							m_playerLife = false;
						}
						break;
					default:
						break;
				}
			}
		}
	}

	if (arrowXMY == m_currentMapY)
	{
		if (arrowXMX == m_currentMapX || arrowXMX - 1 == m_currentMapX || arrowXMX - 2 == m_currentMapX)
		{
			if (KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_Sheild &&
				m_playerView == 6)
			{
				// 플레이어의 방패 기믹조건이 맞지 않으면 else로 간다
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Player_Guard.wav");

			}
			else
			{
				if (m_mapXYStatus3[m_currentMapX + 1][m_currentMapY] != ROCK)
				{
					m_playerLife = false;
				}
			}
		}
	}

	if (arrowYPX == m_currentMapX)
	{
		if (arrowYPY == m_currentMapY || arrowYPY + 1 == m_currentMapY || arrowYPY + 2 == m_currentMapY)
		{
			if (KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_Sheild &&
				m_playerView == 2)
			{
				// 플레이어의 방패 기믹조건이 맞지 않으면 else로 간다
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Player_Guard.wav");

			}
			else
			{
				if (m_mapXYStatus3[m_currentMapX][m_currentMapY - 1] != ROCK)
				{
					m_playerLife = false;
				}
			}
		}
	}

	if (arrowYMX == m_currentMapX)
	{
		if (arrowYMY == m_currentMapY || arrowYMY - 1 == m_currentMapY || arrowYMY - 2 == m_currentMapY)
		{
			if (KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_Sheild &&
				m_playerView == 8)
			{
				// 플레이어의 방패 기믹조건이 맞지 않으면 else로 간다
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Player_Guard.wav");

			}
			else
			{
				switch (m_currentMapNumber)
				{
					case 1:
						if (m_mapXYStatus1[m_currentMapX][m_currentMapY + 1] != ROCK)
						{
							m_playerLife = false;
						}
						break;
					case 2:
						if (m_mapXYStatus2[m_currentMapX][m_currentMapY + 1] != ROCK)
						{
							m_playerLife = false;
						}
						break;
					case 3:
						if (m_mapXYStatus3[m_currentMapX][m_currentMapY + 1] != ROCK)
						{
							m_playerLife = false;
						}
						break;
					default:
						break;
				}
			}
		}
	}

}

void GameClient::MapMatrix::ArrowMove()
{
	// 시이이이이이이이바아아아아아아아아아
	int arrowXPX = KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XPlus")->GetComponent<ArrowComponent>()->ReturnPositionX();
	int arrowXPY = KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XPlus")->GetComponent<ArrowComponent>()->ReturnPositionY();
	int arrowXMX = KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XMinus")->GetComponent<ArrowComponent>()->ReturnPositionX();
	int arrowXMY = KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XMinus")->GetComponent<ArrowComponent>()->ReturnPositionY();
	int arrowYPX = KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YPlus")->GetComponent<ArrowComponent>()->ReturnPositionX();
	int arrowYPY = KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YPlus")->GetComponent<ArrowComponent>()->ReturnPositionY();
	int arrowYMX = KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YMinus")->GetComponent<ArrowComponent>()->ReturnPositionX();
	int arrowYMY = KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YMinus")->GetComponent<ArrowComponent>()->ReturnPositionY();



	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XPlus")->GetComponent<ArrowComponent>()->MoveForward();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XMinus")->GetComponent<ArrowComponent>()->MoveForward();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YPlus")->GetComponent<ArrowComponent>()->MoveForward();
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YMinus")->GetComponent<ArrowComponent>()->MoveForward();

	// 화살이 벽에 닿았을때 처리
	switch (m_currentMapNumber)
	{
		case 1:
			if (m_mapXYStatus1[arrowYMX][arrowYMY] == WALL || m_mapXYStatus1[arrowYMX][arrowYMY - 1] == WALL || m_mapXYStatus1[arrowYMX][arrowYMY - 2] == WALL ||
				m_mapXYStatus1[arrowYMX][arrowYMY] == ROCK || m_mapXYStatus1[arrowYMX][arrowYMY - 1] == ROCK || m_mapXYStatus1[arrowYMX][arrowYMY - 2] == ROCK)
			{
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YMinus")->GetComponent<ArrowComponent>()->SetPosition(2, 10);
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Trap_Arrow.wav");
				if (m_currentMapX == 2 && m_currentMapY == 10)
				{
					m_playerLife = false;
				}

			}

			// 플레이어에 겹쳤을때
			if (arrowYMX == m_currentMapX && arrowYMY == m_currentMapY)
			{
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YMinus")->GetComponent<ArrowComponent>()->SetPosition(2, 10);
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Trap_Arrow.wav");

			}
			if (arrowYMX == m_currentMapX && arrowYMY - 1 == m_currentMapY)
			{
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YMinus")->GetComponent<ArrowComponent>()->SetPosition(2, 10);
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Trap_Arrow.wav");

			}
			if (arrowYMX == m_currentMapX && arrowYMY - 2 == m_currentMapY)
			{
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YMinus")->GetComponent<ArrowComponent>()->SetPosition(2, 10);
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Trap_Arrow.wav");

			}
			break;
		case 2:
			if (m_mapXYStatus2[arrowXPX][arrowXPY] == WALL || m_mapXYStatus2[arrowXPX + 1][arrowXPY] == WALL || m_mapXYStatus2[arrowXPX + 2][arrowXPY] == WALL ||
				m_mapXYStatus2[arrowXPX][arrowXPY] == ROCK || m_mapXYStatus2[arrowXPX + 1][arrowXPY] == ROCK || m_mapXYStatus2[arrowXPX + 2][arrowXPY] == ROCK)
			{
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XPlus")->GetComponent<ArrowComponent>()->SetPosition(1, 2);
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Trap_Arrow.wav");
				if (m_currentMapX == 1 && m_currentMapY == 2)
				{
					m_playerLife = false;
				}
			}
			if (m_mapXYStatus2[arrowYMX][arrowYMY] == WALL || m_mapXYStatus2[arrowYMX][arrowYMY - 1] == WALL || m_mapXYStatus2[arrowYMX][arrowYMY - 2] == WALL ||
				m_mapXYStatus2[arrowYMX][arrowYMY] == ROCK || m_mapXYStatus2[arrowYMX][arrowYMY - 1] == ROCK || m_mapXYStatus2[arrowYMX][arrowYMY - 2] == ROCK)
			{
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YMinus")->GetComponent<ArrowComponent>()->SetPosition(1, 10);
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Trap_Arrow.wav");
				if (m_currentMapX == 1 && m_currentMapY == 10)
				{
					m_playerLife = false;
				}
			}

			// 플레이어에 겹쳤을때
			if (arrowXPX == m_currentMapX && arrowXPY == m_currentMapY)
			{
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XPlus")->GetComponent<ArrowComponent>()->SetPosition(1, 2);
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Trap_Arrow.wav");

			}
			if (arrowYMX == m_currentMapX && arrowYMY == m_currentMapY)
			{
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YMinus")->GetComponent<ArrowComponent>()->SetPosition(1, 10);
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Trap_Arrow.wav");

			}
			if (arrowXPX + 1 == m_currentMapX && arrowXPY == m_currentMapY)
			{
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XPlus")->GetComponent<ArrowComponent>()->SetPosition(1, 2);
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Trap_Arrow.wav");
			}
			if (arrowXPX + 2 == m_currentMapX && arrowXPY == m_currentMapY)
			{
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XPlus")->GetComponent<ArrowComponent>()->SetPosition(1, 2);
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Trap_Arrow.wav");
			}
			if (arrowYMX == m_currentMapX && arrowYMY - 1 == m_currentMapY)
			{
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YMinus")->GetComponent<ArrowComponent>()->SetPosition(1, 10);
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Trap_Arrow.wav");
			}
			if (arrowYMX == m_currentMapX && arrowYMY - 2 == m_currentMapY)
			{
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YMinus")->GetComponent<ArrowComponent>()->SetPosition(1, 10);
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Trap_Arrow.wav");
			}

			break;
		case 3:
			if (m_mapXYStatus3[arrowXPX][arrowXPY] == WALL || m_mapXYStatus3[arrowXPX + 1][arrowXPY] == WALL || m_mapXYStatus3[arrowXPX + 2][arrowXPY] == WALL ||
				m_mapXYStatus3[arrowXPX][arrowXPY] == ROCK || m_mapXYStatus3[arrowXPX + 1][arrowXPY] == ROCK || m_mapXYStatus3[arrowXPX + 2][arrowXPY] == ROCK)
			{
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XPlus")->GetComponent<ArrowComponent>()->SetPosition(1, 5);
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Trap_Arrow.wav");
				if (m_currentMapX == 1 && m_currentMapY == 5)
				{
					m_playerLife = false;
				}
			}
			if (m_mapXYStatus3[arrowXMX][arrowXMY] == WALL || m_mapXYStatus3[arrowXMX - 1][arrowXMY] == WALL || m_mapXYStatus3[arrowXMX - 2][arrowXMY] == WALL ||
				m_mapXYStatus3[arrowXMX][arrowXMY] == ROCK || m_mapXYStatus3[arrowXMX - 1][arrowXMY] == ROCK || m_mapXYStatus3[arrowXMX - 2][arrowXMY] == ROCK)
			{
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XMinus")->GetComponent<ArrowComponent>()->SetPosition(15, 7);
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Trap_Arrow.wav");
				if (m_currentMapX == 15 && m_currentMapY == 7)
				{
					m_playerLife = false;
				}
			}
			if (m_mapXYStatus3[arrowYPX][arrowYPY] == WALL || m_mapXYStatus3[arrowYPX][arrowYPY + 1] == WALL || m_mapXYStatus3[arrowYPX][arrowYPY + 2] == WALL ||
				m_mapXYStatus3[arrowYPX][arrowYPY] == ROCK || m_mapXYStatus3[arrowYPX][arrowYPY + 1] == ROCK || m_mapXYStatus3[arrowYPX][arrowYPY + 2] == ROCK)
			{
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YPlus")->GetComponent<ArrowComponent>()->SetPosition(6, 1);
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Trap_Arrow.wav");
				if (m_currentMapX == 6 && m_currentMapY == 1)
				{
					m_playerLife = false;
				}
			}
			if (m_mapXYStatus3[arrowYMX][arrowYMY] == WALL || m_mapXYStatus3[arrowYMX][arrowYMY - 1] == WALL || m_mapXYStatus3[arrowYMX][arrowYMY - 2] == WALL ||
				m_mapXYStatus3[arrowYMX][arrowYMY] == ROCK || m_mapXYStatus3[arrowYMX][arrowYMY - 1] == ROCK || m_mapXYStatus3[arrowYMX][arrowYMY - 2] == ROCK)
			{
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YMinus")->GetComponent<ArrowComponent>()->SetPosition(2, 15);
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Trap_Arrow.wav");
				if (m_currentMapX == 2 && m_currentMapY == 15)
				{
					m_playerLife = false;
				}
			}

			// 플레이어에 겹쳤을때
			if (arrowXPX == m_currentMapX && arrowXPY == m_currentMapY)
			{
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XPlus")->GetComponent<ArrowComponent>()->SetPosition(1, 5);
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Trap_Arrow.wav");
			}
			if (arrowXMX == m_currentMapX && arrowXMY == m_currentMapY)
			{
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XMinus")->GetComponent<ArrowComponent>()->SetPosition(15, 7);
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Trap_Arrow.wav");
			}
			if (arrowYPX == m_currentMapX && arrowYPY == m_currentMapY)
			{
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YPlus")->GetComponent<ArrowComponent>()->SetPosition(6, 1);
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Trap_Arrow.wav");
			}
			if (arrowYMX == m_currentMapX && arrowYMY == m_currentMapY)
			{
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YMinus")->GetComponent<ArrowComponent>()->SetPosition(2, 15);
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Trap_Arrow.wav");
			}
			if (arrowXPX + 1 == m_currentMapX && arrowXPY == m_currentMapY)
			{
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XPlus")->GetComponent<ArrowComponent>()->SetPosition(1, 5);
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Trap_Arrow.wav");
			}
			if (arrowXMX - 1 == m_currentMapX && arrowXMY == m_currentMapY)
			{
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XMinus")->GetComponent<ArrowComponent>()->SetPosition(15, 7);
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Trap_Arrow.wav");
			}
			if (arrowYPX == m_currentMapX && arrowYPY + 1 == m_currentMapY)
			{
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YPlus")->GetComponent<ArrowComponent>()->SetPosition(6, 1);
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Trap_Arrow.wav");
			}
			if (arrowYMX == m_currentMapX && arrowYMY - 1 == m_currentMapY)
			{
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YMinus")->GetComponent<ArrowComponent>()->SetPosition(2, 15);
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Trap_Arrow.wav");
			}
			if (arrowXPX + 2 == m_currentMapX && arrowXPY == m_currentMapY)
			{
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XPlus")->GetComponent<ArrowComponent>()->SetPosition(1, 5);
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Trap_Arrow.wav");
			}
			if (arrowXMX - 2 == m_currentMapX && arrowXMY == m_currentMapY)
			{
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_XMinus")->GetComponent<ArrowComponent>()->SetPosition(15, 7);
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Trap_Arrow.wav");
			}
			if (arrowYPX == m_currentMapX && arrowYPY + 2 == m_currentMapY)
			{
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YPlus")->GetComponent<ArrowComponent>()->SetPosition(6, 1);
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Trap_Arrow.wav");
			}
			if (arrowYMX == m_currentMapX && arrowYMY - 2 == m_currentMapY)
			{
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Arrow_YMinus")->GetComponent<ArrowComponent>()->SetPosition(2, 15);
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Trap_Arrow.wav");
			}

			break;
		default:
			break;
	}

}

void GameClient::MapMatrix::MonsterTouch()
{
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->GetPlayerPos(m_currentMapX, m_currentMapY);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<MonsterAi>()->GetPlayerPos(m_currentMapX, m_currentMapY);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<MonsterAi>()->GetPlayerPos(m_currentMapX, m_currentMapY);
	KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<MonsterAi>()->GetPlayerPos(m_currentMapX, m_currentMapY);


	if (KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->MonsterDieCheck() != true)
	{
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->MonsterPosInitialize();

		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->
			MonsterFollowPlayer(MonsterFrontTileCheck(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->MonsterPosition(), 0, 1),
				MonsterFrontTileCheck(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->MonsterPosition(), -1, 0),
				MonsterFrontTileCheck(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->MonsterPosition(), 0, -1),
				MonsterFrontTileCheck(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->MonsterPosition(), 1, 0));

		if (KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->PlayerSearch() == true)
			KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Monster_Move.wav");
	}

	if (KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<MonsterAi>()->MonsterDieCheck() != true)
	{
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<MonsterAi>()->MonsterPosInitialize();

		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<MonsterAi>()	->
			MonsterFollowPlayer(MonsterFrontTileCheck(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<MonsterAi>()->MonsterPosition(), 0, 1),
				MonsterFrontTileCheck(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<MonsterAi>()->MonsterPosition(), -1, 0),
				MonsterFrontTileCheck(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<MonsterAi>()->MonsterPosition(), 0, -1),
				MonsterFrontTileCheck(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<MonsterAi>()->MonsterPosition(), 1, 0));
		if (KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<MonsterAi>()->PlayerSearch() == true)
			KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Monster_Move.wav");
	}
	if (KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<MonsterAi>()->MonsterDieCheck() != true)
	{
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<MonsterAi>()->MonsterPosInitialize();

		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<MonsterAi>()	->
			MonsterFollowPlayer(MonsterFrontTileCheck(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<MonsterAi>()->MonsterPosition(), 0, 1),
				MonsterFrontTileCheck(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<MonsterAi>()->MonsterPosition(), -1, 0),
				MonsterFrontTileCheck(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<MonsterAi>()->MonsterPosition(), 0, -1),
				MonsterFrontTileCheck(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<MonsterAi>()->MonsterPosition(), 1, 0));
		if (KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<MonsterAi>()->PlayerSearch() == true)
			KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Monster_Move.wav");
	}
	if (KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<MonsterAi>()->MonsterDieCheck() != true)
	{
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<MonsterAi>()->MonsterPosInitialize();

		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<MonsterAi>()	->
			MonsterFollowPlayer(MonsterFrontTileCheck(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<MonsterAi>()->MonsterPosition(), 0, 1),
				MonsterFrontTileCheck(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<MonsterAi>()->MonsterPosition(), -1, 0),
				MonsterFrontTileCheck(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<MonsterAi>()->MonsterPosition(), 0, -1),
				MonsterFrontTileCheck(KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<MonsterAi>()->MonsterPosition(), 1, 0));
		if (KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<MonsterAi>()->PlayerSearch() == true)
			KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Monster_Move.wav");
	}

	if (KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->MonsterPosition().newX == m_currentMapX &&
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->MonsterPosition().newY == m_currentMapY)
	{
		m_playerLife = false;
	}
	if (KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<MonsterAi>()->MonsterPosition().newX == m_currentMapX &&
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<MonsterAi>()->MonsterPosition().newY == m_currentMapY)
	{
		m_playerLife = false;
	}
	if (KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<MonsterAi>()->MonsterPosition().newX == m_currentMapX &&
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<MonsterAi>()->MonsterPosition().newY == m_currentMapY)
	{
		m_playerLife = false;
	}
	if (KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<MonsterAi>()->MonsterPosition().newX == m_currentMapX &&
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<MonsterAi>()->MonsterPosition().newY == m_currentMapY)
	{
		m_playerLife = false;
	}

}

void GameClient::MapMatrix::MonsterMove()
{
	if (m_mapXYStatus2[KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->MonsterPosition().newX]
		[KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->MonsterPosition().newY] == WATER)
	{
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->MonsterDie();
		TileChange(2,
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->MonsterPosition().nowX,
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->MonsterPosition().nowY,
			NORMAL);
	}
	if (m_mapXYStatus3[KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<MonsterAi>()->MonsterPosition().newX]
		[KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<MonsterAi>()->MonsterPosition().newY] == WATER)
	{
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<MonsterAi>()->MonsterDie();
		TileChange(3,
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<MonsterAi>()->MonsterPosition().nowX,
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<MonsterAi>()->MonsterPosition().nowY,
			NORMAL);
	}
	if (m_mapXYStatus3[KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<MonsterAi>()->MonsterPosition().newX]
		[KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<MonsterAi>()->MonsterPosition().newY] == WATER)
	{
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<MonsterAi>()->MonsterDie();
		TileChange(3,
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<MonsterAi>()->MonsterPosition().nowX,
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<MonsterAi>()->MonsterPosition().nowY,
			NORMAL);
	}
	if (m_mapXYStatus3[KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<MonsterAi>()->MonsterPosition().newX]
		[KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<MonsterAi>()->MonsterPosition().newY] == WATER)
	{
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<MonsterAi>()->MonsterDie();
		TileChange(3,
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<MonsterAi>()->MonsterPosition().nowX,
			KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<MonsterAi>()->MonsterPosition().nowY,
			NORMAL);
	}

	switch (m_currentMapNumber)
	{
		// 몬스터가 트랩에도 갈 수 있다면
// 		if (m_mapXYStatus2[KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->MonsterPosition().nowX]
// 			[KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->MonsterPosition().nowY] == WATER ||
// 			m_mapXYStatus2[KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->MonsterPosition().nowX]
// 			[KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->MonsterPosition().nowY] == TRAP ||
// 			m_mapXYStatus2[KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->MonsterPosition().nowX]
// 			[KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->MonsterPosition().nowY] == TRAPOFF)


		case 2:

			if (m_mapXYStatus2[KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->MonsterPosition().newX]
				[KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->MonsterPosition().newY] == WATER)
			{
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->MonsterDie();
			}
			else
			{
				if (m_mapXYStatus2[KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->MonsterPosition().nowX]
					[KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->MonsterPosition().nowY] == WATER)
				{
					// 작동이 안되어야 함
				}
				else
				{
					TileChange(2,
						KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->MonsterPosition().nowX,
						KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->MonsterPosition().nowY,
						NORMAL);
					TileChange(2,
						KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->MonsterPosition().newX,
						KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->MonsterPosition().newY,
						MONSTER);
				}
			}
			break;

		case 3:
			if (m_mapXYStatus3[KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<MonsterAi>()->MonsterPosition().newX]
				[KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<MonsterAi>()->MonsterPosition().newY] == WATER)
			{
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<MonsterAi>()->MonsterDie();
			}
			else
			{
				if (m_mapXYStatus3[KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<MonsterAi>()->MonsterPosition().nowX]
					[KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<MonsterAi>()->MonsterPosition().nowY] == WATER)
				{
					// 작동이 안되어야 함
				}
				else
				{
					TileChange(3,
						KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<MonsterAi>()->MonsterPosition().nowX,
						KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<MonsterAi>()->MonsterPosition().nowY,
						NORMAL);
				}
			}

			if (m_mapXYStatus3[KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<MonsterAi>()->MonsterPosition().newX]
				[KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<MonsterAi>()->MonsterPosition().newY] == WATER)
			{
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<MonsterAi>()->MonsterDie();
			}
			else
			{
				if (m_mapXYStatus3[KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<MonsterAi>()->MonsterPosition().nowX]
					[KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<MonsterAi>()->MonsterPosition().nowY] == WATER)
				{
					// 작동이 안되어야 함
				}
				else
				{
					TileChange(3,
						KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<MonsterAi>()->MonsterPosition().nowX,
						KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<MonsterAi>()->MonsterPosition().nowY,
						NORMAL);
				}
			}

			if (m_mapXYStatus3[KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<MonsterAi>()->MonsterPosition().newX]
				[KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<MonsterAi>()->MonsterPosition().newY] == WATER)
			{
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<MonsterAi>()->MonsterDie();
			}
			else
			{
				if (m_mapXYStatus3[KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<MonsterAi>()->MonsterPosition().nowX]
					[KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<MonsterAi>()->MonsterPosition().nowY] == WATER)
				{
					// 작동이 안되어야 함
				}
				else
				{
					TileChange(3,
						KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<MonsterAi>()->MonsterPosition().nowX,
						KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<MonsterAi>()->MonsterPosition().nowY,
						NORMAL);
				}
			}

			TileChange(3,
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<MonsterAi>()->MonsterPosition().newX,
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<MonsterAi>()->MonsterPosition().newY,
				MONSTER);
			TileChange(3,
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<MonsterAi>()->MonsterPosition().newX,
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<MonsterAi>()->MonsterPosition().newY,
				MONSTER);
			TileChange(3,
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<MonsterAi>()->MonsterPosition().newX,
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<MonsterAi>()->MonsterPosition().newY,
				MONSTER);
			break;

		default:
			break;
	}

	if (KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->MonsterDieCheck() == true)
	{
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent <KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		if (KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_1")->GetComponent<MonsterAi>()->PlayerSearch() == true)
			KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Monster_Die_Water.wav");
	}
	if (KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<MonsterAi>()->MonsterDieCheck() == true)
	{
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent <KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		if (KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_2")->GetComponent<MonsterAi>()->PlayerSearch() == true)
			KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Monster_Die_Water.wav");
	}
	if (KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<MonsterAi>()->MonsterDieCheck() == true)
	{
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent <KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		if (KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_3")->GetComponent<MonsterAi>()->PlayerSearch() == true)
			KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Monster_Die_Water.wav");
	}
	if (KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<MonsterAi>()->MonsterDieCheck() == true)
	{
		KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent <KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
		if (KunrealEngine::GetScene(m_sceneName)->GetGameObject("Monster_4")->GetComponent<MonsterAi>()->PlayerSearch() == true)
			KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Monster_Die_Water.wav");
	}
}

KunrealEngine::GameObject* GameClient::MapMatrix::SearchRockObjectInVector(int posX, int posY)
{
	switch (m_currentMapNumber)
	{
		case 1:
			for (auto x : m_vRock1)
			{
				if (x->GetComponent<GameClient::RockComponent>()->ComparePosition(posX, posY))
				{
					return x;
				}
			}
			break;
		case 2:
			for (auto x : m_vRock2)
			{
				if (x->GetComponent<GameClient::RockComponent>()->ComparePosition(posX, posY))
				{
					return x;
				}
			}
			break;
		case 3:
			for (auto x : m_vRock3)
			{
				if (x->GetComponent<GameClient::RockComponent>()->ComparePosition(posX, posY))
				{
					return x;
				}
			}
			break;
		default:
			break;
	}
	return nullptr;
}

bool GameClient::MapMatrix::PushRock(int creaseX, int creaseY)
{
	if (!KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_glove)
	{
		return false;
	}
	switch (m_currentMapNumber)
	{
		case 1:
			// 지면에 밀때
			if (m_mapXYStatus1[m_currentMapX + creaseX + creaseX][m_currentMapY + creaseY + creaseY] == NORMAL
				|| m_mapXYStatus1[m_currentMapX + creaseX + creaseX][m_currentMapY + creaseY + creaseY] == TRAPOFF)
			{
				m_mapXYStatus1[m_currentMapX + creaseX + creaseX][m_currentMapY + creaseY + creaseY] = ROCK;
				m_mapXYStatus1[m_currentMapX + creaseX][m_currentMapY + creaseY] = NORMAL;

				if (SearchRockObjectInVector(m_currentMapX + creaseX, m_currentMapY + creaseY) != nullptr)
				{
					SearchRockObjectInVector(m_currentMapX + creaseX, m_currentMapY + creaseY)->GetComponent<RockComponent>()->SetPosition(m_currentMapX + creaseX + creaseX, m_currentMapY + creaseY + creaseY);
					KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Player_Rock_Push.wav");

				}

				return true;
			}
			// 물에 밀때
			if (m_mapXYStatus1[m_currentMapX + creaseX + creaseX][m_currentMapY + creaseY + creaseY] == WATER)
			{
				m_mapXYStatus1[m_currentMapX + creaseX + creaseX][m_currentMapY + creaseY + creaseY] = NORMAL;
				m_mapXYStatus1[m_currentMapX + creaseX][m_currentMapY + creaseY] = NORMAL;

				if (SearchRockObjectInVector(m_currentMapX + creaseX, m_currentMapY + creaseY) != nullptr)
				{
					SearchRockObjectInVector(m_currentMapX + creaseX, m_currentMapY + creaseY)->GetComponent<RockComponent>()->AtWater(m_currentMapX + creaseX + creaseX, m_currentMapY + creaseY + creaseY);
					KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Player_Rock_Push.wav");

				}

				return true;
			}
			break;

		case 2:
			// 지면에 밀때
			if (m_mapXYStatus2[m_currentMapX + creaseX + creaseX][m_currentMapY + creaseY + creaseY] == NORMAL
				|| m_mapXYStatus1[m_currentMapX + creaseX + creaseX][m_currentMapY + creaseY + creaseY] == TRAPOFF)
			{
				m_mapXYStatus2[m_currentMapX + creaseX + creaseX][m_currentMapY + creaseY + creaseY] = ROCK;
				m_mapXYStatus2[m_currentMapX + creaseX][m_currentMapY + creaseY] = NORMAL;

				if (SearchRockObjectInVector(m_currentMapX + creaseX, m_currentMapY + creaseY) != nullptr)
				{
					SearchRockObjectInVector(m_currentMapX + creaseX, m_currentMapY + creaseY)->GetComponent<RockComponent>()->SetPosition(m_currentMapX + creaseX + creaseX, m_currentMapY + creaseY + creaseY);
					KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Player_Rock_Push.wav");

				}

				return true;
			}
			// 물에 밀때
			if (m_mapXYStatus2[m_currentMapX + creaseX + creaseX][m_currentMapY + creaseY + creaseY] == WATER)
			{
				m_mapXYStatus2[m_currentMapX + creaseX + creaseX][m_currentMapY + creaseY + creaseY] = NORMAL;
				m_mapXYStatus2[m_currentMapX + creaseX][m_currentMapY + creaseY] = NORMAL;

				if (SearchRockObjectInVector(m_currentMapX + creaseX, m_currentMapY + creaseY) != nullptr)
				{
					SearchRockObjectInVector(m_currentMapX + creaseX, m_currentMapY + creaseY)->GetComponent<RockComponent>()->AtWater(m_currentMapX + creaseX + creaseX, m_currentMapY + creaseY + creaseY);
					KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Player_Rock_Push.wav");

				}

				return true;
			}
			break;

		case 3:
			// 지면에 밀때
			if (m_mapXYStatus3[m_currentMapX + creaseX + creaseX][m_currentMapY + creaseY + creaseY] == NORMAL
				|| m_mapXYStatus1[m_currentMapX + creaseX + creaseX][m_currentMapY + creaseY + creaseY] == TRAPOFF)
			{
				m_mapXYStatus3[m_currentMapX + creaseX + creaseX][m_currentMapY + creaseY + creaseY] = ROCK;
				m_mapXYStatus3[m_currentMapX + creaseX][m_currentMapY + creaseY] = NORMAL;

				if (SearchRockObjectInVector(m_currentMapX + creaseX, m_currentMapY + creaseY) != nullptr)
				{
					SearchRockObjectInVector(m_currentMapX + creaseX, m_currentMapY + creaseY)->GetComponent<RockComponent>()->SetPosition(m_currentMapX + creaseX + creaseX, m_currentMapY + creaseY + creaseY);
					KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Player_Rock_Push.wav");

				}

				return true;
			}
			// 물에 밀때
			if (m_mapXYStatus3[m_currentMapX + creaseX + creaseX][m_currentMapY + creaseY + creaseY] == WATER)
			{
				m_mapXYStatus3[m_currentMapX + creaseX + creaseX][m_currentMapY + creaseY + creaseY] = NORMAL;
				m_mapXYStatus3[m_currentMapX + creaseX][m_currentMapY + creaseY] = NORMAL;

				if (SearchRockObjectInVector(m_currentMapX + creaseX, m_currentMapY + creaseY) != nullptr)
				{
					SearchRockObjectInVector(m_currentMapX + creaseX, m_currentMapY + creaseY)->GetComponent<RockComponent>()->AtWater(m_currentMapX + creaseX + creaseX, m_currentMapY + creaseY + creaseY);
					KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Player_Rock_Push.wav");

				}

				return true;
			}
			break;
	}
	return false;
}

bool GameClient::MapMatrix::GetEquipment(int creaseX, int creaseY)
{
	switch (m_currentMapNumber)
	{
		case 1:
			if (m_currentMapX + creaseX == 7 && m_currentMapY + creaseY == 1)
			{
				if (KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_getGlove)
				{
					return true;
				}
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->GetGlove();
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Glove")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
			}

			if (m_currentMapX + creaseX == 7 && m_currentMapY + creaseY == 2)
			{
				if (KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_getSheild)
				{
					return true;
				}
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->GetSheild();
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Sheild")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
			}
			break;
		case 2:
			if (m_currentMapX + creaseX == 8 && m_currentMapY + creaseY == 3)
			{
				if (KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_rope)
				{
					return true;
				}
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->GetRope();
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Rope")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
			}
			break;
		case 3:
			break;
		default:
			return  false;
			break;
	}
	return true;
}

bool GameClient::MapMatrix::GetRopeSurport(int creaseX, int creaseY)
{
	switch (m_currentMapNumber)
	{
		case 2:
			if (KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_ropeSurport == 0)
			{
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->GetRopeSurport();
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("RopeSurport_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
				KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->SetRopeSurport(KunrealEngine::GetScene(m_sceneName)->GetGameObject("UsingRopeSurport_1"));

				TileChange(2, 4, 3, NORMAL);
			}
			break;
		case 3:
			if (m_currentMapX + creaseX == 3 && m_currentMapY + creaseY == 8)
			{
				if (!m_ropeSurport_1)
				{
					KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->GetRopeSurport();
					KunrealEngine::GetScene(m_sceneName)->GetGameObject("RopeSurport_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
					m_ropeSurport_1 = true;
					TileChange(2, 3, 8, NORMAL);
					KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->SetRopeSurport(KunrealEngine::GetScene(m_sceneName)->GetGameObject("UsingRopeSurport_2"));

				}
			}

			if (m_currentMapX + creaseX == 10 && m_currentMapY + creaseY == 1)
			{
				if (!m_ropeSurport_2)
				{
					KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->GetRopeSurport();
					KunrealEngine::GetScene(m_sceneName)->GetGameObject("RopeSurport_3")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
					m_ropeSurport_2 = true;
					TileChange(2, 10, 1, NORMAL);
					KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->SetRopeSurport(KunrealEngine::GetScene(m_sceneName)->GetGameObject("UsingRopeSurport_3"));

				}
			}

			if (m_currentMapX + creaseX == 11 && m_currentMapY + creaseY == 6)
			{
				if (!m_ropeSurport_3)
				{
					KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->GetRopeSurport();
					KunrealEngine::GetScene(m_sceneName)->GetGameObject("RopeSurport_4")->GetComponent<KunrealEngine::Transform>()->SetPosition(100.f, 0.05f, 100.f);
					m_ropeSurport_3 = true;
					TileChange(2, 11, 6, NORMAL);
					KunrealEngine::GetScene(m_sceneName)->GetGameObject("Player")->GetComponent<PlayerComponent>()->SetRopeSurport(KunrealEngine::GetScene(m_sceneName)->GetGameObject("UsingRopeSurport_4"));

				}
			}
			break;
		default:
			return  false;
			break;
	}
	return true;
}

bool GameClient::MapMatrix::RopeCheck(int creasX, int creaseY)
{
	switch (m_currentMapNumber)
	{
		case 2:
			for (int checkRopeCount = 1; checkRopeCount < 15; checkRopeCount++)
			{
				if (m_mapXYStatus2[m_currentMapX + creasX * checkRopeCount][m_currentMapY + creaseY * checkRopeCount] == ROCK ||
					m_mapXYStatus2[m_currentMapX + creasX * checkRopeCount][m_currentMapY + creaseY * checkRopeCount] == WALL)
				{
					return false;
				}

				if (m_mapXYStatus2[m_currentMapX + creasX * checkRopeCount][m_currentMapY + creaseY * checkRopeCount] == ROPESURPORTBUILD)
				{
					for (int actionRopeCount = 1; actionRopeCount <= checkRopeCount; actionRopeCount++)
					{
						if (m_mapXYStatus2[m_currentMapX + creasX * (checkRopeCount + actionRopeCount)][m_currentMapY + creaseY * (checkRopeCount + actionRopeCount)] == ROCK ||
							m_mapXYStatus2[m_currentMapX + creasX * (checkRopeCount + actionRopeCount)][m_currentMapY + creaseY * (checkRopeCount + actionRopeCount)] == WALL)
						{
							m_currentMapX = m_currentMapX + creasX * (checkRopeCount + actionRopeCount - 2);
							m_currentMapY = m_currentMapY + creaseY * (checkRopeCount + actionRopeCount - 2);
							m_playerLife = false;
							KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Player_Crash.wav");

							return false;
						}
					}
					if (m_mapXYStatus2[m_currentMapX + creasX * checkRopeCount * 2][m_currentMapY + creaseY * checkRopeCount * 2] == WATER ||
					m_mapXYStatus2[m_currentMapX + creasX * checkRopeCount * 2][m_currentMapY + creaseY * checkRopeCount * 2] == MONSTER)
					{
						m_currentMapX = m_currentMapX + creasX * checkRopeCount * 2;
						m_currentMapY = m_currentMapY + creaseY * checkRopeCount * 2;
						m_playerLife = false;

						return false;
					}
					//KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->GetComponent<KunrealEngine::Transform>()->Translate(0.2f * checkRopeCount * creasX, 0.f, 0.2f * checkRopeCount * creaseY);
					m_currentMapX = m_currentMapX + creasX * checkRopeCount * 2;
					m_currentMapY = m_currentMapY + creaseY * checkRopeCount * 2;
					KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Player_Rope_Use.wav");

					return true;
				}
			}
			break;

		case 3:
			for (int checkRopeCount = 1; checkRopeCount < 15; checkRopeCount++)
			{
				if (m_mapXYStatus3[m_currentMapX + creasX * checkRopeCount][m_currentMapY + creaseY * checkRopeCount] == ROCK ||
					m_mapXYStatus3[m_currentMapX + creasX * checkRopeCount][m_currentMapY + creaseY * checkRopeCount] == WALL)
				{
					return false;
				}

				if (m_mapXYStatus3[m_currentMapX + creasX * checkRopeCount][m_currentMapY + creaseY * checkRopeCount] == ROPESURPORTBUILD)
				{
					for (int actionRopeCount = 1; actionRopeCount <= checkRopeCount; actionRopeCount++)
					{
						if (m_mapXYStatus3[m_currentMapX + creasX * (checkRopeCount + actionRopeCount)][m_currentMapY + creaseY * (checkRopeCount + actionRopeCount)] == ROCK ||
							m_mapXYStatus3[m_currentMapX + creasX * (checkRopeCount + actionRopeCount)][m_currentMapY + creaseY * (checkRopeCount + actionRopeCount)] == WALL)
						{
							m_currentMapX = m_currentMapX + creasX * (checkRopeCount + actionRopeCount - 2);
							m_currentMapY = m_currentMapY + creaseY * (checkRopeCount + actionRopeCount - 2);
							m_playerLife = false;
							KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Player_Crash.wav");

							return false;
						}
					}
					if (m_mapXYStatus3[m_currentMapX + creasX * checkRopeCount * 2][m_currentMapY + creaseY * checkRopeCount * 2] == WATER ||
						m_mapXYStatus3[m_currentMapX + creasX * checkRopeCount * 2][m_currentMapY + creaseY * checkRopeCount * 2] == MONSTER)
					{
						m_currentMapX = m_currentMapX + creasX * checkRopeCount * 2;
						m_currentMapY = m_currentMapY + creaseY * checkRopeCount * 2;
						m_playerLife = false;

						return false;
					}
					//KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->GetComponent<KunrealEngine::Transform>()->Translate(0.2f * checkRopeCount * creasX, 0.f, 0.2f * checkRopeCount * creaseY);
					m_currentMapX = m_currentMapX + creasX * checkRopeCount * 2;
					m_currentMapY = m_currentMapY + creaseY * checkRopeCount * 2;
					KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Player_Rope_Use.wav");

					return true;
				}
			}
			break;

		default:
			break;
	}
	return false;
}
