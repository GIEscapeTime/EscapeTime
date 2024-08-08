#include "GameObject.h"
#include "Component.h"
#include "SceneManager.h"
#include "Scene.h"

KunrealEngine::GameObject::GameObject()
	:m_isActivated(true), m_objectName(""), m_layer(0)
{
	SceneManager& smInstance = SceneManager::GetInstance();
	int objectCount = 0;
	m_objectName = "GameObject";

	for (auto object : smInstance.GetCurrentScene()->GetObjectList())
	{
		if (m_objectName == object->GetObjectName())
		{
			objectCount++;
			m_objectName = m_objectName + " (" + std::to_string(objectCount) + ")";
		}
	}
}

KunrealEngine::GameObject::GameObject(std::string name)
	:m_isActivated(true), m_objectName(name), m_layer(0)
{
	SceneManager& smInstance = SceneManager::GetInstance();
	int objectCount = 0;

	for (auto object : smInstance.GetCurrentScene()->GetObjectList())
	{
		if (m_objectName == object->GetObjectName())
		{
			objectCount++;
			m_objectName = m_objectName + " (" + std::to_string(objectCount) + ")";
		}
	}
}

KunrealEngine::GameObject::~GameObject()
{

}

void KunrealEngine::GameObject::Initialize()
{
	//오브젝트가 생성되자마자 트랜스폼을 만들어서 넣어줌
	m_transform = new Transform(this);
	AddComponent(m_transform);
}

void KunrealEngine::GameObject::Finalize()
{
	ClearComponent();			//컴포넌트들 finalize하고 메모리 해제
	delete m_parent;
}

void KunrealEngine::GameObject::FixedUpdate()
{
	if (m_isActivated)
	{
		for (auto& components : m_componentContainer)
		{
			if (components->GetActivated()) components->FixedUpdate();
		}
	}
}

void KunrealEngine::GameObject::Update()
{
	if (m_isActivated)
	{
		for (auto& components : m_componentContainer)
		{
			if (components->GetActivated()) components->Update();
		}
	}
}

void KunrealEngine::GameObject::LateUpdate()
{
	if (m_isActivated)
	{
		for (auto& components : m_componentContainer)
		{
			if (components->GetActivated()) components->LateUpdate();
		}
	}
}

void KunrealEngine::GameObject::Render()
{
	if (m_isActivated)
	{
		//렌더러 컴포넌트는 Render 함수가 따로 존재할것
	}
}

//오브젝트의 이름을 반환
std::string KunrealEngine::GameObject::GetObjectName()
{
	return m_objectName;
}

//오브젝트의 이름을 설정
void KunrealEngine::GameObject::SetObjectName(std::string name)
{
	this->m_objectName = name;
}

//부모 오브젝트 설정
void KunrealEngine::GameObject::SetParent(GameObject* obj)
{
	m_parent = obj;
}

//부모 오브젝트 반환해주는 함수
KunrealEngine::GameObject* KunrealEngine::GameObject::GetParent()
{
	if (m_parent != nullptr) return m_parent;
	else return nullptr;
}

//활성화 여부를 정해주는 함수
//true = 활성화 / false = 비활성화
void KunrealEngine::GameObject::SetActive(bool active)
{
	this->m_isActivated = active;
}

bool KunrealEngine::GameObject::GetActivated()
{
	return m_isActivated;
}

//렌더 순서와 관련된 레이어를 설정하는 함수
//낮을 수록 먼저 렌더 -> 밑으로 깔림
void KunrealEngine::GameObject::SetLayer(int layer)
{
	m_layer = layer;
}

//레이어를 반환하는 함수
int KunrealEngine::GameObject::GetLayer() const
{
	return this->m_layer;
}

//태그 설정
void KunrealEngine::GameObject::SetTag(std::string tag)
{
	m_tag = tag;
}

std::string KunrealEngine::GameObject::GetTag()
{
	return m_tag;
}

//오브젝트에 컴포넌트를 추가해주는 함수
void KunrealEngine::GameObject::AddComponent(Component* component)
{
	m_componentContainer.push_back(component);
	if (component->GetOwner() == nullptr)
	{
		component->SetOwner(this);					//컴포넌트의 소유주를 이 오브젝트로
	}
	component->Initialize();
}

nlohmann::json KunrealEngine::GameObject::Serialize()
{
	nlohmann::json jsonData;   // 저장 할 json 객체 생성

	jsonData["m_isActivated"] = m_isActivated;  // 각 멤버 넣어줌
	jsonData["m_objectName"] = m_objectName;
	jsonData["m_sceneName"] = m_sceneName;

	nlohmann::json transformData;  // 트랜스 폼을 직렬화

	transformData["position_x"] = m_transform->GetPosition().x;
	transformData["position_y"] = m_transform->GetPosition().y;
	transformData["position_z"] = m_transform->GetPosition().z;

	transformData["rotation_x"] = m_transform->GetRotation().x;
	transformData["rotation_y"] = m_transform->GetRotation().y;
	transformData["rotation_z"] = m_transform->GetRotation().z;
	transformData["rotation_w"] = m_transform->GetRotation().w;

	transformData["scale_x"] = m_transform->GetScale().x;
	transformData["scale_y"] = m_transform->GetScale().y;
	transformData["scale_z"] = m_transform->GetScale().z;

	jsonData["m_transform"] = transformData;  // 전체 json 객체에 m_transform 키를 걸어 추가

	nlohmann::json componentArray;  // 컴포넌트 컨테이너를 저장할 배열
	int componentID = 0;


	for (const auto& component : m_componentContainer) // 컨테이너 수 만큼 반복
	{
		nlohmann::json componentData;
		componentData["component_ID"] = componentID;
		componentID++;

		componentArray.push_back(componentData);
	}

	jsonData["m_componentContainer"] = componentArray;

	return jsonData;
}

//특정 컴포넌트를 삭제하는 함수
void KunrealEngine::GameObject::DeleteComponent(Component* component)
{
	auto iter = find(m_componentContainer.begin(), m_componentContainer.end(), component);

	if (iter != m_componentContainer.end())
	{
		(*iter)->Finalize();						//컴포넌트에서 해제할게 있으면 해주고
		delete* iter;								//해당 컴포넌트를 삭제
		m_componentContainer.erase(iter);				//컨테이너에서 비워줌
	}
}

//오브젝트에 할당된 컴포넌트들을 모두 제거하는 함수
void KunrealEngine::GameObject::ClearComponent()
{
	for (Component* component : m_componentContainer)
	{
		component->Finalize();
		delete component;							//clear하기전에 delete 다 해줘야함
	}

	m_componentContainer.clear();
}
