#include "ClaVGraphics.h"
#include "DeviceAdapter.h"
#include "SpriteAdapter.h"
#include "ResourceManager.h"
#include "SimpleMath.h"

using namespace ClaVGraphics;

ClaVRenderer::ClaVRenderer()
	:m_hWnd(), m_4xMsaaQuality(), m_Enable4xMsaa(), m_width(), m_height()
{
}

ClaVRenderer::~ClaVRenderer()
{
	Finalize();
}

bool ClaVRenderer::Initialize(long hwnd, int Width, int Height)
{
	m_hWnd = (HWND)hwnd;
	m_width = Width;
	m_height = Height;

	CreateDevice();
	CreateSwapChain();
	OnResize();

	DeviceAdapter::GetInstance().SetDevice(m_pDevice);
	DeviceAdapter::GetInstance().SetDeviceContext(m_pDeviceContext);

	ResourceManager::GetInstance().Init();

	SpriteAdapter::GetInstance().Init();
	SpriteAdapter::GetInstance().SetSpriteBatch(new SpriteBatch(m_pDeviceContext.Get()));

	D3D11_DEPTH_STENCIL_DESC equalsDesc;
	ZeroMemory(&equalsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	equalsDesc.DepthEnable = true;
	equalsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;		// 깊이버퍼에 쓰기는 한다
	equalsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	HR(m_pDevice->CreateDepthStencilState(&equalsDesc, &m_NormalDSS));

	return true;
}

void ClaVRenderer::OnResize()
{
	CreateRenderTargetView();
	CreateDepthStencilView();
	CreateViewPort();
}

void ClaVRenderer::BeginRender()
{
	assert(m_pDeviceContext);
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), Colors::Black);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void ClaVRenderer::BeginRender(float r, float g, float b, float alpha)
{
	assert(m_pDeviceContext);

	float color[4];

	// Setup the color to clear the buffer to.
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = alpha;

	//m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_pDeviceContext->OMSetDepthStencilState(m_NormalDSS.Get(), 0);
}

void ClaVRenderer::EndRender()
{
	assert(m_pSwapChain);
	HR(m_pSwapChain->Present(1, 0));
}

void ClaVRenderer::Finalize()
{
}

bool ClaVRenderer::CreateDevice()
{
	UINT flags = 0;
	D3D_FEATURE_LEVEL FeatureLevel;
	HRESULT hr;

#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	hr = D3D11CreateDevice(
		0,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		flags,	0,	0,
		D3D11_SDK_VERSION,
		&m_pDevice,
		&FeatureLevel,
		&m_pDeviceContext);

	if (FAILED(hr))
	{
		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		return false;
	}

	if (FeatureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return false;
	}

	// 4X MSAA 품질수준 지원 점검
	HR(m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMsaaQuality));
	assert(m_4xMsaaQuality > 0);

	return true;
}

void ClaVRenderer::CreateSwapChain()
{
	// Swap Chain
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = m_width;
	sd.BufferDesc.Height = m_height;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if (m_Enable4xMsaa)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = m_hWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;


	// Create Swap Chain
	
	// DXGI(DirectX Graphics Infrastructure)를 사용하여 DXGI 디바이스 및 팩토리에 접근하는 방법을 보여준다
	// DXGI는..
	// - 그래픽스 장치(DXGI 디바이스)
	// - 디스플레이 모니터
	// - 윈도우 창과의 통합
	// 을 당담하는 인터페이스 및 기능을 제공
	IDXGIDevice* pDxgiDevice = 0;
	HR(m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDxgiDevice));

	IDXGIAdapter* pDxgiAdapter = 0;
	//IDXGIAdapter는 그래픽스 어댑터를 나타내며, 그래픽 카드에 해당한다
	HR(pDxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pDxgiAdapter));

	IDXGIFactory* pFactory = 0;
	// IDXGIFactory는 DXGI 객테를 생성하고 관리하는 팩토리이다
	HR(pDxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory));

	HR(pFactory->CreateSwapChain(m_pDevice.Get(), &sd, &m_pSwapChain));

	pDxgiDevice->Release();
	pDxgiAdapter->Release();
	pFactory->Release();
}

void ClaVRenderer::CreateRenderTargetView()
{
	assert(m_pDeviceContext);
	assert(m_pDevice);
	assert(m_pSwapChain);

	ReleaseCOM(m_pRenderTargetView);
	ReleaseCOM(m_pDepthStencilView);
	ReleaseCOM(m_pTexture2D);

	HR(m_pSwapChain->ResizeBuffers(1, m_width, m_height, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

	ID3D11Texture2D* pBackBuffer;
	HR(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer)));
	HR(m_pDevice->CreateRenderTargetView(pBackBuffer, 0, &m_pRenderTargetView));
	SafeRelease(pBackBuffer);
}

void ClaVRenderer::CreateDepthStencilView()
{
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = m_width;
	depthStencilDesc.Height = m_height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	if (m_Enable4xMsaa)
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	HR(m_pDevice->CreateTexture2D(&depthStencilDesc, 0, &m_pTexture2D));
	HR(m_pDevice->CreateDepthStencilView(m_pTexture2D.Get(), 0, &m_pDepthStencilView));

	// Bind View
	m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
}

void ClaVRenderer::CreateViewPort()
{
	// 뷰포트는 3D장면을 표시할 화면의 영역을 지정하는데 사용된다
	// 그래픽스 파이프라인의 출력을 해당 영역에 표시한다
	// 포함된 정보
	// - TopLeftX, TopLeftY : 왼쪽 위 모서리 좌표
	// - Width, Height		: 뷰포트의 너비와 높이(화면에 표시할 영역의 크기를 지정)
	// - MinDepth, MaxDepth	: 뷰포트에서 사용할 깊이값의 범위(0 ~ 1.0)
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = static_cast<float>(m_width);
	vp.Height = static_cast<float>(m_height);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_pDeviceContext->RSSetViewports(1, &vp);
}