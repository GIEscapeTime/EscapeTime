#include "UIController.h"
#include "KunrealAPI.h"
#include "MapMatrix.h"
#include "PlayerComponent.h"
#include "TimeManager.h"
#include "SoundManager.h"

#include <windows.h>
#include "Camera.h"

#define TITLE KunrealEngine::GetScene("TitleScene")
#define INGAME KunrealEngine::GetScene("GAMESCENE")
#define INTRO KunrealEngine::GetScene("IntroScene")

GameClient::UIController::UIController()
	:m_localMM(nullptr)
{

}

GameClient::UIController::~UIController()
{

}

void GameClient::UIController::Initialize(MapMatrix* MM)
{
	KunrealEngine::CreateScene("TitleScene");

	CreateTitleSceneUI();
	CreateGameSceneUI();
	CreateCutSceneUI();
	CreateEndingSceneUI();
	TextMan();
	CloseMenu();

	m_localMM = MM; 
}

void GameClient::UIController::Finalize()
{

}

//반복적으로 실행해야하는 것들
//여유되면 컴포넌트로 빼자
void GameClient::UIController::Update()
{
	if (KunrealEngine::GetCurrentScene()->GetSceneName() == "TitleScene")
	{
		if (TITLE->GetGameObject("StartButton")->GetActivated()
			&& TITLE->GetGameObject("StartButton")->GetComponent<KunrealEngine::Button>()->OnLeftClick() == true)
		{
			ChangeToGameScene();
		}

		if (TITLE->GetGameObject("OptionButton")->GetActivated()
			&& TITLE->GetGameObject("OptionButton")->GetComponent<KunrealEngine::Button>()->OnLeftClick() == true)
		{
			OpenMenu();
		}

		if (TITLE->GetGameObject("MenuExitButton")->GetActivated()
			&& TITLE->GetGameObject("MenuExitButton")->GetComponent<KunrealEngine::Button>()->OnLeftClick() == true)
		{
			CloseMenu();
		}
	}

	if (KunrealEngine::GetCurrentScene()->GetSceneName() == "GAMESCENE")
	{
		if (INGAME->GetGameObject("IngameMenuButton")->GetActivated()
			&& INGAME->GetGameObject("IngameMenuButton")->GetComponent<KunrealEngine::Button>()->OnLeftClick() == true)
		{
			InGameActivateMenu();
		}

		if (INGAME->GetGameObject("MenuExitButton")->GetActivated()
			&& INGAME->GetGameObject("MenuExitButton")->GetComponent<KunrealEngine::Button>()->OnLeftClick() == true)
		{
			InGameInactivateMenu();
		}

		if (INGAME->GetGameObject("ToTitleButton")->GetActivated()
			&& INGAME->GetGameObject("ToTitleButton")->GetComponent<KunrealEngine::Button>()->OnLeftClick() == true)
		{
			ChangeToTitleScene();
		}

		//INGAME->GetGameObject("Text1")->GetComponent<KunrealEngine::TextRenderer>()->SetText(INGAME->GetMainCamera()->GetPosition().x);
		//INGAME->GetGameObject("Text2")->GetComponent<KunrealEngine::TextRenderer>()->SetText(INGAME->GetMainCamera()->GetPosition().y);
		//INGAME->GetGameObject("Text3")->GetComponent<KunrealEngine::TextRenderer>()->SetText(INGAME->GetMainCamera()->GetPosition().z);
		//INGAME->GetGameObject("Text4")->GetComponent<KunrealEngine::TextRenderer>()->SetText(INGAME->GetMainCamera()->m_Look.x);
		//INGAME->GetGameObject("Text5")->GetComponent<KunrealEngine::TextRenderer>()->SetText(INGAME->GetMainCamera()->m_Look.y);
		//INGAME->GetGameObject("Text6")->GetComponent<KunrealEngine::TextRenderer>()->SetText(INGAME->GetMainCamera()->m_Look.z);
		//INGAME->GetGameObject("Text7")->GetComponent<KunrealEngine::TextRenderer>()->SetText(INGAME->GetMainCamera()->m_Up.x);
		//INGAME->GetGameObject("Text8")->GetComponent<KunrealEngine::TextRenderer>()->SetText(INGAME->GetMainCamera()->m_Up.y);
		//INGAME->GetGameObject("Text9")->GetComponent<KunrealEngine::TextRenderer>()->SetText(INGAME->GetMainCamera()->m_Up.z);
	}

	if (KunrealEngine::GetCurrentScene()->GetSceneName() == "EndingScene")
	{
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			ChangeToTitleScene();
		}
	}

	InventoryControl();
	IntroSceneControl();
}

