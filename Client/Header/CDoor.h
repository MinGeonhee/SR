#pragma once
#include "CGameObject.h"
#include "CProtoTypeManager.h"

class CDoor :
    public CGameObject
{
private:
    explicit CDoor(const CDoor& rhs);
    explicit CDoor(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CDoor();

    virtual void  Free();

public:

    virtual     HRESULT     Ready_GameObject();
    virtual     _int        Update_GameObject(const _float& fTimeDelta);
    virtual     void        LateUpdate_GameObject(const _float& fTimeDelta);
    virtual     void        Render_GameObject();

public:

    virtual void            Set_SubType(void* SubType);

private:
    // 프로토타입에게서 컴포넌트 복사본을 받아서 내꺼에 저장해줘야겠지.
    HRESULT                 Add_Component();
    void                    Move_Camera(const _float& fTimeDelta);
    void                    Change_CameraState();
    bool                    Set_Animation();
    void                    Set_Texture();
    
public:


public:
    static CDoor* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:

    Engine::CDoor_Component*     m_pDoorCom;
    Engine::CAnimation*          m_pAnimationCom;
    CTexture*                    m_pTexture = nullptr;

private:

    CGameObject*                 m_pTarget = nullptr;
    float                        m_fSpeed = 0.f;
    _bool                        m_bDoorOpen = false;
    DOORSTATE                    m_ePrevState = DOORSTATE::DOOR_NONE;
    DOORSTATE                    m_eCurState = DOORSTATE::DOOR_NONE;
    DOORTYPE                     m_eDoorType = DOORTYPE::NONE_DOOR;
};

