#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

typedef struct MonsterSpawner
{
    _vec3 vPos = {};
    int iRange = 0;
    int iCount = 0; 
    float Spawntime = 0.f;
}SPAWNER;

class ENGINE_DLL CMonsterSystem :
    public CBase
{
    DECLARE_SINGLETON(CMonsterSystem)

public:
    explicit CMonsterSystem();
    virtual ~CMonsterSystem();

public:

    _int    System_Update(const float& fTimeDelta);
    void    System_LateUpdate(const float& fTimeDelta);

    void AddMonsterKill() { m_iMonsterKill++; }
    int  GetMonsterKill() { return m_iMonsterKill; }
    void  ResetMonsterKill() { m_iMonsterKill = 0; }

private:
    void    Change_Monster_Status();
    void    Dynamic_Monster_Spawn();

    void    PlayingSpawnSound(const float& fTimeDelta);
    void    Make_Monster(const float& fTimeDelta); // 발생위치, 범위, 객체 수, 스폰 시간 



private:
    void    Free();

private:
    SPAWNER                 m_tMonsterSpawner;
    int                     m_iMonsterCount = 0;

    int                     m_iSpawnSoundCount = 3;
    int                     m_iSpawnCount = 0;

    float                   m_fElapsedTime = 0.f;
    float                   m_fSpawnSoundElapsedTime = 0.f;

    _bool                   m_bSpawnSoundActive = false;
    _bool                   m_bMonsterTrapActive = false;

    int                     m_iMonsterKill = 0;

};

END