#pragma once
#include "ICameraState.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL ICam_BossOrbital :
    public ICameraState
{
public:
    explicit ICam_BossOrbital() {};

public:
    virtual void Enter(CCamera* pCamera, _vec3 vPos, _vec3 vDir, float Distance, float fSpeed) override;
    virtual void Update(CCamera* pCamera, CGameObject* pOwner, _float fDeltaTime) override;
    virtual void Exit(CCamera* pCamera, CGameObject* pOwner) override;

protected:

    virtual void Set_eye(CCamera* pCamera, CGameObject* pOwner);
    virtual void Set_Look(CCamera* pCamera, CGameObject* pOwner);
    virtual void Set_Matview(CCamera* pCamera, CGameObject* pOwner);
    virtual void Set_MatProj(CCamera* pCamera, CGameObject* pOwner);

protected:
    virtual void        Free() {};

private:
    void    FirstIntro_CameraAnglepos(const float& fDeltaTime);
    void    SecondIntro_CameraAnglepos(const float& fDeltaTime);

private:
    _bool               m_bfirst_Intro = false; // 첫번째 회전값에서는 0 ~ 90도만큼 회전한다.
    _bool               m_bSecond_Intro = false; // 두번째 회전에서는 0 ~ -90도만큼 회전한다.

    ORBIT               m_tOrbit;

    CGameObject*        m_pPivotObject = nullptr;
    _float              m_fCameraSwitchTime = 0.f;

    _float              m_fMonsterSpawnTime = 0.f;
    _vec3               m_vCameratoPivotDir = {};
    _vec3               m_vMonsterSpawnPos = {};
    uint16_t            m_iMonsterCount = 1;
};

END