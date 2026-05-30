#pragma once
#include "CGameObject.h"
#include "CProtoTypeManager.h"

class CObjectCamera :
    public CGameObject
{
private:
    explicit CObjectCamera(const CObjectCamera& rhs);
    explicit CObjectCamera(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CObjectCamera();

    virtual void  Free();

public:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

private:
    // 프로토타입에게서 컴포넌트 복사본을 받아서 내꺼에 저장해줘야겠지.
    HRESULT                 Add_Component();
    void                    Move_Camera(const _float& fTimeDelta);
    void                    Change_CameraState();

public:
    static CObjectCamera*  Create(LPDIRECT3DDEVICE9 pGraphicDev);
    CCamera*               Get_Camera() { return m_pCameraCom; }

private:

    Engine::CCamera*        m_pCameraCom;

private:

    CGameObject*            m_pTarget = nullptr;
    float                   m_fSpeed = 0.f;
};

