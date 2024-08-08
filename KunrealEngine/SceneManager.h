#pragma once
#include "CommonHeader.h"

/// <summary>
/// Scene을 관리해줄 클래스
/// 
/// 하나만 존재해야하고 여기저기 불려다니는 친구다보니 싱글톤으로 했다
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
		//생성, 초기화 관련
		void Initialize();
		void Finalize();

	public:
		//scene을 관리하기 위한 함수들
		Scene* CreateScene(std::string sceneName);
		Scene* GetCurrentScene();
		Scene* GetScene(std::string sceneName);
		void ChangeScene(std::string sceneName);
		std::vector<Scene*> GetSceneList();

	public:
		void UpdateScene(Scene* scene);
		void FixedUpdateScene(Scene* scene);
		void LateUpdateScene(Scene* scene);
		void DeleteScene();					//필요할까 싶지만 혹시 모르니 일단 모양새만
	private:
		Scene* m_currentScene;				//현재 scene을 담아둘 변수
		std::vector<Scene*> m_sceneList;	//만들어진 scene을 담아둘 벡터컨테이너
	};
}


