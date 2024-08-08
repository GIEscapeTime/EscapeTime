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

		void SettingArrow(int moveX, int moveY, int& stageNumber);	// �߰� �ʱ�ȭ
		void SetPosition(int posX, int posY);						// ��ġ ����
		void MoveForward();											// ȭ�� ����

		int ReturnPositionX();										// x��ġ ��ȯ
		int ReturnPositionY();										// y��ġ ��ȯ

	private:
		// ���� �������� �ѹ�
		int m_currentStageNumber;

		// ��ǥ ����
		int m_mapX;
		int m_mapY;
		int m_moveX;
		int m_moveY;

	};

}

