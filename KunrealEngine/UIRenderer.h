#pragma once
#include "CommonHeader.h"
#include "Component.h"

/// <summary>
/// ��������Ʈ �̹���, ��Ʈ ���� ������ִ� ������Ʈ
/// 
/// ����� �̹����� ��Ʈ�� ���� �ð� ������ �и��� �� �����
/// </summary>

namespace ClaVGraphics
{
	class SpriteRender;
	class Text;
}

namespace KunrealEngine
{
	class Transform;

	class UIRenderer : public Component
	{
	public:
		UIRenderer();
		~UIRenderer();

	public:
		virtual void Initialize() override;
		virtual void Finalize() override;

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void SetSpritePath(std::string filePath);
		void SetSpriteActiveColor(bool active);
		void SetSpriteSelectColor(bool select, float r = 1.f, float g = 1.f, float b = 0.f, float a = 1.f);
		void Render();		

	private:
		ClaVGraphics::SpriteRender* m_renderer;		//�׷��Ƚ��� UI������
		Transform* m_transform;						//�θ� ������Ʈ�� Ʈ�������� ��� ������ ��ü
		std::string m_filePath;						//�̹��� ���
	};
}

