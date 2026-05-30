#pragma once
#include "CColliderBase.h"

BEGIN(Engine)


class ENGINE_DLL CAAABBB :
    public CColliderBase
{
private:
    explicit CAAABBB(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CAAABBB();

public:

    virtual _int Update_Component(const _float& iTimeDelta) override;
    virtual void LateUpdate_Component(const _float& iTimeDelta) override;

public:
    virtual bool         RayIntersect(const _vec3& rayPos, const _vec3& rayDir, float* outDist, _vec3* outPos) override;
    virtual bool         Collide(CColliderBase* others) override;

private:
    virtual HRESULT         Ready_CCollider() override;
    // 큐브에서 가장 작은 값 -0.5 -0.5 -0.5
    // 큐브에서 가장 큰 값 +0.5 +0.5 +0.5

public:

    _vec3               Get_Min() { return (m_vWorldMin); }
    _vec3               Get_Max() { return (m_vWorldMax); }

    virtual void                  Free() override;

    virtual CColliderBase*        Clone() override;
    static  CColliderBase*        Create(LPDIRECT3DDEVICE9 pGraphicDev) ;

private:

    _vec3               m_vWorldMin{};
    _vec3               m_vWorldMax{};

    _vec3               m_vMin = {-0.5f, -0.5f, -0.5f};
    _vec3               m_vMax = {+0.5f, +0.5f, +0.5f};
};

END