#include "CAABB.h"
#include "CCollisionManager.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CDebugLineHelper.h"

CAABB::CAABB(LPDIRECT3DDEVICE9 pGraphicDev)
    :CColliderBase(pGraphicDev)
{
    m_eColliderType = COLLIDER_AABB;
    m_vScale = {1.f,1.f,1.f};
    m_vOffset = { 0.f, 0.f, 0.f };
}

CAABB::~CAABB()
{

}

_int CAABB::Update_Component(const _float& iTimeDelta)
{


    return 0;
}

void CAABB::LateUpdate_Component(const _float& iTimeDelta)
{

    m_vOwnerPos = *(Get_MyOwner()->Get_Transform()->GetInfoPos());
    m_vScale = Get_MyOwner()->Get_Transform()->Get_Scale();

    m_vPos = m_vOwnerPos + m_vOffset;

    m_vWorldMin.x = (m_vPos.x + m_vMin.x * m_vScale.x);
    m_vWorldMin.y = (m_vPos.y + m_vMin.y * m_vScale.y);
    m_vWorldMin.z = (m_vPos.z + m_vMin.z * m_vScale.z);

    m_vWorldMax.x = (m_vPos.x + m_vMax.x * m_vScale.x);
    m_vWorldMax.y = (m_vPos.y + m_vMax.y * m_vScale.y);
    m_vWorldMax.z = (m_vPos.z + m_vMax.z * m_vScale.z);

    //CDebugLineHelper::GetInstance()->DrawAABBTile(m_vWorldMin, m_vWorldMax, { 1, 0, 0, 1 });
 
}

HRESULT CAABB::Ready_CCollider()
{
    return S_OK;
}

bool CAABB::RayIntersect(const _vec3& rayPos, const _vec3& rayDir, float* outDist, _vec3* outPos)
{
    // dir 정규화(거리값을 '실제 거리'로 쓰고 싶다면)
    _vec3 dir = rayDir;
    float len = D3DXVec3Length(&dir);

    if (len <= 1e-8f)
        return false;

    dir /= len;

    float tmin = -FLT_MAX, tmax = FLT_MAX;

    auto slab = [&](float bmin, float bmax, float ro, float rd) -> bool
        {
            if (fabsf(rd) < 1e-8f) {
                // 레이가 이 축으로 평행: 원점이 슬랩 범위 밖이면 교차 없음
                return (ro >= bmin && ro <= bmax);
            }
            else {
                float ood = 1.0f / rd;
                float t0 = (bmin - ro) * ood;
                float t1 = (bmax - ro) * ood;
                if (t0 > t1) std::swap(t0, t1);
                tmin = max(tmin, t0);
                tmax = min(tmax, t1);
                return tmin <= tmax;
            }
        };

    if (!slab(m_vWorldMin.x, m_vWorldMax.x, rayPos.x, dir.x)) 
        return false;

    if (!slab(m_vWorldMin.y, m_vWorldMax.y, rayPos.y, dir.y)) 
        return false;

    if (!slab(m_vWorldMin.z, m_vWorldMax.z, rayPos.z, dir.z)) 
        return false;

    // 가장 앞에 맞는 지점
    float tHit = (tmin >= 0.f) ? tmin : tmax;

    if (tHit < 0.f)
        return false;

    if (outPos)  *outPos = rayPos + dir * tHit;
    if (outDist) *outDist = tHit;          // dir를 정규화했으니 '실제 거리'
    return true;
    

}

bool CAABB::Collide(CColliderBase* others)
{
    return CCollisionManager::GetInstance()->CallCollision(this, others);
}


void CAABB::Free()
{

}

CColliderBase* CAABB::Clone()
{
    return new CAABB(*this);
}

CColliderBase* CAABB::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CAABB* pCollider = new CAABB(pGraphicDev);

    if (FAILED(pCollider->Ready_CCollider()))
    {
        Safe_Release(pCollider);
        MSG_BOX("Transform Create Failed");
        return nullptr;
    }

    return pCollider;
}