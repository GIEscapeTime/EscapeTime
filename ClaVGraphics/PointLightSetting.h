#pragma once
#include "PointLightAdapter.h"

namespace ClaVGraphics
{
	class PointLightSetting
	{
	public:
		PointLightSetting() { m_light = new PointLight(); m_light->Init(); }

		static void DeleteLight() { PointLightAdapter::GetInstance().Clear(); }

		void SetPosition(float x, float y, float z) { m_light->SetPosition(x, y, z); }
		void SetRange(float range) { m_light->SetRange(range); }
		void SetColor(float r, float g, float b) { m_light->SetAmbientColor(r, g, b); m_light->SetDiffuseColor(r, g, b); m_light->SetSpecularColor(r, g, b); }
		void AddLight() { PointLightAdapter::GetInstance().AddLight(m_light); }

	private:
		PointLight* m_light;
	};
}
