#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include <Windows.h>

KunrealEngine::SceneManager::SceneManager()
	:m_currentScene(nullptr)
{

}

KunrealEngine::SceneManager::~SceneManager()
{
	
}

void KunrealEngine::SceneManager::Initialize()
{
	//scene목록이 비어있으면 Main이라는 이름으로 하나 만들어줌
	if (m_sceneList.empty())
	{
		CreateScene("Main");
	}
	ChangeScene("Main");
	//GetScene("Main")->Initialize();
}

//먼저 scene들을 finalize 해주고 그 다음에 delete로 삭제해
void KunrealEngine::SceneManager::Finalize()
{
	for (Scene* scenes : m_sceneList)
	{
		scenes->Finalize();						//먼저 finalize를 해주고
		delete scenes;							//그 다음에 삭제
	}

	m_sceneList.clear();
}

//scene을 만들어주는 함수
KunrealEngine::Scene* KunrealEngine::SceneManager::CreateScene(std::string sceneName)
{
	auto iter = std::find_if(m_sceneList.begin(), m_sceneList.end(), [&sceneName](KunrealEngine::Scene* scene) { return scene->GetSceneName() == sceneName; });

	if (iter != m_sceneList.end())
	{
		//중복된 이름이 있기 때문에 이 이름으로는 만들 수 없어
		//툴을 통해서 만들 수 없다는 메세지를 사용자가 볼 수 있게 해줘야함
		return nullptr;
	}
	else
	{
		Scene* createdScene = new Scene(sceneName);
		m_sceneList.emplace_back(createdScene);					//밖에서 할 수도 있지만 어차피 넣을거니까

		if (m_currentScene == nullptr)
		{
			m_currentScene = createdScene;
		}

		createdScene->Initialize();
		return createdScene;
	}
}

//현재 scene을 return해주는 함수
KunrealEngine::Scene* KunrealEngine::SceneManager::GetCurrentScene()
{
	if (m_currentScene != nullptr) return m_currentScene;
	else return nullptr;
}

//특정 scene을 리턴하는 함수
//없으면 nullptr
KunrealEngine::Scene* KunrealEngine::SceneManager::GetScene(std::string sceneName)
{
	auto iter = find_if(m_sceneList.begin(), m_sceneList.end(), [&](Scene* scene) {
		return scene->GetSceneName() == sceneName;
		}
	);

	if (iter != m_sceneList.end())
	{
		return *iter;
	}
	else
	{
		MessageBox(0, L"해당 씬이 없습니다 nullptr를 반환합니다", 0, 0);
		return nullptr;
	}
}

//현재 scene을 바꿔주는 함수
//string을 매개변수로 받아서 해당 이름이 있으면 m_currentScene에 넣어줌
void KunrealEngine::SceneManager::ChangeScene(std::string sceneName)
{
	auto iter = std::find_if(m_sceneList.begin(), m_sceneList.end(), [&sceneName](KunrealEngine::Scene* scene) { return scene->GetSceneName() == sceneName; });

	if (iter != m_sceneList.end())
	{
		//찾은게 있다면 현재 scene을 요청한 scene으로 변경해줌
		m_currentScene = *iter;
		//여기서 Initialize는 안한다 초기화랑 scene 전환은 달라

		//불러온 scene에 있는 모든 오브젝트 활성화
		//특정 오브젝트는 비활성화하고 싶다면 Object의 SetActive 함수 사용
		if (m_currentScene->GetObjectList().size() > 0)
		{
			for (auto objects : m_currentScene->GetObjectList())
			{
				objects->SetActive(true);
			}
		}
		
		//scene이 전환됐을 때 현재scene에 없는 오브젝트들은 비활성화
		for (auto scenes : m_sceneList)
		{
			if (scenes != m_currentScene)
			{
				for (auto objects : scenes->GetObjectList())
				{
					objects->SetActive(false);
				}
			}
		}
	}
	else
	{
		//입력한 이름을 가지는 scene이 없기 때문에 설정 불가
		//이것도 툴을 통해서 전환할 수 없다는 메세지를 보여줘야할듯
		MessageBox(0, L"해당 씬이 없습니다 전환할 수 없습니다", 0, 0);
	}
}

//scene 목록을 리턴해주는 함수
std::vector<KunrealEngine::Scene*> KunrealEngine::SceneManager::GetSceneList()
{
	return m_sceneList;
}

//scene별로 업데이트 방식이 다를 수 있기 때문에
//오브젝트들의 업데이트나 개별 업데이트를 매니저 클래스에서 해주도록 해봤음
void KunrealEngine::SceneManager::UpdateScene(Scene* scene)
{
	if (!scene->GetObjectList().empty())
	{
		for (auto& objects : scene->GetObjectList())
		{
			if (objects->GetActivated())
			{
				objects->Update();
			}
		}
	}
	scene->Update();
}



void KunrealEngine::SceneManager::FixedUpdateScene(Scene* scene)
{
	if (!scene->GetObjectList().empty())
	{
		for (auto& objects : scene->GetObjectList())
		{
			if (objects->GetActivated())
			{
				objects->FixedUpdate();
			}
		}
	}
	scene->Update();
}

void KunrealEngine::SceneManager::LateUpdateScene(Scene* scene)
{
	if (!scene->GetObjectList().empty())
	{
		for (auto& objects : scene->GetObjectList())
		{
			if (objects->GetActivated())
			{
				objects->LateUpdate();
			}
		}
	}
	scene->Update();
}

void KunrealEngine::SceneManager::DeleteScene()
{

}
