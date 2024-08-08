#pragma once
#pragma comment(lib,"fmod_vc.lib")

#include <vector>

#include "fmod.hpp"
#include "fmod_common.h"

/// <summary>
/// 23.08.01 �����
/// 
/// �̱������� ���� ���� �÷��̾�
/// 
/// ������ �ٻ��� ��� ���� (������ �� ���: �赵��)
/// </summary>
namespace KunrealEngine
{
	class SoundManager
	{
	public:

		enum SoundGroup
		{
			BACKGROUNDSOUND,
			EFFECTSOUND,
			MAINSOUND,
			SUBSOUND
		};

	public:
		static SoundManager& GetInstance();

		// �ʱ�ȭ
		void Initialize(unsigned int maxSound, unsigned int maxChannel);
		
		/// <summary>
		/// ���� �߰�
		/// </summary>
		/// <param name="soundName">�߰��� ���� ���� �׷� �̸� BACKGROUNDSOUND, EFFECTSOUND,	MAINSOUND, SUBSOUND< / param>
		void AddSound(const char* soundPath, SoundGroup groupName);	    // ���� ����Ʈ�� ����

		void SoundOnce(const char* soundPath);							// �ѹ� ���			
		void SoundRepeat(const char* soundPath);						// �ݺ� ��� ����		
		void SoundEnd(const char* soundPath);							// ��� ����			
		void SoundAllEnd();												// ��� ���� All

		void SoundMute(SoundGroup groupName);							// ���Ұ�
		void SoundVolumeSetting(SoundGroup groupName, float volume);	// ���� ����

		void SoundMute(int i)
		{
			// �ӽ� ����
			bool paused = true;

			m_pChannelGroupVector[i]->getPaused(&paused);	  // �ӽú����� �Ұ� ����
			m_pChannelGroupVector[i]->setPaused(!paused);	  // ��Ʈ ���� ����
		}

	private:
		SoundManager();
		~SoundManager();
		SoundManager(const SoundManager& ref);
		SoundManager& operator=(const SoundManager& ref) = delete;

	private:
		FMOD::System* m_pSystem;									    // ���ҽ� ���� ��ü
		std::vector<FMOD::Sound*> m_pSoundVector;					    // ���� ����Ʈ
		std::vector<FMOD::Channel*> m_pChannelVector;				    // ä�� ����Ʈ
		std::vector<FMOD::ChannelGroup*> m_pChannelGroupVector;		    // ä�� �׷� ����Ʈ(int ��ȣ�� ����)
		FMOD::ChannelGroup* m_pAllChannelGroup;						    // ä�� ��ü �׷�
																	     
		unsigned int m_soundCoutMax;								    // �ִ� ���� ������ ���� ����
		unsigned int m_channelGroupMax;								    // �ִ� ���� ������ ä�� ����
		int m_currentSoundCount;									    // ���� ��ȣ
		std::vector<const char*> m_pSoundPathVector;				    // ���� ����Ʈ
		
		std::vector<int> m_vVolumeSurport;		// ���� ������
	};
}

