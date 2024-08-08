#pragma once
#include "PlayerComponent.h"
#include "KunrealAPI.h"
#include "SoundManager.h"


namespace GameClient
{
	//��������1 ����Ʈ ����
	void SetStage1Light()
	{
		KunrealEngine::Light::GetInstance().DeleteLight();
		KunrealEngine::Light::GetInstance().AddLight(1.f, 0.12f, 1.f, 0.35f);
		KunrealEngine::Light::GetInstance().AddLight(1.f, 0.12f, -0.1f, 0.35f);
		KunrealEngine::Light::GetInstance().AddLight(1.f, 0.12f, -1.f, 0.35f);
		KunrealEngine::Light::GetInstance().AddLight(-0.05f, 0.12f, -1.f, 0.35f);
		KunrealEngine::Light::GetInstance().AddLight(-1.f, 0.12f, -1.f, 0.35f);
		KunrealEngine::Light::GetInstance().AddLight(-0.49f, 0.12f, -1.5f, 0.19f, 1.f, 1.f, 0.8f);
	}

	//��������2 ����Ʈ ����
	void SetStage2Light()
	{
		// 2��������
		KunrealEngine::Light::GetInstance().DeleteLight();
		KunrealEngine::Light::GetInstance().AddLight(1.f, 0.12f, 1.f, 0.35f);
		KunrealEngine::Light::GetInstance().AddLight(1.f, 0.12f, -1.0f, 0.35f);
		KunrealEngine::Light::GetInstance().AddLight(-1.f, 0.12f, -1.f, 0.35f);
		KunrealEngine::Light::GetInstance().AddLight(-0.49f, 0.12f, -1.5f, 0.19f, 1.f, 1.f, 0.6f);
	}

	//��������3 ����Ʈ ����
	void SetStage3Light()
	{
		KunrealEngine::Light::GetInstance().DeleteLight();
		KunrealEngine::Light::GetInstance().AddLight(1.f, 0.12f, 1.f, 0.35f);
		KunrealEngine::Light::GetInstance().AddLight(1.f, 0.12f, -0.5f, 0.35f);
		KunrealEngine::Light::GetInstance().AddLight(1.1f, 0.12f, -2.1f, 0.35f);
		KunrealEngine::Light::GetInstance().AddLight(0.0f, 0.12f, -2.0f, 0.35f);
		KunrealEngine::Light::GetInstance().AddLight(-1.0f, 0.12f, -2.0f, 0.35f);
		KunrealEngine::Light::GetInstance().AddLight(-2.0f, 0.12f, -2.0f, 0.35f);
		KunrealEngine::Light::GetInstance().AddLight(-0.50f, 0.12f, -2.5f, 0.19f, 1.f, 1.f, 0.6f);
	}

