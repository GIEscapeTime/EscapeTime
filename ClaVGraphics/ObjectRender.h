#pragma once
#include <string>
#include <vector>
#include <DirectXMath.h>

namespace ClaVGraphics
{
	class MeshObject;

	class ObjectRender
	{
	public:
		void Init(std::string fileName);	// Load할 FileName
		void Update(DirectX::XMMATRIX trs, DirectX::XMMATRIX cameraView, DirectX::XMMATRIX cameraProj);
		void Render();	// 기본 컬러(White)	
		void Render(float r, float g, float b, float a);	// 지정 컬러

	private:
		MeshObject* m_meshObject;
	};
}