#pragma once
#include "CComponent.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CTransform;

class ENGINE_DLL CHoverItem :
    public CComponent
{
private:
    explicit CHoverItem();
    explicit CHoverItem(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CHoverItem(const CHoverItem& rhs);
    virtual ~CHoverItem() { Free(); }

public:

    virtual _int Update_Component(const _float& iTimeDelta);
    virtual void LateUpdate_Component(const _float& fTimeDelta);

public:

    // 아이템 둥실거리게 Owner의 Transfrom 가지고 위아래로 움직이게 하는 로직. 
    HRESULT     Ready_HoverItem();
    void        HoverItem_Initialize();
    void        HoverItemEffect(const _float& fTimeDelta);

    // 아이템 둥실거리는 속도 정하는 함수
    void        Set_HoverOffset(float fDelay) { m_fHoverTimeoffset = fDelay; };  // 시작 타이밍
    void        Set_HoverSpeed(float fSpeed) { m_fSpeed = fSpeed; }

    // 
    void        Set_HoverPhase(float fPhase);   // 둥실 거리는걸 어느 시점에서 스타트 할 것인지.
                                               // Sin으로 치면 0 ~ 1 에서 어떤 지점에서 스타트할건지.

    void        Set_HoverInitialTime(float fTime);  // 시작 시간 값.
    
    // 아이템 둥실거리게 하는 초기 offset 시간 주기.
public:
    virtual CComponent* Clone();
    static  CHoverItem* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void Free();

private:

    CTransform* m_pOwnerTransform;
    _vec3       m_vOriginPos;
    _vec3       m_vMoveOffset{ 0.0f, 0.15f, 0.0f };

    _vec3       m_vUpdir{ 0.f,+1.f,0.f };
    _vec3       m_vDownDir{ 0.f,-1.f,0.f };

    float       m_fSpeed = 0.f;
    float       m_fElapsedtime = 0.f;

    float       m_fHoverTimeoffset; // 둥실 언제부터 시작할래.
    bool        m_bHoverStart = false; // 둥실 시작

    bool        m_bHoverDown = false;
    bool        m_bHoverUp = false;
};

END