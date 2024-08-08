#pragma once
#include "CommonHeader.h"

/// <summary>
/// Scene 클래스들의 부모가 될 클래스
/// 인터페이스로 만들까도 했지만 공통적으로 구현되는 부분이 좀 있는거 같아서 클래스로
/// </summary>

namespace KunrealEngine
{
	class GameObject;
	class Camera;

	class Scene
	{
		friend class SceneManager;
	private:
		Scene();
		Scene(std::string sceneName);
		~Scene();

	public:
		void Initialize();
		void Finalize();
		void FixedUpdate();
		void Update();
		void LateUpdate();

		void Render();

	private:
		std::vector<GameObject*> m_objectList;	//현재 scene에 담긴 오브젝트들
		std::string m_sceneName;				//scene을 구분지을 이름
		Camera* m_mainCamera;					//scene의 화면을 띄워줄 메인 카메라
	public:
		void CreateObject();
		void CreateObject(std::string objectName);
		void AddObject(GameObject* obj);
		void DeleteGameObject(std::string objectName);
		void DeleteGameObject(GameObject* obj);
		std::string GetSceneName();
		std::vector<GameObject*> GetObjectList();
		GameObject* GetGameObject(std::string objectName);
		void SetMainCamera(GameObject* obj);
		Camera* GetMainCamera();
	};
}

