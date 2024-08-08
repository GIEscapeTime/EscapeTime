#pragma once
#include "CommonHeader.h"
#include "Component.h"

namespace ClaVGraphics
{
	class Text;
}

/// <summary>
/// ���� ��� �����ϰ� �ϴ� ������Ʈ
/// UIRenderer���� �и���Ŵ
/// 
/// �ٰ��ݰ� �۾�ũ�� �⺻���� 10
/// </summary>

namespace KunrealEngine
{
	class Transform;

	class TextRenderer : public Component
	{
	public:
		TextRenderer();
		~TextRenderer();

	public:
		virtual void Initialize() override;
		virtual void Finalize() override;

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

	public:
		void SetTextBoxData(float posX, float posY, float fontSize, float lineSpace);
		void SetText(std::string txt);
		void SetText(float txt);
		void SetFontColor(float r, float g, float b);
		void SetFontAlpha(float alp);
		
		void TextRender();

	private:
		Transform* m_transform;
		ClaVGraphics::Text* m_textLoader;
		std::string m_txt;
		float m_lineSpace;
		float m_fontSize;
		float m_alpha;
	};
}

