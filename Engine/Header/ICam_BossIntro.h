#pragma once
#include "ICameraState.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ICam_BossIntro :
    public ICameraState
{
public:
    explicit ICam_BossIntro() {};

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
    _bool               m_bfirst_Intro = false;
    _bool               m_bSecond_Intro = false;
    _bool               m_bThird_Intro = false;
};

END