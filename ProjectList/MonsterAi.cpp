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


void GameClient::MonsterAi::GetPlayerPos(int x, int y) //����  xy�� �ٲ�µ� m_PlayerPosX Y�� �ȹٲ����
{
	m_PlayerPosX = x;
	m_PlayerPosY = y;
}

void GameClient::MonsterAi::SetMonsterPos(int posX, int posY)
{
	m_monsterTilelist.nowX = posX;
	m_monsterTilelist.nowY = posY;
	m_monsterTilelist.newX = posX; // ó�� �����Ҷ��� ������ ���� ���� �ʱ�ȭ
	m_monsterTilelist.newY = posY;
}

int GameClient::MonsterAi::RandomNextMove() // 1,0�� �����ϰ� ��ȯ
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
	m_monsterTilelist.nowX = m_monsterTilelist.newX;						// ���� ��¥ Ÿ���� �޾ƿ;� �Ұ��ε�....
	m_monsterTilelist.nowY = m_monsterTilelist.newY;						// ������ ���� ������ �ȵ����� ��ɻ� ������ ������
}

bool GameClient::MonsterAi::MonsterDieCheck() // ���Ͱ� �׾����� true�� ��ȯ
{
	if (isMonsterDie == true)
	{
		return true;
	}
	return false;
}



GameClient::MonsterTileIndex GameClient::MonsterAi::MonsterPosition()
// ������ ��ġ�� ��ȯ�ϴ� �Լ� ���ó�� ���� ������ �ӽ÷� ������
{
	return m_monsterTilelist;
}

bool GameClient::MonsterAi::PlayerSearch()
{
	int sightRange = 3;

	for (int y = m_monsterTilelist.nowY - sightRange; y <= m_monsterTilelist.nowY + sightRange; ++y)  // �ִ� ũ���� 3�� ���
	{
		int xOffset = sightRange - abs(y - m_monsterTilelist.nowY);  // abs�� �������� ��ȯ�ϴ� �κ�,  xOffset�� �־������� ���� �þ߰��� ���������� ����
		// 
		for (int x = m_monsterTilelist.nowX - xOffset; x <= m_monsterTilelist.nowX + xOffset; ++x)
		{												 // �÷��̾� ��ġ�� Ȯ�εǸ� Ʈ���
			if(m_PlayerPosX == x && m_PlayerPosY == y)
			{
				return true;
			}		
		}
	}

	return false;
}


