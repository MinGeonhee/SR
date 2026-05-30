#pragma once
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CColliderBase :
    public CComponent
{
protected:
    explicit CColliderBase(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CColliderBase();

public:

    // 순수 가상함수로 둔다. 
    // CColiiderBase를 상속받는 자식 클래스인 
    // AABB, OBB, SPHERE, 레이저 스펠링 뭐냐. 아무튼 다 받도록.

    virtual bool                RayIntersect(const _vec3& rayPos, const _vec3& rayDir, float* outDist, _vec3* outPos) = 0;
    virtual bool                Collide(CColliderBase* other) { return false; }
    virtual COLLIDERTYPE        Get_ColliderType() { return m_eColliderType; }
    virtual void                Set_ColliderType(COLLIDERTYPE type) { m_eColliderType = type; }

    virtual void                Set_ColliderScale(const _vec3& a) { m_vScale = a; }
    virtual void                Set_Collide_CenterPos(_vec3 vpos) { m_vPos = vpos; }
    virtual void                Set_Offset(_vec3 offset) { m_vOffset = offset; }
    virtual _vec3               Get_Collide_Center() { return m_vPos; }

public:



    virtual HRESULT             Ready_CCollider();

private:
    virtual void                Free() override;

protected:

    _vec3               m_vScale;
    _vec3               m_vPos;
    _vec3               m_vOwnerPos;
    _vec3               m_vOffset;
    COLLIDERTYPE        m_eColliderType;
};

END

