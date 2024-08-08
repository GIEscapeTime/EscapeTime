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
	//������Ʈ�� �������ڸ��� Ʈ�������� ���� �־���
	m_transform = new Transform(this);
	AddComponent(m_transform);
}

void KunrealEngine::GameObject::Finalize()
{
	ClearComponent();			//������Ʈ�� finalize�ϰ� �޸� ����
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
		//������ ������Ʈ�� Render �Լ��� ���� �����Ұ�
	}
}

//������Ʈ�� �̸��� ��ȯ
std::string KunrealEngine::GameObject::GetObjectName()
{
	return m_objectName;
}

//������Ʈ�� �̸��� ����
void KunrealEngine::GameObject::SetObjectName(std::string name)
{
	this->m_objectName = name;
}

//�θ� ������Ʈ ����
void KunrealEngine::GameObject::SetParent(GameObject* obj)
{
	m_parent = obj;
}

//�θ� ������Ʈ ��ȯ���ִ� �Լ�
KunrealEngine::GameObject* KunrealEngine::GameObject::GetParent()
{
	if (m_parent != nullptr) return m_parent;
	else return nullptr;
}

//Ȱ��ȭ ���θ� �����ִ� �Լ�
//true = Ȱ��ȭ / false = ��Ȱ��ȭ
void KunrealEngine::GameObject::SetActive(bool active)
{
	this->m_isActivated = active;
}

bool KunrealEngine::GameObject::GetActivated()
{
	return m_isActivated;
}

//���� ������ ���õ� ���̾ �����ϴ� �Լ�
//���� ���� ���� ���� -> ������ ��
void KunrealEngine::GameObject::SetLayer(int layer)
{
	m_layer = layer;
}

//���̾ ��ȯ�ϴ� �Լ�
int KunrealEngine::GameObject::GetLayer() const
{
	return this->m_layer;
}

//�±� ����
void KunrealEngine::GameObject::SetTag(std::string tag)
{
	m_tag = tag;
}

std::string KunrealEngine::GameObject::GetTag()
{
	return m_tag;
}

//������Ʈ�� ������Ʈ�� �߰����ִ� �Լ�
void KunrealEngine::GameObject::AddComponent(Component* component)
{
	m_componentContainer.push_back(component);
	if (component->GetOwner() == nullptr)
	{
		component->SetOwner(this);					//������Ʈ�� �����ָ� �� ������Ʈ��
	}
	component->Initialize();
}

nlohmann::json KunrealEngine::GameObject::Serialize()
{
	nlohmann::json jsonData;   // ���� �� json ��ü ����

	jsonData["m_isActivated"] = m_isActivated;  // �� ��� �־���
	jsonData["m_objectName"] = m_objectName;
	jsonData["m_sceneName"] = m_sceneName;

	nlohmann::json transformData;  // Ʈ���� ���� ����ȭ

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

	jsonData["m_transform"] = transformData;  // ��ü json ��ü�� m_transform Ű�� �ɾ� �߰�

	nlohmann::json componentArray;  // ������Ʈ �����̳ʸ� ������ �迭
	int componentID = 0;


	for (const auto& component : m_componentContainer) // �����̳� �� ��ŭ �ݺ�
	{
		nlohmann::json componentData;
		componentData["component_ID"] = componentID;
		componentID++;

		componentArray.push_back(componentData);
	}

	jsonData["m_componentContainer"] = componentArray;

	return jsonData;
}

//Ư�� ������Ʈ�� �����ϴ� �Լ�
void KunrealEngine::GameObject::DeleteComponent(Component* component)
{
	auto iter = find(m_componentContainer.begin(), m_componentContainer.end(), component);

	if (iter != m_componentContainer.end())
	{
		(*iter)->Finalize();						//������Ʈ���� �����Ұ� ������ ���ְ�
		delete* iter;								//�ش� ������Ʈ�� ����
		m_componentContainer.erase(iter);				//�����̳ʿ��� �����
	}
}

//������Ʈ�� �Ҵ�� ������Ʈ���� ��� �����ϴ� �Լ�
void KunrealEngine::GameObject::ClearComponent()
{
	for (Component* component : m_componentContainer)
	{
		component->Finalize();
		delete component;							//clear�ϱ����� delete �� �������
	}

	m_componentContainer.clear();
}
