#pragma once
#include "DxDefine.h"
#include "Dxeffect.h"

namespace ClaVGraphics
{
	class Box
	{
	public:
		Box();
		~Box();

		void Initialize();
		void Update(XMMATRIX Matrix, XMMATRIX cameraView, XMMATRIX cameraProj);
		void Render();
		void SetColor(float r, float g, float b, float a);

		struct Vertex
		{
			XMFLOAT3 Pos;
			XMFLOAT4 Color;
		};

	private:
		void BuildRenderState();
		void BuildGeometryBuffers();
		void BuildFX();
		void BuildVertexLayout();

	private:
		ID3D11Device* m_pDevice;
		ID3D11DeviceContext* m_pDeviceContext;
		ID3D11RasterizerState* m_pRenderState;
		ID3D11InputLayout* m_pInputLayout;

		ID3D11Buffer* m_VB;
		ID3D11Buffer* m_IB;

		ID3DX11Effect* m_Effect;
		ID3DX11EffectTechnique* m_Tech;
		ID3DX11EffectMatrixVariable* m_fxWorldViewProj;

		XMMATRIX m_World;	// Transform Matrix
		XMMATRIX m_View;
		XMMATRIX m_Proj;

		XMFLOAT4 m_color;
	};
}