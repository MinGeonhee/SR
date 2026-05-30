#pragma once
#include "CColliderBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CSphere :
    public CColliderBase
{
private:
    explicit CSphere(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CSphere();

public:
    virtual bool RayIntersect(const _vec3& rayPos, const _vec3& rayDir, float* outDist, _vec3* outPos)override;
    virtual _int Update_Component(const _float& iTimeDelta) override;
    virtual void LateUpdate_Component(const _float& iTimeDelta) override;

public:

    virtual HRESULT      Ready_CCollider() override;
    virtual bool         Collide(CColliderBase* others) override;
    
    float               Get_Radius() ;

    void                ForceSyncFromOwner();
public:

    void Reset() {
        m_vOwnerPos = { 0.f, -100.f, 0.f };
        m_vPos = m_vOwnerPos;
        m_vScale = { 1.f,1.f,1.f };
        m_vWorldMin = m_vPos;
        m_vWorldMax = m_vPos;
    }

    virtual void         Free() override;

    virtual CColliderBase* Clone() override;
    static CColliderBase* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:

    _vec3               m_vWorldMin{};
    _vec3               m_vWorldMax{};

    _vec3               m_vMin = { -0.5f, -0.0f, -0.5f };
    _vec3               m_vMax = { +0.5f, +0.0f, +0.5f };

    float               m_fRadius = 0.5;
};
END
