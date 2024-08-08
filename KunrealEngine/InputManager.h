#pragma once
#include <windows.h>
#include <map>

#define UP_KEYCODE		0x26
#define DOWN_KEYCODE	0x28
#define LEFT_KEYCODE	0x25
#define RIGHT_KEYCODE	0x27
#define W_KEYCODE		0x57
#define A_KEYCODE		0x41
#define S_KEYCODE		0x53
#define D_KEYCODE		0x44
#define Q_KEYCODE		0x51
#define E_KEYCODE		0x45
#define R_KEYCODE		0x52
#define F_KEYCODE		0x46





/// <summary>
/// 23.07.27 주희용
/// 
/// 팀장님이 바쁜 관계로 대신 만듦
/// </summary>
namespace KunrealEngine
{
	class InputManager
	{
		// 나중에 공용 수학 구조체로 바꿀꺼면 바꾸시오.
		struct MouseStateXY
		{
			long x;
			long y;
		};

	public:
		static InputManager& GetInstance();

		// 초기화(핸들 윈도우 받기)
		void InitialLize(HWND hWnd);

		void InPutUpdateAll();

		// 키보드 입력(동시입력 2개까지..마우스는 따로)
		bool OnceKeyInput(int keyCode);
		bool SecondKeyInput(int keyCode);
		bool HoldKeyInput(int keyCode);
		bool HoldOffKeyInput(int keyCode);


		// 마우스 입력(0x8000 은 키를 오래 누르는 것을 조정)
		bool MouseRightInput();
		bool MouseMiddleInput();
		bool MouseLeftInput();
	
		// 마우스 위치 업데이트
		void UpdateMouseLocation();
		MouseStateXY ReturnMouseState();
	private:

		// 키목록 초기화(키가 추가될 때 마다 고쳐줘야 합니다. 필요 없으면 지우시고)
		void MapInitialLize();


		// 싱글톤
		InputManager();
		~InputManager();
		InputManager(const InputManager& ref);
		InputManager& operator=(const InputManager& ref) = delete;

	private:

		// 마우스 위치
		MouseStateXY m_mouseState;				   // 저장할 마우스 위치
		POINT m_cursorPoint;					   // 가져올 마우스 위치

		// 버튼 눌림 상태
		bool m_mouseRightClickJustPressed;
		bool m_mouseMiddleClickJustPressed;
		bool m_mouseLeftClickJustPressed;
		bool m_firstKeyClickJustPressed;
		bool m_secondKeyClickJustPressed;

		// 핸들
		HDC m_hdc;
		HWND m_hwnd;

		// 키 상태 맵(키를 두개 이상 누를 때를 생각해서 만들었는데 필요 없으면 수정 할 것)
		std::map<int, bool> m_keyMap;

	};
}

