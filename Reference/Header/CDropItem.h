#pragma once
#include "CComponent.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CTransform;

class ENGINE_DLL CDropItem :
    public CComponent
{
private:
    explicit CDropItem();
    explicit CDropItem(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CDropItem(const CDropItem& rhs);
    virtual ~CDropItem() { Free(); }

public:
    HRESULT     Ready_DropItem();
    virtual _int Update_Component(const _float& iTimeDelta);
    virtual void LateUpdate_Component(const _float& fTimeDelta);
 
public:
    virtual CComponent* Clone();
    static  CDropItem* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void Free();

private:
    CTransform* m_pTargetTransform;
public:
    void        Set_TargetTransform(CTransform* _t) { m_pTargetTransform = _t; };

private:
    float       m_fDropTimer = 0.f;
private:
    bool        m_bIsDropped = false;
    bool        m_bIsOnFloor = false;
    _vec3       m_vDir_Drop = { 0.f, 0.f, 0.f };
    float       m_fSpeed_XZ_Drop = 2.f;
    float       m_fSpeed_Y_Drop = 9.f;
    float       m_fGravity_Drop = 9.8f;
    float       m_fSpeed = 2.5f;

public:
    void        Drop_Update(const _float& fTimeDelta);
};

END