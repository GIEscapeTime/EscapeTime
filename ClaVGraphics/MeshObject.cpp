#include "MeshObject.h"
#include "DeviceAdapter.h"
#include "ResourceManager.h"

using namespace ClaVGraphics;
using namespace DirectX;
using namespace std;

MeshObject::MeshObject()
	:m_pDevice(DeviceAdapter::GetInstance().GetDevice().Get()),
	m_pDeviceContext(DeviceAdapter::GetInstance().GetDeviceContext().Get()),
	m_pNormalMapSRV(nullptr)
{

}

MeshObject::~MeshObject()
{
	SafeRelease(m_pIndexBuffer);
	SafeRelease(m_pVertexBuffer);
	SafeDelete(m_meshData);
	SafeRelease(m_pNormalMapSRV);
	SafeRelease(m_pDiffuseMapSRV);
	SafeRelease(m_pNormalMap);
	SafeRelease(m_pDiffuseMap);
	SafeRelease(m_pEyePow);
	SafeRelease(m_pFxWorldInvTranspose);
	SafeRelease(m_pFxWorld);
	SafeRelease(m_pTech);
	SafeRelease(m_pEffect);
	SafeRelease(m_pInputLayout);
	SafeRelease(m_pRenderState);
	SafeRelease(m_pDeviceContext);
	SafeRelease(m_pDevice);
}

void MeshObject::Initialize(string fileName)
{
	if (fileName == "Glove" || fileName == "Rope" ||
		fileName == "Rope_Connect" || fileName == "Shield")	isIcon = true;
	else isIcon = false;

	m_meshData = ResourceManager::GetInstance().GetFBXData(fileName)->meshData;


	if (!isIcon)
	{
		if (ResourceManager::GetInstance().GetTextureData(fileName) != nullptr)
		{
			m_pDiffuseMapSRV = ResourceManager::GetInstance().GetTextureData(fileName)->diffuse;
			m_pNormalMapSRV = ResourceManager::GetInstance().GetTextureData(fileName)->normal;
		}
	}
	else
	{
		m_pDiffuseMapSRV = ResourceManager::GetInstance().GetResourceData(fileName);
	}


	BuildRenderState();
	BuildGeometryBuffer();
	BuildEffect();
	BuildVertexLayout();

	m_pDirLightsData = new DirectionalLightData();
}

void MeshObject::Update(XMMATRIX trs, XMMATRIX cameraView, XMMATRIX cameraProj)
{
	m_World = trs;
	m_View = cameraView;
	m_Proj = cameraProj;
}

void MeshObject::Render()
{
	// 렌더스테이트
	m_pDeviceContext->RSSetState(m_pRenderState);

	// 입력 배치 객체 셋팅
	m_pDeviceContext->IASetInputLayout(m_pInputLayout);
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 인덱스버퍼와 버텍스버퍼 셋팅
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	/// WVP TM등을 셋팅
	// Set constants
	XMMATRIX WorldViewProj = m_World * m_View * m_Proj;
	m_pFxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&WorldViewProj));

	XMMATRIX worldCopy = m_World;
	worldCopy.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR det = XMMatrixDeterminant(worldCopy);
	XMMATRIX worldInvTranspose = XMMatrixTranspose(XMMatrixInverse(&det, worldCopy));

	m_pFxWorld->SetMatrix(reinterpret_cast<float*>(&m_World));
	m_pFxWorldInvTranspose->SetMatrix(reinterpret_cast<float*>(&worldInvTranspose));
	m_pDiffuseMap->SetResource(m_pDiffuseMapSRV);

	if (!isIcon)
	{
		m_pEyePow->SetRawValue(new XMFLOAT3(-0.73f, 2.3f, 0.73f), 0, sizeof(XMFLOAT3));

		m_pNormalMap->SetResource(m_pNormalMapSRV);
		//if(!m_pNormalMapSRV) ;

		m_pDirLightsData->Ambient = XMFLOAT4(0.14f, 0.11f, 0.11f, 1.f);
		m_pDirLightsData->Diffuse = XMFLOAT4(0.14f, 0.11f, 0.11f, 1.f);
		m_pDirLightsData->Specular = XMFLOAT4(0.14f, 0.11f, 0.11f, 1.f);
		m_pDirLightsData->Direction = XMFLOAT3(0.f, -1.f, 0.f);
		m_pDirectionLight->SetRawValue(m_pDirLightsData, 0, sizeof(DirectionalLightData));

		SetLight(PointLightAdapter::GetInstance().GetPointLights());

		m_pPointLights->SetRawValue(m_pPointLightsData, 0, sizeof(PointLightData) * m_pointLightSize);
		m_pLightCount->SetRawValue(new FLOAT(m_pointLightSize), 0, sizeof(FLOAT));
	}

	D3DX11_TECHNIQUE_DESC techDesc;
	m_pTech->GetDesc(&techDesc);

	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_pTech->GetPassByIndex(p)->Apply(0, m_pDeviceContext);
		m_pDeviceContext->DrawIndexed(m_FbxIndexCount, 0, 0);
	}
}

