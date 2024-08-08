#pragma once
#include "IRenderer.h"	// �������̽�
#include "DxDefine.h"
#include <wrl.h>

/// �׷��Ƚ� ������ ��ü header
/// 2023.07.20
/// ������

namespace ClaVGraphics
{
	class ClaVRenderer : public IRenderer
	{
	public:
		ClaVRenderer();
		virtual ~ClaVRenderer();

	public:
		// �ʱ�ȭ
		virtual bool Initialize(long hwnd, int Width, int Height) override;
		virtual void Finalize() override;

		// âũ�Ⱑ �ٲ�����
		virtual void OnResize() override;

		// ������ �����ϱ����� �õ��ɱ�
		virtual void BeginRender() override;	// �⺻ ���� ����(BLACK)
		virtual void BeginRender(float r, float g, float b, float alpha) override;	// ����� ���� ���� ����

		// �׸��� �׸���
		virtual void EndRender() override;

	private:
		/// �̰����� Initialize�ȿ� �� �Լ����� �־�а�
		/// ������ Initialize�ȿ� ���� ������ ������ 
		/// ��ɺ��� �Լ��� ���� ���°� �� ����Ұ� ����
		bool CreateDevice();
		void CreateSwapChain();

		/// OnResize�ȿ� ���� ���� �͵�
		void CreateRenderTargetView();
		void CreateDepthStencilView();
		void CreateViewPort();

	private:
		HWND m_hWnd;			// Window Handle
		UINT m_4xMsaaQuality;

		bool m_Enable4xMsaa;

		int m_width;			// ������ â ũ��(����)
		int m_height;			// ������ â ũ��(����)

	private:
		// �׷��� ����̽��� �����ϰ�, �׷��Ƚ� ������ �۾��� ����
		// ���
		// - �׷��� ���ҽ� ����
		// - ���̴� ������ �� ����
		// - ������ ���ؽ�Ʈ ����(Render Target View, Depth Stencil View ���� ��´��� �Բ� ���)
		// - ���̴� ��� �� �ڿ� ����
		// - ������ ��� ����
		Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;

		// Device�� ��ȣ�ۿ��Ͽ� �׷��Ƚ� ����� �����ϰ�, 
		// ������ ���¸� �����ϸ�, 
		// �׷��Ƚ� �ڿ��� ����
		// 3D ��ü�� ������, ���̴� ���α׷��� ����, ������ ������Ʈ, �ؽ����� ���ε� ���� ����
		// ���
		// - �׷��� ���ҽ� ���ε�
		// - ���� Ÿ�� ����
		// - ���� ���ٽ� ����
		// - ���̴� ����
		// - DrawIndexed() : �ε��� ���۸� ����ؼ� ������ �׸��µ� ���
		// - ClearRenderTargetView()�� ClearDepthStencilView(): ���� Ÿ�� ��� ���� ���ٽ� ���� ������ ���� �� ���
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;

		// ���� ü���� �׷��� ī�尡 �������� �̹����� ���÷��� ����Ϳ� ǥ���ϱ� ���� ���
		// ������۸�, Ʈ���� ���۸��� ���� ������� ȭ�鿡 �ε巯�� �������� ����
		// �� ���ۿ� ����Ʈ ���۸� ��ȯ�ϰų�
		// ������� ������ ����� ȭ�鿡 ǥ���ϴ� ���� 
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;

		// ���� Ÿ���� �׷��Ƚ� ������������ ��°���� �����ϴ� �ؽ���
		// ����Ÿ�ٺ��� ���
		// - ���� Ÿ�� �ؽ�ó�� ID3D11RenderTargetView ����
		// - ���� Ÿ�� ���ε�
		// - ���� ���� Ÿ��(Multiple Render Target)
		// - ����Ʈ(Viewport) ����
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;

		// ���� ���ٽ� ����
		// ���� ���� : 3D������Ʈ���� ȭ�鿡 � ������ �׷����� �����ϴµ� ���
		// ���ٽ� ���� : Ư�� �ȼ��� �������� ����ϰų� �����ϴµ� ���
		//				���ٽ� ���۸��� �Բ� ������ ������ ����� �����ϴ� �� ���ȴ�
		// �ֿ���
		// - ���� ���ٽ� ���� ����
		// - ���� ���ٽ� �� ����
		// - ���� ���ٽ� ���� ���ε� : �׷��Ƚ� ������������ ���� ���ٽ� ���ε� ����Ʈ�� ���ε����ش�
		// - ���� �׽�Ʈ, �׸���, ���� ó�� �� �ð��� ȿ���� ����
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;

		// 2D�ؽ�ó�� ��Ÿ���� �������̽�
		// �ؽ�ó �������� �ʺ�, ����, �ȼ� ���� ��� ���� 2D �ؽ�ó�� Ư���� ������ ����
		Microsoft::WRL::ComPtr <ID3D11Texture2D>		m_pTexture2D;

		// �׷��Ƚ� ���������ο��� vertex�����͸� �Է����� �ޱ� ���� ���Ǵ� �Է� ���̾ƿ��� �����Ѵ�
		// �׷��Ƚ� ���������� �Է� �ܰ迡�� ���� �����͸� ó���ϱ� ���� ���
		// �̸� �����ؾ߸� �׷��Ƚ� ������������ ���������� �۵��Ѵ�
		//ID3D11InputLayout* m_pInputLayout;

		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_NormalDSS;
	};
}