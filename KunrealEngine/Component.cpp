#include "Component.h"

KunrealEngine::Component::Component()
	:ownerObject(nullptr), isActivated(true)
{

}

//컴포넌트를 생성할때 매개변수로 오브젝트를 줘서 두번 일하는걸 방지해도 된다
KunrealEngine::Component::Component(GameObject* object)
{
	SetOwner(object);
}

KunrealEngine::Component::~Component()
{

}

//어떤 오브젝트가 컴포넌트를 가지고 있는지
//오브젝트에 컴포넌트를 부여하고 이 함수를 실행시켜주자
void KunrealEngine::Component::SetOwner(GameObject* object)
{
	ownerObject = object;
}

KunrealEngine::GameObject* KunrealEngine::Component::GetOwner()
{
	return this->ownerObject;
}

//컴포넌트를 활성화 혹은 비활성화 하고 싶을 수도 있다 활성 여부를 조작할 수 있게 해주는 함수들
//활성 여부에 따라서 업데이트를 하게 해야할텐데..
void KunrealEngine::Component::ActivateComponent()
{
	isActivated = true;
}

void KunrealEngine::Component::InactivateComponent()
{
	isActivated = false;
}

//외부에서 활성화 여부를 체크하고 싶을 때
bool KunrealEngine::Component::GetActivated()
{
	return this->isActivated;
}
