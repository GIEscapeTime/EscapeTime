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
		void Init(std::string fileName);	// Load�� FileName
		void Update(DirectX::XMMATRIX trs, DirectX::XMMATRIX cameraView, DirectX::XMMATRIX cameraProj);
		void Render();	// �⺻ �÷�(White)	
		void Render(float r, float g, float b, float a);	// ���� �÷�

	private:
		MeshObject* m_meshObject;
	};
}