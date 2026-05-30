#pragma once
#include "CColliderBase.h"

BEGIN(Engine)

class ENGINE_DLL COBB 
	: public CColliderBase
{
private:
    explicit COBB(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~COBB();

public:

    virtual _int Update_Component(const _float& iTimeDelta) override;
    virtual void LateUpdate_Component(const _float& iTimeDelta) override;

public:

    virtual bool         RayIntersect(const _vec3& rayPos, const _vec3& rayDir, float* outDist, _vec3* outPos)override;
    virtual bool         Collide(CColliderBase* others);

    _vec3               Get_HalfSize();
    _vec3               Get_Min() { return (m_vWorldMin); }
    _vec3               Get_Max() { return (m_vWorldMax); }

    virtual void        Free();

public:

    virtual HRESULT          Ready_CCollider();
    virtual CColliderBase*   Clone();
    static  CColliderBase*   Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:

    // OBB 충돌을 할 때 큐브의 x,y,z 축의 절반길이를 알아야한다.
    // SAT를 할 때 각 축으로 투영을 할 때 필요하다.
    // 우리는 전부 1,1,1로 해둘거니까 스케일 조정 못하게 막자.
    _vec3               m_vWorldMin{};
    _vec3               m_vWorldMax{};

    _vec3               m_vHalfSize;
    _vec3               m_vMin;
    _vec3               m_vMax;
};

END