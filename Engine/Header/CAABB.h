#pragma once
#include "CColliderBase.h"

BEGIN(Engine)

class ENGINE_DLL CAABB :
    public CColliderBase
{
private:
    explicit CAABB(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CAABB();

public:

    virtual _int Update_Component(const _float& iTimeDelta) override;
    virtual void LateUpdate_Component(const _float& iTimeDelta) override;

public:
    virtual bool         RayIntersect(const _vec3& rayPos, const _vec3& rayDir, float* outDist, _vec3* outPos) override;
    virtual bool         Collide(CColliderBase* others) override;

    // 큐브에서 가장 작은 값 -0.5 -0.5 -0.5
    // 큐브에서 가장 큰 값 +0.5 +0.5 +0.5

    _vec3               Get_Min() { return (m_vWorldMin); }
    _vec3               Get_Max() { return (m_vWorldMax); }

public:
    virtual HRESULT     Ready_CCollider() override;
    virtual void        Free();

public:

    virtual CColliderBase* Clone();
    static  CColliderBase* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:

    _vec3               m_vWorldMin{};
    _vec3               m_vWorldMax{};

    _vec3               m_vMin = { -0.5f, 0.f, -0.5f };
    _vec3               m_vMax = { +0.5f, +0.f, +0.5f };
};

END