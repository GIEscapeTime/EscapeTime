#pragma once
#include "CommonHeader.h"
#include "Component.h"
#include <functional>

/// <summary>
/// 버튼 컴포넌트
/// 
/// UI 오브젝트에서 사용하게 될 컴포넌트
/// </summary>

namespace KunrealEngine
{
	class Transform;

	class Button : public Component
	{
	public:
		Button();
		~Button();

		virtual void Initialize() override;
		virtual void Finalize() override;
		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

	private:
		float m_width;												//크기&높이 렌더할 크기와는 다름
		float m_height;

		float m_left;												//버튼 범위를 체크하기 위한 RECT 변수들
		float m_right;
		float m_top;
		float m_bottom;

		Transform* m_transform;										//부모 오브젝트의 트랜스폼
		std::vector<std::function<void()>> m_functionContainer;		//실행할 함수 컨테이너	//람다도 가능

	public:
		bool OnLeftClick();
		bool OnLeftClickUp();
		bool OnLeftHold();

		bool OnRightClick();
		bool OnRightClickUp();
		bool OnRightHold();

		void SetSize(float width, float height);
	};
}