void GameClient::UIController::ChangeToTitleScene()
{
	KunrealEngine::ChangeScene("TitleScene");
}

void GameClient::UIController::ChangeToGameScene()
{
	KunrealEngine::ChangeScene("IntroScene");
	//InGameInactivateMenu();
	//m_localMM->ReStart();
	//KunrealEngine::ChangeScene("GAMESCENE");

}
///

//타이틀 씬에 들어갈 오브젝트들을 생성
void GameClient::UIController::CreateTitleSceneUI()
{
	TITLE->CreateObject("TitleBackground");
	TITLE->CreateObject("TitleLogo");
	TITLE->CreateObject("StartButton");
	TITLE->CreateObject("OptionButton");
	TITLE->CreateObject("ExitButton");

	TITLE->CreateObject("MenuBackground");
	TITLE->CreateObject("BGMLogo");
	TITLE->CreateObject("SFXLogo");
	TITLE->CreateObject("SoundBarUpper");
	TITLE->CreateObject("SoundBarLower");
	TITLE->CreateObject("SoundTextUpper");
	TITLE->CreateObject("SoundTextLower");
	TITLE->CreateObject("SoundButtonUpper");
	TITLE->CreateObject("SoundButtonLower");

	TITLE->CreateObject("OptionLogo");
	TITLE->CreateObject("MenuExitButton");

	m_UIList.emplace_back(TITLE->GetGameObject("TitleBackground"));
	m_UIList.emplace_back(TITLE->GetGameObject("TitleLogo"));
	m_UIList.emplace_back(TITLE->GetGameObject("StartButton"));
	m_UIList.emplace_back(TITLE->GetGameObject("OptionButton"));
	m_UIList.emplace_back(TITLE->GetGameObject("ExitButton"));

	m_UIList.emplace_back(TITLE->GetGameObject("BGMLogo"));
	m_UIList.emplace_back(TITLE->GetGameObject("SFXLogo"));
	m_UIList.emplace_back(TITLE->GetGameObject("SoundBarUpper"));
	m_UIList.emplace_back(TITLE->GetGameObject("SoundBarLower"));
	m_UIList.emplace_back(TITLE->GetGameObject("SoundTextUpper"));
	m_UIList.emplace_back(TITLE->GetGameObject("SoundTextLower"));
	m_UIList.emplace_back(TITLE->GetGameObject("SoundButtonUpper"));
	m_UIList.emplace_back(TITLE->GetGameObject("SoundButtonLower"));

	m_UIList.emplace_back(TITLE->GetGameObject("OptionLogo"));
	m_UIList.emplace_back(TITLE->GetGameObject("MenuExitButton"));
	m_UIList.emplace_back(TITLE->GetGameObject("MenuBackground"));

	//TITLE->GetGameObject("SoundButtonUpper")->AddComponent<KunrealEngine::VolumeControl>();
	//TITLE->GetGameObject("SoundButtonLower")->AddComponent<KunrealEngine::VolumeControl>();

	//만들어준 UI들에 UI관련 컴포넌트를 넣어줌
	for (auto UIs : m_UIList)
	{
		UIs->AddComponent<KunrealEngine::UIRenderer>();
	}

	//메뉴 관련 UI들은 따로 관리
	m_menuUI.emplace_back(TITLE->GetGameObject("BGMLogo"));
	m_menuUI.emplace_back(TITLE->GetGameObject("SFXLogo"));
	m_menuUI.emplace_back(TITLE->GetGameObject("SoundBarUpper"));
	m_menuUI.emplace_back(TITLE->GetGameObject("SoundBarLower"));
	m_menuUI.emplace_back(TITLE->GetGameObject("SoundTextUpper"));
	m_menuUI.emplace_back(TITLE->GetGameObject("SoundTextLower"));
	m_menuUI.emplace_back(TITLE->GetGameObject("SoundButtonUpper"));
	m_menuUI.emplace_back(TITLE->GetGameObject("SoundButtonLower"));

	m_menuUI.emplace_back(TITLE->GetGameObject("OptionLogo"));
	m_menuUI.emplace_back(TITLE->GetGameObject("MenuExitButton"));
	m_menuUI.emplace_back(TITLE->GetGameObject("MenuBackground"));

	//텍스트 띄울 애들은 텍스트 컴포넌트 추가
	TITLE->GetGameObject("SoundTextUpper")->AddComponent<KunrealEngine::TextRenderer>();
	TITLE->GetGameObject("SoundBarLower")->AddComponent<KunrealEngine::TextRenderer>();

	//UI배치
	TITLE->GetGameObject("TitleBackground")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(800, 450);
	TITLE->GetGameObject("TitleBackground")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("titlescene");
	TITLE->GetGameObject("TitleBackground")->SetLayer(BACKGROUND_LAYER);

	TITLE->GetGameObject("TitleLogo")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(1230, 235);
	TITLE->GetGameObject("TitleLogo")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("title");
	TITLE->GetGameObject("TitleLogo")->SetLayer(TITLE_UI_LAYER);

	TITLE->GetGameObject("StartButton")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(1300, 515);
	TITLE->GetGameObject("StartButton")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("start");
	TITLE->GetGameObject("StartButton")->AddComponent<KunrealEngine::Button>();
	TITLE->GetGameObject("StartButton")->GetComponent<KunrealEngine::Button>()->SetSize(500, 90);
	TITLE->GetGameObject("StartButton")->SetLayer(TITLE_UI_LAYER);

	TITLE->GetGameObject("OptionButton")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(1300, 655);
	TITLE->GetGameObject("OptionButton")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("option");
	TITLE->GetGameObject("OptionButton")->AddComponent<KunrealEngine::Button>();
	TITLE->GetGameObject("OptionButton")->GetComponent<KunrealEngine::Button>()->SetSize(500, 90);
	TITLE->GetGameObject("OptionButton")->SetLayer(TITLE_UI_LAYER);

	TITLE->GetGameObject("ExitButton")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(1300, 795);
	TITLE->GetGameObject("ExitButton")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("quit");
	TITLE->GetGameObject("ExitButton")->AddComponent<KunrealEngine::Button>();
	TITLE->GetGameObject("ExitButton")->GetComponent<KunrealEngine::Button>()->SetSize(500, 90);
	TITLE->GetGameObject("ExitButton")->SetLayer(TITLE_UI_LAYER);

	TITLE->GetGameObject("MenuBackground")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(800, 450);
	TITLE->GetGameObject("MenuBackground")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("option_bg");
	TITLE->GetGameObject("MenuBackground")->SetLayer(MENU_BACKGROUND_LAYER);

	TITLE->GetGameObject("BGMLogo")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(485, 380);
	TITLE->GetGameObject("BGMLogo")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("bgm");
	TITLE->GetGameObject("BGMLogo")->SetLayer(MENU_UI_LAYER);

	TITLE->GetGameObject("SFXLogo")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(485, 490);
	TITLE->GetGameObject("SFXLogo")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("sfx");
	TITLE->GetGameObject("SFXLogo")->SetLayer(MENU_UI_LAYER);

	TITLE->GetGameObject("OptionLogo")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(800, 255);
	TITLE->GetGameObject("OptionLogo")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("option_title");
	TITLE->GetGameObject("OptionLogo")->SetLayer(MENU_UI_LAYER);

	TITLE->GetGameObject("SoundBarUpper")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(860, 380);
	TITLE->GetGameObject("SoundBarUpper")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("bar");
	TITLE->GetGameObject("SoundBarUpper")->SetLayer(MENU_UI_LAYER);

	TITLE->GetGameObject("SoundBarLower")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(860, 490);
	TITLE->GetGameObject("SoundBarLower")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("bar");
	TITLE->GetGameObject("SoundBarLower")->SetLayer(MENU_UI_LAYER);

	TITLE->GetGameObject("SoundButtonUpper")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(860, 380);
	TITLE->GetGameObject("SoundButtonUpper")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("bar_butten");
	TITLE->GetGameObject("SoundButtonUpper")->SetLayer(MENU_BUTTON_LAYER);

	TITLE->GetGameObject("SoundButtonLower")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(860, 490);
	TITLE->GetGameObject("SoundButtonLower")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("bar_butten");
	TITLE->GetGameObject("SoundButtonLower")->SetLayer(MENU_BUTTON_LAYER);

	TITLE->GetGameObject("MenuExitButton")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(1255, 205);
	TITLE->GetGameObject("MenuExitButton")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("x");
	TITLE->GetGameObject("MenuExitButton")->SetLayer(MENU_BUTTON_LAYER);
	TITLE->GetGameObject("MenuExitButton")->AddComponent<KunrealEngine::Button>();
	TITLE->GetGameObject("MenuExitButton")->GetComponent<KunrealEngine::Button>()->SetSize(150, 150);

	
}

