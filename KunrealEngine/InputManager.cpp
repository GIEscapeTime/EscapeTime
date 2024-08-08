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
	// 사용하는 곳에서 InputManager::GetInstance.함수 로 사용하면 된다 싱글톤이라 가능
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
	// 23.07.31 나중에 필요한것만 넣고 수정하시오
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
	if (m_keyMap.find(keyCode) == m_keyMap.end())		// 맵에 없는 키입력시 예외 처리
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
	// 아직 실사용은 애매 한 것 같다.
// 	if (m_firstKeyClickJustPressed == false)			 // 키 두개를 동시에 누르는 경우 이외 예외 처리
// 	{
// 		return false;
// 	}
// 	if (m_keyMap.find(keyCode) == m_keyMap.end())		 // 맵에 없는 키입력시 예외 처리
// 	{
// 		return false;
// 	}
// 
// 	// 첫 번째 누른 키와 같을 때의 예외처리? (필요하면 구현)
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
	if (m_keyMap.find(keyCode) == m_keyMap.end())		// 맵에 없는 키입력시 예외 처리
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
	if (m_keyMap.find(keyCode) == m_keyMap.end())		// 맵에 없는 키입력시 예외 처리
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
