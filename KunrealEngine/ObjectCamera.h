#pragma once
#include "CommonHeader.h"
#include "Component.h"

/// <summary>
/// 그래픽스의 카메라에 정보를 전달해줄 오브젝트 내부의 카메라 컴포넌트
/// Transform 컴포넌트에서 정보를 가져옴
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

