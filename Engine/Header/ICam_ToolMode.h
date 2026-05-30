#pragma once
#include "ICameraState.h"



class ICam_ToolMode :
    public ICameraState
{   // Owner -> 타겟이다
    // 카메라 자기 자신의 CCamera컴포넌트
    // ICameraState을(를) 통해 상속됨
    virtual void Enter(CCamera* pCamera, _vec3 vPos, _vec3 vDir, float Distance, float fSpeed) override;
    void Update(CCamera* pCamera, CGameObject* pOwner, float fDeltaTime) override;
    void Exit(CCamera* pCamera, CGameObject* pOwner) override;
    void Set_eye(CCamera* pCamera, CGameObject* pOwner) override;
    void Set_Look(CCamera* pCamera, CGameObject* pOwner) override;
    void Set_Matview(CCamera* pCamera, CGameObject* pOwner) override;
    void Set_MatProj(CCamera* pCamera, CGameObject* pOwner) override;
};