//게임 메인로직에서 사용할 UI 생성
void GameClient::UIController::CreateGameSceneUI()
{
	INGAME->CreateObject("MenuBackground");
	INGAME->CreateObject("BGMLogo");
	INGAME->CreateObject("SFXLogo");
	INGAME->CreateObject("SoundBarUpper");
	INGAME->CreateObject("SoundBarLower");
	INGAME->CreateObject("SoundTextUpper");
	INGAME->CreateObject("SoundTextLower");
	INGAME->CreateObject("SoundButtonUpper");
	INGAME->CreateObject("SoundButtonLower");
	
	INGAME->CreateObject("OptionLogo");
	INGAME->CreateObject("MenuExitButton");
	INGAME->CreateObject("ToTitleButton");
	INGAME->CreateObject("IngameMenuButton");
								  
	m_IngameMenuList.emplace_back(INGAME->GetGameObject("BGMLogo"));
	m_IngameMenuList.emplace_back(INGAME->GetGameObject("SFXLogo"));
	m_IngameMenuList.emplace_back(INGAME->GetGameObject("SoundBarUpper"));
	m_IngameMenuList.emplace_back(INGAME->GetGameObject("SoundBarLower"));
	m_IngameMenuList.emplace_back(INGAME->GetGameObject("SoundTextUpper"));
	m_IngameMenuList.emplace_back(INGAME->GetGameObject("SoundTextLower"));
	m_IngameMenuList.emplace_back(INGAME->GetGameObject("SoundButtonUpper"));
	m_IngameMenuList.emplace_back(INGAME->GetGameObject("SoundButtonLower"));
								  
	m_IngameMenuList.emplace_back(INGAME->GetGameObject("OptionLogo"));
	m_IngameMenuList.emplace_back(INGAME->GetGameObject("MenuExitButton"));
	m_IngameMenuList.emplace_back(INGAME->GetGameObject("MenuBackground"));
	m_IngameMenuList.emplace_back(INGAME->GetGameObject("ToTitleButton"));

	//만들어준 UI들에 UI관련 컴포넌트를 넣어줌
	for (auto UIs : m_IngameMenuList)
	{
		UIs->AddComponent<KunrealEngine::UIRenderer>();
	}

	INGAME->GetGameObject("MenuBackground")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(800, 450);
	INGAME->GetGameObject("MenuBackground")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("option_bg");
	INGAME->GetGameObject("MenuBackground")->SetLayer(MENU_BACKGROUND_LAYER);
	
	INGAME->GetGameObject("BGMLogo")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(485, 380);
	INGAME->GetGameObject("BGMLogo")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("bgm");
	INGAME->GetGameObject("BGMLogo")->SetLayer(MENU_UI_LAYER);
	
	INGAME->GetGameObject("SFXLogo")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(485, 490);
	INGAME->GetGameObject("SFXLogo")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("sfx");
	INGAME->GetGameObject("SFXLogo")->SetLayer(MENU_UI_LAYER);
	
	INGAME->GetGameObject("OptionLogo")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(800, 255);
	INGAME->GetGameObject("OptionLogo")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("option_title");
	INGAME->GetGameObject("OptionLogo")->SetLayer(MENU_UI_LAYER);
	
	INGAME->GetGameObject("SoundBarUpper")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(860, 380);
	INGAME->GetGameObject("SoundBarUpper")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("bar");
	INGAME->GetGameObject("SoundBarUpper")->SetLayer(MENU_UI_LAYER);
	
	INGAME->GetGameObject("SoundBarLower")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(860, 490);
	INGAME->GetGameObject("SoundBarLower")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("bar");
	INGAME->GetGameObject("SoundBarLower")->SetLayer(MENU_UI_LAYER);
	
	INGAME->GetGameObject("SoundButtonUpper")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(860, 380);
	INGAME->GetGameObject("SoundButtonUpper")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("bar_butten");
	INGAME->GetGameObject("SoundButtonUpper")->SetLayer(MENU_BUTTON_LAYER);
	
	INGAME->GetGameObject("SoundButtonLower")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(860, 490);
	INGAME->GetGameObject("SoundButtonLower")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("bar_butten");
	INGAME->GetGameObject("SoundButtonLower")->SetLayer(MENU_BUTTON_LAYER);
	
	INGAME->GetGameObject("MenuExitButton")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(1255, 205);
	INGAME->GetGameObject("MenuExitButton")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("x");
	INGAME->GetGameObject("MenuExitButton")->SetLayer(MENU_BUTTON_LAYER);
	INGAME->GetGameObject("MenuExitButton")->AddComponent<KunrealEngine::Button>();
	INGAME->GetGameObject("MenuExitButton")->GetComponent<KunrealEngine::Button>()->SetSize(150, 150);

	INGAME->GetGameObject("ToTitleButton")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(800, 635);
	INGAME->GetGameObject("ToTitleButton")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("mainbutten");
	INGAME->GetGameObject("ToTitleButton")->SetLayer(MENU_BUTTON_LAYER);
	INGAME->GetGameObject("ToTitleButton")->AddComponent<KunrealEngine::Button>();
	INGAME->GetGameObject("ToTitleButton")->GetComponent<KunrealEngine::Button>()->SetSize(350, 70);
	

	INGAME->GetGameObject("IngameMenuButton")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(1510, 90);
	INGAME->GetGameObject("IngameMenuButton")->AddComponent<KunrealEngine::UIRenderer>();
	INGAME->GetGameObject("IngameMenuButton")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("Pause_button");
	INGAME->GetGameObject("IngameMenuButton")->SetLayer(MENU_UI_LAYER);
	INGAME->GetGameObject("IngameMenuButton")->AddComponent<KunrealEngine::Button>();
	INGAME->GetGameObject("IngameMenuButton")->GetComponent<KunrealEngine::Button>()->SetSize(100, 100);

	INGAME->CreateObject("InventoryRope");

	INGAME->GetGameObject("InventoryRope")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(575, 790);
	INGAME->GetGameObject("InventoryRope")->AddComponent<KunrealEngine::UIRenderer>();
	INGAME->GetGameObject("InventoryRope")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("item_Rope");
	INGAME->GetGameObject("InventoryRope")->SetLayer(MENU_UI_LAYER);

	INGAME->CreateObject("InventoryGlove");

	INGAME->GetGameObject("InventoryGlove")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(725, 790);
	INGAME->GetGameObject("InventoryGlove")->AddComponent<KunrealEngine::UIRenderer>();
	INGAME->GetGameObject("InventoryGlove")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("item_Glove");
	INGAME->GetGameObject("InventoryGlove")->SetLayer(MENU_UI_LAYER);

	INGAME->CreateObject("InventoryShield");

	INGAME->GetGameObject("InventoryShield")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(875, 790);
	INGAME->GetGameObject("InventoryShield")->AddComponent<KunrealEngine::UIRenderer>();
	INGAME->GetGameObject("InventoryShield")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("item_Shield");
	INGAME->GetGameObject("InventoryShield")->SetLayer(MENU_UI_LAYER);

	INGAME->CreateObject("InventoryRopering");

	INGAME->GetGameObject("InventoryRopering")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(1025, 790);
	INGAME->GetGameObject("InventoryRopering")->AddComponent<KunrealEngine::UIRenderer>();
	INGAME->GetGameObject("InventoryRopering")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("item_Ropering");
	INGAME->GetGameObject("InventoryRopering")->SetLayer(MENU_UI_LAYER);
	
	INGAME->CreateObject("CameraController");
	INGAME->GetGameObject("CameraController")->AddComponent<KunrealEngine::CameraController>();

	for (auto UIs : m_IngameMenuList)
	{
		UIs->GetComponent<KunrealEngine::UIRenderer>()->InactivateComponent();
		UIs->SetActive(false);
	}
}