	void CreateSceneAndUIObject(std::string sceneName)
	{
		// �� ����
		KunrealEngine::CreateScene(sceneName);
		KunrealEngine::GetScene(sceneName)->Initialize();

		// ���� �� UI������Ʈ ����
		//KunrealEngine::GetScene(sceneName)->CreateObject("BACKGROUNDIMAGE");
		//KunrealEngine::GetScene(sceneName)->CreateObject("ITEM1");
		//KunrealEngine::GetScene(sceneName)->CreateObject("ITEM2");
		//KunrealEngine::GetScene(sceneName)->CreateObject("ITEM3");
		//KunrealEngine::GetScene(sceneName)->CreateObject("ITEM4");
		//KunrealEngine::GetScene(sceneName)->CreateObject("SETTINGITEM");
		//KunrealEngine::GetScene(sceneName)->CreateObject("SELFKILLBUTTON");
		//KunrealEngine::GetScene(sceneName)->CreateObject("SETTINGBUTTON");
		//
		//KunrealEngine::GetScene(sceneName)->CreateObject("SETTINGBACKGROUND");
		//KunrealEngine::GetScene(sceneName)->CreateObject("SETTINGEXITBUTTON");
		//KunrealEngine::GetScene(sceneName)->CreateObject("BGMSOUNDIMAGE");
		//KunrealEngine::GetScene(sceneName)->CreateObject("SFXSOUNDIMAGE");
		//KunrealEngine::GetScene(sceneName)->CreateObject("BGMON/OFBUTTON");
		//KunrealEngine::GetScene(sceneName)->CreateObject("SFXON/OFBUTTON");
		//KunrealEngine::GetScene(sceneName)->CreateObject("STARTSCENEBUTTON");
		//
		//KunrealEngine::GetScene(sceneName)->CreateObject("EXTRAUI");
		//
		//// ���� �� UI ���̾� ����
		//KunrealEngine::GetScene(sceneName)->GetGameObject("BACKGROUNDIMAGE")->SetLayer(0);
		//KunrealEngine::GetScene(sceneName)->GetGameObject("ITEM1")->SetLayer(20);
		//KunrealEngine::GetScene(sceneName)->GetGameObject("ITEM2")->SetLayer(20);
		//KunrealEngine::GetScene(sceneName)->GetGameObject("ITEM3")->SetLayer(20);
		//KunrealEngine::GetScene(sceneName)->GetGameObject("ITEM4")->SetLayer(20);
		//KunrealEngine::GetScene(sceneName)->GetGameObject("SETTINGITEM")->SetLayer(20);
		//KunrealEngine::GetScene(sceneName)->GetGameObject("SELFKILLBUTTON")->SetLayer(20);
		//KunrealEngine::GetScene(sceneName)->GetGameObject("SETTINGBUTTON")->SetLayer(20);
		//
		//KunrealEngine::GetScene(sceneName)->GetGameObject("SETTINGBACKGROUND")->SetLayer(30);
		//KunrealEngine::GetScene(sceneName)->GetGameObject("SETTINGEXITBUTTON")->SetLayer(31);
		//KunrealEngine::GetScene(sceneName)->GetGameObject("BGMSOUNDIMAGE")->SetLayer(31);
		//KunrealEngine::GetScene(sceneName)->GetGameObject("SFXSOUNDIMAGE")->SetLayer(31);
		//KunrealEngine::GetScene(sceneName)->GetGameObject("BGMON/OFBUTTON")->SetLayer(31);
		//KunrealEngine::GetScene(sceneName)->GetGameObject("SFXON/OFBUTTON")->SetLayer(31);
		//KunrealEngine::GetScene(sceneName)->GetGameObject("STARTSCENEBUTTON")->SetLayer(31);
		//
		//KunrealEngine::GetScene(sceneName)->GetGameObject("EXTRAUI")->SetLayer(40);

	}

	void CreateGameObject(std::string sceneName)
	{
		KunrealEngine::ChangeScene(sceneName);
		KunrealEngine::GetScene(sceneName)->CreateObject("Player");
		KunrealEngine::GetScene(sceneName)->GetGameObject("Player")->SetLayer(50);
		KunrealEngine::GetScene(sceneName)->GetGameObject("Player")->AddComponent<PlayerComponent>();
		KunrealEngine::GetScene(sceneName)->GetGameObject("Player")->AddComponent<KunrealEngine::Renderer>();
		KunrealEngine::GetScene(sceneName)->GetGameObject("Player")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Jade");
		KunrealEngine::GetScene(sceneName)->GetGameObject("Player")->GetComponent<KunrealEngine::Transform>()->SetPosition(-0.725f, 0, 0.725f);
		KunrealEngine::GetScene(sceneName)->GetGameObject("Player")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
		//KunrealEngine::GetScene(sceneName)->GetGameObject("Player")->GetComponent<KunrealEngine::Transform>()->SetRotation(0.f, 0.f, 30.f);

// 		KunrealEngine::GetScene(sceneName)->CreateObject("box1");
// 		KunrealEngine::GetScene(sceneName)->GetGameObject("box1")->SetLayer(50);
// 		KunrealEngine::GetScene(sceneName)->GetGameObject("box1")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.0f, -0.1f, -0.75f);
// 		KunrealEngine::GetScene(sceneName)->GetGameObject("box1")->GetComponent<KunrealEngine::Transform>()->SetScale(1.5f, 0.1f, 1.5f);
// 		KunrealEngine::GetScene(sceneName)->GetGameObject("box1")->AddComponent<KunrealEngine::Renderer>();
// 		KunrealEngine::GetScene(sceneName)->GetGameObject("box1")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("box");
		//SetStage1Light();
	}

