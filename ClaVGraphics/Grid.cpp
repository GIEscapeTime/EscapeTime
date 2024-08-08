#include "Grid.h"
#include <fstream>


Grid::Grid(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRenderState)
	: m_d3dDevice(pDevice), m_d3dDeviceContext(pDeviceContext), m_renderstate(pRenderState),
	m_VB(nullptr), m_IB(nullptr), m_FX(nullptr), m_Tech(nullptr), m_fxWorldViewProj(nullptr), m_InputLayout(nullptr),
	m_World(), m_View(), m_Proj()
{
}

Grid::~Grid()
{
	SafeRelease(m_FX);
	SafeRelease(m_fxWorldViewProj);

	SafeRelease(m_VB);
	SafeRelease(m_IB);
}

void Grid::Initialize()
{
	BuildGeometryBuffers();
	BuildFX();
	BuildVertexLayout();
}

void Grid::Update(const Camera& camera)
{
	m_World = XMMatrixIdentity();
	m_View = camera.GetViewXMMATRIX();
	m_Proj = camera.GetProjXMMATRIX();
}

void Grid::Render()
{
	// �Է� ��ġ ��ü ����
	m_d3dDeviceContext->IASetInputLayout(m_InputLayout);
	m_d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// �ε������ۿ� ���ؽ����� ����
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_d3dDeviceContext->IASetVertexBuffers(0, 1, &m_VB, &stride, &offset);
	m_d3dDeviceContext->IASetIndexBuffer(m_IB, DXGI_FORMAT_R32_UINT, 0);

	/// WVP TM���� ����
	// Set constants
	XMMATRIX worldViewProj = m_World * m_View * m_Proj;
	m_fxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&worldViewProj));

	// ����������Ʈ
	m_d3dDeviceContext->RSSetState(m_renderstate);

	// ��ũ����...
	D3DX11_TECHNIQUE_DESC techDesc;
	m_Tech->GetDesc(&techDesc);

	// �����н���...
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_Tech->GetPassByIndex(p)->Apply(0, m_d3dDeviceContext);

		// 20���� �ε����� �׸��带 �׸���.
		m_d3dDeviceContext->DrawIndexed(40, 0, 0);
	}
}

void Grid::BuildGeometryBuffers()
{
	// ���� ���۸� �����Ѵ�. 
	// 40���� ������ �������.
	Vertex vertices[100];
	for (int i = 0; i < 100; i++)
	{
		vertices[i].Pos = XMFLOAT3((float)(i % 10) - 5.0f, 0.0f, (float)(i / 10) - 5.0f);
		vertices[i].Color = XMFLOAT4((const float*)&Colors::DarkGray);	// (��ο� ȸ��)
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * 100;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	m_d3dDevice->CreateBuffer(&vbd, &vinitData, &m_VB);


	// �ε��� ���۸� �����Ѵ�.
	// ���� 40���� ������ ��Ÿ������ �ߴ�.
	UINT indices[40];
	for (int i = 0; i < 10; i++)
	{
		indices[i * 2] = i;
		indices[i * 2 + 1] = i + 90;
	}

	for (int i = 0; i < 10; i++)
	{
		indices[20 + (i * 2)] = i * 10;
		indices[20 + (i * 2) + 1] = i * 10 + 9;
	}

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 40;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	m_d3dDevice->CreateBuffer(&ibd, &iinitData, &m_IB);
}

void Grid::BuildFX()
{
	std::ifstream fin("fx/color.fxo", std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();

	D3DX11CreateEffectFromMemory(&compiledShader[0], size,
		0, m_d3dDevice, &m_FX);

	m_Tech = m_FX->GetTechniqueByName("ColorTech");
	m_fxWorldViewProj = m_FX->GetVariableByName("gWorldViewProj")->AsMatrix();
}

void Grid::BuildVertexLayout()
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
	m_d3dDevice->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_InputLayout);
}