//컷신에서 사용할 오브젝트들
void GameClient::UIController::CreateCutSceneUI()
{
	KunrealEngine::CreateScene("IntroScene");

	INTRO->CreateObject("CutSceneBack");
	INTRO->CreateObject("CutSceneLeft");
	INTRO->CreateObject("CutSceneCenter");
	INTRO->CreateObject("CutSceneRight");

	INTRO->GetGameObject("CutSceneBack")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(800.f, 450.f);
	INTRO->GetGameObject("CutSceneLeft")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(800.f, 450.f);
	INTRO->GetGameObject("CutSceneCenter")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(800.f, 450.f);
	INTRO->GetGameObject("CutSceneRight")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(800.f, 450.f);

	INTRO->GetGameObject("CutSceneBack")->AddComponent<KunrealEngine::UIRenderer>();
	INTRO->GetGameObject("CutSceneLeft")->AddComponent<KunrealEngine::UIRenderer>();
	INTRO->GetGameObject("CutSceneCenter")->AddComponent<KunrealEngine::UIRenderer>();
	INTRO->GetGameObject("CutSceneRight")->AddComponent<KunrealEngine::UIRenderer>();

	INTRO->GetGameObject("CutSceneBack")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("background");
	INTRO->GetGameObject("CutSceneLeft")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("escapetime_cut_001");
	INTRO->GetGameObject("CutSceneCenter")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("escapetime_cut_002");
	INTRO->GetGameObject("CutSceneRight")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("escapetime_cut_003");

	INTRO->GetGameObject("CutSceneLeft")->GetComponent<KunrealEngine::UIRenderer>()->InactivateComponent();
	INTRO->GetGameObject("CutSceneCenter")->GetComponent<KunrealEngine::UIRenderer>()->InactivateComponent();
	INTRO->GetGameObject("CutSceneRight")->GetComponent<KunrealEngine::UIRenderer>()->InactivateComponent();
}

