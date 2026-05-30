#include "CSoundManager.h"
#include "fmod.h"
#include "fmod.hpp"

#if defined(_DEBUG)
#  pragma comment(lib, "fmodL_vc.lib")
#else
#  pragma comment(lib, "fmod_vc.lib")
#endif

IMPLEMENT_SINGLETON(CSoundManager)

CSoundManager::CSoundManager()
	:m_pSystem(nullptr)
{
	for (int i = 0; i < MAX_CHANNEL; ++i)
		m_pChannelArr[i] = nullptr;
}

CSoundManager::~CSoundManager()
{
	Free();
}

void CSoundManager::Initialize()
{
	FMOD_System_Create(&m_pSystem, FMOD_VERSION);
	FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);
	LoadSoundFile();
}

void CSoundManager::Free()
{
	for (auto& kv : m_mapSound)
	{
		FMOD_Sound_Release(kv.second);
	}
	m_mapSound.clear();

	FMOD_System_Close(m_pSystem);
	FMOD_System_Release(m_pSystem);
	m_pSystem = nullptr;
}

void CSoundManager::SoundPlay(const std::wstring& soundKey, SOUNDCHANNEL eID, float fVolume, bool loop)
{
	auto iter = m_mapSound.find(soundKey);
	if (iter == m_mapSound.end())
		return;

	if (m_pChannelArr[eID])
		FMOD_Channel_Stop(m_pChannelArr[eID]);

	FMOD_System_PlaySound(m_pSystem, iter->second, 0, FALSE, &m_pChannelArr[eID]);

	if (loop) {
		FMOD_Channel_SetMode(m_pChannelArr[eID], FMOD_LOOP_NORMAL);
		FMOD_Channel_SetLoopCount(m_pChannelArr[eID], -1); // 무한 반복
	}
	else {
		FMOD_Channel_SetMode(m_pChannelArr[eID], FMOD_LOOP_OFF);
	}

	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume * m_vols.master * m_fSoundVolumeSetting);
	FMOD_System_Update(m_pSystem);
}

void CSoundManager::PlayBGM(const std::wstring& soundKey, float fVolume)
{
	auto iter = m_mapSound.find(soundKey);
	if (iter == m_mapSound.end())
		return;

	if (m_pChannelArr[SOUND_BGM])
		FMOD_Channel_Stop(m_pChannelArr[SOUND_BGM]);

	FMOD_System_PlaySound(
		m_pSystem,
		iter->second,
		0,
		FALSE,
		&m_pChannelArr[SOUND_BGM]
	);
	FMOD_Channel_SetMode(m_pChannelArr[SOUND_BGM], FMOD_LOOP_NORMAL);
	FMOD_Channel_SetVolume(m_pChannelArr[SOUND_BGM], fVolume * m_vols.master * m_fSoundVolumeSetting);
	FMOD_System_Update(m_pSystem);
}

void CSoundManager::SoundStop(SOUNDCHANNEL eID)
{
	FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void CSoundManager::StopAll()
{
	for (int i = 0; i < MAX_CHANNEL; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSoundManager::SetChannelVolume(SOUNDCHANNEL eID, float fVolume)
{
	FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume * m_vols.master * m_fSoundVolumeSetting);
	FMOD_System_Update(m_pSystem);
}

void CSoundManager::LoadSoundFile()
{
	// exe 기준 ../../../content/sound
	std::wstring soundDir = UpW(ExeDirW(), 3) + L"\\content\\sound";

	// 디버그 출력
	OutputDebugStringW((L"[Sound] Scan Dir: " + soundDir + L"\n").c_str());

	WIN32_FIND_DATAW wfd{};
	HANDLE h = FindFirstFileW((soundDir + L"\\*.*").c_str(), &wfd); // ★ 패턴 중요
	if (h == INVALID_HANDLE_VALUE) {
		OutputDebugStringW(L"[Sound] 폴더가 없거나 비었습니다.\n");
		return;
	}

	do {
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;

		std::wstring wfull = soundDir + L"\\" + wfd.cFileName; // 전체 경로(유니코드)
		std::string  u8 = W2U8(wfull);                      // FMOD용 UTF-8

		FMOD_SOUND* pSound = nullptr;
		FMOD_RESULT r = FMOD_System_CreateSound(m_pSystem, u8.c_str(), FMOD_DEFAULT, 0, &pSound);
		if (r == FMOD_OK) {
			// 키는 파일명(wstring) 그대로
			m_mapSound.emplace(wfd.cFileName, pSound);
		}
		else 
		{
			MSG_BOX("파일 찾기 실패");
		}
	} while (FindNextFileW(h, &wfd));
	FindClose(h);

	FMOD_System_Update(m_pSystem);
}
