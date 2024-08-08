#pragma once
//#include "RockComponent.h"

namespace KunrealEngine
{
	class GameObject;
}

namespace GameClient
{
	enum MapStatus
	{
		NORMAL,
		WATER,
		ARROW,
		TRAP,
		TRAPOFF,
		ROPESURPORTITEM,
		ROPESURPORTBUILD,
		Equipment,
		EXIT,
		MONSTER,
		ROCK,
		WALL
	};

	struct MonsterTileIndex
	{
		int nowX;
		int nowY;
		int newX;
		int newY;
	};

	class MapMatrix
	{
	public:
		MapMatrix(std::string sceneName);
		~MapMatrix();

		void Initialize();
		void ChangeMap(int i);						   // �������� ��ȯ
		void CurrentMapXCrease(int crease);			   // �÷��̾� ���� ��ġ ����
		void CurrentMapYCrease(int crease);			   // �÷��̾� ���� ��ġ ����
		void UpdatePlayerPosition();				   // �÷��̾� ���� ��ġ ������Ʈ

		bool ObjectCheck(int creaseX, int creaseY);	   // �÷��̾ ���� ������Ʈ üũ
		void PlayerDieCheck();						   // �÷��̾ ������ ����

		void ResetXYObjectInMap();					   // ��� ������Ʈ ī�޶� ������
		void SetXYObjectInMap();					   // ����� ������Ʈ�� ����ġ��
		void DistroyObjectInMap();
		void MapReset();							   // Ÿ�� ����(������������ ����)
		
		void TurnTrapONOFF();						   // Ʈ�� ���� ��ȯ
		void TurnChange();							   // �� ��ȭ (�ӽ�)

		int ReturnCurrentX();						   // ���� ��ġ ��ȯ �Լ�
		int ReturnCurrentY();						   // ���� ��ġ ��ȯ �Լ�

		void UseRopeSurport();						   // ���� ������ ��ġ
		void UseRope();								   // ���� ���
		
		void ReStart();									// ����

	/// <summary>
	/// ���� ����
	/// </summary>	
		MapStatus MonsterFrontTileCheck(GameClient::MonsterTileIndex monsteIndex, int creaseX, int creaseY);		// ���� ������Ʈ�� ������ ����

	private:
		void TileChange(int mapNumber, int tileX, int tiley, MapStatus status);		// �ʱ�ȭ ��ġ ���� �Լ�
		void TrapChange(int stageNumber, int tileX, int tiley, MapStatus status);	// Ʈ�� ���� ��ȯ ���� �Լ�

		void CreateObjectInMap();												    // ���� �ѹ� ������Ʈ ����
		void CreateTrapObject();													// Ʈ�� ������Ʈ ���� ���� �Լ�
		void CreateArrowObject();													// ȭ�� ������Ʈ ���� ���� �Լ�
		void CreateRockObject(std::string rockName);								// ���� ���� ���� �Լ�
		void CreateEquipmentItem();													// ��� �� ������ ���� ���� �Լ�
		void SetEquipmentPosition();												// ���, ������ �� ��ġ
		void SetArrowPosition();													// ȭ�� �� ��ġ
		void CreateMapBase();														// �ٴ� ������Ʈ ���� �����Լ�
		void SetMapBase();															// �ٴ� ������Ʈ ����
		void ReSetMapBase();														// �ٴ� ������Ʈ ����
		void CreatePlayerBody();													// ��ü ������Ʈ ���� �����Լ�
		void SetPlayerBody();														// �÷��̾� ������ ��ü ����
		void CreateMonsterObject();													// ���� ���� �Լ�
		void SetMonsterObject();													// ���� ����ġ�� ���� �Լ�

		
		void ArrowAction();															// ȭ�� ų �Լ�
		void ArrowMove();															// ȭ�� �̵�  �Լ�
		void MonsterTouch();														// ���Ϳ� ����� ��
		void MonsterMove();															// ���� �̵� �Լ�

		KunrealEngine::GameObject* SearchRockObjectInVector(int posX, int posY);	// �÷��̾� �� ����������Ʈ ��ü ã��
		bool PushRock(int creaseX, int creaseY);									// ������ġ�� ó��
		
		bool GetEquipment(int creaseX, int creaseY);								// ����� ȹ�� ���� �Լ�
		bool GetRopeSurport(int creaseX, int creaseY);								// ���������� ȹ�� ���� �Լ�
		bool RopeCheck(int creasX, int creaseY);									// ���� ��� ���� �Լ�
		

		MapStatus m_mapXYStatus1[12][12]{};
		MapStatus m_mapXYStatus2[12][12]{};
		MapStatus m_mapXYStatus3[17][17]{};

		std::vector<int>  m_vPlayerBodyX{};											// �÷��̾ ���� ��ġ���� x��ǥ
		std::vector<int>  m_vPlayerBodyY{};											// �÷��̾ ���� ��ġ���� y��ǥ
		std::vector<KunrealEngine::GameObject*> m_vPlayerBody;						// ��ü��

		int m_currentMapX;
		int m_currentMapY;
		int m_currentMapNumber;

		int m_playerView;									   // 6 4 8 2 (������ ���� �� �Ʒ�)

		std::vector<KunrealEngine::GameObject*> m_vRock1;
		std::vector<KunrealEngine::GameObject*> m_vRock2;
		std::vector<KunrealEngine::GameObject*> m_vRock3;

		std::vector<KunrealEngine::GameObject*> m_vTrap;
		std::vector<KunrealEngine::GameObject*> m_vUnTrap;

		bool m_ropeSurport_1;
		bool m_ropeSurport_2;
		bool m_ropeSurport_3;

		bool m_mapChange;
		bool m_playerLife;
		bool m_turnChanger;

		std::string m_sceneName;

	};
}
