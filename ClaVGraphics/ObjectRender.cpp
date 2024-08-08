#include "ObjectRender.h"
#include "ResourceManager.h"
#include "MeshObject.h"

using namespace ClaVGraphics;
using namespace DirectX;
using namespace std;

void ObjectRender::Init(string fileName)
{
	m_meshObject = new MeshObject();
	m_meshObject->Initialize(fileName);
}

void ObjectRender::Update(XMMATRIX trs, XMMATRIX cameraView, XMMATRIX cameraProj)
{
	m_meshObject->Update(trs, cameraView, cameraProj);
}

void ObjectRender::Render()
{
	m_meshObject->Render();
}

void ObjectRender::Render(float r, float g, float b, float a)
{
}
