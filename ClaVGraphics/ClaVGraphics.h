#pragma once
#include "IRenderer.h"	// 인터페이스
#include "DxDefine.h"
#include <wrl.h>

/// 그래픽스 엔진의 본체 header
/// 2023.07.20
/// 김현재

namespace ClaVGraphics
{
	class ClaVRenderer : public IRenderer
	{
	public:
		ClaVRenderer();
		virtual ~ClaVRenderer();

	public:
		// 초기화
		virtual bool Initialize(long hwnd, int Width, int Height) override;
		virtual void Finalize() override;

		// 창크기가 바꼈을때
		virtual void OnResize() override;

		// 렌더를 시작하기위해 시동걸기
		virtual void BeginRender() override;	// 기본 배경색 설정(BLACK)
		virtual void BeginRender(float r, float g, float b, float alpha) override;	// 사용자 지정 배경색 설정

		// 그림을 그린다
		virtual void EndRender() override;

	private:
		/// 이곳에는 Initialize안에 들어갈 함수들을 넣어둔곳
		/// 원레는 Initialize안에 때려 박을수 있지만 
		/// 기능별로 함수로 나눠 놓는게 더 깔끔할거 같다
		bool CreateDevice();
		void CreateSwapChain();

		/// OnResize안에 집어 넣을 것들
		void CreateRenderTargetView();
		void CreateDepthStencilView();
		void CreateViewPort();

	private:
		HWND m_hWnd;			// Window Handle
		UINT m_4xMsaaQuality;

		bool m_Enable4xMsaa;

		int m_width;			// 윈도우 창 크기(가로)
		int m_height;			// 윈도우 창 크기(세로)

	private:
		// 그래픽 디바이스를 관리하고, 그래픽스 렌더링 작업을 수행
		// 기능
		// - 그래픽 리소스 관리
		// - 셰이더 컴파일 및 생성
		// - 렌더링 컨텍스트 관리(Render Target View, Depth Stencil View 등의 출력대상과 함께 사용)
		// - 셰이더 상수 및 자원 설정
		// - 렌더링 명령 발행
		Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;

		// Device와 상호작용하여 그래픽스 명령을 발행하고, 
		// 렌더링 상태를 설정하며, 
		// 그래픽스 자원을 관리
		// 3D 객체의 렌더링, 셰이더 프로그램의 실행, 버퍼의 업데이트, 텍스쳐의 바인딩 등을 수행
		// 기능
		// - 그래픽 리소스 바인딩
		// - 렌더 타겟 설정
		// - 깊이 스텐실 설정
		// - 셰이더 실행
		// - DrawIndexed() : 인덱스 버퍼를 사용해서 정점을 그리는데 사용
		// - ClearRenderTargetView()및 ClearDepthStencilView(): 렌더 타겟 뷰와 깊이 스텐실 뷰의 내용을 지울 때 사용
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;

		// 스왑 체인은 그래픽 카드가 렌더링한 이미지를 디스플레이 모니터에 표시하기 위해 사용
		// 더블버퍼링, 트리플 버퍼링과 같은 기법으로 화면에 부드러운 렌더링을 제공
		// 백 버퍼와 프론트 버퍼를 교환하거나
		// 백버퍼의 렌더링 결과를 화면에 표시하는 역할 
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;

		// 렌더 타겟은 그래픽스 파이프라인의 출력결과를 저장하는 텍스쳐
		// 렌더타겟뷰의 기능
		// - 렌더 타겟 텍스처와 ID3D11RenderTargetView 연결
		// - 렌더 타겟 바인딩
		// - 다중 렌더 타겟(Multiple Render Target)
		// - 뷰포트(Viewport) 설정
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;

		// 깊이 스텐실 버퍼
		// 깊이 정보 : 3D오브젝트들이 화면에 어떤 순서로 그려질지 결정하는데 사용
		// 스텐실 정보 : 특정 픽셀의 렌더링을 허용하거나 제한하는데 사용
		//				스텐실 버퍼링과 함께 복잡한 렌더링 기법을 구현하는 데 사용된다
		// 주요기능
		// - 깊이 스텐실 버퍼 생성
		// - 깊이 스텐실 뷰 생성
		// - 깊이 스텐실 버퍼 바인딩 : 그래픽스 파이프라인의 깊이 스텐식 바인드 포인트에 바인드해준다
		// - 깊이 테스트, 그림자, 투명도 처리 등 시각적 효과를 구현
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;

		// 2D텍스처를 나타내는 인터페이스
		// 텍스처 데이터의 너비, 높이, 픽셀 형식 등과 같은 2D 텍스처의 특성과 동작을 정의
		Microsoft::WRL::ComPtr <ID3D11Texture2D>		m_pTexture2D;

		// 그래픽스 파이프라인에서 vertex데이터를 입력으로 받기 위해 사용되는 입력 레이아웃을 정의한다
		// 그래픽스 파이프라인 입력 단계에서 정점 데이터를 처리하기 위해 사용
		// 이를 설정해야만 그래픽스 파이프라인이 정상적으로 작동한다
		//ID3D11InputLayout* m_pInputLayout;

		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_NormalDSS;
	};
}