#include "Box.h"
#include <fstream>
#include "DeviceAdapter.h"

using namespace ClaVGraphics;

Box::Box()
	:m_pDevice(DeviceAdapter::GetInstance().GetDevice().Get()),
	m_pDeviceContext(DeviceAdapter::GetInstance().GetDeviceContext().Get()),
	m_VB(nullptr), m_IB(nullptr), m_Effect(nullptr), m_Tech(nullptr), m_fxWorldViewProj(nullptr), m_pInputLayout(nullptr),
	m_World(), m_View(), m_Proj(),
	m_color(1.f, 1.f, 1.f, 1.f)
{
}

Box::~Box()
{
	SafeRelease(m_Effect);
	SafeRelease(m_fxWorldViewProj);

	SafeRelease(m_VB);
	SafeRelease(m_IB);
}

void Box::Initialize()
{
	BuildRenderState();
	BuildGeometryBuffers();
	BuildFX();
	BuildVertexLayout();
}

void Box::Update(XMMATRIX Matrix, XMMATRIX cameraView, XMMATRIX cameraProj)
{
	m_World = Matrix;
	m_View = cameraView;
	m_Proj = cameraProj;
}

void Box::Render()
{
	// 입력 배치 객체 셋팅
	m_pDeviceContext->IASetInputLayout(m_pInputLayout);
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 인덱스버퍼와 버텍스버퍼 셋팅
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_VB, &stride, &offset);
	m_pDeviceContext->IASetIndexBuffer(m_IB, DXGI_FORMAT_R32_UINT, 0);

	/// WVP TM등을 셋팅
	// Set constants
	XMMATRIX worldViewProj = m_World * m_View * m_Proj;
	m_fxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&worldViewProj));

	// 렌더스테이트
	m_pDeviceContext->RSSetState(m_pRenderState);

	// 테크닉은...
	D3DX11_TECHNIQUE_DESC techDesc;
	m_Tech->GetDesc(&techDesc);

	// 랜더패스는...
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_Tech->GetPassByIndex(p)->Apply(0, m_pDeviceContext);

		// 20개의 인덱스로 그리드를 그린다.
		m_pDeviceContext->DrawIndexed(40, 0, 0);
	}
}

void Box::BuildRenderState()
{
	D3D11_RASTERIZER_DESC RenderDesc;
	ZeroMemory(&RenderDesc, sizeof(D3D11_RASTERIZER_DESC));
	RenderDesc.FillMode = D3D11_FILL_WIREFRAME;
	RenderDesc.CullMode = D3D11_CULL_BACK;
	RenderDesc.FrontCounterClockwise = false;
	RenderDesc.DepthClipEnable = true;

	HR(m_pDevice->CreateRasterizerState(&RenderDesc, &m_pRenderState));
}

void Box::BuildGeometryBuffers()
{
	Vertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, -0.0f), m_color },
		{ XMFLOAT3(-1.0f, +1.0f, +0.0f), m_color },
		{ XMFLOAT3(+1.0f, +1.0f, +0.0f), m_color },	// 우상 증가
		{ XMFLOAT3(+1.0f, -1.0f, +0.0f), m_color },
		{ XMFLOAT3(-1.0f, -1.0f, +1.0f), m_color },
		{ XMFLOAT3(-1.0f, +1.0f, +1.0f), m_color },
		{ XMFLOAT3(+1.0f, +1.0f, +1.0f), m_color },
		{ XMFLOAT3(+1.0f, -1.0f, +1.0f), m_color }
	};

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * 8;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	HR(DeviceAdapter::GetInstance().GetDevice()->CreateBuffer(&vbd, &vinitData, &m_VB));


	/// 인덱스 버퍼를 생성한다.

	UINT indices[] =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 36;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	HR(DeviceAdapter::GetInstance().GetDevice()->CreateBuffer(&ibd, &iinitData, &m_IB));
}

void Box::BuildFX()
{
	std::ifstream fin("../fx/color.cso", std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();

	D3DX11CreateEffectFromMemory(&compiledShader[0], size,
		0, DeviceAdapter::GetInstance().GetDevice().Get(), &m_Effect);

	m_Tech = m_Effect->GetTechniqueByName("ColorTech");
	m_fxWorldViewProj = m_Effect->GetVariableByName("gWorldViewProj")->AsMatrix();
	assert(m_fxWorldViewProj->IsValid());
}

void Box::BuildVertexLayout()
{
	// Create the vertex input layout.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// Create the input layout
	D3DX11_PASS_DESC passDesc;
	m_Tech->GetPassByIndex(0)->GetDesc(&passDesc);
	m_pDevice->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_pInputLayout);
}

void Box::SetColor(float r, float g, float b, float a)
{
	m_color.x = r;
	m_color.y = g;
	m_color.z = b;
	m_color.w = a;
}
