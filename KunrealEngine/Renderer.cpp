#include "Renderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "ObjectRender.h"

KunrealEngine::Renderer::Renderer()
	:m_objectRender(nullptr), m_transform(nullptr), m_fbxPath(), m_debug(false)
{

}

KunrealEngine::Renderer::~Renderer()
{

}

//트랜스폼이 nullptr일때 예외가 발생할 수 있지만
//오브젝트는 생성될 때 반드시 트랜스폼 컴포넌트를 가지게 되어있음
///주의해야할건 오브젝트가 비활성화 되어있으면 좌표 변경 등 또한 안되니까 참고
void KunrealEngine::Renderer::Initialize()
{
	m_objectRender = new ClaVGraphics::ObjectRender;
	
	m_transform = GetOwner()->GetComponent<Transform>();
}

void KunrealEngine::Renderer::Finalize()
{
	delete m_objectRender;
}

void KunrealEngine::Renderer::FixedUpdate()
{

}

void KunrealEngine::Renderer::Update()
{
	
}

void KunrealEngine::Renderer::LateUpdate()
{

}

//scene의 메인카메라의 view와 projection을 받아 오브젝트렌더러의 update를 돌려줌
//그 후 렌더
void KunrealEngine::Renderer::Render(DirectX::XMMATRIX ViewTM, DirectX::XMMATRIX projTM)
{
	
	if (!m_fbxPath.empty())
	{
		m_objectRender->Update(m_transform->LocalToWorldTM(), ViewTM, projTM);
		m_objectRender->Render();
	}
}

//FBX 경로 설정		//이후 그래픽스를 통해 FBX관련 객체 초기화
void KunrealEngine::Renderer::SetFBXPath(std::string path)
{
	m_fbxPath = path;
	m_objectRender->Init(m_fbxPath);
}

std::string KunrealEngine::Renderer::GetFBXfileName()
{
	return m_fbxPath;
}

//디버그모드 유무결정		//텍스처가 없는건 true
void KunrealEngine::Renderer::SetDebugMode(bool debug)
{
	m_debug = debug;
}
