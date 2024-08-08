#pragma once
#include "CommonHeader.h"
#include "Component.h"

/// <summary>
/// �������� ��ư�� �� ������Ʈ
/// 
/// �⺻ ������ 1
/// ������� ���̴� 500px
/// </summary>
/// 
namespace KunrealEngine
{
	class Transform;
	class Button;

	class VolumeControl : public Component
	{
	public:
		VolumeControl();
		VolumeControl(std::string soundType);
		~VolumeControl();

		virtual void Initialize() override;
		virtual void Finalize() override;

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

		void SetBG();
		void SetSFX();

	private:
		float m_minCnt;			//������ ������ �ø���
		float m_maxCnt;			//������ �� ���ΰ�
		float m_currentVolume;	//���� ����
		float m_increaseAmount;	//������	//����� �Ⱦ����� ������ ��
		float m_UIMovement;
		Transform* m_transform;	//�θ� ������Ʈ�� Ʈ������
		Button* m_button;

		bool m_isBG;			//��׶��� �������ΰ�
		bool m_isSFX;			//ȿ���� �������ΰ�

		//bool m_low;
		//bool m_medium;
		//bool m_high;

	private:
		void UpdateVolume();
		void MoveButton();
		void CheckInput();
		void ButtonClicked();
	};
}

