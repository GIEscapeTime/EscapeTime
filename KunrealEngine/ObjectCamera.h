#pragma once
#include "CommonHeader.h"
#include "Component.h"

/// <summary>
/// �׷��Ƚ��� ī�޶� ������ �������� ������Ʈ ������ ī�޶� ������Ʈ
/// Transform ������Ʈ���� ������ ������
/// </summary>

namespace KunrealEngine
{
	class Transform;
	class Camera;

	class ObjectCamera : public Component
	{
	public:
		ObjectCamera();
		~ObjectCamera();

	public:
		virtual void Initialize() override;
		virtual void Finalize() override;

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		Camera* GetCamera();

	private:
		Camera* m_camera;
		Transform* m_transform;
	};
}

