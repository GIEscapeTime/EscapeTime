#include "Scene.h"
#include "Camera.h"
#include "GameObject.h"
#include "TextRenderer.h"
#include "Axis.h"
#include "Light.h"
#include "PointLight.h"
#include <Windows.h>		//�޼��� �ڽ��� ������ ���� ���� �ϼ��Ǹ� �����

//Axis* axx;

KunrealEngine::Scene::Scene()
	:m_sceneName(), m_mainCamera(nullptr)
{

}

//sceneName�� string���� �Ű������� �޾Ƽ� ���� �̸����� �������
KunrealEngine::Scene::Scene(std::string sceneName)
	:m_mainCamera(nullptr)
{
	this->m_sceneName = sceneName;
}

KunrealEngine::Scene::~Scene()
{

}

void KunrealEngine::Scene::Initialize()
{
	if (m_objectList.empty())
	{
		//scene�� �����Ǹ� ī�޶� ������Ʈ�� �������
		CreateObject("MainCamera");
		GetGameObject("MainCamera")->AddComponent<ObjectCamera>();
		//�⺻ ī�޶� ������Ʈ�� ���� ī�޶� ���� ī�޶�� ����
		SetMainCamera(GetGameObject("MainCamera"));
	}
	//axx = new Axis();
	//axx->Initialize();
}

//scene�� ��� �ִ� ������Ʈ���� �������� ���ְ�
//scene ��ü�� ���� �޸� ������ SceneManager���� ����
void KunrealEngine::Scene::Finalize()
{
	for (auto objects : m_objectList)
	{
		objects->Finalize();
		delete objects;
	}

	m_objectList.clear();
}

//scene �ȿ� ����ִ� ������Ʈ���� ������Ʈ
//���⼭�� ������ �ϴ����� �� �����
void KunrealEngine::Scene::FixedUpdate()
{
	
}

void KunrealEngine::Scene::Update()
{
	//axx->Update(m_mainCamera->GetViewXMMATRIX(), m_mainCamera->GetProjXMMATRIX());
}

void KunrealEngine::Scene::LateUpdate()
{
	
}

//������Ʈ �� Renderer ������Ʈ�� �ִ� �͵��� ã�� ������ ������
//Renderer�� ���� �ִ��� Ȯ�� ��, active ���θ� Ȯ��
void KunrealEngine::Scene::Render()
{
	for (auto objects : m_objectList)
	{
		if (objects->GetComponent<Renderer>() != nullptr)
		{
			//������Ʈ�� active���οʹ� �����̱� ������ ������Ʈ�� �����־ Renderer������Ʈ�� ��������� �׸��� �׸���
			if (objects->GetComponent<Renderer>()->GetActivated())
			{
				objects->GetComponent<Renderer>()->Render(m_mainCamera->GetViewXMMATRIX(), m_mainCamera->GetProjXMMATRIX());
			}
		}

		//UI���� ������
		if (objects->GetComponent<UIRenderer>() != nullptr)
		{
			//�������� ���������� �갡 ��������� �׸��� �׸�
			if (objects->GetComponent<UIRenderer>()->GetActivated())
			{
				objects->GetComponent<UIRenderer>()->Render();
			}
		}

		//�ؽ�Ʈ ���� ������
		if (objects->GetComponent<TextRenderer>() != nullptr)
		{
			//�������� ���������� �갡 ��������� �׸��� �׸�
			if (objects->GetComponent<TextRenderer>()->GetActivated())
			{
				objects->GetComponent<TextRenderer>()->TextRender();
			}
		}
	}

	//axx->Render();
}

//scene�� ������Ʈ�� ������ִ� �Լ�
void KunrealEngine::Scene::CreateObject()
{
	GameObject* gameObject = new GameObject();
	m_objectList.emplace_back(gameObject);
	gameObject->Initialize();
}

//�̹� ������� ������Ʈ�� ������ �־��ִ� ����
//�ʿ������� �𸣰����� �ϴ� ��������
void KunrealEngine::Scene::AddObject(GameObject* obj)
{
	GameObject* gameObject = obj;					//������Կ�����
	m_objectList.emplace_back(gameObject);
}

//�� �Լ��� ����� ������ �̸��� �̸� �޾Ƽ� ������Ʈ�� m_objectName�� �־���
void KunrealEngine::Scene::CreateObject(std::string objectName)
{
	GameObject* gameObject = new GameObject(objectName);
	gameObject->SetObjectName(objectName);
	m_objectList.emplace_back(gameObject);
	gameObject->Initialize();
}

//���޹��� ������Ʈ�� ����
void KunrealEngine::Scene::DeleteGameObject(std::string objectName)
{
	auto iter = find_if(m_objectList.begin(), m_objectList.end(), [&](GameObject* object)
		{
			return object->GetObjectName() == objectName;
		});

	if (iter != m_objectList.end())
	{
		m_objectList.erase(iter);					//�����̳ʿ��� ���ְ�
		delete *iter;								//�����Ѵ�
	}
	else
	{
		MessageBox(0, L"�ش� ���� ������Ʈ�� �����ϴ� ������ �� �����ϴ�", 0, 0);
		//���޹��� �̸��� ������Ʈ�� ����
		return;
	}
}

void KunrealEngine::Scene::DeleteGameObject(GameObject* obj)
{
	auto iter = find(m_objectList.begin(), m_objectList.end(), obj);

	if (iter != m_objectList.end())
	{
		m_objectList.erase(iter);
		delete* iter;
	}
	else
	{
		MessageBox(0, L"�ش� ���� ������Ʈ�� �����ϴ� ������ �� �����ϴ�", 0, 0);
		//���޹��� �̸��� ������Ʈ�� ����
		return;
	}
}

//scene�� �������� string Ÿ�� �̸��� ��ȯ�ϴ� �Լ�
std::string KunrealEngine::Scene::GetSceneName()
{
	return this->m_sceneName;
}

//scene�� ������ �ִ� ������Ʈ���� ����Ʈ�� ��ȯ�ϴ� �Լ�
std::vector<KunrealEngine::GameObject*> KunrealEngine::Scene::GetObjectList()
{
	return this->m_objectList;
}

//�Է��� �̸��� ���� ������Ʈ�� ����
KunrealEngine::GameObject* KunrealEngine::Scene::GetGameObject(std::string objectName)
{
	//������Ʈ ����Ʈ���� ���޹��� �̸��� ���� ������Ʈ�� �ִ��� Ȯ��
	auto iter = find_if(m_objectList.begin(), m_objectList.end(), [&](GameObject* object)
		{
			return object->GetObjectName() == objectName;
		});

	if (iter != m_objectList.end())
	{
		return *iter;
	}
	else
	{
		MessageBox(0, L"�ش� ���� ������Ʈ�� �����ϴ� nullptr�� ��ȯ�մϴ�", 0, 0);
		//�ش� ������Ʈ�� ���ٴ� ǥ��
		return nullptr;
	}
}

//������Ʈ�� ī�޶� ������Ʈ�� �ִ��� Ȯ�� �� ������ �� ī�޶� ���� ī�޶��
void KunrealEngine::Scene::SetMainCamera(GameObject* obj)
{
	if (obj->GetComponent<ObjectCamera>() != nullptr)
	{
		m_mainCamera = obj->GetComponent<ObjectCamera>()->GetCamera();
	}
}

KunrealEngine::Camera* KunrealEngine::Scene::GetMainCamera()
{
	return this->m_mainCamera;
}