//엔딩씬
void GameClient::UIController::CreateEndingSceneUI()
{
	KunrealEngine::CreateScene("EndingScene");

	KunrealEngine::GetScene("EndingScene")->CreateObject("EndingCut");
	KunrealEngine::GetScene("EndingScene")->GetGameObject("EndingCut")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(800.f, 450.f);
	KunrealEngine::GetScene("EndingScene")->GetGameObject("EndingCut")->AddComponent<KunrealEngine::UIRenderer>();
	KunrealEngine::GetScene("EndingScene")->GetGameObject("EndingCut")->GetComponent<KunrealEngine::UIRenderer>()->SetSpritePath("ending");

}

//메뉴를 활성화 시키거나
void GameClient::UIController::ActivateMenu()
{
	for (auto UIs : m_menuUI)
	{
		UIs->GetComponent<KunrealEngine::UIRenderer>()->ActivateComponent();

		if (UIs->GetComponent<KunrealEngine::TextRenderer>() != nullptr)
		{
			UIs->GetComponent<KunrealEngine::TextRenderer>()->ActivateComponent();
		}

		UIs->SetActive(true);
	}
}

//메뉴를 비활성화 시킨다
void GameClient::UIController::InactivateMenu()
{
	for (auto UIs : m_menuUI)
	{
		UIs->GetComponent<KunrealEngine::UIRenderer>()->InactivateComponent();

		if (UIs->GetComponent<KunrealEngine::TextRenderer>() != nullptr)
		{
			UIs->GetComponent<KunrealEngine::TextRenderer>()->InactivateComponent();
		}

		UIs->SetActive(false);
	}
}

