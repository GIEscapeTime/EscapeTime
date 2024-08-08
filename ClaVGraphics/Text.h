#pragma once
#include "../DXTK/Inc/SpriteFont.h"
#include "SimpleMath.h"
#include <tchar.h>
#include <string>

namespace ClaVGraphics
{
	class Text
	{
	public:
		void Init(std::wstring fontType = L"Medium");
		void SetPos(float x, float y);
		void SetSize(float size);
		void SetColor(float r, float g, float b, float a = 1.f);
		void RenderText(std::wstring text, float lineSpacing);

	private:
		DirectX::SpriteFont* m_font;
		DirectX::SimpleMath::Vector2 m_pos;
		DirectX::SimpleMath::Vector2 m_scale;

		DirectX::SimpleMath::Vector4 m_color;
	};
}
