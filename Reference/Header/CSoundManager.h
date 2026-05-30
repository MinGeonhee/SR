#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

struct FMOD_SYSTEM;
struct FMOD_SOUND;
struct FMOD_CHANNEL;

class ENGINE_DLL CSoundManager :
    public CBase
{
    DECLARE_SINGLETON(CSoundManager)
public:
    explicit CSoundManager();
    virtual ~CSoundManager();

public:

    struct AudioVolumes {
        float master = 1.0f;
        float sfx = 0.8f;
        float music = 0.2f;
        float ambient = 0.5f;
    };
    
    void    Initialize();
public:
    void    SoundPlay(const wstring& pSoundKey, SOUNDCHANNEL eID, float fVolume, bool loop = false);
    void    PlayBGM(const std::wstring& soundKey, float fVolume);

    void    SoundStop(SOUNDCHANNEL eID);
    void    StopAll();

    void    SetChannelVolume(SOUNDCHANNEL eID, float fVolume);

    void    SetSoundSettingScale(float fSound) { m_fSoundVolumeSetting = fSound; };

    inline void SetVol(const AudioVolumes& v) { m_vols = v; }
    inline AudioVolumes GetVol() const { return m_vols; }

private:
    void    LoadSoundFile();

    static std::wstring ExeDirW() {
        wchar_t buf[MAX_PATH] = {};
        GetModuleFileNameW(nullptr, buf, MAX_PATH);
        std::wstring s(buf);
        size_t p = s.find_last_of(L"\\/");
        if (p != std::wstring::npos) s.resize(p);
        return s; // ...\Client\Bin\Debug
    }

    static std::wstring UpW(std::wstring p, int n) {
        while (n-- > 0) {
            size_t pos = p.find_last_of(L"\\/");
            if (pos == std::wstring::npos) break;
            p.resize(pos);
        }
        return p;
    }
    // UTF-8 변환
    static std::string W2U8(const std::wstring& ws) {
        int len = WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::string out(len ? len - 1 : 0, '\0');
        if (len > 1) WideCharToMultiByte(CP_UTF8, 0, ws.c_str(), -1, &out[0], len - 1, nullptr, nullptr);
        return out;
    }
private:
    map<std::wstring, FMOD_SOUND*> m_mapSound;

    // FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체
    FMOD_CHANNEL* m_pChannelArr[MAX_CHANNEL];

    // 사운드, 채널 객체 및 장치를 관리하는 객체
    FMOD_SYSTEM* m_pSystem;
    float       m_fSoundVolumeSetting = 1.f;

private:
    virtual     void    Free()override;

    AudioVolumes m_vols;
};

END