//옵션 메뉴를 눌렀을때
void GameClient::UIController::OpenMenu()
{
	ActivateMenu();
	KunrealEngine::GetScene("TitleScene")->GetGameObject("StartButton")->SetActive(false);
	KunrealEngine::GetScene("TitleScene")->GetGameObject("OptionButton")->SetActive(false);
	KunrealEngine::GetScene("TitleScene")->GetGameObject("ExitButton")->SetActive(false);
}

//메뉴를 껐을때
void GameClient::UIController::CloseMenu()
{
	InactivateMenu();
	KunrealEngine::GetScene("TitleScene")->GetGameObject("StartButton")->SetActive(true);
	KunrealEngine::GetScene("TitleScene")->GetGameObject("OptionButton")->SetActive(true);
	KunrealEngine::GetScene("TitleScene")->GetGameObject("ExitButton")->SetActive(true);
}

//일단은 당장 떠오르는데로..
void GameClient::UIController::InGameActivateMenu()
{
	for (auto UIs : m_IngameMenuList)
	{
		UIs->GetComponent<KunrealEngine::UIRenderer>()->ActivateComponent();

		if (UIs->GetComponent<KunrealEngine::TextRenderer>() != nullptr)
		{
			UIs->GetComponent<KunrealEngine::TextRenderer>()->ActivateComponent();
		}

		UIs->SetActive(true);
	}

	KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->SetActive(false);
}

