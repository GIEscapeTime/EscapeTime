#include "TextRenderer.h"
#include "Text.h"
#include "GameObject.h"
#include "Transform.h"

#include <locale>
#include <codecvt>

KunrealEngine::TextRenderer::TextRenderer()
	:m_transform(nullptr), m_textLoader(nullptr), m_txt(), m_lineSpace(10), m_fontSize(10), m_alpha(1)
{

}

KunrealEngine::TextRenderer::~TextRenderer()
{

}

//초기화하는 부분에서 그래픽스의 Text 객체를 만들어주고 부모 오브젝트의 Transform을 가져옴
void KunrealEngine::TextRenderer::Initialize()
{
	m_textLoader = new ClaVGraphics::Text();
	m_transform = GetOwner()->GetComponent<KunrealEngine::Transform>();

	//기본 폰트는 굴림
	m_textLoader->Init(L"Bold");
}

void KunrealEngine::TextRenderer::Finalize()
{
	delete m_textLoader;
}

void KunrealEngine::TextRenderer::FixedUpdate()
{

}

void KunrealEngine::TextRenderer::Update()
{

}

void KunrealEngine::TextRenderer::LateUpdate()
{

}

//텍스트를 담을 텍스트 박스에 대한 데이터
//포지션 매개변수 두 개는 트랜스폼의 UIPosition을 조정해도 됌
//나머지 두 개는 폰트크기, 줄간격
///텍스트 출력하기 위해서 반드시 필요
void KunrealEngine::TextRenderer::SetTextBoxData(float posX, float posY, float fontSize, float lineSpace)
{
	m_transform->SetUIPosition(posX, posY);
	m_fontSize = fontSize;
	m_lineSpace = lineSpace;

	m_textLoader->SetPos(m_transform->GetUIPosition().x, m_transform->GetUIPosition().y);
	m_textLoader->SetSize(m_fontSize);
}

//출력하고자 하는 텍스트를 넣어줌
//실시간으로 변하는 수치를 넣으려면 이게 업데이트문에 있어야함
void KunrealEngine::TextRenderer::SetText(std::string txt)
{
	m_txt = txt;
}

//위와 같은 함수		숫자를 띄우고 싶을 때 사용
void KunrealEngine::TextRenderer::SetText(float txt)
{
	//std::wstring wideString = std::to_wstring(txt);
	std::string newStr = std::to_string(txt);
	m_txt = newStr;
}

//폰트 색상을 변경하는 함수	//기본값은 검정
void KunrealEngine::TextRenderer::SetFontColor(float r, float g, float b)
{
	m_textLoader->SetColor(r, g, b, m_alpha);
}

//폰트의 알파값 설정	//투명도	//기본값은 1
void KunrealEngine::TextRenderer::SetFontAlpha(float alp)
{
	m_alpha = alp;
}

void KunrealEngine::TextRenderer::TextRender()
{
	if (!m_txt.empty())
	{
		//string 변수를 const wchar_t*로 변환
		//int wideStrLength = MultiByteToWideChar(CP_UTF8, 0, m_txt.c_str(), -1, nullptr, 0);
		//wchar_t* wideStr = new wchar_t[wideStrLength];
		//MultiByteToWideChar(CP_UTF8, 0, m_txt.c_str(), -1, wideStr, wideStrLength);
		//const wchar_t* wideCString = wideStr;
		
		int wideStrLength = MultiByteToWideChar(CP_ACP, 0, m_txt.c_str(), -1, nullptr, NULL);
		wchar_t* wideStr = new WCHAR[wideStrLength];
		MultiByteToWideChar(CP_ACP, 0, m_txt.c_str(), strlen(m_txt.c_str()) + 1, wideStr, wideStrLength);
		std::wstring wideCString = wideStr;

		m_textLoader->RenderText(wideCString, m_lineSpace);
	}
}


