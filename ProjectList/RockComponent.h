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

		void SetPosition(int posX, int posY);				// ��ġ ����
		bool ComparePosition(int posX, int posY);			// ��ġ ��
		void AtWater(int posX, int posY);

	private:
		// �κ��丮 ���
		bool m_boolTouch;
		int m_mapX;
		int m_mapY;
		float m_heightPoint;

	};

}