void GameClient::UIController::InGameInactivateMenu()
{
	for (auto UIs : m_IngameMenuList)
	{
		UIs->GetComponent<KunrealEngine::UIRenderer>()->InactivateComponent();

		if (UIs->GetComponent<KunrealEngine::TextRenderer>() != nullptr)
		{
			UIs->GetComponent<KunrealEngine::TextRenderer>()->InactivateComponent();
		}

		UIs->SetActive(false);
	}

	KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->SetActive(true);
}


void GameClient::UIController::InventoryControl()
{
	/// 아이템 획득시 2D Sprite 색 변환
	bool getRope = KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_rope;
	bool getGlove = KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_getGlove;
	bool getSheild = KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_getSheild;
	bool getRopeSupport = KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_ropeSurport;

	INGAME->GetGameObject("InventoryRope")->GetComponent<KunrealEngine::UIRenderer>()->SetSpriteActiveColor(getRope);
	INGAME->GetGameObject("InventoryGlove")->GetComponent<KunrealEngine::UIRenderer>()->SetSpriteActiveColor(getGlove);
	INGAME->GetGameObject("InventoryShield")->GetComponent<KunrealEngine::UIRenderer>()->SetSpriteActiveColor(getSheild);
	INGAME->GetGameObject("InventoryRopering")->GetComponent<KunrealEngine::UIRenderer>()->SetSpriteActiveColor(getRopeSupport);


	/// 아이템 선택시 2D Sprite 색 변환
	bool useGlove = KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_glove;
	bool useSheild = KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_Sheild;

	if(getGlove) INGAME->GetGameObject("InventoryGlove")->GetComponent<KunrealEngine::UIRenderer>()->SetSpriteSelectColor(useGlove);
	if(getSheild) INGAME->GetGameObject("InventoryShield")->GetComponent<KunrealEngine::UIRenderer>()->SetSpriteSelectColor(useSheild);

}

//컷씬 시간에 맞춰
void GameClient::UIController::IntroSceneControl()
{
	if (KunrealEngine::GetCurrentScene()->GetSceneName() == "IntroScene")
	{
		m_startTime += KunrealEngine::TimeManager::GetInstance().GetDeltaTime();

		if (m_startTime >= 1.5f)
		{
			INTRO->GetGameObject("CutSceneLeft")->GetComponent<KunrealEngine::UIRenderer>()->ActivateComponent();
		}
		if (m_startTime >= 4.5f)
		{
			INTRO->GetGameObject("CutSceneCenter")->GetComponent<KunrealEngine::UIRenderer>()->ActivateComponent();
		}

		if (m_startTime >= 7.5f)
		{
			INTRO->GetGameObject("CutSceneRight")->GetComponent<KunrealEngine::UIRenderer>()->ActivateComponent();
		}

		if (m_startTime >= 10.5f)
		{
			InGameInactivateMenu();
			m_localMM->ReStart();
			KunrealEngine::ChangeScene("GAMESCENE");
			KunrealEngine::SoundManager::GetInstance().SoundEnd("..\\..\\..\\4_Resources\\Sound\\BGM_MainLobby.wav");
			KunrealEngine::SoundManager::GetInstance().SoundRepeat("..\\..\\..\\4_Resources\\Sound\\BGM_InGame.wav");

			m_startTime = 0.0f;

			INTRO->GetGameObject("CutSceneLeft")->GetComponent<KunrealEngine::UIRenderer>()->InactivateComponent();
			INTRO->GetGameObject("CutSceneCenter")->GetComponent<KunrealEngine::UIRenderer>()->InactivateComponent();
			INTRO->GetGameObject("CutSceneRight")->GetComponent<KunrealEngine::UIRenderer>()->InactivateComponent();

			return;
		}
	}
}

