#include "CameraController.h"
#include "KunrealAPI.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Camera.h"

KunrealEngine::CameraController::CameraController()
	:m_sceneCamera(nullptr), m_playerTransform(nullptr), m_zoomedIn(false),
	m_cameraPos(0.f, 0.f, 0.f), m_cameraLook(0.f, 0.f, 0.f), m_cameraUp(0.f, 0.f, 0.f), m_zoomDuration(1.5f),
	m_targetPos(0.f, 0.f, 0.f), m_targetLook(0.f, 0.f, 0.f), m_targetUp(0.f, 0.f, 0.f),
	m_curtime(0.0f), m_yesman(false), m_followPlayer(false), m_stage3(false),
	m_outPos(0.f, 0.f, 0.f), m_outLook(0.f, 0.f, 0.f), m_outUp(0.f, 0.f, 0.f),
	m_inPos(0.f, 0.f, 0.f), m_inLook(0.f, 0.f, 0.f), m_inUp(0.f, 0.f, 0.f)
{

}

KunrealEngine::CameraController::~CameraController()
{

}

void KunrealEngine::CameraController::Initialize()
{
	m_sceneCamera = KunrealEngine::SceneManager::GetInstance().GetScene("GAMESCENE")->GetGameObject("MainCamera");
	m_playerTransform = KunrealEngine::SceneManager::GetInstance().GetScene("GAMESCENE")->GetGameObject("Player")->GetComponent<KunrealEngine::Transform>();

	//m_sceneCamera = KunrealEngine::GetScene("Main")->GetGameObject("MainCamera");
	//m_playerTransform = KunrealEngine::GetScene("Main")->GetGameObject("MapBase1_Door")->GetComponent<KunrealEngine::Transform>();

	CheckTargetData();
	//1,2스테이지 일 때
	m_sceneCamera->GetComponent<KunrealEngine::ObjectCamera>()->GetCamera()->m_Position = { -0.892722f, 2.057804f, 0.892722f };
	m_sceneCamera->GetComponent<KunrealEngine::ObjectCamera>()->GetCamera()->m_Look = { 0.353499f, -0.866070f, - 0.353499f };
	m_sceneCamera->GetComponent<KunrealEngine::ObjectCamera>()->GetCamera()->m_Up = { 0.612404f, 0.499922f, -0.612404f };

	m_outPos =  { -0.892722f, 2.057804f, 0.892722f };
	m_outLook = { 0.353499f, -0.866070f, -0.353499f };
	m_outUp =   { 0.612404f, 0.499922f, -0.612404f };

	m_inPos =   { -0.329828f, 0.821922f, 0.329827f };
	m_inLook =  { 0.377705f, -0.845286f, -0.377706f };
	m_inUp =    { 0.597778f, 0.534157f, -0.597778f };
}

void KunrealEngine::CameraController::Finalize()
{

}

void KunrealEngine::CameraController::FixedUpdate()
{

}

void KunrealEngine::CameraController::Update()
{
	CheckCameraData();
}

void KunrealEngine::CameraController::LateUpdate()
{
	if (KunrealEngine::InputManager::GetInstance().OnceKeyInput('Z'))
	{
		m_yesman = true;
	}
	ZoomInCamera();
	ZoomOutCamera();
	FollowPlayerView();
}

