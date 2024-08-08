#include "Core.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Renderer.h"
#include "UIRenderer.h"
#include "TextRenderer.h"
#include "KunMath.h"
#include "TimeManager.h"
#include "Camera.h"
#include "ObjectCamera.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "Light.h"
#include "CameraController.h"
#include "TestComponent.h"

#include "ClaVGraphics.h"

KunrealEngine::SceneManager& smInstance = KunrealEngine::SceneManager::GetInstance();
KunrealEngine::TimeManager& tmInstance = KunrealEngine::TimeManager::GetInstance();
KunrealEngine::InputManager& imInstance = KunrealEngine::InputManager::GetInstance();
KunrealEngine::SoundManager& sndInstance = KunrealEngine::SoundManager::GetInstance();

KunrealEngine::Core::Core()
	:m_renderer(nullptr)
{

}

KunrealEngine::Core::~Core()
{

}

//초기화
void KunrealEngine::Core::Initialize(HWND hwnd)
{
	smInstance.Initialize();
	tmInstance.Initialize();
	imInstance.InitialLize(hwnd);
	sndInstance.Initialize(30, 30);
	
	m_renderer = new ClaVGraphics::ClaVRenderer();
	m_renderer->Initialize(reinterpret_cast<long long>(hwnd), 1600, 900);

	///엔진 사용 예시 코드
	///실사용시엔 지워줘야한다
	smInstance.GetCurrentScene()->CreateObject("Sample_Object");		//임시
	//smInstance.GetCurrentScene()->CreateObject("Sample_Object2");
	//smInstance.GetCurrentScene()->CreateObject("Sample_Object3");

	//objecter->GetComponent<KunrealEngine::Transform>()->SetRotation(3, 3, 3, 3);

	//smInstance.GetScene("Main")->GetGameObject("Sample_Object")->GetComponent<Transform>()->SetPosition(0.f, 0.f, 0.f);
	//smInstance.GetScene("Main")->GetGameObject("Sample_Object")->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	//smInstance.GetScene("Main")->GetGameObject("Sample_Object")->AddComponent<Renderer>();
	//smInstance.GetScene("Main")->GetGameObject("Sample_Object")->GetComponent<Renderer>()->SetFBXPath("tri");
	smInstance.GetScene("Main")->CreateObject("MapBase1_Door");
	
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Door")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Door")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.05f, 0.f);
	//smInstance.GetScene("Main")->GetGameObject("MapBase1_Door")->GetComponent<KunrealEngine::Transform>()->SetRotation(0.f, 180.0f, 0.f);
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Door")->AddComponent<KunrealEngine::Renderer>();
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Door")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Jade");
	
	smInstance.GetScene("Main")->CreateObject("MapBase1_Door_Wall");
	
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Door_Wall")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Door_Wall")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.05f, 0.f);
	//smInstance.GetScene("Main")->GetGameObject("MapBase1_Door_Wall")->GetComponent<KunrealEngine::Transform>()->SetRotation(0.f, 180.0f, 0.f);
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Door_Wall")->AddComponent<KunrealEngine::Renderer>();
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Door_Wall")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Door_Wall");
	
	smInstance.GetScene("Main")->CreateObject("MapBase1_Half_Tile");
	
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Half_Tile")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Half_Tile")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.05f, 0.f);
	//smInstance.GetScene("Main")->GetGameObject("MapBase1_Half_Tile")->GetComponent<KunrealEngine::Transform>()->SetRotation(0.f, 180.0f, 0.f);
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Half_Tile")->AddComponent<KunrealEngine::Renderer>();
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Half_Tile")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Half_Tile");
	
	smInstance.GetScene("Main")->CreateObject("MapBase1_Pillar");
	
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Pillar")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Pillar")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.05f, 0.f);
	//smInstance.GetScene("Main")->GetGameObject("MapBase1_Pillar")->GetComponent<KunrealEngine::Transform>()->SetRotation(0.f, 180.0f, 0.f);
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Pillar")->AddComponent<KunrealEngine::Renderer>();
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Pillar")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Pillar");
	
	smInstance.GetScene("Main")->CreateObject("MapBase1_Stair");
	
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Stair")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Stair")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.05f, 0.f);
	//smInstance.GetScene("Main")->GetGameObject("MapBase1_Stair")->GetComponent<KunrealEngine::Transform>()->SetRotation(0.f, 180.0f, 0.f);
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Stair")->AddComponent<KunrealEngine::Renderer>();
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Stair")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Stair");
	
	smInstance.GetScene("Main")->CreateObject("MapBase1_Statue");
	
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Statue")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Statue")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.05f, 0.f);
	//smInstance.GetScene("Main")->GetGameObject("MapBase1_Statue")->GetComponent<KunrealEngine::Transform>()->SetRotation(0.f, 180.0f, 0.f);
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Statue")->AddComponent<KunrealEngine::Renderer>();
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Statue")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Statue");
	
	smInstance.GetScene("Main")->CreateObject("MapBase1_Tile_1");
	
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Tile_1")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Tile_1")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.05f, 0.f);
	//smInstance.GetScene("Main")->GetGameObject("MapBase1_Tile_1")->GetComponent<KunrealEngine::Transform>()->SetRotation(0.f, 180.0f, 0.f);
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Tile_1")->AddComponent<KunrealEngine::Renderer>();
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Tile_1")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Tile_1");
	
	smInstance.GetScene("Main")->CreateObject("MapBase1_Tile_2");
	
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Tile_2")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Tile_2")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.05f, 0.f);
	//smInstance.GetScene("Main")->GetGameObject("MapBase1_Tile_2")->GetComponent<KunrealEngine::Transform>()->SetRotation(0.f, 180.0f, 0.f);
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Tile_2")->AddComponent<KunrealEngine::Renderer>();
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Tile_2")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Tile_2");
	
	smInstance.GetScene("Main")->CreateObject("MapBase1_Torch");
	
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Torch")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Torch")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.05f, 0.f);
	//smInstance.GetScene("Main")->GetGameObject("MapBase1_Torch")->GetComponent<KunrealEngine::Transform>()->SetRotation(0.f, 180.0f, 0.f);
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Torch")->AddComponent<KunrealEngine::Renderer>();
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Torch")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Torch");
	
	smInstance.GetScene("Main")->CreateObject("MapBase1_Wall");
	
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Wall")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Wall")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.05f, 0.f);
	//smInstance.GetScene("Main")->GetGameObject("MapBase1_Wall")->GetComponent<KunrealEngine::Transform>()->SetRotation(0.f, 180.0f, 0.f);
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Wall")->AddComponent<KunrealEngine::Renderer>();
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Wall")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Wall");
	
	smInstance.GetScene("Main")->CreateObject("MapBase1_Water");
	
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Water")->GetComponent<KunrealEngine::Transform>()->SetScale(0.1f, 0.1f, 0.1f);
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Water")->GetComponent<KunrealEngine::Transform>()->SetPosition(0.f, 0.05f, 0.f);
	//smInstance.GetScene("Main")->GetGameObject("MapBase1_Water")->GetComponent<KunrealEngine::Transform>()->SetRotation(0.f, 180.0f, 0.f);
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Water")->AddComponent<KunrealEngine::Renderer>();
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Water")->GetComponent<KunrealEngine::Renderer>()->SetFBXPath("Water");
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Water")->AddComponent<TextRenderer>();
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Water")->GetComponent<TextRenderer>()->Initialize();
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Water")->GetComponent<TextRenderer>()->SetTextBoxData(100.f, 100.f, 25.f, 14.f);
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Water")->GetComponent<TextRenderer>()->SetFontColor(1.f, 1.f, 1.f);
	smInstance.GetScene("Main")->GetGameObject("MapBase1_Water")->GetComponent<TextRenderer>()->SetText("출력이 되나요? 테스트 텍스트");

	//smInstance.GetScene("Main")->CreateObject("CameraController");
	//smInstance.GetScene("Main")->GetGameObject("CameraController")->AddComponent<CameraController>();
	///

	/*smInstance.GetScene("Main")->CreateObject("Text1");
	smInstance.GetScene("Main")->CreateObject("Text2");
	smInstance.GetScene("Main")->CreateObject("Text3");
	smInstance.GetScene("Main")->CreateObject("Text4");
	smInstance.GetScene("Main")->CreateObject("Text5");
	smInstance.GetScene("Main")->CreateObject("Text6");
	smInstance.GetScene("Main")->CreateObject("Text7");
	smInstance.GetScene("Main")->CreateObject("Text8");
	smInstance.GetScene("Main")->CreateObject("Text9");

	smInstance.GetScene("Main")->GetGameObject("Text1")->GetComponent<Transform>()->SetUIPosition(100, 100);
	smInstance.GetScene("Main")->GetGameObject("Text2")->GetComponent<Transform>()->SetUIPosition(100, 120);
	smInstance.GetScene("Main")->GetGameObject("Text3")->GetComponent<Transform>()->SetUIPosition(100, 140);
	smInstance.GetScene("Main")->GetGameObject("Text4")->GetComponent<Transform>()->SetUIPosition(100, 160);
	smInstance.GetScene("Main")->GetGameObject("Text5")->GetComponent<Transform>()->SetUIPosition(120, 160);
	smInstance.GetScene("Main")->GetGameObject("Text6")->GetComponent<Transform>()->SetUIPosition(140, 160);
	smInstance.GetScene("Main")->GetGameObject("Text7")->GetComponent<Transform>()->SetUIPosition(100, 180);
	smInstance.GetScene("Main")->GetGameObject("Text8")->GetComponent<Transform>()->SetUIPosition(120, 180);
	smInstance.GetScene("Main")->GetGameObject("Text9")->GetComponent<Transform>()->SetUIPosition(140, 180);

	smInstance.GetScene("Main")->GetGameObject("Text1")->AddComponent<TextRenderer>();
	smInstance.GetScene("Main")->GetGameObject("Text2")->AddComponent<TextRenderer>();
	smInstance.GetScene("Main")->GetGameObject("Text3")->AddComponent<TextRenderer>();
	smInstance.GetScene("Main")->GetGameObject("Text4")->AddComponent<TextRenderer>();
	smInstance.GetScene("Main")->GetGameObject("Text5")->AddComponent<TextRenderer>();
	smInstance.GetScene("Main")->GetGameObject("Text6")->AddComponent<TextRenderer>();
	smInstance.GetScene("Main")->GetGameObject("Text7")->AddComponent<TextRenderer>();
	smInstance.GetScene("Main")->GetGameObject("Text8")->AddComponent<TextRenderer>();
	smInstance.GetScene("Main")->GetGameObject("Text9")->AddComponent<TextRenderer>();

	smInstance.GetScene("Main")->GetGameObject("Text1")->GetComponent<TextRenderer>()->SetTextBoxData(100, 100, 14, 14);
	smInstance.GetScene("Main")->GetGameObject("Text2")->GetComponent<TextRenderer>()->SetTextBoxData(100, 120, 14, 14);
	smInstance.GetScene("Main")->GetGameObject("Text3")->GetComponent<TextRenderer>()->SetTextBoxData(100, 140, 14, 14);
	smInstance.GetScene("Main")->GetGameObject("Text4")->GetComponent<TextRenderer>()->SetTextBoxData(100, 160, 14, 14);
	smInstance.GetScene("Main")->GetGameObject("Text5")->GetComponent<TextRenderer>()->SetTextBoxData(200, 160, 14, 14);
	smInstance.GetScene("Main")->GetGameObject("Text6")->GetComponent<TextRenderer>()->SetTextBoxData(300, 160, 14, 14);
	smInstance.GetScene("Main")->GetGameObject("Text7")->GetComponent<TextRenderer>()->SetTextBoxData(100, 180, 14, 14);
	smInstance.GetScene("Main")->GetGameObject("Text8")->GetComponent<TextRenderer>()->SetTextBoxData(200, 180, 14, 14);
	smInstance.GetScene("Main")->GetGameObject("Text9")->GetComponent<TextRenderer>()->SetTextBoxData(300, 180, 14, 14);


	smInstance.GetScene("Main")->GetGameObject("Text1")->GetComponent<TextRenderer>()->SetFontColor(1, 1, 1);
	smInstance.GetScene("Main")->GetGameObject("Text2")->GetComponent<TextRenderer>()->SetFontColor(1, 1, 1);
	smInstance.GetScene("Main")->GetGameObject("Text3")->GetComponent<TextRenderer>()->SetFontColor(1, 1, 1);
	smInstance.GetScene("Main")->GetGameObject("Text4")->GetComponent<TextRenderer>()->SetFontColor(1, 1, 1);
	smInstance.GetScene("Main")->GetGameObject("Text5")->GetComponent<TextRenderer>()->SetFontColor(1, 1, 1);
	smInstance.GetScene("Main")->GetGameObject("Text6")->GetComponent<TextRenderer>()->SetFontColor(1, 1, 1);
	smInstance.GetScene("Main")->GetGameObject("Text7")->GetComponent<TextRenderer>()->SetFontColor(1, 1, 1);
	smInstance.GetScene("Main")->GetGameObject("Text8")->GetComponent<TextRenderer>()->SetFontColor(1, 1, 1);
	smInstance.GetScene("Main")->GetGameObject("Text9")->GetComponent<TextRenderer>()->SetFontColor(1, 1, 1);*/
}

