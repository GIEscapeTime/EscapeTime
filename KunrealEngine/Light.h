#pragma once
#include "CommonHeader.h"
#include "Component.h"

namespace ClaVGraphics
{
	class PointLightSetting;
}

namespace KunrealEngine
{
	class Transform;

	class Light : public Component
	{
	private:
		Light();
		~Light();

	//싱글톤
	public:
		static Light& GetInstance()
		{
			static Light m_instance;
			return m_instance;
		}

		virtual void Initialize() override;
		virtual void Finalize() override;

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

	public:
		void AddLight(float x, float y, float z, float range, float r = 1.f, float g = 0.5f, float b = 0.f);
		void DeleteLight();
		void SetLightIntensity();
		void Render();
	private:
		Transform* m_transform;
		//라이트를 담을 벡터 컨테이너 추가 예정
	};
}
