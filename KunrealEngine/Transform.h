#pragma once
#include "Component.h"
#include "CommonHeader.h"
#include "KunMath.h"
#include <DirectXMath.h>

/// <summary>
/// ��ǥ, ȸ����, ũ�� ���� ������ �ִ� Ʈ������ ������Ʈ
/// ���� DirectX Math�� ������� ������� ������ ����
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

		//UI���� Transform ���ε�.. �ϴ� �ʿ��ϴٴϱ� ������
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

