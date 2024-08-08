#pragma once
#include "CommonHeader.h"
#include "Component.h"
#include "DirectXMath.h"

/// <summary>
/// �ϵ��ڵ��� ����
/// </summary>

namespace KunrealEngine
{
	class GameObject;
	class Transform;

	class CameraController : public Component
	{
	public:
		CameraController();
		~CameraController();

		virtual void Initialize() override;
		virtual void Finalize() override;

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

	public:
		void ZoomInCamera();
		void ZoomOutCamera();
		void CheckStage3();
	private:
		bool m_followPlayer;						//ī�޶� �÷��̾ ����ٴ���
		bool m_zoomedIn;							//Ȯ�밡 �Ǿ����� ����
		KunrealEngine::GameObject* m_sceneCamera;	//����ī�޶� ��� ���� ������
		KunrealEngine::Transform* m_playerTransform;//�÷��̾��� ��ǥ�� ���� ī�޶� ��ġ�� ����
		float m_zoomDuration;						//���ʿ� ���� �����ܾƿ� �Ұ���

		DirectX::XMFLOAT3 m_cameraPos;
		DirectX::XMFLOAT3 m_cameraLook;
		DirectX::XMFLOAT3 m_cameraUp;

		DirectX::XMFLOAT3 m_targetPos;
		DirectX::XMFLOAT3 m_targetLook;
		DirectX::XMFLOAT3 m_targetUp;

		DirectX::XMFLOAT3 m_outPos;
		DirectX::XMFLOAT3 m_outLook;
		DirectX::XMFLOAT3 m_outUp;

		DirectX::XMFLOAT3 m_inPos;
		DirectX::XMFLOAT3 m_inLook;
		DirectX::XMFLOAT3 m_inUp;

		float m_curtime;
		bool m_yesman;
	private:
		bool m_stage3;
		float Lerp(float start, float end, float t);
		DirectX::XMFLOAT3 Lerp(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, float t);
		void CheckTargetData();
		void CheckCameraData();
		void FollowPlayerView();
	};
}

