#include <iostream>
#include "SoundManager.h"

KunrealEngine::SoundManager::SoundManager()
	: m_pSystem(nullptr), m_pSoundVector(), m_pChannelVector(), m_pAllChannelGroup(nullptr)
	, m_pChannelGroupVector(), m_soundCoutMax(0), m_channelGroupMax(0), m_currentSoundCount(0)
	, m_pSoundPathVector(), m_vVolumeSurport()
{

}


KunrealEngine::SoundManager::~SoundManager()
{
	if (!m_pSoundPathVector.empty())
	{
		m_pSoundPathVector.clear();
	}
	if (!m_vVolumeSurport.empty())
	{
		m_vVolumeSurport.clear();
	}

	// fmod 해제
	for (unsigned int i = 0; i < m_soundCoutMax; i++)
	{
		m_pSoundVector[i]->release();
	}
	for (unsigned int i = 0; i < m_channelGroupMax; i++)
	{
		m_pChannelGroupVector[i]->release();
	}

	m_pSystem->close();
	m_pSystem->release();
}

KunrealEngine::SoundManager::SoundManager(const SoundManager& ref)
	: m_pSystem(nullptr), m_pSoundVector(), m_pChannelVector(), m_pAllChannelGroup(nullptr)
	, m_pChannelGroupVector(), m_soundCoutMax(0), m_channelGroupMax(0), m_currentSoundCount(0)
	, m_pSoundPathVector()
{

}


KunrealEngine::SoundManager& KunrealEngine::SoundManager::GetInstance()
{
	static SoundManager instance;
	return instance;
}

void KunrealEngine::SoundManager::Initialize(unsigned int maxSound, unsigned int maxChannel)
{
	m_soundCoutMax = maxSound;
	m_channelGroupMax = maxChannel;

	// 벡터 resize
	m_pChannelVector.resize(m_soundCoutMax);
	m_pSoundPathVector.resize(m_soundCoutMax);
	m_pSoundVector.resize(m_soundCoutMax);
	m_pChannelGroupVector.resize(m_channelGroupMax);

	// fmod 초기화
	FMOD::System_Create(&m_pSystem);
	m_pSystem->init(m_soundCoutMax, FMOD_INIT_NORMAL, NULL);
	m_pSystem->getMasterChannelGroup(&m_pAllChannelGroup);

	// 채널 초기화
	for (unsigned int i = 0; i < m_soundCoutMax; i++)
	{
		m_pChannelVector[i] = 0;
	}
	for (unsigned int i = 0; i < m_channelGroupMax; i++)
	{
		m_pChannelGroupVector[i] = 0;
	}
	for (unsigned int i = 0; i < m_channelGroupMax; i++)
	{
		m_pAllChannelGroup->addGroup(m_pChannelGroupVector[i]);
	}

	// 볼륨조절용 벡터 초기화
	

}

void KunrealEngine::SoundManager::AddSound(const char* soundPath, SoundGroup groupName)
{
	m_pSystem->createSound(soundPath, FMOD_LOOP_NORMAL | FMOD_DEFAULT, NULL, &m_pSoundVector[m_currentSoundCount]);	  // 사운드 추가
	m_pChannelVector[m_currentSoundCount]->setChannelGroup(m_pChannelGroupVector[groupName]);						  // 그룹에 저장

	switch (groupName)
	{
		case KunrealEngine::SoundManager::BACKGROUNDSOUND:
			m_vVolumeSurport.push_back(0);
			break;
		case KunrealEngine::SoundManager::EFFECTSOUND:
			m_vVolumeSurport.push_back(1);
			break;
		case KunrealEngine::SoundManager::MAINSOUND:
			m_vVolumeSurport.push_back(2);
			break;
		case KunrealEngine::SoundManager::SUBSOUND:
			m_vVolumeSurport.push_back(3);
			break;
	}

	m_pSoundPathVector[m_currentSoundCount] = soundPath;		// 사운드 경로 저장
	m_currentSoundCount++;										// 현재 사운드 개수 증가
}

void KunrealEngine::SoundManager::SoundOnce(const char* soundPath)
{
	for (unsigned int i = 0; i < m_soundCoutMax; i++)
	{
		if (m_pSoundPathVector[i] == soundPath)
		{
			m_pSoundVector[i]->setMode(FMOD_LOOP_OFF);
			m_pSystem->playSound(m_pSoundVector[i], 0, true, &m_pChannelVector[i]);
			m_pChannelVector[i]->setPaused(false);
			break;
		}
	}
}

