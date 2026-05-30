#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "CCollisionManager.h"

BEGIN(Engine)

class CGameObject;
// CollisionEventManager은 물리적 충돌 이벤트를 담당하는 매니저이고
// TriggerEventManager는 물리적 반응 없이 상태 변화를 일으키는 것을 담당하는 매니저이다.

// ★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
// 반드시 TriggerEventManager가 먼저 호출되고 선 판단을 한 후, CollisionManager & CollisionEventManager가 호출되어야 한다.

class ENGINE_DLL CTriggerEventManager :
    public CBase
{
    DECLARE_SINGLETON(CTriggerEventManager)

private:
    explicit CTriggerEventManager();
    virtual ~CTriggerEventManager();

public:
    _int    Update_Event(const float& fTimeDelta);
    void    Late_Update(const float& fTimeDelta);

public:
    // 이벤트 콜링을 
    void    Set_BossDooropen(bool b) { m_bBossDoorOpen = b; }
    void    Boss_DoorOpen(const float& fTimeDelta);

    void    Set_BossZoomCutSceneActive(bool b) { m_bBossZoomCutSceneActive = b; }
    void    Boss_ZoomCutScene(const float& fTimeDelta);

    void    Set_Boss_AppearSceneActive(bool b) { m_bBossAppearSceneActive = b; }
    void    Boss_AppearScene(const float& fTimeDelta);

    void    Set_Boss_gimmikSuccessSceneActive(bool b) { m_bBossgimmikSuccessSceneActive = b; }
    void    Boss_GimmikScene(const float& fTimeDelta);

    void    Set_Boss_GimmikSceneEnd(bool b) { m_bBoss_GimmikSceneEnd = b; }
    void    Boss_GimmikSCeneEnd(const float& fTimeDelta);

    void    Set_Boss_1Phaseto2CutsceneBegin(bool b) { m_bBoss_1Phaseto2CutsceneBegin = b; }
    void    Boss_1Phaseto2CutsceneBegin(const float& fTimeDelta);

    void    Set_Boss_1Phaseto2CutsceneUpdate(bool b) { m_bBoss_1Phaseto2CutsceneUpdate = b; }
    void    Boss_1Phaseto2CutsceneUpdate(const float& fTimeDelta);

    void    Set_Boss_2PhaseBegin(bool b) { m_bBoss_2PhaseBegin = b; }
    void    Boss_2PhaseEnter(const float& fTimeDelta);
private:

    virtual void    Free() override;

private:
    _bool           m_bBossDoorOpen = false;
    _bool           m_bBossZoomCutSceneActive = false;
    _bool           m_bBossAppearSceneActive = false;
    _bool           m_bBossgimmikSuccessSceneActive = false;
    _bool           m_bBoss_GimmikSceneEnd = false;
    _bool           m_bBoss_1Phaseto2CutsceneBegin = false;
    _bool           m_bBoss_1Phaseto2CutsceneUpdate = false;
    _bool           m_bBoss_2PhaseBegin = false;

    float           m_fBossTimeElapsed = 0.f;
    int             m_fBossAppeareffectdeletecount = 0;

};

END