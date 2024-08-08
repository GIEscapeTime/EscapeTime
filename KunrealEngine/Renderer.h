#pragma once
#include "CommonHeader.h"
#include "Component.h"
#include <DirectXMath.h>

namespace ClaVGraphics
{
	class ObjectRender;
}

/// <summary>
/// ������Ʈ�� �׸��� �׷��ִ� ������Ʈ
/// 
/// ������Ʈ�� active ���οʹ� ������ ���ư��� ������
/// ������Ʈ�� ���� �־ �� ������Ʈ�� ��������� �׸��� �׸�
/// 
/// �� ������� �� ������
/// �׸��� �׸��� ������Ʈ�� �ٸ� ������Ʈ���� ��ɸ��ϰ� �ϰ� �������� �ְ�
/// �ݴ�� �׸��� ������� �ٸ� ������Ʈ���� �۵��ϰ� �ϰ� �������� �ֱ� ����
/// </summary>

namespace KunrealEngine
{
	class Transform;

	class Renderer : public Component
	{
	public:
		Renderer();
		~Renderer();

	public:
		virtual void Initialize() override;
		virtual void Finalize() override;

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void Render(DirectX::XMMATRIX ViewTM, DirectX::XMMATRIX projTM);
		void SetFBXPath(std::string path);
		std::string GetFBXfileName();

		void SetDebugMode(bool debug);
	private:
		ClaVGraphics::ObjectRender* m_objectRender;		//�׷��Ƚ� ������ ������Ʈ������
		Transform* m_transform;							//�θ� ������Ʈ�� transform
		std::string m_fbxPath;							//FBX ���
		bool m_debug;									//����� ��� ����	//�ؽ�ó�� ������ ����
	};
}


