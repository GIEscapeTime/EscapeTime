#include "InputManager.h"


KunrealEngine::InputManager::InputManager()
	: m_hdc(0), m_mouseLeftClickJustPressed(false), m_mouseMiddleClickJustPressed(false), m_mouseRightClickJustPressed(false)
	, m_secondKeyClickJustPressed(false), m_firstKeyClickJustPressed(false), m_mouseState(), m_keyMap(), m_cursorPoint()
{

}

KunrealEngine::InputManager::~InputManager()
{
	
}

KunrealEngine::InputManager::InputManager(const InputManager& ref)
	: m_hdc(0), m_mouseLeftClickJustPressed(false), m_mouseMiddleClickJustPressed(false), m_mouseRightClickJustPressed(false)
	, m_secondKeyClickJustPressed(false), m_firstKeyClickJustPressed(false), m_mouseState(), m_keyMap(), m_cursorPoint()
{

}

KunrealEngine::InputManager& KunrealEngine::InputManager::GetInstance()
{
	// ����ϴ� ������ InputManager::GetInstance.�Լ� �� ����ϸ� �ȴ� �̱����̶� ����
	static InputManager instance;
	return instance;
}

void KunrealEngine::InputManager::InitialLize(HWND hWnd)
{
	m_hdc = GetDC(hWnd);
	m_hwnd = hWnd;
	MapInitialLize();
}

void KunrealEngine::InputManager::MapInitialLize()
{
	m_keyMap.insert(std::pair(0x26, false));
	m_keyMap.insert(std::pair(0x28, false));
	m_keyMap.insert(std::pair(0x25, false));
	m_keyMap.insert(std::pair(0x27, false));
	m_keyMap.insert(std::pair(0x41, false));
	m_keyMap.insert(std::pair(0x53, false));
	m_keyMap.insert(std::pair(0x57, false));
	m_keyMap.insert(std::pair(0x44, false));
	m_keyMap.insert(std::pair(0x51, false));
	m_keyMap.insert(std::pair(0x45, false));
	m_keyMap.insert(std::pair(0x52, false));
	m_keyMap.insert(std::pair(0x46, false));
	m_keyMap.insert(std::pair(0x5A, false));

	m_keyMap.insert(std::pair(0x31, false));	//1
	m_keyMap.insert(std::pair(0x32, false));	//2
	m_keyMap.insert(std::pair(0x33, false));	//3
	m_keyMap.insert(std::pair(0x34, false));	//4
}

void KunrealEngine::InputManager::InPutUpdateAll()
{
	// 23.07.31 ���߿� �ʿ��Ѱ͸� �ְ� �����Ͻÿ�
	UpdateMouseLocation();
	MouseRightInput();
	MouseMiddleInput();
	MouseLeftInput();
	//OnceKeyInput(inputkey);
	//SecondKeyInput(inputkey);
}

void KunrealEngine::InputManager::UpdateMouseLocation()
{
	if (GetCursorPos(&m_cursorPoint))
	{
		ScreenToClient(m_hwnd, &m_cursorPoint);
		m_mouseState.x = m_cursorPoint.x;
		m_mouseState.y = m_cursorPoint.y;
	}
}

bool KunrealEngine::InputManager::MouseRightInput()
{
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		m_mouseRightClickJustPressed = true;
		return m_mouseRightClickJustPressed;
	}
	else
	{
		m_mouseRightClickJustPressed = false;
		return m_mouseRightClickJustPressed;
	}
}

bool KunrealEngine::InputManager::MouseMiddleInput()
{
	if (GetAsyncKeyState(VK_MBUTTON) & 0x8000)
	{
		m_mouseMiddleClickJustPressed = true;
		return m_mouseMiddleClickJustPressed;
	}
	else
	{
		m_mouseMiddleClickJustPressed = false;
		return m_mouseMiddleClickJustPressed;
	}
}

bool KunrealEngine::InputManager::MouseLeftInput()
{
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		m_mouseLeftClickJustPressed = true;
		return m_mouseLeftClickJustPressed;
	}
	else
	{
		m_mouseLeftClickJustPressed = false;
		return m_mouseLeftClickJustPressed;
	}
}

KunrealEngine::InputManager::MouseStateXY KunrealEngine::InputManager::ReturnMouseState()
{
	return m_mouseState;
}

bool KunrealEngine::InputManager::OnceKeyInput(int keyCode)
{
	if (m_keyMap.find(keyCode) == m_keyMap.end())		// �ʿ� ���� Ű�Է½� ���� ó��
	{
		return false;
	}

	if (GetAsyncKeyState(keyCode) & 0x8000)
	{
		if (m_keyMap.find(keyCode)->second == false)
		{
			m_keyMap.find(keyCode)->second = true;
			m_firstKeyClickJustPressed = true;
			return m_keyMap.find(keyCode)->second;
		}
	}
	else
	{
		m_firstKeyClickJustPressed = false;
		m_keyMap.find(keyCode)->second = false;
		return m_keyMap.find(keyCode)->second;
	}

	return false;
}

bool KunrealEngine::InputManager::SecondKeyInput(int keyCode)
{
	// ���� �ǻ���� �ָ� �� �� ����.
// 	if (m_firstKeyClickJustPressed == false)			 // Ű �ΰ��� ���ÿ� ������ ��� �̿� ���� ó��
// 	{
// 		return false;
// 	}
// 	if (m_keyMap.find(keyCode) == m_keyMap.end())		 // �ʿ� ���� Ű�Է½� ���� ó��
// 	{
// 		return false;
// 	}
// 
// 	// ù ��° ���� Ű�� ���� ���� ����ó��? (�ʿ��ϸ� ����)
// 
// 	if (GetAsyncKeyState(keyCode) & 0x8000)
// 	{
// 		if (m_keyMap.find(keyCode)->second == false)
// 		{
// 			m_keyMap.find(keyCode)->second = true;
// 			m_secondKeyClickJustPressed = true;
// 			return m_keyMap.find(keyCode)->second;
// 		}
// 	}
// 	else
// 	{
// 		m_secondKeyClickJustPressed = false;
// 		m_keyMap.find(keyCode)->second = false;
// 		return m_keyMap.find(keyCode)->second;
// 	}

	return false;
}

bool KunrealEngine::InputManager::HoldKeyInput(int keyCode)
{
	if (m_keyMap.find(keyCode) == m_keyMap.end())		// �ʿ� ���� Ű�Է½� ���� ó��
	{
		return false;
	}

	if (GetAsyncKeyState(keyCode) & 0x8000)
	{
		m_keyMap.find(keyCode)->second = true;
		return m_keyMap.find(keyCode)->second;
	}
	else
	{
		m_keyMap.find(keyCode)->second = false;
		return m_keyMap.find(keyCode)->second;
	}

	return false;
}

bool KunrealEngine::InputManager::HoldOffKeyInput(int keyCode)
{
	if (m_keyMap.find(keyCode) == m_keyMap.end())		// �ʿ� ���� Ű�Է½� ���� ó��
	{
		return false;
	}

	if (GetAsyncKeyState(keyCode) & 0x8001)
	{
		if (m_keyMap.find(keyCode)->second == false)
		{
			m_firstKeyClickJustPressed = false;
			m_keyMap.find(keyCode)->second = false;
			return m_keyMap.find(keyCode)->second;
		}
	}
	else
	{		
		m_keyMap.find(keyCode)->second = true;
		m_firstKeyClickJustPressed = true;
		return m_keyMap.find(keyCode)->second;
	}

	return false;
}
