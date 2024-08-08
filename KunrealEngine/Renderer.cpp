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

//Ʈ�������� nullptr�϶� ���ܰ� �߻��� �� ������
//������Ʈ�� ������ �� �ݵ�� Ʈ������ ������Ʈ�� ������ �Ǿ�����
///�����ؾ��Ұ� ������Ʈ�� ��Ȱ��ȭ �Ǿ������� ��ǥ ���� �� ���� �ȵǴϱ� ����
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

//scene�� ����ī�޶��� view�� projection�� �޾� ������Ʈ�������� update�� ������
//�� �� ����
void KunrealEngine::Renderer::Render(DirectX::XMMATRIX ViewTM, DirectX::XMMATRIX projTM)
{
	
	if (!m_fbxPath.empty())
	{
		m_objectRender->Update(m_transform->LocalToWorldTM(), ViewTM, projTM);
		m_objectRender->Render();
	}
}

//FBX ��� ����		//���� �׷��Ƚ��� ���� FBX���� ��ü �ʱ�ȭ
void KunrealEngine::Renderer::SetFBXPath(std::string path)
{
	m_fbxPath = path;
	m_objectRender->Init(m_fbxPath);
}

std::string KunrealEngine::Renderer::GetFBXfileName()
{
	return m_fbxPath;
}

//����׸�� ��������		//�ؽ�ó�� ���°� true
void KunrealEngine::Renderer::SetDebugMode(bool debug)
{
	m_debug = debug;
}
