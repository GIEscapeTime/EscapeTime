#pragma once
#include "CommonHeader.h"
#include "Component.h"
#include <functional>

/// <summary>
/// ��ư ������Ʈ
/// 
/// UI ������Ʈ���� ����ϰ� �� ������Ʈ
/// </summary>

namespace KunrealEngine
{
	class Transform;

	class Button : public Component
	{
	public:
		Button();
		~Button();

		virtual void Initialize() override;
		virtual void Finalize() override;
		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

	private:
		float m_width;												//ũ��&���� ������ ũ��ʹ� �ٸ�
		float m_height;

		float m_left;												//��ư ������ üũ�ϱ� ���� RECT ������
		float m_right;
		float m_top;
		float m_bottom;

		Transform* m_transform;										//�θ� ������Ʈ�� Ʈ������
		std::vector<std::function<void()>> m_functionContainer;		//������ �Լ� �����̳�	//���ٵ� ����

	public:
		bool OnLeftClick();
		bool OnLeftClickUp();
		bool OnLeftHold();

		bool OnRightClick();
		bool OnRightClickUp();
		bool OnRightHold();

		void SetSize(float width, float height);
	};
}

