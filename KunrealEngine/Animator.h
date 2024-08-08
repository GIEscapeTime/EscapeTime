#pragma once
#include "CommonHeader.h"
#include "Component.h"

/// <summary>
/// ���簡 ���� FBX �ִϸ��̼��� ������ִ� ������Ʈ
/// </summary>

namespace KunrealEngine
{
	class Animator : public Component
	{
	public:
		Animator();
		~Animator();

		virtual void Initialize() override;
		virtual void Finalize() override;

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;

	public:
		void AddAnimation();
		void PlayAnimation();

	private:
		std::wstring m_meshName;
		std::wstring m_currentAnimation;
		float m_updateTime;
		int m_curFrame;
		int m_nextFrame;
	};
}

