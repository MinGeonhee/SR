#pragma once
#include "ICameraState.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL ICam_OrbitalCam :
    public ICameraState
{
public:
    explicit ICam_OrbitalCam() {};

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
    void    Camera_Anglepos(const float& fDeltaTime);

private:

    ORBIT               m_tOrbit;

    CGameObject*        m_pPivotObject = nullptr;
    _vec3               m_vCameratoPivotDir = {};

};

END