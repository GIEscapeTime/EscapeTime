#pragma once
#include "CommonHeader.h"
#include "ComponentHeaders.h"
#include "nlohmann/json.hpp"

/// <summary>
/// ������Ʈ���� �� Ŭ������ ���� �����ȴ�
/// 
/// ������Ʈ���� Ŭ������ ����� ���� �ƴ϶� �� Ŭ������ ���� ������ְ�, �̸��� �����ָ� �ȴ�
/// </summary>
using json = nlohmann::json;

namespace KunrealEngine
{
	class Component;

	class GameObject
	{
		friend class Scene;
		friend class SceneManager;
	private:
		GameObject();
		GameObject(std::string name);
		~GameObject();

		void Initialize();
		void Finalize();
		void FixedUpdate();
		void Update();
		void LateUpdate();

		void Render();				//�������� ������Ʈ�� ����ٸ� ���⼭�� �ʿ���� ������

	private:
		std::vector<Component*> m_componentContainer;		//������Ʈ���� ���� ����
		bool m_isActivated;									//Ȱ��ȭ ����
		std::string m_objectName;							//������Ʈ �̸�
		std::string m_sceneName;							//� scene�� ����ִ°�
		Transform* m_transform;								//������Ʈ�� transform�� ���°� ���̾ȵǴϱ� �̸� �־���
		GameObject* m_parent;								//�θ� ������Ʈ
		int m_layer;										//���� ������ ���õ� ���̾�
		std::string m_tag;

	public:
		std::string GetObjectName();
		void SetObjectName(std::string name);
		void SetParent(GameObject* obj);
		GameObject* GetParent();
		void SetActive(bool active);
		bool GetActivated();
	public:
		void SetLayer(int layer);
		int GetLayer() const;
		void SetTag(std::string tag);
		std::string GetTag();

	///������Ʈ ����
	public:
		//���ø� ���� ���� ����
		//�ϴ� ���ΰ� ���߿� �����ϱ��
		void AddComponent(Component* component);
		void DeleteComponent(Component* component);
		void ClearComponent();

		//�̷��� �ϸ� ������Ʈ�� �ٱ����� �����ϰ� ������� ���ٵ�..
		//�ȿ��� new�� �ϸ�?
		template<typename T>
		void AddComponent(T* component)
		{
			this->m_componentContainer.emplace_back(component);
			if (component->GetOwner() == nullptr)
			{
				component->SetOwner(this);					//������Ʈ�� �����ָ� �� ������Ʈ��
			}
			component->Initialize();
		}

		//�׷��� �����
		template<typename T>
		void AddComponent()
		{
			T* newComponent = new T();

			this->m_componentContainer.emplace_back(newComponent);
			if (newComponent->GetOwner() == nullptr)
			{
				newComponent->SetOwner(this);					//������Ʈ�� �����ָ� �� ������Ʈ��
			}
			newComponent->Initialize();
		}

		//componentContainer�� ������Ʈ �������� �����̳�
		//������ ��ȣ�� �Բ��� �ڵ�
		template<typename T>
		T* GetComponent()
		{
			for (int i = 0; i < m_componentContainer.size(); i++)
			{
				//dynamic_cast�� ����ȯ ���н� nullptr�� ��ȯ�Ѵ�
				Component* component = dynamic_cast<T*>(m_componentContainer[i]);
				if (component != nullptr)			//ã�°� �־ nullptr�� �ƴ� ���
				{
					return (T*)component;			//������Ʈ�� ���޹��� T �������� ��ȯ�ؼ� ����
				}
			}

			return nullptr;
		}
		nlohmann::json Serialize();
	};
}

