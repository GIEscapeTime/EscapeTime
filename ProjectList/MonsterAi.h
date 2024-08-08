#pragma once
#include "KunrealAPI.h"
#include "MapMatrix.h"

namespace KunrealEngine
{
	class Transform;
}

namespace GameClient
{
	class MapMatrix;

	enum MapStatus; 


	/// �ϴ� Ÿ�� �޾ƿ��°� �ȰŰ���....

	class MonsterAi : public KunrealEngine::Component
	{
	public:
		

		MonsterAi(); 
		~MonsterAi();
	public:
		virtual void Initialize()override;
		virtual void Finalize()override;

		//�� ������Ʈ�� ���� �ּ��� Core �κ� ����
		virtual void FixedUpdate() override;
		virtual void Update()override;
		virtual void LateUpdate()override;

	public:
		void GetPlayerPos(int x, int y);  // �÷��̾� ��ġ�� �޾ƿ�
		void SetMonsterPos(int posX, int posY);
		GameClient::MonsterTileIndex MonsterPosition();
		
								//�÷��̾ �����ϴ� ������ Ÿ���̶� �� Ÿ�ϵ��� ��� ����
		bool PlayerSearch();	// �׳� ����־ �տ� ��ĭ�� �޾ƿ��� �ȴٰ� �Ǵ܉�

		bool MonsterDieCheck();
		
		void MonsterFollowPlayer(GameClient::MapStatus statusW, GameClient::MapStatus statusA, GameClient::MapStatus statusS, GameClient::MapStatus statusD);  //���Ͱ� �÷��̾ �����ϴ� �Լ�
																																									  //�Ű������� ���Ͱ� �̵��� Ÿ���� �Ӽ��� ������
		
		int RandomNextMove(); // Ȯ�� 50���� ������ ��ȯ

		void MonsterDie();

		void MonsterReset();

		void MonsterPosInitialize(); // ��� ó���� ������ now, new�� �����ϰ� �ʱ�ȭ
	private:	
		int m_MonsterLookTileX;
		int m_MonsterLookTileY;

		int m_PlayerPosX;
		int m_PlayerPosY;
		bool m_PlayerSearch;
		bool isMonsterDie;

		MonsterTileIndex m_monsterTilelist;  // ���Ͱ� ��ġ�� Ÿ���� ����

		MapStatus LookTileStaus; // ���� Ÿ���� ����
	};

	// ������Ʈ �Լ� �˾Ƽ� �����̰� �����صα�
	// ���� �Ǵ°� : ��, ���, Ʈ�� ��, ����
	// ������

	// �� �ذ��ߴµ� 3���� ���� ��� �׷��� ���� ������ �־�̴µ� �� �Ⱥ���. ���� ��ġ�� �� �̻��ϰ� ���µ� �̻���
}

//������Ʈ�� �� ���� �־�� �ұ�?

// ���Ͱ� �÷��̾ ���̴°Ϳ� ���� ����ó��  => ������ �װ��� ������ ��� �״� �κ��� ������ ����
// ���Ͱ� ���� ������ �״� �κ�
// ���Ͱ� Ʈ���� �ƿ� ���� �ʵ��� ����   �̰� ����(�ذ�)