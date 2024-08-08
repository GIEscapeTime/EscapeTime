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
		void ChangeMap(int i);						   // 스테이지 전환
		void CurrentMapXCrease(int crease);			   // 플레이어 현재 위치 증감
		void CurrentMapYCrease(int crease);			   // 플레이어 현재 위치 증감
		void UpdatePlayerPosition();				   // 플레이어 렌더 위치 업데이트

		bool ObjectCheck(int creaseX, int creaseY);	   // 플레이어가 닿은 오브젝트 체크
		void PlayerDieCheck();						   // 플레이어가 죽으면 실행

		void ResetXYObjectInMap();					   // 모든 오브젝트 카메라 밖으로
		void SetXYObjectInMap();					   // 사용할 오브젝트를 제위치로
		void DistroyObjectInMap();
		void MapReset();							   // 타일 리셋(스테이지별로 전부)
		
		void TurnTrapONOFF();						   // 트랩 상태 전환
		void TurnChange();							   // 턴 변화 (임시)

		int ReturnCurrentX();						   // 현재 위치 반환 함수
		int ReturnCurrentY();						   // 현재 위치 반환 함수

		void UseRopeSurport();						   // 로프 서포터 설치
		void UseRope();								   // 로프 사용
		
		void ReStart();									// 막판

	/// <summary>
	/// 여긴 몬스터
	/// </summary>	
		MapStatus MonsterFrontTileCheck(GameClient::MonsterTileIndex monsteIndex, int creaseX, int creaseY);		// 몬스터 컴포넌트에 보내줄 변수

	private:
		void TileChange(int mapNumber, int tileX, int tiley, MapStatus status);		// 초기화 배치 보조 함수
		void TrapChange(int stageNumber, int tileX, int tiley, MapStatus status);	// 트랩 상태 전환 보조 함수

		void CreateObjectInMap();												    // 최초 한번 오브젝트 생성
		void CreateTrapObject();													// 트랩 오브젝트 생성 보조 함수
		void CreateArrowObject();													// 화살 오브젝트 생성 보조 함수
		void CreateRockObject(std::string rockName);								// 바위 생성 보조 함수
		void CreateEquipmentItem();													// 장비 및 아이템 생성 보조 함수
		void SetEquipmentPosition();												// 장비, 아이템 맵 배치
		void SetArrowPosition();													// 화살 맵 배치
		void CreateMapBase();														// 바닥 오브젝트 생성 보조함수
		void SetMapBase();															// 바닥 오브젝트 셋팅
		void ReSetMapBase();														// 바닥 오브젝트 리셋
		void CreatePlayerBody();													// 시체 오브젝트 생성 보조함수
		void SetPlayerBody();														// 플레이어 죽은후 시체 셋팅
		void CreateMonsterObject();													// 몬스터 생성 함수
		void SetMonsterObject();													// 몬스터 제위치에 놓는 함수

		
		void ArrowAction();															// 화살 킬 함수
		void ArrowMove();															// 화살 이동  함수
		void MonsterTouch();														// 몬스터에 닿았을 때
		void MonsterMove();															// 몬스터 이동 함수

		KunrealEngine::GameObject* SearchRockObjectInVector(int posX, int posY);	// 플레이어 앞 바위오브젝트 객체 찾기
		bool PushRock(int creaseX, int creaseY);									// 바위밀치기 처리
		
		bool GetEquipment(int creaseX, int creaseY);								// 장비템 획득 보조 함수
		bool GetRopeSurport(int creaseX, int creaseY);								// 로프서포터 획득 보조 함수
		bool RopeCheck(int creasX, int creaseY);									// 로프 사용 보조 함수
		

		MapStatus m_mapXYStatus1[12][12]{};
		MapStatus m_mapXYStatus2[12][12]{};
		MapStatus m_mapXYStatus3[17][17]{};

		std::vector<int>  m_vPlayerBodyX{};											// 플레이어가 죽은 위치들의 x좌표
		std::vector<int>  m_vPlayerBodyY{};											// 플레이어가 죽은 위치들의 y좌표
		std::vector<KunrealEngine::GameObject*> m_vPlayerBody;						// 시체들

		int m_currentMapX;
		int m_currentMapY;
		int m_currentMapNumber;

		int m_playerView;									   // 6 4 8 2 (오른쪽 왼쪽 위 아래)

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
