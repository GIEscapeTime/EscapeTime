#pragma once
#include "DxDefine.h"
#include "DxEffect.h"
#include "GraphicsData.h"
#include "PointLightAdapter.h"
#include <vector>

// FBX�� �ε��� Mesh���� �����͸� ������ �ִ� Ŭ����
// IB, VB�� ������ �ش�

namespace ClaVGraphics
{
	class MeshObject
	{
	public:
		MeshObject();
		~MeshObject();

		void Initialize(std::string fileName);
		void Update(XMMATRIX trs, XMMATRIX cameraView, XMMATRIX cameraProj);
		void Render();

	private:
		void BuildEffect();
		void BuildVertexLayout();
		void BuildGeometryBuffer();
		void BuildRenderState();
		void SetLight(std::vector<PointLight*> light);

	private:
		ID3D11Device* m_pDevice;
		ID3D11DeviceContext* m_pDeviceContext;
		ID3D11RasterizerState* m_pRenderState;
		ID3D11InputLayout* m_pInputLayout;

		ID3DX11Effect* m_pEffect;
		ID3DX11EffectTechnique* m_pTech;

		ID3DX11EffectMatrixVariable* m_pFxWorld;
		ID3DX11EffectMatrixVariable* m_pFxWorldInvTranspose;
		ID3DX11EffectMatrixVariable* m_pFxWorldViewProj;
		ID3DX11EffectVectorVariable* m_pEyePow;
		ID3DX11EffectVariable* m_pDirectionLight;
		ID3DX11EffectVariable* m_pPointLights;
		ID3DX11EffectVariable* m_pLightCount;

		ID3DX11EffectShaderResourceVariable* m_pDiffuseMap;
		ID3DX11EffectShaderResourceVariable* m_pNormalMap;
		
		ID3D11ShaderResourceView* m_pDiffuseMapSRV;
		ID3D11ShaderResourceView* m_pNormalMapSRV;

		DirectionalLightData* m_pDirLightsData;
		PointLightData* m_pPointLightsData;
		int m_pointLightSize;

		// ��ȯ ����
		XMMATRIX m_World;	// ���� ��ȯ ��� (����->����)
		XMMATRIX m_View;	// �þ� ��ȯ ��� (ī�޶� ��)
		XMMATRIX m_Proj;	// ���� ��ȯ ��� (����/����)

		MeshData* m_meshData;
		ID3D11Buffer* m_pVertexBuffer;
		ID3D11Buffer* m_pIndexBuffer;

		UINT m_FbxVertexCount;
		UINT m_FbxIndexCount;

		bool isIcon;
	};
}