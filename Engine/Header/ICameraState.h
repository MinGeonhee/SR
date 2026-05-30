#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

typedef struct OrbitState
{
    _vec3   Pivot{ 0.f, 0.f, 0.f };
    float   radius = 0.f;
    float   yaw = 180.f;
    float   pitch = 0.f;
    float   AngVel = 1.f; // 각속도
    _vec3   Worldup{ 0.f, 1.f, 0.f };
}ORBIT;

typedef struct ZoomMode
{
    _vec3      vPos = { 0.f, 0.f, 0.f };    // 전에 있던 카메라 위치에서부터 Zoom 시작.
    _float     fSpeed = 0.f;
    _float     fDistance = 0.f;
}ZOOM;

class CGameObject;
class CCamera;

class ENGINE_DLL ICameraState :
    public CBase
{
public:
    explicit ICameraState() {};

public:
    virtual void Enter(CCamera* pCamera, _vec3 vPos = {}, _vec3 vDir = {}, float fShakeDuration = 0, float fPower = 0) = 0;
    virtual void Update(CCamera* pCamera, CGameObject* pOwner, float fDeltaTime) = 0;
    virtual void Exit(CCamera* pCamera, CGameObject* pOwner) = 0;
    virtual ~ICameraState() {}

protected:

    virtual void Set_eye(CCamera* pCamera, CGameObject* pOwner) = 0;
    virtual void Set_Look(CCamera* pCamera, CGameObject* pOwner) = 0;
    virtual void Set_Matview(CCamera* pCamera, CGameObject* pOwner) = 0;
    virtual void Set_MatProj(CCamera* pCamera, CGameObject* pOwner) = 0;

protected:

    _vec3           offset;
    _vec3	        vEye, vLook, vAt, vWorldUp;

protected:

    virtual void        Free() {};
 };

END