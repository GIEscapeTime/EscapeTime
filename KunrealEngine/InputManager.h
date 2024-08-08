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
/// 23.07.27 �����
/// 
/// ������� �ٻ� ����� ��� ����
/// </summary>
namespace KunrealEngine
{
	class InputManager
	{
		// ���߿� ���� ���� ����ü�� �ٲܲ��� �ٲٽÿ�.
		struct MouseStateXY
		{
			long x;
			long y;
		};

	public:
		static InputManager& GetInstance();

		// �ʱ�ȭ(�ڵ� ������ �ޱ�)
		void InitialLize(HWND hWnd);

		void InPutUpdateAll();

		// Ű���� �Է�(�����Է� 2������..���콺�� ����)
		bool OnceKeyInput(int keyCode);
		bool SecondKeyInput(int keyCode);
		bool HoldKeyInput(int keyCode);
		bool HoldOffKeyInput(int keyCode);


		// ���콺 �Է�(0x8000 �� Ű�� ���� ������ ���� ����)
		bool MouseRightInput();
		bool MouseMiddleInput();
		bool MouseLeftInput();
	
		// ���콺 ��ġ ������Ʈ
		void UpdateMouseLocation();
		MouseStateXY ReturnMouseState();
	private:

		// Ű��� �ʱ�ȭ(Ű�� �߰��� �� ���� ������� �մϴ�. �ʿ� ������ ����ð�)
		void MapInitialLize();


		// �̱���
		InputManager();
		~InputManager();
		InputManager(const InputManager& ref);
		InputManager& operator=(const InputManager& ref) = delete;

	private:

		// ���콺 ��ġ
		MouseStateXY m_mouseState;				   // ������ ���콺 ��ġ
		POINT m_cursorPoint;					   // ������ ���콺 ��ġ

		// ��ư ���� ����
		bool m_mouseRightClickJustPressed;
		bool m_mouseMiddleClickJustPressed;
		bool m_mouseLeftClickJustPressed;
		bool m_firstKeyClickJustPressed;
		bool m_secondKeyClickJustPressed;

		// �ڵ�
		HDC m_hdc;
		HWND m_hwnd;

		// Ű ���� ��(Ű�� �ΰ� �̻� ���� ���� �����ؼ� ������µ� �ʿ� ������ ���� �� ��)
		std::map<int, bool> m_keyMap;

	};
}

