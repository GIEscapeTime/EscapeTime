#include "Text.h"
#include "SpriteAdapter.h"
#include "DeviceAdapter.h"
#include <dwrite.h>

using namespace ClaVGraphics;
using namespace DirectX;
using namespace std;

void Text::Init(wstring fontType)
{
	wstring fontPath = L"../Resource/Font/DNFF_";
	fontPath += fontType;
	fontPath += L".spritefont";

	m_font =  new SpriteFont(DeviceAdapter::GetInstance().GetDevice().Get(), fontPath.c_str());
}

void Text::SetPos(float x, float y)
{
	m_pos.x = x;
	m_pos.y = y;
}

void Text::SetSize(float size)
{
	m_scale.x = 0.1 * size;
	m_scale.y = 0.1 * size;
}

void Text::SetColor(float r, float g, float b, float a)
{
	m_color.x = r;
	m_color.y = g;
	m_color.z = b;
	m_color.w = a;
}

void Text::RenderText(std::wstring text, float lineSpacing)
{
	SpriteAdapter::GetInstance().GetSpriteBatch()->Begin();

	m_font->SetLineSpacing(lineSpacing);
	m_font->DrawString(SpriteAdapter::GetInstance().GetSpriteBatch(),
		text.c_str(), m_pos, m_color, 
		0.f, DirectX::SimpleMath::Vector2(0.5f, 0.5f),
		m_scale, DX11::SpriteEffects_None, 0.f);

	SpriteAdapter::GetInstance().GetSpriteBatch()->End();
}