//물리가 적용되지 않은 업데이트
//게임의 메인로직등이 여기서 처리될거야
void KunrealEngine::Core::Update()
{
	smInstance.UpdateScene(smInstance.GetCurrentScene());
	imInstance.InPutUpdateAll();
	//smInstance.GetScene("Main")->GetGameObject("MouseLeft")->GetComponent<TextRenderer>()->SetText(imInstance.ReturnMouseState().x);
	//smInstance.GetScene("Main")->GetGameObject("MouseRight")->GetComponent<TextRenderer>()->SetText(imInstance.ReturnMouseState().y);
	//axx->Update(smInstance.GetScene("Main")->GetGameObject("MainCamera")->GetComponent<ObjectCamera>()->GetCamera()->GetViewXMMATRIX(), smInstance.GetScene("Main")->GetGameObject("MainCamera")->GetComponent<ObjectCamera>()->GetCamera()->GetProjXMMATRIX());

	//smInstance.GetScene("Main")->GetGameObject("Text1")->GetComponent<TextRenderer>()->SetText(smInstance.GetScene("Main")->GetGameObject("MainCamera")->GetComponent<ObjectCamera>()->GetCamera()->GetPosition().x);
	//smInstance.GetScene("Main")->GetGameObject("Text2")->GetComponent<TextRenderer>()->SetText(smInstance.GetScene("Main")->GetGameObject("MainCamera")->GetComponent<ObjectCamera>()->GetCamera()->GetPosition().y);
	//smInstance.GetScene("Main")->GetGameObject("Text3")->GetComponent<TextRenderer>()->SetText(smInstance.GetScene("Main")->GetGameObject("MainCamera")->GetComponent<ObjectCamera>()->GetCamera()->GetPosition().z);
	//smInstance.GetScene("Main")->GetGameObject("Text4")->GetComponent<TextRenderer>()->SetText(smInstance.GetScene("Main")->GetGameObject("MainCamera")->GetComponent<ObjectCamera>()->GetCamera()->m_Look.x);
	//smInstance.GetScene("Main")->GetGameObject("Text5")->GetComponent<TextRenderer>()->SetText(smInstance.GetScene("Main")->GetGameObject("MainCamera")->GetComponent<ObjectCamera>()->GetCamera()->m_Look.y);
	//smInstance.GetScene("Main")->GetGameObject("Text6")->GetComponent<TextRenderer>()->SetText(smInstance.GetScene("Main")->GetGameObject("MainCamera")->GetComponent<ObjectCamera>()->GetCamera()->m_Look.z);
	//smInstance.GetScene("Main")->GetGameObject("Text7")->GetComponent<TextRenderer>()->SetText(smInstance.GetScene("Main")->GetGameObject("MainCamera")->GetComponent<ObjectCamera>()->GetCamera()->m_Up.x);
	//smInstance.GetScene("Main")->GetGameObject("Text8")->GetComponent<TextRenderer>()->SetText(smInstance.GetScene("Main")->GetGameObject("MainCamera")->GetComponent<ObjectCamera>()->GetCamera()->m_Up.y);
	//smInstance.GetScene("Main")->GetGameObject("Text9")->GetComponent<TextRenderer>()->SetText(smInstance.GetScene("Main")->GetGameObject("MainCamera")->GetComponent<ObjectCamera>()->GetCamera()->m_Up.z);
}