//줌인
void KunrealEngine::CameraController::ZoomInCamera()
{
	//줌인이 안되어 있을 때
	if (!m_zoomedIn && m_yesman)
	{
		//목표 좌표를 계산
		CheckTargetData();

		//카메라 무빙중엔 플레이어가 못 움직이게
		KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->SetActive(false);

		//1.5초동안 카메라를 움직인다

		DirectX::XMFLOAT3 endPos = m_targetPos;
		DirectX::XMFLOAT3 endLook = m_targetLook;
		DirectX::XMFLOAT3 endUp = m_targetUp;

		if (m_curtime < m_zoomDuration)
		{
			float t = m_curtime / m_zoomDuration;

			m_sceneCamera->GetComponent<KunrealEngine::ObjectCamera>()->GetCamera()->m_Position = Lerp(m_outPos, m_targetPos, t);
			//m_sceneCamera->GetComponent<KunrealEngine::ObjectCamera>()->GetCamera()->m_Look = Lerp(m_outLook, m_targetLook, t);
			//m_sceneCamera->GetComponent<KunrealEngine::ObjectCamera>()->GetCamera()->m_Up = Lerp(m_outUp, m_targetUp, t);

		}
		else
		{
			m_yesman = false;
			m_curtime = 0.0f;
			m_zoomedIn = true;

			m_inPos = m_sceneCamera->GetComponent<KunrealEngine::ObjectCamera>()->GetCamera()->m_Position;
			m_inLook = m_sceneCamera->GetComponent<KunrealEngine::ObjectCamera>()->GetCamera()->m_Look;
			m_inUp = m_sceneCamera->GetComponent<KunrealEngine::ObjectCamera>()->GetCamera()->m_Up;

			KunrealEngine::GameObject* asd = KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player");

			//다시 활동 가능하게
			KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->SetActive(true);

			//줌인이 된 후 플레이어를 따라 다니게
			m_followPlayer = true;

			return;
		}
		m_curtime += KunrealEngine::TimeManager::GetInstance().GetDeltaTime();

	}
}

//줌아웃
void KunrealEngine::CameraController::ZoomOutCamera()
{
	//줌인이 되어 있을 때
	if (m_zoomedIn && m_yesman)
	{
		//플레이어를 따라다니지 않게 하고
		m_followPlayer = false;
		
		//목표 좌표를 계산
		CheckTargetData();

		//카메라 무빙중엔 플레이어가 못 움직이게
		KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->SetActive(false);

		//1.5초동안 카메라를 움직인다

		DirectX::XMFLOAT3 endPos = m_targetPos;
		DirectX::XMFLOAT3 endLook = m_targetLook;
		DirectX::XMFLOAT3 endUp = m_targetUp;

		if (m_curtime < m_zoomDuration)
		{
			float t = m_curtime / m_zoomDuration;

			m_sceneCamera->GetComponent<KunrealEngine::ObjectCamera>()->GetCamera()->m_Position = Lerp(m_inPos, m_targetPos, t);
			//m_sceneCamera->GetComponent<KunrealEngine::ObjectCamera>()->GetCamera()->m_Look = Lerp(m_inLook, m_targetLook, t);
			//m_sceneCamera->GetComponent<KunrealEngine::ObjectCamera>()->GetCamera()->m_Up = Lerp(m_inUp, m_targetUp, t);

		}
		else
		{
			m_yesman = false;
			m_curtime = 0.0f;
			m_zoomedIn = false;

			m_outPos = m_sceneCamera->GetComponent<KunrealEngine::ObjectCamera>()->GetCamera()->m_Position;
			m_outLook = m_sceneCamera->GetComponent<KunrealEngine::ObjectCamera>()->GetCamera()->m_Look;
			m_outUp = m_sceneCamera->GetComponent<KunrealEngine::ObjectCamera>()->GetCamera()->m_Up;

			//다시 캐릭터 움직이게
			KunrealEngine::GetScene("GAMESCENE")->GetGameObject("Player")->SetActive(true);

			return;
		}
		m_curtime += KunrealEngine::TimeManager::GetInstance().GetDeltaTime();
	}
}

float KunrealEngine::CameraController::Lerp(float start, float end, float t)
{
	return start + t * (end - start);
}

DirectX::XMFLOAT3 KunrealEngine::CameraController::Lerp(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, float t)
{
	return DirectX::XMFLOAT3(
		start.x + t * (end.x - start.x),
		start.y + t * (end.y - start.y),
		start.z + t * (end.z - start.z)
	);
}

