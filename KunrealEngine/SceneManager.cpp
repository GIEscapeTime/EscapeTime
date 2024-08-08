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
	//scene����� ��������� Main�̶�� �̸����� �ϳ� �������
	if (m_sceneList.empty())
	{
		CreateScene("Main");
	}
	ChangeScene("Main");
	//GetScene("Main")->Initialize();
}

//���� scene���� finalize ���ְ� �� ������ delete�� ������
void KunrealEngine::SceneManager::Finalize()
{
	for (Scene* scenes : m_sceneList)
	{
		scenes->Finalize();						//���� finalize�� ���ְ�
		delete scenes;							//�� ������ ����
	}

	m_sceneList.clear();
}

//scene�� ������ִ� �Լ�
KunrealEngine::Scene* KunrealEngine::SceneManager::CreateScene(std::string sceneName)
{
	auto iter = std::find_if(m_sceneList.begin(), m_sceneList.end(), [&sceneName](KunrealEngine::Scene* scene) { return scene->GetSceneName() == sceneName; });

	if (iter != m_sceneList.end())
	{
		//�ߺ��� �̸��� �ֱ� ������ �� �̸����δ� ���� �� ����
		//���� ���ؼ� ���� �� ���ٴ� �޼����� ����ڰ� �� �� �ְ� �������
		return nullptr;
	}
	else
	{
		Scene* createdScene = new Scene(sceneName);
		m_sceneList.emplace_back(createdScene);					//�ۿ��� �� ���� ������ ������ �����Ŵϱ�

		if (m_currentScene == nullptr)
		{
			m_currentScene = createdScene;
		}

		createdScene->Initialize();
		return createdScene;
	}
}

//���� scene�� return���ִ� �Լ�
KunrealEngine::Scene* KunrealEngine::SceneManager::GetCurrentScene()
{
	if (m_currentScene != nullptr) return m_currentScene;
	else return nullptr;
}

//Ư�� scene�� �����ϴ� �Լ�
//������ nullptr
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
		MessageBox(0, L"�ش� ���� �����ϴ� nullptr�� ��ȯ�մϴ�", 0, 0);
		return nullptr;
	}
}

//���� scene�� �ٲ��ִ� �Լ�
//string�� �Ű������� �޾Ƽ� �ش� �̸��� ������ m_currentScene�� �־���
void KunrealEngine::SceneManager::ChangeScene(std::string sceneName)
{
	auto iter = std::find_if(m_sceneList.begin(), m_sceneList.end(), [&sceneName](KunrealEngine::Scene* scene) { return scene->GetSceneName() == sceneName; });

	if (iter != m_sceneList.end())
	{
		//ã���� �ִٸ� ���� scene�� ��û�� scene���� ��������
		m_currentScene = *iter;
		//���⼭ Initialize�� ���Ѵ� �ʱ�ȭ�� scene ��ȯ�� �޶�

		//�ҷ��� scene�� �ִ� ��� ������Ʈ Ȱ��ȭ
		//Ư�� ������Ʈ�� ��Ȱ��ȭ�ϰ� �ʹٸ� Object�� SetActive �Լ� ���
		if (m_currentScene->GetObjectList().size() > 0)
		{
			for (auto objects : m_currentScene->GetObjectList())
			{
				objects->SetActive(true);
			}
		}
		
		//scene�� ��ȯ���� �� ����scene�� ���� ������Ʈ���� ��Ȱ��ȭ
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
		//�Է��� �̸��� ������ scene�� ���� ������ ���� �Ұ�
		//�̰͵� ���� ���ؼ� ��ȯ�� �� ���ٴ� �޼����� ��������ҵ�
		MessageBox(0, L"�ش� ���� �����ϴ� ��ȯ�� �� �����ϴ�", 0, 0);
	}
}

//scene ����� �������ִ� �Լ�
std::vector<KunrealEngine::Scene*> KunrealEngine::SceneManager::GetSceneList()
{
	return m_sceneList;
}

//scene���� ������Ʈ ����� �ٸ� �� �ֱ� ������
//������Ʈ���� ������Ʈ�� ���� ������Ʈ�� �Ŵ��� Ŭ�������� ���ֵ��� �غ���
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
