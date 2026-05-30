#include "CAAABBB.h"
#include "CCollisionManager.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CDebugLineHelper.h"

CAAABBB::CAAABBB(LPDIRECT3DDEVICE9 pGraphicDev)
    :CColliderBase(pGraphicDev)
{
    m_eColliderType = COLLIDER_AAABBB;
    m_vScale = { 1.f,1.f,1.f };
    m_vOffset = { 0.f, 0.f, 0.f };
}

CAAABBB::~CAAABBB()
{

}

_int CAAABBB::Update_Component(const _float& iTimeDelta)
{
	return 0;
}

void CAAABBB::LateUpdate_Component(const _float& iTimeDelta)
{
    m_vOwnerPos = *(Get_MyOwner()->Get_Transform()->GetInfoPos());

    m_vPos = m_vOwnerPos + m_vOffset;

    m_vWorldMin.x = (m_vPos.x + m_vMin.x * m_vScale.x);
    m_vWorldMin.y = (m_vPos.y + m_vMin.y * m_vScale.y);
    m_vWorldMin.z = (m_vPos.z + m_vMin.z * m_vScale.z);

    m_vWorldMax.x = (m_vPos.x + m_vMax.x * m_vScale.x);
    m_vWorldMax.y = (m_vPos.y + m_vMax.y * m_vScale.y);
    m_vWorldMax.z = (m_vPos.z + m_vMax.z * m_vScale.z);

    CDebugLineHelper::GetInstance()->DrawAABBBox(m_vWorldMin, m_vWorldMax, { 1, 0, 0, 1 });
}

HRESULT CAAABBB::Ready_CCollider()
{

    return S_OK;
}

bool CAAABBB::RayIntersect(const _vec3& rayPos, const _vec3& rayDir, float* outDist, _vec3* outPos)
{
    _vec3 boxMin = m_vWorldMin; // 로컬 기준 또는 world 기준에 따라 다름
    _vec3 boxMax = m_vWorldMax;

    float tMin = (boxMin.x - rayPos.x) / rayDir.x;
    float tMax = (boxMax.x - rayPos.x) / rayDir.x;

    if (tMin > tMax) std::swap(tMin, tMax);

    float tyMin = (boxMin.y - rayPos.y) / rayDir.y;
    float tyMax = (boxMax.y - rayPos.y) / rayDir.y;

    if (tyMin > tyMax) std::swap(tyMin, tyMax);

    if ((tMin > tyMax) || (tyMin > tMax))
        return false;

    if (tyMin > tMin)
        tMin = tyMin;
    if (tyMax < tMax) tMax = tyMax;

    float tzMin = (boxMin.z - rayPos.z) / rayDir.z;
    float tzMax = (boxMax.z - rayPos.z) / rayDir.z;

    if (tzMin > tzMax) std::swap(tzMin, tzMax);

    if ((tMin > tzMax) || (tzMin > tMax))
        return false;

    if (tzMin > tMin) tMin = tzMin;
    if (tzMax < tMax) tMax = tzMax;

    if (tMin < 0.f)
        return false;

    if (outDist)
        *outDist = tMin;

    // 만약에 광선에 맞은 자리에서 Effect가 나오게 하고 싶다! 해당 좌표 가져가기.

    if (outPos)
        *outPos = rayPos + rayDir * tMin;


    return true;
}

bool CAAABBB::Collide(CColliderBase* others)
{
	return CCollisionManager::GetInstance()->CallCollision(this, others);
}


void CAAABBB::Free()
{

}



CColliderBase* CAAABBB::Clone()
{
    return new CAAABBB(*this);
}

CColliderBase* CAAABBB::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CAAABBB* pCollider = new CAAABBB(pGraphicDev);

    if (FAILED(pCollider->Ready_CCollider()))
    {
        Safe_Release(pCollider);
        MSG_BOX("Transform Create Failed");
        return nullptr;
    }

    return pCollider;
}