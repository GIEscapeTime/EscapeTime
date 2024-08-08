#include "SpriteRender.h"
#include "ResourceManager.h"

using namespace ClaVGraphics;
using namespace DirectX;
using namespace std;

void SpriteRender::Init(string fileName)
{
	m_spriteBatch = SpriteAdapter::GetInstance().GetSpriteBatch();
	m_fileName = fileName;
}

void ClaVGraphics::SpriteRender::SetPosition(float x, float y)
{
	m_pos.x = x;
	m_pos.y = y;
}

void ClaVGraphics::SpriteRender::SetScale(float x, float y)
{
	m_scale.x = x;
	m_scale.y = y;
}

void ClaVGraphics::SpriteRender::SetRotation(float angle)
{
	m_angle = angle * DirectX::XM_PI / 180.f;
}

void ClaVGraphics::SpriteRender::SetColor(float r, float g, float b, float a)
{
	m_colorR = r;
	m_colorG = g;
	m_colorB = b;
	m_colorA = a;
}

void ClaVGraphics::SpriteRender::Render()
{
	ID3D11Resource* resource;
	ResourceManager::GetInstance().GetResourceData(m_fileName)->GetResource(&resource);

	ID3D11Texture2D* texture = static_cast<ID3D11Texture2D*>(resource);

	D3D11_TEXTURE2D_DESC desc;
	texture->GetDesc(&desc);

	SimpleMath::Vector2 origin = SimpleMath::Vector2(desc.Width / 2, desc.Height / 2);

	SpriteAdapter::GetInstance().SpriteBatchBegin();
	m_spriteBatch->Draw(ResourceManager::GetInstance().GetResourceData(m_fileName),
		m_pos,
		nullptr, { m_colorA, m_colorG, m_colorB, m_colorA }, m_angle,
		origin,	// 회전시 중심점(0.5f, 0.5f -> 텍스쳐 중심점)
		m_scale);
	SpriteAdapter::GetInstance().GetSpriteBatch()->End();	
}
