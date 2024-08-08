#pragma once
#include "KunrealAPI.h"


namespace GameClient
{
	class RockComponent : public KunrealEngine::Component
	{
	public:
		RockComponent();
		virtual ~RockComponent();

		virtual void Initialize() override;
		virtual void Finalize() override;

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void SetPosition(int posX, int posY);				// 위치 저장
		bool ComparePosition(int posX, int posY);			// 위치 비교
		void AtWater(int posX, int posY);

	private:
		// 인벤토리 목록
		bool m_boolTouch;
		int m_mapX;
		int m_mapY;
		float m_heightPoint;

	};

}


