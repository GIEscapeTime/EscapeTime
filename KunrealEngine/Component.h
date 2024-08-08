#pragma once
#include "CommonHeader.h"

/// <summary>
/// 컴포넌트들의 부모가 될 클래스
/// 컴포넌트를 만들 때 이 클래스를 상속받고 만들면 된다
/// </summary>

namespace KunrealEngine
{
	class Component
	{	
		friend class GameObject;
	public:
		Component();
		Component(GameObject* object);
		virtual ~Component();

	private:
		virtual void Initialize() {};
		virtual void Finalize() {};

		//각 업데이트에 대한 주석은 Core 부분 참조
		virtual void FixedUpdate() {};
		virtual void Update() {};
		virtual void LateUpdate() {};

		virtual void OnTriggerEnter() {}
		virtual void OnTriggerStay() {}
		virtual void OnTriggerExit() {}

	private:
		GameObject* ownerObject;					//컴포넌트가 어떤 오브젝트에 부여되고 있는지
		bool isActivated;							//활성화 여부
		std::string m_componentName;				//구분을 위한 이름인데 다른 방법이 있지 않을까
	public:
		void SetOwner(GameObject* object);				//private로 ownerObject를 가지고 있으니 Setter함수 만들어줌
		GameObject* GetOwner();
		void ActivateComponent();
		void InactivateComponent();
		bool GetActivated();
	};
}

