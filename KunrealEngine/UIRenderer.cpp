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

//�ʱ�ȭ�Ҷ� �׷��Ƚ��� SpriteRender�� Text�� �������ְ� �θ� ������Ʈ�� Ʈ�������� �޾ƿ�
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
	//Ʈ�������� UI ��ġ, ũ�� ������ �޾Ƽ� �׷��Ƚ��� ����
	//2D�ε� ����ؼ� �������� �ʿ䰡 ������? �������� ������ �� ������ �κ� 1����
	m_renderer->SetPosition(m_transform->GetUIPosition().x, m_transform->GetUIPosition().y);
	m_renderer->SetScale(m_transform->GetUIScale().x, m_transform->GetUIScale().y);
}

void KunrealEngine::UIRenderer::LateUpdate()
{

}

//ȭ�鿡 ��� ��������Ʈ �̹����� ��θ� �޾Ƽ� �׷��Ƚ��� �Ѱ���
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

//UI ������Ʈ��
void KunrealEngine::UIRenderer::Render()
{
	if(!m_filePath.empty())
	m_renderer->Render();
}