void KunrealEngine::SoundManager::SoundRepeat(const char* soundPath)
{
	for (unsigned int i = 0; i < m_soundCoutMax; i++)
	{
		if (m_pSoundPathVector[i] == soundPath)
		{
			m_pSoundVector[i]->setMode(FMOD_LOOP_NORMAL);
			m_pSystem->playSound(m_pSoundVector[i], 0, true, &m_pChannelVector[i]);
			m_pChannelVector[i]->setPaused(false);
			break;
		}
	}
}

void KunrealEngine::SoundManager::SoundEnd(const char* soundPath)
{
	for (unsigned int i = 0; i < m_soundCoutMax; i++)
	{
		if (m_pSoundPathVector[i] == soundPath)
		{
			m_pSoundVector[i]->setMode(FMOD_LOOP_OFF);
			m_pChannelVector[i]->stop();
			break;
		}
	}
}

void KunrealEngine::SoundManager::SoundAllEnd()
{
	for (unsigned int i = 0; i < m_soundCoutMax; i++)
	{
		m_pChannelVector[i]->stop();
		break;
	}
}

void KunrealEngine::SoundManager::SoundMute(SoundGroup groupName)
{
	// 임시 변수
	bool paused = false;

	switch (groupName)
	{
		case KunrealEngine::SoundManager::BACKGROUNDSOUND:
			for (int i = 0; i < m_currentSoundCount; i++)
			{
				if (m_vVolumeSurport[i] == 0)
				{
					m_pChannelVector[i]->getPaused(&paused);
					m_pChannelVector[i]->setPaused(!paused);
				}
			}
			break;
		case KunrealEngine::SoundManager::EFFECTSOUND:
			for (int i = 0; i < m_currentSoundCount; i++)
			{
				if (m_vVolumeSurport[i] == 1)
				{
					m_pChannelVector[i]->getPaused(&paused);
					m_pChannelVector[i]->setPaused(!paused);
				}
			}
			break;
		case KunrealEngine::SoundManager::MAINSOUND:
			for (int i = 0; i < m_currentSoundCount; i++)
			{
				if (m_vVolumeSurport[i] == 2)
				{
					m_pChannelVector[i]->getPaused(&paused);
					m_pChannelVector[i]->setPaused(!paused);
				}
			}
			break;
		case KunrealEngine::SoundManager::SUBSOUND:
			for (int i = 0; i < m_currentSoundCount; i++)
			{
				if (m_vVolumeSurport[i] == 3)
				{
					m_pChannelVector[i]->getPaused(&paused);
					m_pChannelVector[i]->setPaused(!paused);
				}
			}
			break;
	}

	//m_pChannelGroupVector[groupName]->getPaused(&paused);	  // 임시변수에 불값 저장
	//m_pChannelGroupVector[groupName]->setPaused(!paused);	  // 뮤트 상태 반전
	//m_pAllChannelGroup->setPaused(!paused);	 
}

void KunrealEngine::SoundManager::SoundVolumeSetting(SoundGroup groupName, float volume)
{
	switch (groupName)
	{
		case KunrealEngine::SoundManager::BACKGROUNDSOUND:
			for (int i = 0; i<m_currentSoundCount; i++)
			{
				if (m_vVolumeSurport[i] == 0)
				{
					m_pChannelVector[i]->setVolume(volume);
				}
			}
			break;
		case KunrealEngine::SoundManager::EFFECTSOUND:
			for (int i = 0; i < m_currentSoundCount; i++)
			{
				if (m_vVolumeSurport[i] == 1)
				{
					m_pChannelVector[i]->setVolume(volume);
				}
			}
			break;
		case KunrealEngine::SoundManager::MAINSOUND:
			for (int i = 0; i < m_currentSoundCount; i++)
			{
				if (m_vVolumeSurport[i] == 2)
				{
					m_pChannelVector[i]->setVolume(volume);
				}
			}
			break;
		case KunrealEngine::SoundManager::SUBSOUND:
			for (int i = 0; i < m_currentSoundCount; i++)
			{
				if (m_vVolumeSurport[i] == 3)
				{
					m_pChannelVector[i]->setVolume(volume);
				}
			}
			break;
	}

	//m_pChannelVector[groupName]->setVolume(volume);
	//m_pChannelGroupVector[groupName]->setVolume(volume);	  // 볼륨 설정	
}
