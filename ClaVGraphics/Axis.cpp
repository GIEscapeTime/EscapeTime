#include "Axis.h"
#include "DeviceAdapter.h"

Axis::Axis()
	: m_d3dDevice(DeviceAdapter::GetInstance().GetDevice().Get()),
	m_d3dDeviceContext(DeviceAdapter::GetInstance().GetDeviceContext().Get()), m_renderstate(),
	m_VB(nullptr), m_IB(nullptr), m_FX(nullptr), m_Tech(nullptr), m_fxWorldViewProj(nullptr), m_InputLayout(nullptr),
	m_World(), m_View(), m_Proj()
{

}

Axis::~Axis()
{
	SafeRelease(m_FX);
	SafeRelease(m_fxWorldViewProj);

	SafeRelease(m_VB);
	SafeRelease(m_IB);
}

void Axis::Initialize()
{
	BuildGeometryBuffers();
	BuildFX();
	BuildVertexLayout();
}

void Axis::Update(XMMATRIX cameraView, XMMATRIX cameraProj)
{
	m_World = XMMatrixIdentity();
	m_View = cameraView;
	m_Proj = cameraProj;
}

void Axis::Render()
{
	// 입력 배치 객체 셋팅
	m_d3dDeviceContext->IASetInputLayout(m_InputLayout);
	m_d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// 인덱스버퍼와 버텍스버퍼 셋팅
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_d3dDeviceContext->IASetVertexBuffers(0, 1, &m_VB, &stride, &offset);
	m_d3dDeviceContext->IASetIndexBuffer(m_IB, DXGI_FORMAT_R32_UINT, 0);

	/// WVP TM등을 셋팅
	// Set constants
	XMMATRIX worldViewProj = m_World * m_View * m_Proj;
	m_fxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&worldViewProj));

	// 렌더스테이트
	m_d3dDeviceContext->RSSetState(m_renderstate);

	// 테크닉은...
	D3DX11_TECHNIQUE_DESC techDesc;
	m_Tech->GetDesc(&techDesc);

	// 렌더패스는...
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_Tech->GetPassByIndex(p)->Apply(0, m_d3dDeviceContext);

		// 6개의 인덱스로 축을 그린다.
		m_d3dDeviceContext->DrawIndexed(6, 0, 0);
	}
}

void Axis::BuildGeometryBuffers()
{
	// 정점 버퍼를 생성한다. 
	// 각 축에 맞도록 6개의 정점을 만들었다.
	Vertex vertices[] =
	{
		{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Red)  },	// x축 (빨강)
		{ XMFLOAT3(10.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Red)  },

		{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Green)},	// y축 (초록)
		{ XMFLOAT3(0.0f, 10.0f, 0.0f), XMFLOAT4((const float*)&Colors::Green)},

		{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Blue)	},	// z축 (파랑)
		{ XMFLOAT3(0.0f, 0.0f, 10.0f), XMFLOAT4((const float*)&Colors::Blue) }
	};

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * 6;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	HR(m_d3dDevice->CreateBuffer(&vbd, &vinitData, &m_VB));


	// 인덱스 버퍼를 생성한다.
	// 역시 3개의 축을 나타내도록 했다.

	UINT indices[] = {
		// x축
		0, 1,

		// y축
		2, 3,

		// z축
		4, 5,
	};

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 6;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	HR(m_d3dDevice->CreateBuffer(&ibd, &iinitData, &m_IB));
}

void Axis::BuildFX()
{
	std::ifstream fin("../Fx/color.fxo", std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();

	HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size,
		0, m_d3dDevice, &m_FX));

	m_Tech = m_FX->GetTechniqueByName("ColorTech");
	m_fxWorldViewProj = m_FX->GetVariableByName("gWorldViewProj")->AsMatrix();
}

void Axis::BuildVertexLayout()
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
	HR(m_d3dDevice->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_InputLayout));
}
