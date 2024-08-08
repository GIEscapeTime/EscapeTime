#pragma once
#include "CommonHeader.h"
#include "Component.h"
#include "DirectXMath.h"

/// <summary>
/// 하드코딩의 정수
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
		bool m_followPlayer;						//카메라가 플레이어를 따라다닐지
		bool m_zoomedIn;							//확대가 되었는지 여부
		KunrealEngine::GameObject* m_sceneCamera;	//메인카메라를 담고 있을 포인터
		KunrealEngine::Transform* m_playerTransform;//플레이어의 좌표에 따라 카메라 위치도 변경
		float m_zoomDuration;						//몇초에 걸쳐 줌인줌아웃 할건지

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

