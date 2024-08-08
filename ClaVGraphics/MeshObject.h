#pragma once
#include "DxDefine.h"
#include "DxEffect.h"
#include "GraphicsData.h"
#include "PointLightAdapter.h"
#include <vector>

// FBX로 로드한 Mesh들의 데이터를 가지고 있는 클래스
// IB, VB도 구현해 준다

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

		// 변환 관련
		XMMATRIX m_World;	// 월드 변환 행렬 (로컬->월드)
		XMMATRIX m_View;	// 시야 변환 행렬 (카메라 뷰)
		XMMATRIX m_Proj;	// 투영 변환 행렬 (원근/직교)

		MeshData* m_meshData;
		ID3D11Buffer* m_pVertexBuffer;
		ID3D11Buffer* m_pIndexBuffer;

		UINT m_FbxVertexCount;
		UINT m_FbxIndexCount;

		bool isIcon;
	};
}