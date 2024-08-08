#pragma once
#include "CommonHeader.h"

/// <summary>
/// Scene Ŭ�������� �θ� �� Ŭ����
/// �������̽��� ���� ������ ���������� �����Ǵ� �κ��� �� �ִ°� ���Ƽ� Ŭ������
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
		std::vector<GameObject*> m_objectList;	//���� scene�� ��� ������Ʈ��
		std::string m_sceneName;				//scene�� �������� �̸�
		Camera* m_mainCamera;					//scene�� ȭ���� ����� ���� ī�޶�
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

