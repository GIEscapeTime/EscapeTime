#include "MainClient.h"

#include "Core.h"
#include "ClientFunction.h"

#include "InputManager.h"
#include "MapMatrix.h"

#include "UIController.h"

// â ����� ���, Ŭ���̾�Ʈ ������ ũ��� ���� ũ���� ���̸� �����Ѵ�.
void GameClient::MainClient::RecalcWindowSize()
{
	RECT nowRect;
	DWORD _style = (DWORD)GetWindowLong(_hWnd, GWL_STYLE);
	DWORD _exstyle = (DWORD)GetWindowLong(_hWnd, GWL_EXSTYLE);

	GetWindowRect(_hWnd, &nowRect);

	RECT newRect{};
	newRect.left = 0;
	newRect.top = 0;
	newRect.right = m_screenWidth;
	newRect.bottom = m_screenHeight;

	AdjustWindowRectEx(&newRect, _style, NULL, _exstyle);

	// Ŭ���̾�Ʈ �������� ���� ũ��� �� Ŀ�� �Ѵ�. (�ܰ���, Ÿ��Ʋ ��)
	int _newWidth = (newRect.right - newRect.left);
	int _newHeight = (newRect.bottom - newRect.top);

	SetWindowPos(_hWnd, HWND_NOTOPMOST, nowRect.left, nowRect.top,
		_newWidth, _newHeight, SWP_SHOWWINDOW);
}

/// <summary>
/// �ʱ�ȭ
/// </summary>
/// <param name="hInstance">�Κ��Ͻ�</param>
/// <returns></returns>
HRESULT GameClient::MainClient::Initialize(HINSTANCE hInstance)
{
	/// Win32 ����
	// ���� Ŭ����
	wchar_t szAppName[] = L"!(Escape Time)";
	WNDCLASSEXW wcex{};

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = this->WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = szAppName;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);


	// ���� Ŭ���� ���
	RegisterClassExW(&wcex);

	// ���� ����
	_hWnd = CreateWindowW(szAppName, szAppName,
		WS_OVERLAPPEDWINDOW,
		m_windowPosX, m_windowPosY, m_screenWidth, m_screenHeight,
		nullptr, nullptr, hInstance, NULL);

	if (!_hWnd) return -1;

	// ���� ����
	m_core = new KunrealEngine::Core();
	m_core->Initialize(_hWnd);

	// ������ ������ ȭ�鿡 ǥ��
	ShowWindow(_hWnd, SW_SHOWNORMAL);
	UpdateWindow(_hWnd);

	// Ŭ���̾�Ʈ ������ ���ϴ� ���� �ػ󵵰� �ǵ��� ���� ������ 
	RecalcWindowSize();

	GameClient::CreateSceneAndUIObject("GAMESCENE");
	GameClient::InitialSound();
	GameClient::CreateGameObject("GAMESCENE");

	m_mapManager = new MapMatrix("GAMESCENE");
	m_mapManager->Initialize();

	// ���߿� �����
	//m_mapManager->ChangeMap(1);
	UIController::GetInstance().Initialize(m_mapManager);
	//KunrealEngine::ChangeScene("Main");
	KunrealEngine::ChangeScene("TitleScene");
	return S_OK;
}

void GameClient::MainClient::UpdateAll()
{
	m_core->UpdateAll();
	PlayerMoveUpdate(1.5f);
	m_mapManager->PlayerDieCheck();
	m_mapManager->ChangeMap(1);
	m_mapManager->TurnTrapONOFF();
	UIController::GetInstance().Update();
	m_mapManager->UpdatePlayerPosition();
}

void GameClient::MainClient::RenderAll()
{
	m_core->Render();
}

void GameClient::MainClient::Loop()
{
	while (true)
	{
		if (PeekMessage(&_MSG, NULL, 0, 0, PM_REMOVE))
		{
			if (_MSG.message == WM_QUIT) break;

			DispatchMessage(&_MSG);
		}
		else
		{
			UpdateAll();
			RenderAll();
		}
	}
}

void GameClient::MainClient::Finalize()
{
	m_core->Finalize();
}

