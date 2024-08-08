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

//�ʱ�ȭ�ϴ� �κп��� �׷��Ƚ��� Text ��ü�� ������ְ� �θ� ������Ʈ�� Transform�� ������
void KunrealEngine::TextRenderer::Initialize()
{
	m_textLoader = new ClaVGraphics::Text();
	m_transform = GetOwner()->GetComponent<KunrealEngine::Transform>();

	//�⺻ ��Ʈ�� ����
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

//�ؽ�Ʈ�� ���� �ؽ�Ʈ �ڽ��� ���� ������
//������ �Ű����� �� ���� Ʈ�������� UIPosition�� �����ص� ��
//������ �� ���� ��Ʈũ��, �ٰ���
///�ؽ�Ʈ ����ϱ� ���ؼ� �ݵ�� �ʿ�
void KunrealEngine::TextRenderer::SetTextBoxData(float posX, float posY, float fontSize, float lineSpace)
{
	m_transform->SetUIPosition(posX, posY);
	m_fontSize = fontSize;
	m_lineSpace = lineSpace;

	m_textLoader->SetPos(m_transform->GetUIPosition().x, m_transform->GetUIPosition().y);
	m_textLoader->SetSize(m_fontSize);
}

//����ϰ��� �ϴ� �ؽ�Ʈ�� �־���
//�ǽð����� ���ϴ� ��ġ�� �������� �̰� ������Ʈ���� �־����
void KunrealEngine::TextRenderer::SetText(std::string txt)
{
	m_txt = txt;
}

//���� ���� �Լ�		���ڸ� ���� ���� �� ���
void KunrealEngine::TextRenderer::SetText(float txt)
{
	//std::wstring wideString = std::to_wstring(txt);
	std::string newStr = std::to_string(txt);
	m_txt = newStr;
}

//��Ʈ ������ �����ϴ� �Լ�	//�⺻���� ����
void KunrealEngine::TextRenderer::SetFontColor(float r, float g, float b)
{
	m_textLoader->SetColor(r, g, b, m_alpha);
}

//��Ʈ�� ���İ� ����	//����	//�⺻���� 1
void KunrealEngine::TextRenderer::SetFontAlpha(float alp)
{
	m_alpha = alp;
}

void KunrealEngine::TextRenderer::TextRender()
{
	if (!m_txt.empty())
	{
		//string ������ const wchar_t*�� ��ȯ
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


