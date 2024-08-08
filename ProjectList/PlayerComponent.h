#pragma once
#include "Component.h"

namespace KunrealEngine
{
	class GameObject;
}

namespace GameClient
{
	class PlayerComponent : public KunrealEngine::Component
	{
	public:
		PlayerComponent();
		virtual ~PlayerComponent();

		virtual void Initialize() override;
		virtual void Finalize() override;

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

 		void PlayerRelive();
// 		void PlayerDie();

		int UseRopeSurport();
		void UseRope();

		void GetSheild();
		void GetRope();
		void GetGlove();
		bool GetRopeSurport();
		void SetRopeSurport(KunrealEngine::GameObject* ropeSurport);

		// 장비 목록
		bool m_Sheild;
		bool m_glove;
		bool m_rope;

		bool m_getSheild;
		bool m_getGlove;

		bool m_live;
		int m_ropeSurport;


	private:
		std::vector<KunrealEngine::GameObject*> m_vRopeSurport;
		
		int m_useRopeSurport;
	};

}


