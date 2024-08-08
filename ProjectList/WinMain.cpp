#include "MainClient.h"


/// <summary>
/// 프로그램 시작
/// 
/// 2023.06.16 주희용
/// </summary>
/// <param name="hInstance"></param>
/// <param name="hPrevInstance"></param>
/// <param name="lpCmdLine"></param>
/// <param name="nCmdShow"></param>
/// <returns></returns>
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 게임 진행 클래스
	// 내부에서 윈도 생성과 메시지 콜백을 처리한다.
	// 또한 그래픽스 엔진 등을 포함한다.
	GameClient::MainClient* pMainClient = new GameClient::MainClient();
	pMainClient->Initialize(hInstance);
	pMainClient->Loop();
	pMainClient->Finalize();

	delete pMainClient;
}