void GameClient::MonsterAi::MonsterFollowPlayer(GameClient::MapStatus statusW, GameClient::MapStatus statusA, GameClient::MapStatus statusS, GameClient::MapStatus statusD )
{	// ���� ���� Ÿ�� ����, �̵��Ҷ��� WASD�� Ȱ���ؼ� ������ �Ͽ���
	// ���� ������ �ִ� ������Ʈ�� ��ũ�� üũ�ؼ� �÷��̾�� ���󰡵��� �Ұ�
	// 1. ��ã�Ⱑ �ʿ���
	// 2. ���� üũ�� �׿� ���� ��ũ üũ�� �ʿ���


	// ����ó�� �Ұ� : ���� ���� �¿찡 �̵� �Ұ� Ÿ���� ��� = �̵������ʴ´�
	// ������ ����� �̵��ؾ� �ϴ� Ÿ��, 2���� �����ִ��� �̵������ʴ´�.
	// ���� ������ Ÿ���� ������ �ʿ��ϴ�

	int sightRange = 3;
	  // �̵� �� Ÿ���� �Ӽ��� ����

	
	// ������ �и��ص� new �� now�� ���⼭ �ٽ� ���ļ� �ʱ�ȭ
	
	if (PlayerSearch() == true)  // �÷��̾ ���� �ȿ� ����, �� �κ� �Ű������� ����ͼ� �ϴ� �־��µ�, ���� ����, ��ü Ÿ�� ������ �����;��ϴµ�
	{
		for (int x = m_monsterTilelist.nowX - sightRange; x <= m_monsterTilelist.nowX + sightRange; ++x)  // �߾ӱ��� y��ü
		{
			if (m_PlayerPosY == m_monsterTilelist.nowY) // ���� ���� �߾� ���θ� �������� ��
			{
				if (m_PlayerPosX < m_monsterTilelist.nowX)  // ������ �ٶ�
				{
					m_MonsterLookTileX = m_monsterTilelist.nowX - 1;
					m_MonsterLookTileY = m_monsterTilelist.nowY;
					LookTileStaus = statusA;
				}
				else if (m_PlayerPosX > m_monsterTilelist.nowX)  // �������� �ٶ�
				{
					m_MonsterLookTileX = m_monsterTilelist.nowX + 1;
					m_MonsterLookTileY = m_monsterTilelist.nowY;
					LookTileStaus = statusD;
				}

			}
			else  // ������ ������ ��κ��� �������� ���ð�
			{
				if (m_PlayerPosY < m_monsterTilelist.nowY) // �Ʒ����� �ٶ�
				{
					m_MonsterLookTileY = m_monsterTilelist.nowY - 1;
					m_MonsterLookTileX = m_monsterTilelist.nowX;
					LookTileStaus = statusS;
				}
				else if (m_PlayerPosY > m_monsterTilelist.nowY) // ������ �ٶ�
				{
					m_MonsterLookTileY = m_monsterTilelist.nowY + 1;
					m_MonsterLookTileX = m_monsterTilelist.nowX;
					LookTileStaus = statusW;
				}
			}

		}

		// ������� ������ Ÿ���� �Ǵ��ϴ� �κ�

		int chose = RandomNextMove();  // �������� �����ϴ� �κ�

		if ((m_monsterTilelist.newX == m_PlayerPosX || m_monsterTilelist.newY == m_PlayerPosY) && (LookTileStaus == WATER ||
			LookTileStaus == NORMAL))   // ���� Ÿ�� ���� ���� ���⿡ Ÿ�Ͽ� ���� �����Ѵ�  // ������ �ƴ϶�� �� ���� , ���̸� ����
		{
			m_monsterTilelist.newX = m_MonsterLookTileX;  //���� �ִ� �������� �̵�
			m_monsterTilelist.newY = m_MonsterLookTileY;
		}
		else
		{
			if (LookTileStaus != GameClient::MapStatus::NORMAL) // ���� �ִ� Ÿ���� ����� �ƴ϶��  , �� ���� ���� �̵� �Ұ� Ÿ�ϸ� üũ �ؾ���
			{
				if (m_MonsterLookTileY == m_monsterTilelist.nowY)   // �������̳� ������ �ٶ󺸰� �ִ� Ÿ���̶�� �¿� �̵�
				{					
					switch (chose)
					{
					case 0:  // �����ϰ� �¿� ����
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
				else if (m_MonsterLookTileX == m_monsterTilelist.nowX) // ���� �Ʒ��� �ٶ󺸰� �ִ� Ÿ���̶�� ���� �̵� . /// ���� ����ó�� �ʿ�
				{
					if (m_MonsterLookTileX < m_PlayerPosX) // Ÿ���� x���� �÷��̾��� x���� ���ؼ� �÷��̾� �������� �̵��Ѵ�
					{
						if (statusD == GameClient::MapStatus::NORMAL)
							m_monsterTilelist.newX = m_monsterTilelist.nowX + 1;
					}
					else if (m_MonsterLookTileX > m_PlayerPosX)
					{
						if (statusA == GameClient::MapStatus::NORMAL)
							m_monsterTilelist.newX = m_monsterTilelist.nowX - 1;
					}
					else if (m_PlayerPosX == m_PlayerPosX)  // ���� ���� ��ο� �ִٸ� �����ϰ� �¿츦 �ο��Ѵ�
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
				m_monsterTilelist.newX = m_MonsterLookTileX;  //���� �ִ� �������� �̵�
				m_monsterTilelist.newY = m_MonsterLookTileY;
			}
		}

	}

}
