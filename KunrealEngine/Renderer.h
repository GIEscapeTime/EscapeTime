#pragma once
#include "CommonHeader.h"
#include "Component.h"
#include <DirectXMath.h>

namespace ClaVGraphics
{
	class ObjectRender;
}

/// <summary>
/// 오브젝트의 그림을 그려주는 컴포넌트
/// 
/// 오브젝트의 active 여부와는 별개로 돌아가기 때문에
/// 오브젝트가 꺼져 있어도 이 컴포넌트가 살아있으면 그림은 그림
/// 
/// 이 방식으로 한 이유는
/// 그림은 그리되 오브젝트의 다른 컴포넌트들이 기능못하게 하고 싶을때도 있고
/// 반대로 그림만 사라지고 다른 컴포넌트들이 작동하게 하고 싶을때도 있기 때문
/// </summary>

namespace KunrealEngine
{
	class Transform;

	class Renderer : public Component
	{
	public:
		Renderer();
		~Renderer();

	public:
		virtual void Initialize() override;
		virtual void Finalize() override;

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void Render(DirectX::XMMATRIX ViewTM, DirectX::XMMATRIX projTM);
		void SetFBXPath(std::string path);
		std::string GetFBXfileName();

		void SetDebugMode(bool debug);
	private:
		ClaVGraphics::ObjectRender* m_objectRender;		//그래픽스 엔진의 오브젝트렌더러
		Transform* m_transform;							//부모 오브젝트의 transform
		std::string m_fbxPath;							//FBX 경로
		bool m_debug;									//디버그 모드 유무	//텍스처를 넣을지 말지
	};
}


