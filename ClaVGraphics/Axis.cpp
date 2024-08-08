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

		// 6���� �ε����� ���� �׸���.
		m_d3dDeviceContext->DrawIndexed(6, 0, 0);
	}
}

void Axis::BuildGeometryBuffers()
{
	// ���� ���۸� �����Ѵ�. 
	// �� �࿡ �µ��� 6���� ������ �������.
	Vertex vertices[] =
	{
		{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Red)  },	// x�� (����)
		{ XMFLOAT3(10.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Red)  },

		{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Green)},	// y�� (�ʷ�)
		{ XMFLOAT3(0.0f, 10.0f, 0.0f), XMFLOAT4((const float*)&Colors::Green)},

		{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4((const float*)&Colors::Blue)	},	// z�� (�Ķ�)
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


	// �ε��� ���۸� �����Ѵ�.
	// ���� 3���� ���� ��Ÿ������ �ߴ�.

	UINT indices[] = {
		// x��
		0, 1,

		// y��
		2, 3,

		// z��
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
