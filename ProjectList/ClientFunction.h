#pragma once
#include "PlayerComponent.h"
#include "KunrealAPI.h"
#include "SoundManager.h"


namespace GameClient
{
	//스테이지1 라이트 세팅
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

	//스테이지2 라이트 세팅
	void SetStage2Light()
	{
		// 2스테이지
		KunrealEngine::Light::GetInstance().DeleteLight();
		KunrealEngine::Light::GetInstance().AddLight(1.f, 0.12f, 1.f, 0.35f);
		KunrealEngine::Light::GetInstance().AddLight(1.f, 0.12f, -1.0f, 0.35f);
		KunrealEngine::Light::GetInstance().AddLight(-1.f, 0.12f, -1.f, 0.35f);
		KunrealEngine::Light::GetInstance().AddLight(-0.49f, 0.12f, -1.5f, 0.19f, 1.f, 1.f, 0.6f);
	}

	//스테이지3 라이트 세팅
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
		// 씬 생성
		KunrealEngine::CreateScene(sceneName);
		KunrealEngine::GetScene(sceneName)->Initialize();

		// 게임 씬 UI오브젝트 생성
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
		//// 게임 씬 UI 레이어 설정
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
		KunrealEngine::SoundManager::GetInstance().AddSound("..\\..\\..\\4_Resources\\Sound\\BGM_InGame.wav",KunrealEngine::SoundManager::BACKGROUNDSOUND);				// 인게임 브금
		KunrealEngine::SoundManager::GetInstance().AddSound("..\\..\\..\\4_Resources\\Sound\\BGM_MainLobby.wav",KunrealEngine::SoundManager::BACKGROUNDSOUND);			// 로비 브금
		KunrealEngine::SoundManager::GetInstance().AddSound("..\\..\\..\\4_Resources\\Sound\\Item_Get.wav",KunrealEngine::SoundManager::EFFECTSOUND);					// 아이템 먹을때
		KunrealEngine::SoundManager::GetInstance().AddSound("..\\..\\..\\4_Resources\\Sound\\Item_Swap.wav",KunrealEngine::SoundManager::EFFECTSOUND);					// 장갑, 방패 교환
		KunrealEngine::SoundManager::GetInstance().AddSound("..\\..\\..\\4_Resources\\Sound\\Monster_Die_Water.wav",KunrealEngine::SoundManager::EFFECTSOUND);			// 몬스터가 물에 빠져 죽을 때
		KunrealEngine::SoundManager::GetInstance().AddSound("..\\..\\..\\4_Resources\\Sound\\Monster_Move.wav",KunrealEngine::SoundManager::EFFECTSOUND);				// 몬스터가 움직일때
		KunrealEngine::SoundManager::GetInstance().AddSound("..\\..\\..\\4_Resources\\Sound\\Player_Crash.wav",KunrealEngine::SoundManager::EFFECTSOUND);				// 플레이어 로프타다 뒤질 때
		KunrealEngine::SoundManager::GetInstance().AddSound("..\\..\\..\\4_Resources\\Sound\\Player_Die.wav",KunrealEngine::SoundManager::EFFECTSOUND);					// 플레이어가 뒤질 때?
		KunrealEngine::SoundManager::GetInstance().AddSound("..\\..\\..\\4_Resources\\Sound\\Player_Die_Water.wav",KunrealEngine::SoundManager::EFFECTSOUND);			// 플레이어가 물에 빠질 때
		KunrealEngine::SoundManager::GetInstance().AddSound("..\\..\\..\\4_Resources\\Sound\\Player_Guard.wav",KunrealEngine::SoundManager::EFFECTSOUND);				// 화살을 막았을 때
		KunrealEngine::SoundManager::GetInstance().AddSound("..\\..\\..\\4_Resources\\Sound\\Player_Move.wav",KunrealEngine::SoundManager::EFFECTSOUND);				// 플레이어가 움직일 때
		KunrealEngine::SoundManager::GetInstance().AddSound("..\\..\\..\\4_Resources\\Sound\\Player_Rock_Push.wav",KunrealEngine::SoundManager::EFFECTSOUND);			// 바위를 밀 때
		KunrealEngine::SoundManager::GetInstance().AddSound("..\\..\\..\\4_Resources\\Sound\\Player_Rope_Supporter.wav",KunrealEngine::SoundManager::EFFECTSOUND);		// 로프 서포터 설치할 때
		KunrealEngine::SoundManager::GetInstance().AddSound("..\\..\\..\\4_Resources\\Sound\\Player_Rope_Use.wav",KunrealEngine::SoundManager::EFFECTSOUND);			// 로프 사용할 때
		KunrealEngine::SoundManager::GetInstance().AddSound("..\\..\\..\\4_Resources\\Sound\\Stage_Clear.wav",KunrealEngine::SoundManager::EFFECTSOUND);				// 스테이지 클리어 할 때
		KunrealEngine::SoundManager::GetInstance().AddSound("..\\..\\..\\4_Resources\\Sound\\Trap_Arrow.wav",KunrealEngine::SoundManager::EFFECTSOUND);					// 화살 리젠?

		//KunrealEngine::SoundManager::GetInstance().SoundRepeat("..\\..\\..\\4_Resources\\Sound\\BGM_InGame.wav");
		KunrealEngine::SoundManager::GetInstance().SoundRepeat("..\\..\\..\\4_Resources\\Sound\\BGM_MainLobby.wav");
		
		//KunrealEngine::SoundManager::GetInstance().SoundMute(KunrealEngine::SoundManager::BACKGROUNDSOUND);
		KunrealEngine::SoundManager::GetInstance().SoundVolumeSetting(KunrealEngine::SoundManager::BACKGROUNDSOUND, 0.05f);
		KunrealEngine::SoundManager::GetInstance().SoundVolumeSetting(KunrealEngine::SoundManager::EFFECTSOUND, 0.05f);
	}
}