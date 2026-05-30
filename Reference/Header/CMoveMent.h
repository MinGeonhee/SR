#pragma once
#include "CComponent.h"
#include "CTransform.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CMoveMent :
    public CComponent
{
private:
    enum MOVEMENTSTATE
    {
        MOVE_NONE,
        MOVE_STRAIGHT,
        MOVE_CURVE,
        MOVE_PATROL,
        MOVE_CHASE,
        MOVE_JUMP,
        MOVE_FLY,
        MOVE_TELEPORT,

        MOVE_END
    };

private:
    explicit CMoveMent();
    explicit CMoveMent(const CMoveMent& rhs);
    explicit CMoveMent(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CMoveMent();

public:
    HRESULT         Ready_MoveMent();
    virtual _int    Update_Component(const _float& fTimeDelta) override;
    virtual void    LateUpdate_Component(const _float& iTimeDelta) override;

public:
    virtual CComponent* Clone();
    static  CMoveMent* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:

    void            Set_MoveState(MOVEMENTSTATE _MoveState) { m_eMoveMentState = _MoveState; }
    void            Set_TargetTransform(CComponent* mTarget) { m_pTargetTranform = static_cast<CTransform*>(mTarget); }
    void            Set_Speed(_float fSpeed) { m_fSpeed = fSpeed; }
    
    CTransform*     Get_Transform() { return m_pTransform; }

private:

    void            Move(const _float& fTimeDelta);
    virtual void    Free() override;

private:

    CTransform*     m_pTransform = nullptr;     // 미리 캐싱해둔다.
    CTransform*     m_pTargetTranform = nullptr;
    // 움직임 상태값에 따라 움직임 변화 주려고 만듬.
    MOVEMENTSTATE   m_eMoveMentState = MOVEMENTSTATE::MOVE_NONE;

    _float          m_fSpeed = 0.f;

};

END
