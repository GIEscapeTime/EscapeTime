#include "PointLight.h"
#include <DirectXMath.h>

using namespace ClaVGraphics;

void PointLight::Init()
{
	m_pointLight.Ambient = XMFLOAT4(1.f, 1.f, 1.f, 0.5f);
	m_pointLight.Diffuse = XMFLOAT4(1.f, 1.f, 1.f, 0.5f);
	m_pointLight.Specular = XMFLOAT4(1.f, 1.f, 1.f, 0.5f);

	m_pointLight.Position = XMFLOAT3(0.f, 0.f, 0.f);
	m_pointLight.Range = 1.f;

	m_pointLight.Att = XMFLOAT3(3.f, 0.1f, -1.f);
	m_pointLight.Pad = 0.f;
}

void PointLight::SetPosition(float x, float y, float z)
{
	m_pointLight.Position.x = x; 
	m_pointLight.Position.y = y;  
	m_pointLight.Position.z = z;
}

void PointLight::SetRange(float range)
{
	m_pointLight.Range = range;
}

void PointLight::SetAmbientColor(float r, float g, float b)
{
	m_pointLight.Ambient.x = r;
	m_pointLight.Ambient.y = g;
	m_pointLight.Ambient.z = b;
}

void PointLight::SetDiffuseColor(float r, float g, float b)
{
	m_pointLight.Diffuse.x = r;
	m_pointLight.Diffuse.y = g;
	m_pointLight.Diffuse.z = b;
}

void PointLight::SetSpecularColor(float r, float g, float b)
{
	m_pointLight.Specular.x = r;
	m_pointLight.Specular.y = g;
	m_pointLight.Specular.z = b;
}

void PointLight::SetAtt(float x, float y, float z)
{
	m_pointLight.Att = XMFLOAT3(x, y, z);
}

void PointLight::SetPad(float padding)
{
	m_pointLight.Pad = padding;
}


