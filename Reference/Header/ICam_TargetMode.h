#pragma once
#include "ICameraState.h"

BEGIN(Engine)

class ENGINE_DLL ICam_TargetMode :
    public ICameraState
{
public:
    explicit ICam_TargetMode() {};

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
};

END