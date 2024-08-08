#include "Component.h"

KunrealEngine::Component::Component()
	:ownerObject(nullptr), isActivated(true)
{

}

//������Ʈ�� �����Ҷ� �Ű������� ������Ʈ�� �༭ �ι� ���ϴ°� �����ص� �ȴ�
KunrealEngine::Component::Component(GameObject* object)
{
	SetOwner(object);
}

KunrealEngine::Component::~Component()
{

}

//� ������Ʈ�� ������Ʈ�� ������ �ִ���
//������Ʈ�� ������Ʈ�� �ο��ϰ� �� �Լ��� �����������
void KunrealEngine::Component::SetOwner(GameObject* object)
{
	ownerObject = object;
}

KunrealEngine::GameObject* KunrealEngine::Component::GetOwner()
{
	return this->ownerObject;
}

//������Ʈ�� Ȱ��ȭ Ȥ�� ��Ȱ��ȭ �ϰ� ���� ���� �ִ� Ȱ�� ���θ� ������ �� �ְ� ���ִ� �Լ���
//Ȱ�� ���ο� ���� ������Ʈ�� �ϰ� �ؾ����ٵ�..
void KunrealEngine::Component::ActivateComponent()
{
	isActivated = true;
}

void KunrealEngine::Component::InactivateComponent()
{
	isActivated = false;
}

//�ܺο��� Ȱ��ȭ ���θ� üũ�ϰ� ���� ��
bool KunrealEngine::Component::GetActivated()
{
	return this->isActivated;
}
