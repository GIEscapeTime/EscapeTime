#pragma once
#include "KunrealAPI.h"


namespace GameClient
{
	class ArrowComponent : public KunrealEngine::Component
	{
	public:
		ArrowComponent();
		virtual ~ArrowComponent();

		virtual void Initialize() override;
		virtual void Finalize() override;

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void SettingArrow(int moveX, int moveY, int& stageNumber);	// 추가 초기화
		void SetPosition(int posX, int posY);						// 위치 저장
		void MoveForward();											// 화살 진행

		int ReturnPositionX();										// x위치 반환
		int ReturnPositionY();										// y위치 반환

	private:
		// 현재 스테이지 넘버
		int m_currentStageNumber;

		// 좌표 관련
		int m_mapX;
		int m_mapY;
		int m_moveX;
		int m_moveY;

	};

}