void GameClient::UIController::TextMan()
{
	//INGAME->CreateObject("Text1");
	//INGAME->CreateObject("Text2");
	//INGAME->CreateObject("Text3");
	//INGAME->CreateObject("Text4");
	//INGAME->CreateObject("Text5");
	//INGAME->CreateObject("Text6");
	//INGAME->CreateObject("Text7");
	//INGAME->CreateObject("Text8");
	//INGAME->CreateObject("Text9");
	//
	//INGAME->GetGameObject("Text1")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(100, 100);
	//INGAME->GetGameObject("Text2")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(100, 120);
	//INGAME->GetGameObject("Text3")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(100, 140);
	//INGAME->GetGameObject("Text4")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(100, 160);
	//INGAME->GetGameObject("Text5")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(120, 160);
	//INGAME->GetGameObject("Text6")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(140, 160);
	//INGAME->GetGameObject("Text7")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(100, 180);
	//INGAME->GetGameObject("Text8")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(120, 180);
	//INGAME->GetGameObject("Text9")->GetComponent<KunrealEngine::Transform>()->SetUIPosition(140, 180);
	//											
	//INGAME->GetGameObject("Text1")->AddComponent<KunrealEngine::TextRenderer>();
	//INGAME->GetGameObject("Text2")->AddComponent<KunrealEngine::TextRenderer>();
	//INGAME->GetGameObject("Text3")->AddComponent<KunrealEngine::TextRenderer>();
	//INGAME->GetGameObject("Text4")->AddComponent<KunrealEngine::TextRenderer>();
	//INGAME->GetGameObject("Text5")->AddComponent<KunrealEngine::TextRenderer>();
	//INGAME->GetGameObject("Text6")->AddComponent<KunrealEngine::TextRenderer>();
	//INGAME->GetGameObject("Text7")->AddComponent<KunrealEngine::TextRenderer>();
	//INGAME->GetGameObject("Text8")->AddComponent<KunrealEngine::TextRenderer>();
	//INGAME->GetGameObject("Text9")->AddComponent<KunrealEngine::TextRenderer>();
	//											
	//INGAME->GetGameObject("Text1")->GetComponent<KunrealEngine::TextRenderer>()->SetTextBoxData(100, 100, 14, 14);
	//INGAME->GetGameObject("Text2")->GetComponent<KunrealEngine::TextRenderer>()->SetTextBoxData(100, 120, 14, 14);
	//INGAME->GetGameObject("Text3")->GetComponent<KunrealEngine::TextRenderer>()->SetTextBoxData(100, 140, 14, 14);
	//INGAME->GetGameObject("Text4")->GetComponent<KunrealEngine::TextRenderer>()->SetTextBoxData(100, 160, 14, 14);
	//INGAME->GetGameObject("Text5")->GetComponent<KunrealEngine::TextRenderer>()->SetTextBoxData(200, 160, 14, 14);
	//INGAME->GetGameObject("Text6")->GetComponent<KunrealEngine::TextRenderer>()->SetTextBoxData(300, 160, 14, 14);
	//INGAME->GetGameObject("Text7")->GetComponent<KunrealEngine::TextRenderer>()->SetTextBoxData(100, 180, 14, 14);
	//INGAME->GetGameObject("Text8")->GetComponent<KunrealEngine::TextRenderer>()->SetTextBoxData(200, 180, 14, 14);
	//INGAME->GetGameObject("Text9")->GetComponent<KunrealEngine::TextRenderer>()->SetTextBoxData(300, 180, 14, 14);
	//											
	//INGAME->GetGameObject("Text1")->GetComponent<KunrealEngine::TextRenderer>()->SetFontColor(1, 1, 1);
	//INGAME->GetGameObject("Text2")->GetComponent<KunrealEngine::TextRenderer>()->SetFontColor(1, 1, 1);
	//INGAME->GetGameObject("Text3")->GetComponent<KunrealEngine::TextRenderer>()->SetFontColor(1, 1, 1);
	//INGAME->GetGameObject("Text4")->GetComponent<KunrealEngine::TextRenderer>()->SetFontColor(1, 1, 1);
	//INGAME->GetGameObject("Text5")->GetComponent<KunrealEngine::TextRenderer>()->SetFontColor(1, 1, 1);
	//INGAME->GetGameObject("Text6")->GetComponent<KunrealEngine::TextRenderer>()->SetFontColor(1, 1, 1);
	//INGAME->GetGameObject("Text7")->GetComponent<KunrealEngine::TextRenderer>()->SetFontColor(1, 1, 1);
	//INGAME->GetGameObject("Text8")->GetComponent<KunrealEngine::TextRenderer>()->SetFontColor(1, 1, 1);
	//INGAME->GetGameObject("Text9")->GetComponent<KunrealEngine::TextRenderer>()->SetFontColor(1, 1, 1);
}
