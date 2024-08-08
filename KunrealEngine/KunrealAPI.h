#pragma once
#include "Scene.h"
#include "Component.h"
#include "GameObject.h"

/// <summary>
/// 특정 클래스 자체는 클라에 넘겨주고 싶지 않은데 그 클래스의 함수는 꼭 호출할 필요가 있을때
/// ex) sceneManager 클래스 자체는 클라가 몰라야하지만 그 기능들은 필요
/// 
/// 이 클래스를 넘겨주고, 필요한 함수 자체만 return 받아서 사용할 수 있게 만든다
/// </summary>

namespace KunrealEngine
{
	//지금은 sceneManager 관련된 기능만 있지만 추후에 여러가지 기능들이 추가될것
	Scene* CreateScene(std::string sceneName);
	Scene* GetScene(std::string sceneName);
	Scene* GetCurrentScene();
	void ChangeScene(std::string sceneName);
	std::vector<Scene*> GetSceneList();

	float GetDeltaTime();
	float GetTimeScale();
	void SetTimeScale(float scale);
}

