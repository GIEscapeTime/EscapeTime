#include "MonsterAi.h"


#include <random>

GameClient::MonsterAi::MonsterAi()
	:m_MonsterLookTileX(0), m_MonsterLookTileY(0), m_PlayerPosX(0), m_PlayerPosY(0), m_PlayerSearch(false), isMonsterDie(false)
	
{

}

GameClient::MonsterAi::~MonsterAi()
{

}

void GameClient::MonsterAi::Initialize()
{

}

void GameClient::MonsterAi::Finalize()
{

}

void GameClient::MonsterAi::FixedUpdate()
{

}

void GameClient::MonsterAi::Update()
{
	if (isMonsterDie != true)
	{
		//0.8f - ((float)(m_monsterTilelist.newX)) * 0.15, 0.05f, 0.85f - ((float)(m_monsterTilelist.newY)) * 0.15f
		this->GetOwner()->GetComponent <KunrealEngine::Transform>()->SetPosition(1.055f - ((float)(m_monsterTilelist.newX)) * 0.195f, 0.05f, 1.1f - ((float)(m_monsterTilelist.newY)) * 0.195f);
	}
}

void GameClient::MonsterAi::LateUpdate()
{
	
}


void GameClient::MonsterAi::GetPlayerPos(int x, int y) //여기  xy는 바뀌는데 m_PlayerPosX Y는 안바뀌는중
{
	m_PlayerPosX = x;
	m_PlayerPosY = y;
}

void GameClient::MonsterAi::SetMonsterPos(int posX, int posY)
{
	m_monsterTilelist.nowX = posX;
	m_monsterTilelist.nowY = posY;
	m_monsterTilelist.newX = posX; // 처음 설정할때의 오류를 막기 위한 초기화
	m_monsterTilelist.newY = posY;
}

int GameClient::MonsterAi::RandomNextMove() // 1,0을 랜덤하게 반환
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 1);

	return dis(gen);
}


void GameClient::MonsterAi::MonsterDie()
{
	isMonsterDie = true;
}




void GameClient::MonsterAi::MonsterReset()
{
	isMonsterDie = false;
}

void GameClient::MonsterAi::MonsterPosInitialize()
{
	m_monsterTilelist.nowX = m_monsterTilelist.newX;						// 이제 진짜 타일을 받아와야 할것인데....
	m_monsterTilelist.nowY = m_monsterTilelist.newY;						// 원래는 여기 있으면 안되지만 기능상 문제는 없을것
}

bool GameClient::MonsterAi::MonsterDieCheck() // 몬스터가 죽었으면 true를 반환
{
	if (isMonsterDie == true)
	{
		return true;
	}
	return false;
}



GameClient::MonsterTileIndex GameClient::MonsterAi::MonsterPosition()
// 몬스터의 위치를 반환하는 함수 사용처는 아직 없지만 임시로 만들어둠
{
	return m_monsterTilelist;
}

bool GameClient::MonsterAi::PlayerSearch()
{
	int sightRange = 3;

	for (int y = m_monsterTilelist.nowY - sightRange; y <= m_monsterTilelist.nowY + sightRange; ++y)  // 최대 크기인 3을 사용
	{
		int xOffset = sightRange - abs(y - m_monsterTilelist.nowY);  // abs는 절댓값으로 반환하는 부분,  xOffset은 멀어질수록 점점 시야각이 좁아지도록 설정
		// 
		for (int x = m_monsterTilelist.nowX - xOffset; x <= m_monsterTilelist.nowX + xOffset; ++x)
		{												 // 플레이어 위치가 확인되면 트루로
			if(m_PlayerPosX == x && m_PlayerPosY == y)
			{
				return true;
			}		
		}
	}

	return false;
}


