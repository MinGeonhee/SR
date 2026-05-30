#pragma once
#include "ICameraState.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL ICam_Zoom :
    public ICameraState
{
public:
    explicit ICam_Zoom() {};

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
    
    ZOOM                m_tZoom;
private:
    _vec3               m_vStartEye{ 0,0,0 };
    _vec3               m_vZoomDir{ 0,0,1 };
    float               m_fTravel = 0.f; // 누적 이동
};

END