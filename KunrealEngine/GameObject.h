#pragma once
#include "CommonHeader.h"
#include "ComponentHeaders.h"
#include "nlohmann/json.hpp"

/// <summary>
/// 오브젝트들은 이 클래스를 통해 생성된다
/// 
/// 오브젝트별로 클래스를 만드는 것이 아니라 이 클래스를 통해 만들어주고, 이름을 정해주면 된다
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

		void Render();				//렌더러를 컴포넌트로 만든다면 여기서는 필요없지 않을까

	private:
		std::vector<Component*> m_componentContainer;		//컴포넌트들을 담을 벡터
		bool m_isActivated;									//활성화 여부
		std::string m_objectName;							//오브젝트 이름
		std::string m_sceneName;							//어떤 scene에 들어있는가
		Transform* m_transform;								//오브젝트가 transform이 없는건 말이안되니까 미리 넣어줌
		GameObject* m_parent;								//부모 오브젝트
		int m_layer;										//렌더 순서와 관련된 레이어
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

	///컴포넌트 관련
	public:
		//템플릿 적용 이전 잔재
		//일단 냅두고 나중에 정리하기로
		void AddComponent(Component* component);
		void DeleteComponent(Component* component);
		void ClearComponent();

		//이렇게 하면 컴포넌트를 바깥에서 생성하고 지워줘야 할텐데..
		//안에서 new를 하면?
		template<typename T>
		void AddComponent(T* component)
		{
			this->m_componentContainer.emplace_back(component);
			if (component->GetOwner() == nullptr)
			{
				component->SetOwner(this);					//컴포넌트의 소유주를 이 오브젝트로
			}
			component->Initialize();
		}

		//그래서 해줬다
		template<typename T>
		void AddComponent()
		{
			T* newComponent = new T();

			this->m_componentContainer.emplace_back(newComponent);
			if (newComponent->GetOwner() == nullptr)
			{
				newComponent->SetOwner(this);					//컴포넌트의 소유주를 이 오브젝트로
			}
			newComponent->Initialize();
		}

		//componentContainer는 오브젝트 내에서의 컨테이너
		//범신의 가호가 함께한 코드
		template<typename T>
		T* GetComponent()
		{
			for (int i = 0; i < m_componentContainer.size(); i++)
			{
				//dynamic_cast는 형변환 실패시 nullptr를 반환한다
				Component* component = dynamic_cast<T*>(m_componentContainer[i]);
				if (component != nullptr)			//찾는게 있어서 nullptr가 아닐 경우
				{
					return (T*)component;			//컴포넌트를 전달받은 T 형식으로 변환해서 리턴
				}
			}

			return nullptr;
		}
		nlohmann::json Serialize();
	};
}

