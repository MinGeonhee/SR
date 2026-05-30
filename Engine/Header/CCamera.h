#pragma once
#include "CComponent.h"

BEGIN(Engine)

class CTransform;
class ICameraState;

class ENGINE_DLL CCamera :
    public CComponent
{
private:
    explicit CCamera();
    explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CCamera(const CCamera& rhs);
    virtual ~CCamera();

public:
    HRESULT         Ready_Camera();
    virtual _int    Update_Component(const _float& fTimeDelta) override;
    virtual void    LateUpdate_Component(const _float& fTimeDelta) override;
    void            Camera_Inintialize();

public:

    void            Set_Target(CGameObject* pObject) { m_pTarget = pObject; }
    void            Set_TargetTransform(CTransform* ptransform) { m_pTargetTransform = ptransform; }
    void            Set_MyTransform(CTransform* pMyTransform) { m_pTransform = pMyTransform; }

    // 특정 UI가 뜨면 마우스 가운데로 보정하던 것을 못하게 막기 위함.

    void            Set_MouseFixLock(bool a) { m_bMouseFixLock = a; }
    // iCameraState에서 처리한 View 행렬과 Proj행렬들을 전부 가져오기 위함.
    // 1인칭 시점과 3인칭 시점이 이상하면 안되니까.

    void           Set_CameraState(CAMERASTATE eState, _vec3 vPOs = {}, _vec3 vDir = {}, float duration = 0, float power = 0);
    void            Set_ViewMatrix(_matrix pview) { m_ViewMatrix = pview; }
    void            Set_ProjMatrix(_matrix pProj) { m_ProjMatrix = pProj; }

    CAMERASTATE     Get_CameraState() { return m_eCamState; }
    _matrix&        Get_ViewMatrix() { return m_ViewMatrix; }
    _matrix&        Get_ProjMatrix() { return m_ProjMatrix; }
    CTransform*     Get_Transform() { return m_pTransform; }

public:
    virtual CComponent* Clone();
    static  CCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void Free();

private:

    CGameObject*                        m_pTarget;
    CTransform*                         m_pTargetTransform;           // 내 카메라가 따라다닐 대상 Transform.
    CTransform*                         m_pTransform;                // 내 카메라 객체 transform. 
    ICameraState*                       m_pCurState;                 // 현재 내 상태에 따른 함수 객체.
    CAMERASTATE                         m_eCamState = CAM_NONE;
    map<CAMERASTATE, ICameraState*>     m_mCameraState;              // 함수 객체 저장 시킬 놈.

    _matrix                             m_ViewMatrix;
    _matrix                             m_ProjMatrix;

    bool                                m_bMouseFixLock = false;
};

END