//일정 간격마다 업데이트되는 요소들 물리 처리를 여기서 한다
void KunrealEngine::Core::FixedUpdate()
{
	smInstance.FixedUpdateScene(smInstance.GetCurrentScene());
}

//카메라 업데이트 등이 여기서 들어간다
void KunrealEngine::Core::LateUpdate()
{
	smInstance.LateUpdateScene(smInstance.GetCurrentScene());
}

//레이어 순서에 따라서 현재 scene의 object들을 정렬
//레이어가 낮은 오브젝트가 먼저 렌더 -> 밑으로 깔림
//UI등 만들때 꼭 참고
void KunrealEngine::Core::SortObjectByLayer()
{
	int i;
	int j;

	if (smInstance.GetCurrentScene()->GetObjectList().size() > 0)
	{
		for (i = 0; i < smInstance.GetCurrentScene()->GetObjectList().size(); i++)
		{
			GameObject* iter = smInstance.GetCurrentScene()->GetObjectList()[i];
			for (j = i - 1; j >= 0 && smInstance.GetCurrentScene()->GetObjectList()[i]->GetLayer() > iter->GetLayer(); j--)
			{
				smInstance.GetCurrentScene()->GetObjectList()[j + 1] = smInstance.GetCurrentScene()->GetObjectList()[j];
			}
			smInstance.GetCurrentScene()->GetObjectList()[j + 1] = iter;
		}
	}
}

//그린다
void KunrealEngine::Core::Render()
{
	SortObjectByLayer();

	m_renderer->BeginRender(0,0,0,0);
	smInstance.GetCurrentScene()->Render();
	m_renderer->EndRender();
}

KunrealEngine::SceneManager& KunrealEngine::Core::GetSceneManager()
{
	return smInstance;
}

//다 썼으면 메모리를 해제해야지
void KunrealEngine::Core::Finalize()
{
	smInstance.Finalize();
}

//클라이언트에 한방에 넘겨주기 위해 여기서 모두 업데이트 하고 이 함수를 넘겨준다
void KunrealEngine::Core::UpdateAll()
{
	tmInstance.Update();
	FixedUpdate();
	Update();
	LateUpdate();
	Render();
}
