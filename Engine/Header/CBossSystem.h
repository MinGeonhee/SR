#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

enum BOSS_PHASE
{
    BOSS_PHASE_NONE,
    BOSS_PHASE_INTRO,
    BOSS_PHASE_ONE,
    BOSS_PHASE_CUTSCENE_1_TO_2,
    BOSS_PHASE_TWO,
    BOSS_PHASE_END,
};

class CGameObject;

class ENGINE_DLL CBossSystem :
    public CBase
{
    DECLARE_SINGLETON(CBossSystem)

public:
    explicit CBossSystem();
    virtual ~CBossSystem();

public:

    _int    System_Update(const float& fTimeDelta);
    void    System_LateUpdate(const float& fTimeDelta);

public:
    BOSS_PHASE    Get_BossPhase() { return m_eCurBossPhase; }

private:

    void    Boss_Room_Enter();
    void    First_Phase(); // 
    void    Second_Phase();
    void    Third_Phase();

private:
    void    Free();

private:

    BOSS_PHASE      m_eCurBossPhase = BOSS_PHASE::BOSS_PHASE_NONE;
    CGameObject*    m_pBossBarrier = nullptr;

    _bool           m_bBarrierActive = true;
};

END