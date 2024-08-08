#include "Scene.h"
#include "Camera.h"
#include "GameObject.h"
#include "TextRenderer.h"
#include "Axis.h"
#include "Light.h"
#include "PointLight.h"
#include <Windows.h>		//메세지 박스를 띄위기 위한 툴이 완성되면 지울것

//Axis* axx;

KunrealEngine::Scene::Scene()
	:m_sceneName(), m_mainCamera(nullptr)
{

}

//sceneName을 string으로 매개변수로 받아서 고유 이름으로 만들어줌
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
		//scene이 생성되면 카메라 오브젝트를 만들어줌
		CreateObject("MainCamera");
		GetGameObject("MainCamera")->AddComponent<ObjectCamera>();
		//기본 카메라 오브젝트가 가진 카메라를 메인 카메라로 설정
		SetMainCamera(GetGameObject("MainCamera"));
	}
	//axx = new Axis();
	//axx->Initialize();
}

//scene이 담고 있는 오브젝트들을 삭제부터 해주고
//scene 자체에 대한 메모리 해제는 SceneManager에서 해줌
void KunrealEngine::Scene::Finalize()
{
	for (auto objects : m_objectList)
	{
		objects->Finalize();
		delete objects;
	}

	m_objectList.clear();
}

//scene 안에 들어있는 오브젝트들을 업데이트
//여기서도 나눠야 하는지는 좀 고민중
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

//오브젝트 중 Renderer 컴포넌트가 있는 것들을 찾아 렌더를 돌려줌
//Renderer가 먼저 있는지 확인 후, active 여부를 확인
void KunrealEngine::Scene::Render()
{
	for (auto objects : m_objectList)
	{
		if (objects->GetComponent<Renderer>() != nullptr)
		{
			//오브젝트의 active여부와는 별개이기 때문에 오브젝트가 꺼져있어도 Renderer컴포넌트가 살아있으면 그림은 그린다
			if (objects->GetComponent<Renderer>()->GetActivated())
			{
				objects->GetComponent<Renderer>()->Render(m_mainCamera->GetViewXMMATRIX(), m_mainCamera->GetProjXMMATRIX());
			}
		}

		//UI전용 렌더러
		if (objects->GetComponent<UIRenderer>() != nullptr)
		{
			//렌더러와 마찬가지로 얘가 살아있으면 그림은 그림
			if (objects->GetComponent<UIRenderer>()->GetActivated())
			{
				objects->GetComponent<UIRenderer>()->Render();
			}
		}

		//텍스트 전용 렌더러
		if (objects->GetComponent<TextRenderer>() != nullptr)
		{
			//렌더러와 마찬가지로 얘가 살아있으면 그림은 그림
			if (objects->GetComponent<TextRenderer>()->GetActivated())
			{
				objects->GetComponent<TextRenderer>()->TextRender();
			}
		}
	}

	//axx->Render();
}

//scene에 오브젝트를 만들어주는 함수
void KunrealEngine::Scene::CreateObject()
{
	GameObject* gameObject = new GameObject();
	m_objectList.emplace_back(gameObject);
	gameObject->Initialize();
}

//이미 만들어진 오브젝트를 씬에다 넣어주는 역할
//필요할지는 모르겠지만 일단 만들어줬다
void KunrealEngine::Scene::AddObject(GameObject* obj)
{
	GameObject* gameObject = obj;					//복사대입연산자
	m_objectList.emplace_back(gameObject);
}

//위 함수와 기능은 같으나 이름을 미리 받아서 오브젝트의 m_objectName에 넣어줌
void KunrealEngine::Scene::CreateObject(std::string objectName)
{
	GameObject* gameObject = new GameObject(objectName);
	gameObject->SetObjectName(objectName);
	m_objectList.emplace_back(gameObject);
	gameObject->Initialize();
}

//전달받은 오브젝트를 삭제
void KunrealEngine::Scene::DeleteGameObject(std::string objectName)
{
	auto iter = find_if(m_objectList.begin(), m_objectList.end(), [&](GameObject* object)
		{
			return object->GetObjectName() == objectName;
		});

	if (iter != m_objectList.end())
	{
		m_objectList.erase(iter);					//컨테이너에서 빼주고
		delete *iter;								//삭제한다
	}
	else
	{
		MessageBox(0, L"해당 게임 오브젝트가 없습니다 삭제할 수 없습니다", 0, 0);
		//전달받은 이름의 오브젝트가 없음
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
		MessageBox(0, L"해당 게임 오브젝트가 없습니다 삭제할 수 없습니다", 0, 0);
		//전달받은 이름의 오브젝트가 없음
		return;
	}
}

//scene을 구분짓은 string 타입 이름을 반환하는 함수
std::string KunrealEngine::Scene::GetSceneName()
{
	return this->m_sceneName;
}

//scene이 가지고 있는 오브젝트들의 리스트를 반환하는 함수
std::vector<KunrealEngine::GameObject*> KunrealEngine::Scene::GetObjectList()
{
	return this->m_objectList;
}

//입력한 이름을 가진 오브젝트를 리턴
KunrealEngine::GameObject* KunrealEngine::Scene::GetGameObject(std::string objectName)
{
	//오브젝트 리스트에서 전달받은 이름을 가진 오브젝트가 있는지 확인
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
		MessageBox(0, L"해당 게임 오브젝트가 없습니다 nullptr을 반환합니다", 0, 0);
		//해당 오브젝트가 없다는 표시
		return nullptr;
	}
}

//오브젝트에 카메라 컴포넌트가 있는지 확인 후 있으면 그 카메라를 메인 카메라로
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

