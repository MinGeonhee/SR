#pragma once

#include "CGameObject.h"

#include "CCamera.h"



class CCameraMan :
    public CGameObject
{
private:
    explicit CCameraMan(const CCameraMan& rhs);
    explicit CCameraMan(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CCameraMan();

    virtual void  Free();

public:
    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

private:
    // 프로토타입에게서 컴포넌트 복사본을 받아서 내꺼에 저장해줘야겠지.
    HRESULT            Add_Component();
    void                    Move_Camera(const _float& fTimeDelta);
    void                    Change_CameraState();

    void                    Key_Update();

public:
    static CCameraMan* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    CCamera* Get_Camera() { return m_pCameraCom; }
private:
    Engine::CCamera* m_pCameraCom;
private:
    CGameObject* m_pTarget = nullptr;
    float                   m_fSpeed = 0.f;

    _int                    m_iPosYFixedFloor = (int)FLOOR::FLOOR_GROUND;
    _bool                   m_isCameraFixed = false;
};

