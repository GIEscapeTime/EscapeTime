#pragma once
#include "CommonHeader.h"
#include "Component.h"

/// <summary>
/// 볼륨조절 버튼에 들어갈 컴포넌트
/// 
/// 기본 볼륨은 1
/// 사운드바의 길이는 500px
/// </summary>
/// 
namespace KunrealEngine
{
	class Transform;
	class Button;

	class VolumeControl : public Component
	{
	public:
		VolumeControl();
		VolumeControl(std::string soundType);
		~VolumeControl();

		virtual void Initialize() override;
		virtual void Finalize() override;

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void SetBG();
		void SetSFX();

	private:
		float m_minCnt;			//볼륨을 어디까지 올리고
		float m_maxCnt;			//내리게 할 것인가
		float m_currentVolume;	//현재 볼륨
		float m_increaseAmount;	//증가폭	//상수를 안쓰려고 변수로 뺌
		float m_UIMovement;
		Transform* m_transform;	//부모 오브젝트의 트랜스폼
		Button* m_button;

		bool m_isBG;			//백그라운드 조절용인가
		bool m_isSFX;			//효과음 조절용인가

		//bool m_low;
		//bool m_medium;
		//bool m_high;

	private:
		void UpdateVolume();
		void MoveButton();
		void CheckInput();
		void ButtonClicked();
	};
}

