#include "Light.h"
#include "GameObject.h"
#include "PointLightSetting.h"

KunrealEngine::Light::Light()
	:m_transform(nullptr)
{

}

KunrealEngine::Light::~Light()
{

}

void KunrealEngine::Light::Initialize()
{
	m_transform = GetOwner()->GetComponent<KunrealEngine::Transform>();
}

void KunrealEngine::Light::Finalize()
{

}

void KunrealEngine::Light::FixedUpdate()
{

}

void KunrealEngine::Light::Update()
{

}

void KunrealEngine::Light::LateUpdate()
{

}

void KunrealEngine::Light::AddLight(float x, float y, float z, float range, float r, float g, float b)
{
	ClaVGraphics::PointLightSetting* newLight = new ClaVGraphics::PointLightSetting();
	newLight->SetPosition(x, y, z);
	newLight->SetRange(range);
	newLight->SetColor(r, g, b);
	newLight->AddLight();

	//static std::vector<ClaVGraphics::PointLightSetting*> newLights;
	//newLights.emplace_back(new ClaVGraphics::PointLightSetting());
}

//��� ���̿� �������... ���...
void KunrealEngine::Light::DeleteLight()
{
	ClaVGraphics::PointLightSetting::DeleteLight();
}

//��� ����	//�������� ��� �����״� Ư�� ���� �����Ϸ���..
void KunrealEngine::Light::SetLightIntensity()
{
	
}

//�׷��Ƚ����� �����°� ����
void KunrealEngine::Light::Render()
{
	
}
