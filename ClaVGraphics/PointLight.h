#include "LightHelper.h"

namespace ClaVGraphics
{
	// ������ Light �ϳ��� ������ Direction, Point, Spot�� �� �� �ְ� �ؾ� ������
	// �ð� ������ ���� PointLight�� �� �� �ִ� Ŭ������ �����
	// 2023.08.21 �ֹμ�

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
