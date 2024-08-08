#pragma once
#include "SimpleMath.h"
#include "SpriteAdapter.h"
#include <string>

namespace ClaVGraphics
{
	class SpriteRender
	{
	public:
		SpriteRender() : m_spriteBatch(), m_pos(0.f, 0.f), m_scale(1.f, 1.f), m_angle(0.f), m_fileName(),
		m_colorR(1.f), m_colorG(1.f), m_colorB(1.f), m_colorA(1.f) {}

		void Init(std::string fileName);
		void SetPosition(float x, float y);
		void SetScale(float x, float y);
		void SetRotation(float angle);
		void SetColor(float r, float g, float b, float a);
		void Render();

	private:
		DirectX::SpriteBatch* m_spriteBatch;

		DirectX::SimpleMath::Vector2 m_pos;
		DirectX::SimpleMath::Vector2 m_scale;

		float m_angle;
		float m_colorR;
		float m_colorG;
		float m_colorB;
		float m_colorA;

		std::string m_fileName;
	};
}