void GameClient::MonsterAi::MonsterFollowPlayer(GameClient::MapStatus statusW, GameClient::MapStatus statusA, GameClient::MapStatus statusS, GameClient::MapStatus statusD )
{	// 몬스터 앞의 타일 정보, 이동할때의 WASD를 활용해서 구분을 하였음
	// 일정 범위에 있는 오브젝트의 태크를 체크해서 플레이어면 따라가도록 할것
	// 1. 길찾기가 필요함
	// 2. 범위 체크와 그에 따른 태크 체크도 필요함


	// 예외처리 할것 : 몬스터 전방 좌우가 이동 불가 타일일 경우 = 이동하지않는다
	// 몬스터의 전방과 이동해야 하는 타일, 2개가 막혀있더라도 이동하지않는다.
	// 몬스터 전방위 타일의 정보가 필요하다

	int sightRange = 3;
	  // 이동 할 타일의 속성을 담음

	
	// 기존에 분리해둔 new 와 now를 여기서 다시 합쳐서 초기화
	
	if (PlayerSearch() == true)  // 플레이어가 범위 안에 들어옴, 이 부분 매개변수로 갖고와서 일단 넣었는데, 문제 있음, 전체 타일 정보를 가져와야하는데
	{
		for (int x = m_monsterTilelist.nowX - sightRange; x <= m_monsterTilelist.nowX + sightRange; ++x)  // 중앙기준 y전체
		{
			if (m_PlayerPosY == m_monsterTilelist.nowY) // 몬스터 기준 중앙 가로를 기준으로 함
			{
				if (m_PlayerPosX < m_monsterTilelist.nowX)  // 왼쪽을 바라봄
				{
					m_MonsterLookTileX = m_monsterTilelist.nowX - 1;
					m_MonsterLookTileY = m_monsterTilelist.nowY;
					LookTileStaus = statusA;
				}
				else if (m_PlayerPosX > m_monsterTilelist.nowX)  // 오른쪽을 바라봄
				{
					m_MonsterLookTileX = m_monsterTilelist.nowX + 1;
					m_MonsterLookTileY = m_monsterTilelist.nowY;
					LookTileStaus = statusD;
				}

			}
			else  // 한줄을 제외한 대부분은 이쪽으로 들어올것
			{
				if (m_PlayerPosY < m_monsterTilelist.nowY) // 아래쪽을 바라봄
				{
					m_MonsterLookTileY = m_monsterTilelist.nowY - 1;
					m_MonsterLookTileX = m_monsterTilelist.nowX;
					LookTileStaus = statusS;
				}
				else if (m_PlayerPosY > m_monsterTilelist.nowY) // 위쪽을 바라봄
				{
					m_MonsterLookTileY = m_monsterTilelist.nowY + 1;
					m_MonsterLookTileX = m_monsterTilelist.nowX;
					LookTileStaus = statusW;
				}
			}

		}

		// 여기까지 전방의 타일을 판단하는 부분

		int chose = RandomNextMove();  // 랜덤성을 결정하는 부분

		if ((m_monsterTilelist.newX == m_PlayerPosX || m_monsterTilelist.newY == m_PlayerPosY) && (LookTileStaus == WATER ||
			LookTileStaus == NORMAL))   // 이제 타일 수가 많아 졌기에 타일에 따라 가야한다  // 바위가 아니라면 다 들어옴 , 물이면 죽음
		{
			m_monsterTilelist.newX = m_MonsterLookTileX;  //보고 있던 방향으로 이동
			m_monsterTilelist.newY = m_MonsterLookTileY;
		}
		else
		{
			if (LookTileStaus != GameClient::MapStatus::NORMAL) // 보고 있는 타일이 노멀이 아니라면  , 물 바위 등의 이동 불가 타일만 체크 해야함
			{
				if (m_MonsterLookTileY == m_monsterTilelist.nowY)   // 오른쪽이나 왼쪽이 바라보고 있는 타일이라면 좌우 이동
				{					
					switch (chose)
					{
					case 0:  // 랜덤하게 좌우 결정
						if(statusS == GameClient::MapStatus::NORMAL)
							m_monsterTilelist.newY = m_monsterTilelist.nowY - 1;
						break;
					case 1:
						if (statusW == GameClient::MapStatus::NORMAL)
							m_monsterTilelist.newY = m_monsterTilelist.nowY + 1;
						break;
					default:
						break;
					}
				} 
				else if (m_MonsterLookTileX == m_monsterTilelist.nowX) // 위나 아래를 바라보고 있는 타일이라면 상하 이동 . /// 여기 예외처리 필요
				{
					if (m_MonsterLookTileX < m_PlayerPosX) // 타일의 x값과 플레이어의 x값을 비교해서 플레이어 방향으로 이동한다
					{
						if (statusD == GameClient::MapStatus::NORMAL)
							m_monsterTilelist.newX = m_monsterTilelist.nowX + 1;
					}
					else if (m_MonsterLookTileX > m_PlayerPosX)
					{
						if (statusA == GameClient::MapStatus::NORMAL)
							m_monsterTilelist.newX = m_monsterTilelist.nowX - 1;
					}
					else if (m_PlayerPosX == m_PlayerPosX)  // 만약 직선 경로에 있다면 랜덤하게 좌우를 부여한다
					{
						switch (chose)
						{
						case 0:
							if (statusD == GameClient::MapStatus::NORMAL)
								m_monsterTilelist.newX = m_monsterTilelist.nowX + 1;
							break;
						case 1:
							if (statusA == GameClient::MapStatus::NORMAL)
								m_monsterTilelist.newX = m_monsterTilelist.nowX - 1;
							break;
						default:
							break;
						}
					}					
				}
			}
			else
			{
				m_monsterTilelist.newX = m_MonsterLookTileX;  //보고 있던 방향으로 이동
				m_monsterTilelist.newY = m_MonsterLookTileY;
			}
		}

	}

}
