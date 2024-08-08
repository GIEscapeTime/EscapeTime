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


	/// 일단 타일 받아오는건 된거같다....

	class MonsterAi : public KunrealEngine::Component
	{
	public:
		

		MonsterAi(); 
		~MonsterAi();
	public:
		virtual void Initialize()override;
		virtual void Finalize()override;

		//각 업데이트에 대한 주석은 Core 부분 참조
		virtual void FixedUpdate() override;
		virtual void Update()override;
		virtual void LateUpdate()override;

	public:
		void GetPlayerPos(int x, int y);  // 플레이어 위치를 받아옴
		void SetMonsterPos(int posX, int posY);
		GameClient::MonsterTileIndex MonsterPosition();
		
								//플레이어를 감지하는 범위가 타일이라 그 타일들을 담는 벡터
		bool PlayerSearch();	// 그냥 비어있어도 앞에 한칸만 받아오면 된다고 판단됌

		bool MonsterDieCheck();
		
		void MonsterFollowPlayer(GameClient::MapStatus statusW, GameClient::MapStatus statusA, GameClient::MapStatus statusS, GameClient::MapStatus statusD);  //몬스터가 플레이어를 추적하는 함수
																																									  //매개변수는 몬스터가 이동할 타일의 속성을 가져옴
		
		int RandomNextMove(); // 확률 50퍼의 랜덤값 반환

		void MonsterDie();

		void MonsterReset();

		void MonsterPosInitialize(); // 모든 처리가 끝나고 now, new를 적절하게 초기화
	private:	
		int m_MonsterLookTileX;
		int m_MonsterLookTileY;

		int m_PlayerPosX;
		int m_PlayerPosY;
		bool m_PlayerSearch;
		bool isMonsterDie;

		MonsterTileIndex m_monsterTilelist;  // 몬스터가 위치한 타일의 값들

		MapStatus LookTileStaus; // 앞의 타일을 담음
	};

	// 업데이트 함수 알아서 움직이게 설정해두기
	// 가도 되는거 : 물, 노멀, 트랩 온, 오프
	// 나머지

	// 다 해결했는데 3스테 아직 없어서 그런가 뭔가 문제가 있어보이는데 잘 안보임. 몬스터 배치가 좀 이상하고 리셋도 이상함
}

//컴포넌트에 또 뭐가 있어야 할까?

// 몬스터가 플레이어를 죽이는것에 대한 예외처리  => 지금은 그것을 제외한 모든 죽는 부분을 무시함 했음
// 몬스터가 물에 빠지면 죽는 부분
// 몬스터가 트랩에 아예 가지 않도록 수정   이거 먼저(해결)