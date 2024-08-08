#include "UIRenderer.h"
#include "SpriteRender.h"
#include "Text.h"
#include "Transform.h"
#include "GameObject.h"

KunrealEngine::UIRenderer::UIRenderer()
	:m_renderer(nullptr), m_transform(nullptr), m_filePath()
{

}

KunrealEngine::UIRenderer::~UIRenderer()
{

}

//초기화할때 그래픽스의 SpriteRender와 Text를 생성해주고 부모 오브젝트의 트랜스폼을 받아옴
void KunrealEngine::UIRenderer::Initialize()
{
	m_renderer = new ClaVGraphics::SpriteRender;
	m_transform = GetOwner()->GetComponent<KunrealEngine::Transform>();

}

void KunrealEngine::UIRenderer::Finalize()
{
	delete m_renderer;
}

void KunrealEngine::UIRenderer::FixedUpdate()
{

}

void KunrealEngine::UIRenderer::Update()
{
	//트랜스폼의 UI 위치, 크기 정보를 받아서 그래픽스에 전달
	//2D인데 계속해서 전달해줄 필요가 있을까? 프레임이 떨어질 때 개선할 부분 1순위
	m_renderer->SetPosition(m_transform->GetUIPosition().x, m_transform->GetUIPosition().y);
	m_renderer->SetScale(m_transform->GetUIScale().x, m_transform->GetUIScale().y);
}

void KunrealEngine::UIRenderer::LateUpdate()
{

}

//화면에 띄울 스프라이트 이미지의 경로를 받아서 그래픽스에 넘겨줌
void KunrealEngine::UIRenderer::SetSpritePath(std::string filePath)
{
	m_filePath = filePath;

	if (!m_filePath.empty())
	{
		m_renderer->Init(m_filePath);
	}
}

void KunrealEngine::UIRenderer::SetSpriteActiveColor(bool active)
{
	if (active) m_renderer->SetColor(1.f, 1.f, 1.f, 1.f);
	else m_renderer->SetColor(1.f, 1.f, 1.f, 0.5f);
}

void KunrealEngine::UIRenderer::SetSpriteSelectColor(bool select, float r, float g, float b, float a)
{
	if (select) m_renderer->SetColor(r, b, b, a);
	else m_renderer->SetColor(1.f, 1.f, 1.f, 1.f);
}

//UI 업데이트문
void KunrealEngine::UIRenderer::Render()
{
	if(!m_filePath.empty())
	m_renderer->Render();
}