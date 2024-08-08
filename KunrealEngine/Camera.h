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
		float GetNearZ() const;		// ī�޶�� ���� �ּ� �Ÿ�
		float GetFarZ() const;		// ī�޶�� ���� �ִ� �Ÿ�
		float GetAspect() const;	// ī�޶� ����
		float GetFovY() const;		// ī�޶� ����(Y) ��?
		float GetFovX() const;		// ī�޶� ����(x) ��?

		// Get near and far plane dimensions in view space coordinates.
		float GetNearWindowWidth() const;
		float GetNearWindowHeight() const;
		float GetFarWindowWidth() const;
		float GetFarWindowHeight() const;

		// Get View/Proj matrices.
		DirectX::XMMATRIX GetViewXMMATRIX() const;
		DirectX::XMMATRIX GetProjXMMATRIX() const;

		// Strafe/Walk the camera a distance d.
		void Strafe(float distance);		// ��,��
		void Walk(float distance);			// ��,��
		void WorldUpDown(float distance);	// ��,��

		// Rotate the camera.
		void Pitch(float angle);	// ���� ���� ȸ��
		void RotateY(float angle);	// ���� ���� ȸ��

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