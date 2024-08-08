#pragma once
#include "CommonHeader.h"
#include "Component.h"

#define BACKGROUND_LAYER 1
#define TITLE_UI_LAYER 2
#define MENU_BACKGROUND_LAYER 3
#define MENU_UI_LAYER 4
#define MENU_BUTTON_LAYER 5

namespace KunrealEngine
{
	class GameObject;
}

namespace GameClient
{
	class MapMatrix;

	class UIController : public KunrealEngine::Component
	{
	private:
		UIController();
		~UIController();

	//ΩÃ±€≈Ê
	public:
		static UIController& GetInstance()
		{
			static UIController m_instance;
			return m_instance;
		}

	public:
		void Initialize(MapMatrix* MM);
		void Finalize();

		void Update();

	public:
		void ChangeToTitleScene();
		void ChangeToGameScene();
	private:
		void CreateTitleSceneUI();
		void CreateGameSceneUI();
		void CreateCutSceneUI();
		void CreateEndingSceneUI();

		void ActivateMenu();
		void InactivateMenu();
		void OpenMenu();
		void CloseMenu();

		void InGameActivateMenu();
		void InGameInactivateMenu();

		void InventoryControl();
		void IntroSceneControl();

		void TextMan();
	private:
		std::vector<KunrealEngine::GameObject*> m_UIList; 
		std::vector<KunrealEngine::GameObject*> m_IngameMenuList;
		std::vector< KunrealEngine::GameObject*> m_menuUI;
		MapMatrix* m_localMM;
		float m_startTime = 0.0f;
	};
}

