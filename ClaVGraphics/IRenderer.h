#pragma once

/// �������� �������̽�
/// 2023.07.20
/// ������
/// ClaVGraphics���� ����� ���������Լ����� ������ �ִ� �������̽��̴�
/// �׷��� �׷��Ƚ� ���������� ����� ���ε� �� ���� �������̽��� ����ϴ� ������ 
/// Ȯ��ġ �ʾƼ� �˾ƺ��߰ڴ�...
/// 
/// Ȯ�强 : ������ DirectX11���� ����ؼ� �׷��Ƚ� ������ ��������
///		OpenGL�̳� DirectX12�� ����ؼ� �������� �׷��Ƚ� ������ ������鶧
///		Interface �ϳ��� �ҷ��� �� �ְ� ����� ���ؼ��̴�
/// 
/// ���ȼ� : DLL ���Ϸ� ������ Interface ����� ��������� �ϴ���
///		����� �ϰ� ����� �ְ�, ���빰�� ���� �ִ��� Ȯ�� �� �� ���� �Ѵ�
/// 
/// Ȯ�Ǽ� : Interface�� ��� �޴� �������� ��ü�߿� �ϳ��� ��԰� �����Ȱ��� �ִٸ�
///		������ �ȵǹǷ� Ȯ���ϰ� ����üũ�� �����ϴ�

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