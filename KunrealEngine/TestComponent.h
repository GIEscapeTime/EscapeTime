#pragma once
#include "CommonHeader.h"
#include "Component.h"

namespace KunrealEngine
{
	class Transform;

	class TestComponent : public Component
	{
	public:
		TestComponent();
		~TestComponent();

		virtual void Initialize() override;
		virtual void Finalize() override;

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

	private:
		Transform* m_transform;
	};
}

