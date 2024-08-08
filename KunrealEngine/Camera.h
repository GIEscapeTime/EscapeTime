#pragma once
#include "Transform.h"

namespace KunrealEngine
{
	class Camera
	{
	public:
		Camera();
		~Camera() {};


		void SetPosition(float x, float y, float z);
		void SetPosition(const DirectX::XMFLOAT3& pos);
		DirectX::XMFLOAT3 GetPosition() const;

		// Set frustum.
		void SetLens(float fovY, float aspect, float zn, float zf);

		// Define camera space via LookAt parameters.
		void LookAt(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR worldUp);
		void LookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up);
		
		// Get frustum properties.
		float GetNearZ() const;		// 카메라로 비출 최소 거리
		float GetFarZ() const;		// 카메라로 비출 최대 거리
		float GetAspect() const;	// 카메라 비율
		float GetFovY() const;		// 카메라 수직(Y) 값?
		float GetFovX() const;		// 카메라 수평(x) 값?

		// Get near and far plane dimensions in view space coordinates.
		float GetNearWindowWidth() const;
		float GetNearWindowHeight() const;
		float GetFarWindowWidth() const;
		float GetFarWindowHeight() const;

		// Get View/Proj matrices.
		DirectX::XMMATRIX GetViewXMMATRIX() const;
		DirectX::XMMATRIX GetProjXMMATRIX() const;

		// Strafe/Walk the camera a distance d.
		void Strafe(float distance);		// 좌,우
		void Walk(float distance);			// 앞,뒤
		void WorldUpDown(float distance);	// 상,하

		// Rotate the camera.
		void Pitch(float angle);	// 수직 방향 회전
		void RotateY(float angle);	// 수평 방향 회전

		// After modifying camera position/orientation, call to rebuild the view matrix.
		void UpdateViewMatrix();

		DirectX::XMFLOAT3 m_Position;
		DirectX::XMFLOAT3 m_Look;
		DirectX::XMFLOAT3 m_Up;
	private:
		DirectX::XMFLOAT3 m_Right;

		// Cache frustum properties.
		float m_NearZ;
		float m_FarZ;
		float m_Aspect;
		float m_FovY;
		float m_NearWindowHeight;
		float m_FarWindowHeight;

		// Cache View/Proj matrices.
		DirectX::XMFLOAT4X4 m_View;
		DirectX::XMFLOAT4X4 m_Proj;

		// Camera Move Speed;
		float m_Speed;
	};
}