#include "Transform.h"
#include "GameObject.h"

//�Ҽӵ� ������Ʈ ���� �����Ǵ� �� ���� �ȵȴ�
KunrealEngine::Transform::Transform(GameObject* object)
	:m_position(0, 0, 0), m_rotation(0, 0, 0, 1), m_scale(1.0f, 1.0f, 1.0f),
	m_UIPosition(0, 0), m_UIScale(1.0f, 1.0f)
{
	this->SetOwner(object);
}

KunrealEngine::Transform::~Transform()
{

}

void KunrealEngine::Transform::Initialize()
{

}

void KunrealEngine::Transform::Finalize()
{

}

void KunrealEngine::Transform::FixedUpdate()
{

}

void KunrealEngine::Transform::Update()
{
		
}

void KunrealEngine::Transform::LateUpdate()
{

}

//�����°� �ƴ϶� �Է¹��� ������ �ƿ� �ٲ�� �Լ�
void KunrealEngine::Transform::SetRotation(DirectX::XMFLOAT3 rot)
{
	rot.x = DirectX::XMConvertToRadians(rot.x);
	rot.y = DirectX::XMConvertToRadians(rot.y);
	rot.z = DirectX::XMConvertToRadians(rot.z);

	auto Jungandgo = DirectX::XMQuaternionRotationRollPitchYaw(rot.x, rot.y, rot.z);

	DirectX::XMStoreFloat4(&m_rotation, Jungandgo);

	//m_rotation = DirectX::XMQuaternionRotationRollPitchYaw(rot.x, rot.y, rot.z);

	//m_rotation = rot;
}

void KunrealEngine::Transform::SetRotation(float x, float y, float z)
{
	DirectX::XMFLOAT3 newfloat3(x, y, z);

	SetRotation(newfloat3);
}

//SetPosition�� �ٸ��� �Է°���ŭ �����̴°�
void KunrealEngine::Transform::Translate(DirectX::XMFLOAT3 pos)
{
	m_position.x += pos.x;
	m_position.y += pos.y;
	m_position.z += pos.z;
}
                                                   
//default �Ű����� Ȯ���ؼ���
void KunrealEngine::Transform::Translate(float x = 0, float y = 0, float z = 0)
{
	DirectX::XMFLOAT3 newfloat3(x, y, z);

	Translate(newfloat3);
}

//������Ʈ�� ��ǥ�� �������ִ� �Լ�
//������ �̵��ϴ°� �ƴ϶� �����̵� ����
void KunrealEngine::Transform::SetPosition(DirectX::XMFLOAT3 pos)
{
	m_position = pos;
}

///UI ���� position ����
void KunrealEngine::Transform::SetUIPosition(DirectX::XMFLOAT2 pos)
{
	m_UIPosition = pos;
}

void KunrealEngine::Transform::SetUIPosition(float x, float y)
{
	m_UIPosition.x = x;
	m_UIPosition.y = y;
}

DirectX::XMFLOAT2 KunrealEngine::Transform::GetUIPosition()
{
	return this->m_UIPosition;
}

void KunrealEngine::Transform::SetUIScale(DirectX::XMFLOAT2 scale)
{
	m_UIScale = scale;
}

void KunrealEngine::Transform::SetUIScale(float x, float y)
{
	m_UIScale.x = x;
	m_UIScale.y = y;
}

DirectX::XMFLOAT2 KunrealEngine::Transform::GetUIScale()
{
	return this->m_UIScale;
}
///

//������(x, y, z ��ǥ)�� ��ȯ�ϴ� �Լ�
DirectX::XMFLOAT3 KunrealEngine::Transform::GetPosition()
{
	return this->m_position;
}

//ȸ������ ��ȯ�ϴ� �Լ� XMFLOAT4
DirectX::XMFLOAT4 KunrealEngine::Transform::GetRotation()
{
	return this->m_rotation;
}

//ũ�Ⱚ�� ��ȯ�ϴ� �Լ� x y z
DirectX::XMFLOAT3 KunrealEngine::Transform::GetScale()
{
	return this->m_scale;
}

//���� ��ġ���� ���ȭ
DirectX::XMMATRIX KunrealEngine::Transform::GetLocalPositionMatrix()
{
	DirectX::XMMATRIX positionMatrix = DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	return positionMatrix;
}

//���� ȸ������ ���ȭ	//float 4�� ����ü�̹Ƿ� ���ʹϾ�ȭ
DirectX::XMMATRIX KunrealEngine::Transform::GetLocalRotationMatrix()
{
	//���ʹϾ� -> ����� ������
	//�ٵ� ��� -> ���ʹϾ�? ������

	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&this->m_rotation));

	return rotationMatrix;
}

//���� ũ�Ⱚ�� ���ȭ
DirectX::XMMATRIX KunrealEngine::Transform::GetLocalScaleMatrix()
{
	DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	return scaleMatrix;
}

//���� Ʈ������ ����� ���÷�	//�³� �̰�
DirectX::XMMATRIX KunrealEngine::Transform::WorldToLocalTM()
{
	return GetLocalPositionMatrix() * GetLocalRotationMatrix() * GetLocalScaleMatrix();
}

//���� Ʈ������ ����� ���÷�
//S * R * T ������		//�̸� �򰥸��� Position�Լ��̸� �ٲ� ���� �����غ���
DirectX::XMMATRIX KunrealEngine::Transform::LocalToWorldTM()
{
	//�θ� ������ �׳� �ٷ� ��ȯ
 	if (this->GetOwner()->GetParent() == nullptr)
 	{
 		return GetLocalScaleMatrix() * GetLocalRotationMatrix() * GetLocalPositionMatrix();
 	}
 	//�θ� ������ �������� �θ��� WorldTM�� ������
 	else if (this->GetOwner()->GetParent() != nullptr)
 	{		
		Transform* parentTrans = GetOwner()->GetParent()->GetComponent<Transform>();
		const DirectX::XMFLOAT4 parentRotationPtr = parentTrans->GetRotation();

  		DirectX::XMMATRIX positionMatrix = DirectX::XMMatrixTranslation(parentTrans->GetPosition().x, parentTrans->GetPosition().y, parentTrans->GetPosition().z);
  		DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&parentRotationPtr));
  		DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(parentTrans->GetScale().x, parentTrans->GetScale().y, parentTrans->GetScale().z);
  	
  		return GetLocalScaleMatrix() * GetLocalRotationMatrix() * GetLocalPositionMatrix() * (positionMatrix * rotationMatrix * scaleMatrix);
 	}
}

//�Լ� �����ε� //�Ű������� ������Ʈ�� �־������� ���� WorldTM
DirectX::XMMATRIX KunrealEngine::Transform::LocalToWorldTM(GameObject* obj)
{
	if (this->GetOwner()->GetParent() == nullptr)
	{

	}

	//����
	return GetLocalScaleMatrix() * GetLocalRotationMatrix() * GetLocalPositionMatrix();
}

void KunrealEngine::Transform::SetPosition(float x, float y, float z)
{
	DirectX::XMFLOAT3 newfloat3(x, y, z);

	SetPosition(newfloat3);
}

void KunrealEngine::Transform::SetScale(float x, float y, float z)
{
	DirectX::XMFLOAT3 newfloat3(x, y, z);
	m_scale = newfloat3;
}
