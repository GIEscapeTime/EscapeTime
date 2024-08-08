#pragma once
#include "IRendererObject.h"
#include "DxDefine.h"
#include "Dxeffect.h"

class Grid : public IRendererObject
{
public:
	Grid(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRenderState);
	~Grid();

	virtual void Initialize() override;
	virtual void Update(const Camera& camera) override;
	virtual void Render() override;	

	struct Vertex
	{
		XMFLOAT3 Pos;
		XMFLOAT4 Color;
	};

private:
	void BuildGeometryBuffers();
	void BuildFX();
	void BuildVertexLayout();

private:
	ID3D11Device* m_d3dDevice;					// D3D 디바이스
	ID3D11DeviceContext* m_d3dDeviceContext;		// 디바이스 컨텍스트
	ID3D11RasterizerState* m_renderstate;

	ID3D11Buffer* m_VB;
	ID3D11Buffer* m_IB;

	ID3DX11Effect* m_FX;
	ID3DX11EffectTechnique* m_Tech;
	ID3DX11EffectMatrixVariable* m_fxWorldViewProj;

	ID3D11InputLayout* m_InputLayout;

	XMMATRIX m_World;	// Transform Matrix
	XMMATRIX m_View;
	XMMATRIX m_Proj;
};