void MeshObject::BuildEffect()
{
	if (!isIcon)
	{
		ifstream fin(L"../FX/BasicLegacy.cso", ios::binary);
		fin.seekg(0, ios_base::end);
		int size = (int)fin.tellg();
		fin.seekg(0, ios_base::beg);
		vector<char> compliedShader(size);
		fin.read(&compliedShader[0], size);
		fin.close();

		D3DX11CreateEffectFromMemory(&compliedShader[0], size, 0, m_pDevice, &m_pEffect);

		m_pTech = m_pEffect->GetTechniqueByName("LightTextureTech");

		m_pFxWorld = m_pEffect->GetVariableByName("gWorld")->AsMatrix();
		m_pFxWorldInvTranspose = m_pEffect->GetVariableByName("gWorldInvTranspose")->AsMatrix();
		m_pFxWorldViewProj = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
		m_pEyePow = m_pEffect->GetVariableByName("gEyePosW")->AsVector();

		m_pDiffuseMap = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();
		m_pNormalMap = m_pEffect->GetVariableByName("gNormalMap")->AsShaderResource();
		m_pPointLights = m_pEffect->GetVariableByName("gPointLights");
		m_pDirectionLight = m_pEffect->GetVariableByName("gDirectionLight");
		m_pLightCount = m_pEffect->GetVariableByName("gLightCount");
	}
	else
	{
		ifstream fin(L"../FX/Texture.cso", ios::binary);
		fin.seekg(0, ios_base::end);
		int size = (int)fin.tellg();
		fin.seekg(0, ios_base::beg);
		vector<char> compliedShader(size);
		fin.read(&compliedShader[0], size);
		fin.close();

		D3DX11CreateEffectFromMemory(&compliedShader[0], size, 0, m_pDevice, &m_pEffect);

		m_pTech = m_pEffect->GetTechniqueByName("TextureTech");

		m_pFxWorld = m_pEffect->GetVariableByName("gWorld")->AsMatrix();
		m_pFxWorldInvTranspose = m_pEffect->GetVariableByName("gWorldInvTranspose")->AsMatrix();
		m_pFxWorldViewProj = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();

		m_pDiffuseMap = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();
	}
}


void MeshObject::BuildVertexLayout()
{
	if (!isIcon)
	{
		D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		D3DX11_PASS_DESC passDesc;
		m_pTech->GetPassByIndex(0)->GetDesc(&passDesc);

		m_pDevice->CreateInputLayout(vertexDesc, 4, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_pInputLayout);
	}
	else
	{
		D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12 , D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		D3DX11_PASS_DESC passDesc;
		m_pTech->GetPassByIndex(0)->GetDesc(&passDesc);

		m_pDevice->CreateInputLayout(vertexDesc, 3, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_pInputLayout);
	}
}

void MeshObject::BuildGeometryBuffer()
{
	// 버텍스 버퍼
	m_FbxVertexCount = m_meshData->Vertex.size();

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * m_FbxVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &m_meshData->Vertex[0];
	m_pDevice->CreateBuffer(&vertexBufferDesc, &vinitData, &m_pVertexBuffer);

	// 인덱스 버퍼
	m_FbxIndexCount = static_cast<unsigned int>(m_meshData->Index.size());

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * m_FbxIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &m_meshData->Index[0];
	m_pDevice->CreateBuffer(&ibd, &iinitData, &m_pIndexBuffer);
}

void MeshObject::BuildRenderState()
{
	D3D11_RASTERIZER_DESC RenderDesc;
	ZeroMemory(&RenderDesc, sizeof(D3D11_RASTERIZER_DESC));
	RenderDesc.FillMode = D3D11_FILL_SOLID;
	RenderDesc.CullMode = D3D11_CULL_NONE;
	RenderDesc.FrontCounterClockwise = false;
	RenderDesc.DepthClipEnable = true;

	HR(m_pDevice->CreateRasterizerState(&RenderDesc, &m_pRenderState));
}

void MeshObject::SetLight(vector<PointLight*> light)
{
	m_pointLightSize = light.size();
	m_pPointLightsData = new PointLightData[m_pointLightSize];

	int length = 0;
	for (auto& li : light)
	{
		m_pPointLightsData[length] = li->GetLight();
		length++;
	}
}
