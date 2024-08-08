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

//모든 빛이여 사라져라... 어둠...
void KunrealEngine::Light::DeleteLight()
{
	ClaVGraphics::PointLightSetting::DeleteLight();
}

//밝기 조정	//여러개를 들고 있을테니 특정 빛에 접근하려면..
void KunrealEngine::Light::SetLightIntensity()
{
	
}

//그래픽스에서 나오는거 보고
void KunrealEngine::Light::Render()
{
	
}
