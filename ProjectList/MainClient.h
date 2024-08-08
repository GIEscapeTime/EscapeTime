#pragma once
#include <windows.h>

 namespace KunrealEngine
 {
 	class Core;
 }

 namespace GameClient
 {
	 class MapMatrix;
 }


/// <summary>
/// ����
/// ���⼭ �ٸ� �������� �޾Ƽ� �۾�
/// 
/// 23.07.21 �����
/// </summary>

 namespace GameClient
 {
	 class MainClient
	 {
	 private:
		 // ���� �ʱ� ��ġ
		 const int m_windowPosX = 300;
		 const int m_windowPosY = 300;

		 // ȭ���� ũ��
		 const int m_screenWidth = 1600;
		 const int m_screenHeight = 900;

		 // ���� ����
		 HWND _hWnd{};
		 MSG _MSG{};
		 
		 // �� ������
		 MapMatrix* m_mapManager;

		 // ����
		 KunrealEngine::Core* m_core;

	 private:
		 // for Initialize
		 void RecalcWindowSize();

		 // for Loop
		 void UpdateAll();
		 void RenderAll();			// ��Ȳ�ټ� ����

	 public:
		 // ������ �ʱ�ȭ�Ѵ�.
		 HRESULT Initialize(HINSTANCE hInstance);

		 // ���� ���� ����. ���� ������ ����Ǵ� �ݺ�����
		 void Loop();

		 // ������ �����Ѵ�.
		 void Finalize();


		 // �޽��� �ڵ鷯 (���� �ݹ�)
		 static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


		 // ���� ���� ���� �Լ���
	 private:
		 void SubFunction();			  // �ʱ� �� ����(�ʱ�ȭ���� �ѹ� ����)

		 void PlayerMoveUpdate(float movepoint);

		 void EnemyMoveUpdate();

		 bool PlayerMove = false;
	 };

	 
 }
