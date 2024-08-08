#include "LightHelper.h"

namespace ClaVGraphics
{
	// 원래는 Light 하나만 가지고 Direction, Point, Spot을 쓸 수 있게 해야 하지만
	// 시간 상으로 인해 PointLight만 쓸 수 있는 클래스를 만든다
	// 2023.08.21 최민서

	class PointLight
	{
	public:
		void Init();
		void SetPosition(float x, float y, float z);
		void SetRange(float range);
		void SetAmbientColor(float r, float g, float b);
		void SetDiffuseColor(float r, float g, float b);
		void SetSpecularColor(float r, float g, float b);
		void SetAtt(float x, float y, float z);
		void SetPad(float padding);

		PointLightData GetLight() { return m_pointLight; }
	private:
		PointLightData m_pointLight;
	};
}