	void InitialSound()
	{
		KunrealEngine::SoundManager::GetInstance().AddSound("..\\..\\..\\4_Resources\\DummySound\\1.mp3",KunrealEngine::SoundManager::BACKGROUNDSOUND);
		KunrealEngine::SoundManager::GetInstance().AddSound("..\\..\\..\\4_Resources\\Sound\\BGM_InGame.wav",KunrealEngine::SoundManager::BACKGROUNDSOUND);				// �ΰ��� ���
		KunrealEngine::SoundManager::GetInstance().AddSound("..\\..\\..\\4_Resources\\Sound\\BGM_MainLobby.wav",KunrealEngine::SoundManager::BACKGROUNDSOUND);			// �κ� ���
		KunrealEngine::SoundManager::GetInstance().AddSound("..\\..\\..\\4_Resources\\Sound\\Item_Get.wav",KunrealEngine::SoundManager::EFFECTSOUND);					// ������ ������
		KunrealEngine::SoundManager::GetInstance().AddSound("..\\..\\..\\4_Resources\\Sound\\Item_Swap.wav",KunrealEngine::SoundManager::EFFECTSOUND);					// �尩, ���� ��ȯ
		KunrealEngine::SoundManager::GetInstance().AddSound("..\\..\\..\\4_Resources\\Sound\\Monster_Die_Water.wav",KunrealEngine::SoundManager::EFFECTSOUND);			// ���Ͱ� ���� ���� ���� ��
		KunrealEngine::SoundManager::GetInstance().AddSound("..\\..\\..\\4_Resources\\Sound\\Monster_Move.wav",KunrealEngine::SoundManager::EFFECTSOUND);				// ���Ͱ� �����϶�
		KunrealEngine::SoundManager::GetInstance().AddSound("..\\..\\..\\4_Resources\\Sound\\Player_Crash.wav",KunrealEngine::SoundManager::EFFECTSOUND);				// �÷��̾� ����Ÿ�� ���� ��
		KunrealEngine::SoundManager::GetInstance().AddSound("..\\..\\..\\4_Resources\\Sound\\Player_Die.wav",KunrealEngine::SoundManager::EFFECTSOUND);					// �÷��̾ ���� ��?
		KunrealEngine::SoundManager::GetInstance().AddSound("..\\..\\..\\4_Resources\\Sound\\Player_Die_Water.wav",KunrealEngine::SoundManager::EFFECTSOUND);			// �÷��̾ ���� ���� ��
		KunrealEngine::SoundManager::GetInstance().AddSound("..\\..\\..\\4_Resources\\Sound\\Player_Guard.wav",KunrealEngine::SoundManager::EFFECTSOUND);				// ȭ���� ������ ��
		KunrealEngine::SoundManager::GetInstance().AddSound("..\\..\\..\\4_Resources\\Sound\\Player_Move.wav",KunrealEngine::SoundManager::EFFECTSOUND);				// �÷��̾ ������ ��
		KunrealEngine::SoundManager::GetInstance().AddSound("..\\..\\..\\4_Resources\\Sound\\Player_Rock_Push.wav",KunrealEngine::SoundManager::EFFECTSOUND);			// ������ �� ��
		KunrealEngine::SoundManager::GetInstance().AddSound("..\\..\\..\\4_Resources\\Sound\\Player_Rope_Supporter.wav",KunrealEngine::SoundManager::EFFECTSOUND);		// ���� ������ ��ġ�� ��
		KunrealEngine::SoundManager::GetInstance().AddSound("..\\..\\..\\4_Resources\\Sound\\Player_Rope_Use.wav",KunrealEngine::SoundManager::EFFECTSOUND);			// ���� ����� ��
		KunrealEngine::SoundManager::GetInstance().AddSound("..\\..\\..\\4_Resources\\Sound\\Stage_Clear.wav",KunrealEngine::SoundManager::EFFECTSOUND);				// �������� Ŭ���� �� ��
		KunrealEngine::SoundManager::GetInstance().AddSound("..\\..\\..\\4_Resources\\Sound\\Trap_Arrow.wav",KunrealEngine::SoundManager::EFFECTSOUND);					// ȭ�� ����?

		//KunrealEngine::SoundManager::GetInstance().SoundRepeat("..\\..\\..\\4_Resources\\Sound\\BGM_InGame.wav");
		KunrealEngine::SoundManager::GetInstance().SoundRepeat("..\\..\\..\\4_Resources\\Sound\\BGM_MainLobby.wav");
		
		//KunrealEngine::SoundManager::GetInstance().SoundMute(KunrealEngine::SoundManager::BACKGROUNDSOUND);
		KunrealEngine::SoundManager::GetInstance().SoundVolumeSetting(KunrealEngine::SoundManager::BACKGROUNDSOUND, 0.05f);
		KunrealEngine::SoundManager::GetInstance().SoundVolumeSetting(KunrealEngine::SoundManager::EFFECTSOUND, 0.05f);
	}
}