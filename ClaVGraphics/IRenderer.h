#pragma once

/// 렌더러의 인터페이스
/// 2023.07.20
/// 김현재
/// ClaVGraphics에서 사용할 순수가상함수들을 가지고 있는 인터페이스이다
/// 그런데 그래픽스 엔진에서만 사용할 뿐인데 왜 구지 인터페이스를 사용하는 것인지 
/// 확실치 않아서 알아봐야겠다...
/// 
/// 확장성 : 지금은 DirectX11만을 사용해서 그래픽스 엔진을 만들지만
///		OpenGL이나 DirectX12를 사용해서 여러개의 그래픽스 엔진을 만들었들때
///		Interface 하나로 불러올 수 있게 만들기 위해서이다
/// 
/// 보안성 : DLL 파일로 뺐을때 Interface 헤더만 전달해줬다 하더라도
///		기능을 하게 만들수 있고, 내용물이 뭐가 있는지 확인 할 수 없게 한다
/// 
/// 확실성 : Interface를 상속 받는 여려개의 객체중에 하나라도 까먹고 구현된것이 있다면
///		실행이 안되므로 확실하게 더블체크가 가능하다

namespace ClaVGraphics
{
	class IRenderer abstract
	{
	public:
		IRenderer() {};
		virtual ~IRenderer() {};

	public:
		virtual bool Initialize(long hWnd, int Width, int Height) abstract;
		virtual void Finalize() abstract;

		virtual void OnResize() abstract;

		virtual void BeginRender() abstract;
		virtual void BeginRender(float r, float g, float b, float alpha) abstract;

		virtual void EndRender() abstract;
	};
}