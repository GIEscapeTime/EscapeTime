#pragma once
#include "Scene.h"
#include "Component.h"
#include "GameObject.h"

/// <summary>
/// Ư�� Ŭ���� ��ü�� Ŭ�� �Ѱ��ְ� ���� ������ �� Ŭ������ �Լ��� �� ȣ���� �ʿ䰡 ������
/// ex) sceneManager Ŭ���� ��ü�� Ŭ�� ����������� �� ��ɵ��� �ʿ�
/// 
/// �� Ŭ������ �Ѱ��ְ�, �ʿ��� �Լ� ��ü�� return �޾Ƽ� ����� �� �ְ� �����
/// </summary>

namespace KunrealEngine
{
	//������ sceneManager ���õ� ��ɸ� ������ ���Ŀ� �������� ��ɵ��� �߰��ɰ�
	Scene* CreateScene(std::string sceneName);
	Scene* GetScene(std::string sceneName);
	Scene* GetCurrentScene();
	void ChangeScene(std::string sceneName);
	std::vector<Scene*> GetSceneList();

	float GetDeltaTime();
	float GetTimeScale();
	void SetTimeScale(float scale);
}

