#pragma once
#include <vector>
#include "PointLight.h"

namespace ClaVGraphics
{
	class PointLightAdapter
	{
	public:
		static PointLightAdapter& GetInstance() {
			static PointLightAdapter instance;
			return instance;
		}

		void AddLight(PointLight* light) { m_pLight.emplace_back(light); };
		void SetLight(std::vector<PointLight*> light) { for (auto& li : light) { m_pLight.emplace_back(li); } };

		std::vector<PointLight*> GetPointLights() const { return m_pLight; };

		void Clear() { m_pLight.clear(); };

	private:
		std::vector<PointLight*> m_pLight;
	};
}