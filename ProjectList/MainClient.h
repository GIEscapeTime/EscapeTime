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
/// 메인
/// 여기서 다른 엔진들을 받아서 작업
/// 
/// 23.07.21 주희용
/// </summary>

 namespace GameClient
 {
	 class MainClient
	 {
	 private:
		 // 윈도 초기 위치
		 const int m_windowPosX = 300;
		 const int m_windowPosY = 300;

		 // 화면의 크기
		 const int m_screenWidth = 1600;
		 const int m_screenHeight = 900;

		 // 윈도 관련
		 HWND _hWnd{};
		 MSG _MSG{};
		 
		 // 맵 관리자
		 MapMatrix* m_mapManager;

		 // 엔진
		 KunrealEngine::Core* m_core;

	 private:
		 // for Initialize
		 void RecalcWindowSize();

		 // for Loop
		 void UpdateAll();
		 void RenderAll();			// 상황바서 삭제

	 public:
		 // 게임을 초기화한다.
		 HRESULT Initialize(HINSTANCE hInstance);

		 // 메인 게임 루프. 실제 게임이 진행되는 반복구문
		 void Loop();

		 // 게임을 정리한다.
		 void Finalize();


		 // 메시지 핸들러 (윈도 콜백)
		 static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


		 // 게임 제작 내부 함수들
	 private:
		 void SubFunction();			  // 초기 씬 생성(초기화에서 한번 진행)

		 void PlayerMoveUpdate(float movepoint);

		 void EnemyMoveUpdate();

		 bool PlayerMove = false;
	 };

	 
 }
