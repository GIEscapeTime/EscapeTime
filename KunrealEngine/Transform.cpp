#include "Transform.h"
#include "GameObject.h"

//소속될 오브젝트 없이 생성되는 건 말이 안된다
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

//돌리는게 아니라 입력받은 각도로 아예 바뀌는 함수
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

//SetPosition과 다르게 입력값만큼 움직이는것
void KunrealEngine::Transform::Translate(DirectX::XMFLOAT3 pos)
{
	m_position.x += pos.x;
	m_position.y += pos.y;
	m_position.z += pos.z;
}
                                                   
//default 매개변수 확인해서써
void KunrealEngine::Transform::Translate(float x = 0, float y = 0, float z = 0)
{
	DirectX::XMFLOAT3 newfloat3(x, y, z);

	Translate(newfloat3);
}

//오브젝트의 좌표를 설정해주는 함수
//서서히 이동하는게 아니라 순간이동 개념
void KunrealEngine::Transform::SetPosition(DirectX::XMFLOAT3 pos)
{
	m_position = pos;
}

///UI 전용 position 설정
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

//포지션(x, y, z 좌표)를 반환하는 함수
DirectX::XMFLOAT3 KunrealEngine::Transform::GetPosition()
{
	return this->m_position;
}

//회전값을 반환하는 함수 XMFLOAT4
DirectX::XMFLOAT4 KunrealEngine::Transform::GetRotation()
{
	return this->m_rotation;
}

//크기값을 반환하는 함수 x y z
DirectX::XMFLOAT3 KunrealEngine::Transform::GetScale()
{
	return this->m_scale;
}

//로컬 위치값을 행렬화
DirectX::XMMATRIX KunrealEngine::Transform::GetLocalPositionMatrix()
{
	DirectX::XMMATRIX positionMatrix = DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	return positionMatrix;
}

//로컬 회전값을 행렬화	//float 4개 구조체이므로 쿼터니언화
DirectX::XMMATRIX KunrealEngine::Transform::GetLocalRotationMatrix()
{
	//쿼터니언 -> 행렬은 괜찮아
	//근데 행렬 -> 쿼터니언? 하지마

	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&this->m_rotation));

	return rotationMatrix;
}

//로컬 크기값을 행렬화
DirectX::XMMATRIX KunrealEngine::Transform::GetLocalScaleMatrix()
{
	DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	return scaleMatrix;
}

//월드 트랜스폼 행렬을 로컬로	//맞냐 이거
DirectX::XMMATRIX KunrealEngine::Transform::WorldToLocalTM()
{
	return GetLocalPositionMatrix() * GetLocalRotationMatrix() * GetLocalScaleMatrix();
}

//로컬 트랜스폼 행렬을 로컬로
//S * R * T 순서임		//이름 헷갈리면 Position함수이름 바꿀 예정 논의해보자
DirectX::XMMATRIX KunrealEngine::Transform::LocalToWorldTM()
{
	//부모가 없으면 그냥 바로 변환
 	if (this->GetOwner()->GetParent() == nullptr)
 	{
 		return GetLocalScaleMatrix() * GetLocalRotationMatrix() * GetLocalPositionMatrix();
 	}
 	//부모가 있으면 마지막에 부모의 WorldTM을 곱해줌
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

//함수 오버로딩 //매개변수로 오브젝트를 넣어줬을때 그의 WorldTM
DirectX::XMMATRIX KunrealEngine::Transform::LocalToWorldTM(GameObject* obj)
{
	if (this->GetOwner()->GetParent() == nullptr)
	{

	}

	//미정
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
