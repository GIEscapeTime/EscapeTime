#pragma once
#include "CommonHeader.h"
#include "Component.h"

/// <summary>
/// 스프라이트 이미지, 폰트 등을 출력해주는 컴포넌트
/// 
/// 현재는 이미지랑 폰트를 같이 맡고 있지만 분리할 지 고민중
/// </summary>

namespace ClaVGraphics
{
	class SpriteRender;
	class Text;
}

namespace KunrealEngine
{
	class Transform;

	class UIRenderer : public Component
	{
	public:
		UIRenderer();
		~UIRenderer();

	public:
		virtual void Initialize() override;
		virtual void Finalize() override;

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void SetSpritePath(std::string filePath);
		void SetSpriteActiveColor(bool active);
		void SetSpriteSelectColor(bool select, float r = 1.f, float g = 1.f, float b = 0.f, float a = 1.f);
		void Render();		

	private:
		ClaVGraphics::SpriteRender* m_renderer;		//그래픽스의 UI렌더러
		Transform* m_transform;						//부모 오브젝트의 트랜스폼이 담길 포인터 객체
		std::string m_filePath;						//이미지 경로
	};
}

