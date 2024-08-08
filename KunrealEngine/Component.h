#pragma once
#include "CommonHeader.h"

/// <summary>
/// ������Ʈ���� �θ� �� Ŭ����
/// ������Ʈ�� ���� �� �� Ŭ������ ��ӹް� ����� �ȴ�
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

		//�� ������Ʈ�� ���� �ּ��� Core �κ� ����
		virtual void FixedUpdate() {};
		virtual void Update() {};
		virtual void LateUpdate() {};

		virtual void OnTriggerEnter() {}
		virtual void OnTriggerStay() {}
		virtual void OnTriggerExit() {}

	private:
		GameObject* ownerObject;					//������Ʈ�� � ������Ʈ�� �ο��ǰ� �ִ���
		bool isActivated;							//Ȱ��ȭ ����
		std::string m_componentName;				//������ ���� �̸��ε� �ٸ� ����� ���� ������
	public:
		void SetOwner(GameObject* object);				//private�� ownerObject�� ������ ������ Setter�Լ� �������
		GameObject* GetOwner();
		void ActivateComponent();
		void InactivateComponent();
		bool GetActivated();
	};
}

