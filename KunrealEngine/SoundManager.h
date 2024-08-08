#pragma once
#pragma comment(lib,"fmod_vc.lib")

#include <vector>

#include "fmod.hpp"
#include "fmod_common.h"

/// <summary>
/// 23.08.01 주희용
/// 
/// 싱글턴으로 만든 사운드 플레이어
/// 
/// 팀장이 바뻐서 대신 만듦 (도움을 준 사람: 김도우)
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

		// 초기화
		void Initialize(unsigned int maxSound, unsigned int maxChannel);
		
		/// <summary>
		/// 사운드 추가
		/// </summary>
		/// <param name="soundName">추가할 사운드 파일 그룹 이름 BACKGROUNDSOUND, EFFECTSOUND,	MAINSOUND, SUBSOUND< / param>
		void AddSound(const char* soundPath, SoundGroup groupName);	    // 사운드 리스트에 저장

		void SoundOnce(const char* soundPath);							// 한번 재생			
		void SoundRepeat(const char* soundPath);						// 반복 재생 시작		
		void SoundEnd(const char* soundPath);							// 재생 종료			
		void SoundAllEnd();												// 재생 종료 All

		void SoundMute(SoundGroup groupName);							// 음소거
		void SoundVolumeSetting(SoundGroup groupName, float volume);	// 볼륨 설정

		void SoundMute(int i)
		{
			// 임시 변수
			bool paused = true;

			m_pChannelGroupVector[i]->getPaused(&paused);	  // 임시변수에 불값 저장
			m_pChannelGroupVector[i]->setPaused(!paused);	  // 뮤트 상태 반전
		}

	private:
		SoundManager();
		~SoundManager();
		SoundManager(const SoundManager& ref);
		SoundManager& operator=(const SoundManager& ref) = delete;

	private:
		FMOD::System* m_pSystem;									    // 리소스 관리 객체
		std::vector<FMOD::Sound*> m_pSoundVector;					    // 사운드 리스트
		std::vector<FMOD::Channel*> m_pChannelVector;				    // 채널 리스트
		std::vector<FMOD::ChannelGroup*> m_pChannelGroupVector;		    // 채널 그룹 리스트(int 번호로 구별)
		FMOD::ChannelGroup* m_pAllChannelGroup;						    // 채널 전체 그룹
																	     
		unsigned int m_soundCoutMax;								    // 최대 저장 가능한 사운드 개수
		unsigned int m_channelGroupMax;								    // 최대 저장 가능한 채널 개수
		int m_currentSoundCount;									    // 사운드 번호
		std::vector<const char*> m_pSoundPathVector;				    // 사운드 리스트
		
		std::vector<int> m_vVolumeSurport;		// 사운드 조절용
	};
}

