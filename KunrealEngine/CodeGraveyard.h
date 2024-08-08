#pragma once
///엔진 사용 예시 코드
	///실사용시엔 지워줘야한다
// smInstance.GetCurrentScene()->CreateObject("Sample_Object");		//임시
// 
// Scene* scener = smInstance.GetCurrentScene();
// 
// GameObject* objecter = smInstance.GetCurrentScene()->GetGameObject("Sample_Object");
// Transform* trans = objecter->GetComponent<KunrealEngine::Transform>();
// trans->SetPosition(1, 1, 1);

//objecter->GetComponent<KunrealEngine::Transform>()->SetRotation(3, 3, 3, 3);

// smInstance.GetCurrentScene()->GetGameObject("Sample_Object")->GetComponent<Transform>()->SetPosition(0.f, 0.f, 0.f);
// 
// smInstance.GetScene("Main")->GetGameObject("Sample_Object")->GetComponent<Transform>()->SetPosition(0.f, 0.f, 0.f);
// smInstance.GetScene("Main")->GetGameObject("Sample_Object")->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 0.1f);
// 
// smInstance.GetScene("Main")->GetGameObject("Sample_Object")->AddComponent<Renderer>();
// 
// smInstance.GetScene("Main")->GetGameObject("Sample_Object")->GetComponent<Renderer>()->SetFBXPath("Skeleton");
// 
// smInstance.GetCurrentScene()->CreateObject("box1");
// smInstance.GetCurrentScene()->GetGameObject("box1")->GetComponent<Transform>()->SetPosition(0.05f, -0.1f, -0.1f);
// smInstance.GetCurrentScene()->GetGameObject("box1")->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 0.1f);
// smInstance.GetCurrentScene()->GetGameObject("box1")->AddComponent<Renderer>();
// smInstance.GetCurrentScene()->GetGameObject("box1")->GetComponent<Renderer>()->SetDebugMode(true);
// smInstance.GetCurrentScene()->GetGameObject("box1")->GetComponent<Renderer>()->SetFBXPath("box");
// 
// smInstance.GetCurrentScene()->CreateObject("box2");
// smInstance.GetCurrentScene()->GetGameObject("box2")->GetComponent<Transform>()->SetPosition(-0.05f, -0.1f, -0.1f);
// smInstance.GetCurrentScene()->GetGameObject("box2")->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 0.1f);
// smInstance.GetCurrentScene()->GetGameObject("box2")->AddComponent<Renderer>();
// smInstance.GetCurrentScene()->GetGameObject("box2")->GetComponent<Renderer>()->SetDebugMode(true);
// smInstance.GetCurrentScene()->GetGameObject("box2")->GetComponent<Renderer>()->SetFBXPath("box");
// 
// smInstance.GetCurrentScene()->CreateObject("box3");
// smInstance.GetCurrentScene()->GetGameObject("box3")->GetComponent<Transform>()->SetPosition(0.15f, -0.1f, -0.1f);
// smInstance.GetCurrentScene()->GetGameObject("box3")->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 0.1f);
// smInstance.GetCurrentScene()->GetGameObject("box3")->AddComponent<Renderer>();
// smInstance.GetCurrentScene()->GetGameObject("box3")->GetComponent<Renderer>()->SetDebugMode(true);
// smInstance.GetCurrentScene()->GetGameObject("box3")->GetComponent<Renderer>()->SetFBXPath("box");
// 
// smInstance.GetCurrentScene()->CreateObject("box4");
// smInstance.GetCurrentScene()->GetGameObject("box4")->GetComponent<Transform>()->SetPosition(0.15f, -0.1f, -0.2f);
// smInstance.GetCurrentScene()->GetGameObject("box4")->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 0.1f);
// smInstance.GetCurrentScene()->GetGameObject("box4")->AddComponent<Renderer>();
// smInstance.GetCurrentScene()->GetGameObject("box4")->GetComponent<Renderer>()->SetDebugMode(true);
// smInstance.GetCurrentScene()->GetGameObject("box4")->GetComponent<Renderer>()->SetFBXPath("box");
// 
// smInstance.GetCurrentScene()->CreateObject("box5");
// smInstance.GetCurrentScene()->GetGameObject("box5")->GetComponent<Transform>()->SetPosition(0.05f, -0.1f, -0.2f);
// smInstance.GetCurrentScene()->GetGameObject("box5")->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 0.1f);
// smInstance.GetCurrentScene()->GetGameObject("box5")->AddComponent<Renderer>();
// smInstance.GetCurrentScene()->GetGameObject("box5")->GetComponent<Renderer>()->SetDebugMode(true);
// smInstance.GetCurrentScene()->GetGameObject("box5")->GetComponent<Renderer>()->SetFBXPath("box");
// 
// smInstance.GetCurrentScene()->CreateObject("box6");
// smInstance.GetCurrentScene()->GetGameObject("box6")->GetComponent<Transform>()->SetPosition(-0.05f, -0.1f, -0.2f);
// smInstance.GetCurrentScene()->GetGameObject("box6")->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 0.1f);
// smInstance.GetCurrentScene()->GetGameObject("box6")->AddComponent<Renderer>();
// smInstance.GetCurrentScene()->GetGameObject("box6")->GetComponent<Renderer>()->SetDebugMode(true);
// smInstance.GetCurrentScene()->GetGameObject("box6")->GetComponent<Renderer>()->SetFBXPath("box");
// 
// smInstance.GetCurrentScene()->CreateObject("box7");
// smInstance.GetCurrentScene()->GetGameObject("box7")->GetComponent<Transform>()->SetPosition(0.15f, -0.1f, 0.f);
// smInstance.GetCurrentScene()->GetGameObject("box7")->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 0.1f);
// smInstance.GetCurrentScene()->GetGameObject("box7")->AddComponent<Renderer>();
// smInstance.GetCurrentScene()->GetGameObject("box7")->GetComponent<Renderer>()->SetDebugMode(true);
// smInstance.GetCurrentScene()->GetGameObject("box7")->GetComponent<Renderer>()->SetFBXPath("box");
// 
// smInstance.GetCurrentScene()->CreateObject("box8");
// smInstance.GetCurrentScene()->GetGameObject("box8")->GetComponent<Transform>()->SetPosition(0.05f, -0.1f, 0.f);
// smInstance.GetCurrentScene()->GetGameObject("box8")->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 0.1f);
// smInstance.GetCurrentScene()->GetGameObject("box8")->AddComponent<Renderer>();
// smInstance.GetCurrentScene()->GetGameObject("box8")->GetComponent<Renderer>()->SetDebugMode(true);
// smInstance.GetCurrentScene()->GetGameObject("box8")->GetComponent<Renderer>()->SetFBXPath("box");
// 
// smInstance.GetCurrentScene()->CreateObject("box9");
// smInstance.GetCurrentScene()->GetGameObject("box9")->GetComponent<Transform>()->SetPosition(-0.05f, -0.1f, 0.f);
// smInstance.GetCurrentScene()->GetGameObject("box9")->GetComponent<Transform>()->SetScale(0.1f, 0.1f, 0.1f);
// smInstance.GetCurrentScene()->GetGameObject("box9")->AddComponent<Renderer>();
// smInstance.GetCurrentScene()->GetGameObject("box9")->GetComponent<Renderer>()->SetDebugMode(true);
// smInstance.GetCurrentScene()->GetGameObject("box9")->GetComponent<Renderer>()->SetFBXPath("box");
// 
// smInstance.GetCurrentScene()->GetGameObject("Sample_Object")->AddComponent<TestComponent>();

//smInstance.GetCurrentScene()->GetGameObject("Sample_Object")->GetComponent<Transform>()->SetPosition(0.1f, 0.f, 0.f);
///