LRESULT CALLBACK GameClient::MainClient::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC			hdc{};
	PAINTSTRUCT ps{};

	switch (message)
	{
		case WM_ACTIVATE:

			return 0;

		case WM_SIZE:
		{
			// â ũ�� ��ȯ
		}
		return 0;

		case WM_ENTERSIZEMOVE:
		{
			// â ũ�� ����;
		}
		return 0;

		case WM_EXITSIZEMOVE:
		{
			// â ũ�� ����;
		}
		return 0;

		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:

			return 0;

		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:

			return 0;

		case WM_MOUSEMOVE:


		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void GameClient::MainClient::SubFunction()
{
	std::wstring a = L"a";
	OutputDebugString(a.c_str());
}

void GameClient::MainClient::PlayerMoveUpdate(float movepoint)
{
	if (KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->GetActivated())
	{
		if (KunrealEngine::InputManager::GetInstance().OnceKeyInput('A') == true)
		{
			if (m_mapManager->ObjectCheck(-1, 0) == true)
			{
				m_mapManager->CurrentMapXCrease(-1);
				m_mapManager->TurnChange();
				//KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->GetComponent<KunrealEngine::Transform>()->Translate(0.1f * movepoint, 0.f, 0.f);
				KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->GetComponent<KunrealEngine::Transform>()->SetRotation(0.f, 90.f, 0.f);
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Player_Move.wav");

			}
			else
			{
				KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->GetComponent<KunrealEngine::Transform>()->SetRotation(0.f, 90.f, 0.f);
			}
		}

		if (KunrealEngine::InputManager::GetInstance().OnceKeyInput('S') == true)
		{
			if (m_mapManager->ObjectCheck(0, -1) == true)
			{
				m_mapManager->CurrentMapYCrease(-1);
				m_mapManager->TurnChange();
				//KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->GetComponent<KunrealEngine::Transform>()->Translate(0.f, 0.f, 0.1f * movepoint);
				KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->GetComponent<KunrealEngine::Transform>()->SetRotation(0.f, 0.f, 0.f);
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Player_Move.wav");

			}
			else
			{
				KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->GetComponent<KunrealEngine::Transform>()->SetRotation(0.f, 0.f, 0.f);
			}
		}
		if (KunrealEngine::InputManager::GetInstance().OnceKeyInput('D') == true)
		{
			if (m_mapManager->ObjectCheck(1, 0) == true)
			{
				m_mapManager->CurrentMapXCrease(1);
				m_mapManager->TurnChange();
				//KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->GetComponent<KunrealEngine::Transform>()->Translate(-0.1f * movepoint, 0.f, 0.f);
				KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->GetComponent<KunrealEngine::Transform>()->SetRotation(0.f, 270.f, 0.f);
				KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Player_Move.wav");

			}
			else
			{
				KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->GetComponent<KunrealEngine::Transform>()->SetRotation(0.f, 270.f, 0.f);
			}
		}
		if (KunrealEngine::InputManager::GetInstance().OnceKeyInput('W') == true)
		{
			if (m_mapManager->ObjectCheck(0, 1) == true)
			{
				m_mapManager->CurrentMapYCrease(1);
				m_mapManager->TurnChange();
				//KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->GetComponent<KunrealEngine::Transform>()->Translate(0.f, 0.f, -0.1f * movepoint);
				KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->GetComponent<KunrealEngine::Transform>()->SetRotation(0.f, 180.f, 0.f);
				//KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Player_Move.wav");

			}
			else
			{
				KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->GetComponent<KunrealEngine::Transform>()->SetRotation(0.f, 180.f, 0.f);
			}
		}
		if (KunrealEngine::InputManager::GetInstance().OnceKeyInput('1') == true)
		{
			m_mapManager->UseRope();
			KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_Sheild = false;
			KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_glove = false;
		}
		if (KunrealEngine::InputManager::GetInstance().OnceKeyInput('2') == true)
		{
			if (KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_getGlove)
			{
				if (!KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_glove)
				{
					KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Item_Swap.wav");
				}
				KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_Sheild = false;
				KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_glove = true;
			}

		}
		if (KunrealEngine::InputManager::GetInstance().OnceKeyInput('3') == true)
		{
			if (KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_getSheild)
			{
				if (!KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_Sheild)
				{
					KunrealEngine::SoundManager::GetInstance().SoundOnce("..\\..\\..\\4_Resources\\Sound\\Item_Swap.wav");
				}
				KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_Sheild = true;
				KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_glove = false;
			}
		}
		if (KunrealEngine::InputManager::GetInstance().OnceKeyInput('4') == true)
		{
			m_mapManager->UseRopeSurport();
			KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_Sheild = false;
			KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->GetComponent<PlayerComponent>()->m_glove = false;
		}
	}
}

void GameClient::MainClient::EnemyMoveUpdate()  // enemy�� ������Ʈ���� �����ð͵��� �ְ� �̰� ���� ������Ʈ�� ���� �κ�
{


}
