#pragma once
#include "CommonHeader.h"

/// <summary>
/// Scene�� �������� Ŭ����
/// 
/// �ϳ��� �����ؾ��ϰ� �������� �ҷ��ٴϴ� ģ���ٺ��� �̱������� �ߴ�
/// </summary>

namespace KunrealEngine
{
	class Scene;

	class SceneManager
	{
	private:
		SceneManager();
		~SceneManager();

	public:
		static SceneManager& GetInstance()
		{
			static SceneManager m_instance;
			return m_instance;
		}

	public:
		//����, �ʱ�ȭ ����
		void Initialize();
		void Finalize();

	public:
		//scene�� �����ϱ� ���� �Լ���
		Scene* CreateScene(std::string sceneName);
		Scene* GetCurrentScene();
		Scene* GetScene(std::string sceneName);
		void ChangeScene(std::string sceneName);
		std::vector<Scene*> GetSceneList();

	public:
		void UpdateScene(Scene* scene);
		void FixedUpdateScene(Scene* scene);
		void LateUpdateScene(Scene* scene);
		void DeleteScene();					//�ʿ��ұ� ������ Ȥ�� �𸣴� �ϴ� ������
	private:
		Scene* m_currentScene;				//���� scene�� ��Ƶ� ����
		std::vector<Scene*> m_sceneList;	//������� scene�� ��Ƶ� ���������̳�
	};
}


