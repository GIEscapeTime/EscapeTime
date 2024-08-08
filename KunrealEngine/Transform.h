#pragma once
#include "Component.h"
#include "CommonHeader.h"
#include "KunMath.h"
#include <DirectXMath.h>

/// <summary>
/// 좌표, 회전값, 크기 등을 가지고 있는 트랜스폼 컴포넌트
/// 현재 DirectX Math를 기반으로 멤버들을 가지고 있음
/// </summary>

namespace KunrealEngine
{
	class GameObject;

	class Transform : public Component
	{
	public:
		Transform(GameObject* object);
		~Transform();

		virtual void Initialize() override;
		virtual void Finalize() override;

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void SetRotation(DirectX::XMFLOAT3 rot);
		void SetRotation(float x, float y, float z);
		void Translate(DirectX::XMFLOAT3 pos);
		void Translate(float x, float y, float z);
		void SetPosition(DirectX::XMFLOAT3 pos);
		void SetPosition(float x, float y, float z);
		void SetScale(float x, float y, float z);

		//UI전용 Transform 값인데.. 일단 필요하다니까 만들어둠
		void SetUIPosition(DirectX::XMFLOAT2 pos);
		void SetUIPosition(float x, float y);
		DirectX::XMFLOAT2 GetUIPosition();
		void SetUIScale(DirectX::XMFLOAT2 scale);
		void SetUIScale(float x, float y);
		DirectX::XMFLOAT2 GetUIScale();
		//

		DirectX::XMFLOAT3 GetPosition();
		DirectX::XMFLOAT4 GetRotation();
		DirectX::XMFLOAT3 GetScale();

	private:
		DirectX::XMMATRIX GetLocalPositionMatrix();
		DirectX::XMMATRIX GetLocalRotationMatrix();
		DirectX::XMMATRIX GetLocalScaleMatrix();

	public:
		DirectX::XMMATRIX WorldToLocalTM();
		DirectX::XMMATRIX LocalToWorldTM();
		DirectX::XMMATRIX LocalToWorldTM(GameObject* obj);

	private:
		DirectX::XMFLOAT3 m_position;
		DirectX::XMFLOAT4 m_rotation;
		DirectX::XMFLOAT3 m_scale;

		DirectX::XMFLOAT2 m_UIPosition;
		DirectX::XMFLOAT2 m_UIScale;
	};
};