//줌인 줌아웃 상태에 따른 카메라의 목표 pos, look, up
void KunrealEngine::CameraController::CheckTargetData()
{
	//날 용서해라
	if (m_zoomedIn)
	{
		if (!m_stage3)
		{
			m_targetPos = { -0.892722f, 2.057804f, 0.892722f };
			m_targetLook = { 0.353499f, -0.866070f, -0.353499f };
			m_targetUp = { 0.612404f, 0.499922f, -0.612404f };
		}
		else
		{
			m_targetPos = { -1.686127f, 2.931360f, 0.712389f };
			m_targetLook ={ 0.299952f, -0.907497f, -0.294071f };
			m_targetUp = { 0.648019f, 0.420059f, -0.635313f };
		}

		//m_targetPos.x = m_playerTransform->GetPosition().x - 0.892722;
		//m_targetPos.y = m_playerTransform->GetPosition().y + 2.057804;
		//m_targetPos.z = m_playerTransform->GetPosition().z + 0.892722;
		//
		//m_targetLook.x = m_playerTransform->GetPosition().x + 0.353499;
		//m_targetLook.y = m_playerTransform->GetPosition().y - 0.866070;
		//m_targetLook.z = m_playerTransform->GetPosition().z - 0.353499;
		//
		//m_targetUp.x = m_playerTransform->GetPosition().x + 0.612404;
		//m_targetUp.x = m_playerTransform->GetPosition().x + 0.499922;
		//m_targetUp.x = m_playerTransform->GetPosition().x - 0.612404;

	}
	//1,2스테이지
	else
	{
		m_targetPos.x = m_playerTransform->GetPosition().x - 0.329828;
		m_targetPos.y = m_playerTransform->GetPosition().y + 0.821922;
		m_targetPos.z = m_playerTransform->GetPosition().z + 0.329827;
		
		m_targetLook.x = m_playerTransform->GetPosition().x + 0.377705;
		m_targetLook.y = m_playerTransform->GetPosition().y - 0.845386;
		m_targetLook.z = m_playerTransform->GetPosition().z - 0.377706;
		
		m_targetUp.x = m_playerTransform->GetPosition().x + 0.597778;
		m_targetUp.x = m_playerTransform->GetPosition().x + 0.534157;
		m_targetUp.x = m_playerTransform->GetPosition().x - 0.597778;
	}
}

//카메라의 현재 정보
void KunrealEngine::CameraController::CheckCameraData()
{
	m_cameraPos = m_sceneCamera->GetComponent<KunrealEngine::ObjectCamera>()->GetCamera()->m_Position;
	m_cameraLook = m_sceneCamera->GetComponent<KunrealEngine::ObjectCamera>()->GetCamera()->m_Look;
	m_cameraUp = m_sceneCamera->GetComponent<KunrealEngine::ObjectCamera>()->GetCamera()->m_Up;
}

//스테이지 3일때는 카메라의 pos, look, up이 다름
void KunrealEngine::CameraController::CheckStage3()
{
	m_stage3 = true;

	m_sceneCamera->GetComponent<KunrealEngine::ObjectCamera>()->GetCamera()->m_Position = { -1.686127f, 2.931360f, 0.712389f };
	m_sceneCamera->GetComponent<KunrealEngine::ObjectCamera>()->GetCamera()->m_Look     = { 0.299952f, -0.907497f, -0.294071f };
	m_sceneCamera->GetComponent<KunrealEngine::ObjectCamera>()->GetCamera()->m_Up       = { 0.648019f, 0.420059f, -0.635313f };

	m_zoomedIn = false;

	m_outPos  = { -1.686127f, 2.931360f, 0.712389f };
	m_outLook = { 0.299952f, -0.907497f, -0.294071f };
	m_outUp   = { 0.648019f, 0.420059f, -0.635313f };
}

//bool값에 따라 플레이어를 따라다닐지
//줌인이 되어있을때만 따라다님		//줌아웃때는 시점고정
void KunrealEngine::CameraController::FollowPlayerView()
{
	if (m_followPlayer)
	{
		m_sceneCamera->GetComponent<KunrealEngine::ObjectCamera>()->GetCamera()->m_Position.x = m_playerTransform->GetPosition().x - 0.329828;
		m_sceneCamera->GetComponent<KunrealEngine::ObjectCamera>()->GetCamera()->m_Position.y = m_playerTransform->GetPosition().y + 0.821922;
		m_sceneCamera->GetComponent<KunrealEngine::ObjectCamera>()->GetCamera()->m_Position.z = m_playerTransform->GetPosition().z + 0.329827;
	}
}