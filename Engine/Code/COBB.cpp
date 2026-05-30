#include "COBB.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CCollisionManager.h"

COBB::COBB(LPDIRECT3DDEVICE9 pGraphicDev)
	:CColliderBase(pGraphicDev), m_vHalfSize({ 0.5f, 0.5f, 0.5f }), m_vMin({ 0.5f, 0.5f, 0.5f }),
	m_vMax({ -0.5f, -0.5f, -0.5f })
{
	m_eColliderType = COLLIDER_OBB;
	m_vScale = {1.f,1.f,1.f};
	m_vOffset = { 0.f, 0.f, 0.f };
}

COBB::~COBB()
{

}

_int COBB::Update_Component(const _float& iTimeDelta)
{
	return 0;
}

void COBB::LateUpdate_Component(const _float& fTimeDelta)
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
}

HRESULT COBB::Ready_CCollider()
{
	return S_OK;
}

bool COBB::RayIntersect(const _vec3& rayPos, const _vec3& rayDir, float* outDist, _vec3* outPos)
{
	// 월드에서 OBB의 로컬로 변환
	D3DXMATRIX invWorld;
	D3DXMatrixInverse(&invWorld, nullptr, Get_MyOwner()->Get_Transform()->Get_World());
	
	// 모든걸 OBB 기준좌표계로 변형시킨다.
	_vec3 localRaypos, localRayDir;
	D3DXVec3TransformCoord(&localRaypos, &rayPos, &invWorld);
	D3DXVec3TransformNormal(&localRayDir, &rayDir, &invWorld);

	// 로컬 좌표에서 AABB 검사를 진행한다. (OBB 자체는 로컬에서 AABB이기 때문이라고 하시네요)
	_vec3 vMin = m_vMin;
	_vec3 vMax = m_vMax;

	float tMin = (vMin.x - localRaypos.x) / localRayDir.x;
	float tMax = (vMax.x - localRaypos.x) / localRayDir.x;


	if(tMin > tMax) std::swap(tMin, tMax); // 뭔데 이거

	float tyMin = (vMin.y - localRaypos.y) / localRayDir.y;
	float tyMax = (vMax.y - localRaypos.y) / localRayDir.y;

	if (tyMin > tyMax) std::swap(tyMin, tyMax);

	if((tMin > tyMax) || (tyMin > tMax))
	return false;

	if (tyMin > tMin)
		tMin = tyMin;
	if (tyMax < tMax)
		tMax = tyMax;

	float tzMin = (vMin.z - localRaypos.z) / localRayDir.z;
	float tzMax = (vMax.z - localRaypos.z) / localRayDir.z;

	if (tzMin > tzMax) std::swap(tzMin, tzMax);

	if ((tMin > tzMax) || (tzMin > tMax))
		return false;

	if (tzMin > tMin)
		tMin = tzMin;

	if (tMin < 0.f)
		return false;

	if (outDist)
		*outDist = tMin;

	// 만약에 광선으로 맞은 위치를 알고 싶으면. 함수 매개변수에 추가하기.

	if (outPos)
	{
		_vec3 localHit = localRaypos + localRayDir * tMin;
		D3DXVec3TransformCoord(outPos, &localHit, Get_MyOwner()->Get_Transform()->Get_World());
	}

	return true;

}

_vec3 COBB::Get_HalfSize()
{
	m_vHalfSize.x *= m_vScale.x;
	m_vHalfSize.y *= m_vScale.y;
	m_vHalfSize.z *= m_vScale.z;

	m_vMin.x *= m_vScale.x;
	m_vMin.y *= m_vScale.y;
	m_vMin.z *= m_vScale.z;


	m_vMax.x *= m_vScale.x;
	m_vMax.y *= m_vScale.y;
	m_vMax.z *= m_vScale.z;

	return m_vHalfSize;
}

bool COBB::Collide(CColliderBase* others)
{
	return CCollisionManager::GetInstance()->CallCollision(this, others);
}

void COBB::Free()
{

}


CColliderBase* COBB::Clone()
{
	return new COBB(*this);
}

CColliderBase* COBB::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	COBB* pCollider = new COBB(pGraphicDev);

	if (FAILED(pCollider->Ready_CCollider()))
	{
		Safe_Release(pCollider);
		MSG_BOX("Transform Create Failed");
		return nullptr;
	}

	return pCollider;
}