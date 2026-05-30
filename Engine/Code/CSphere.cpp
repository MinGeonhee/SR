#include "CSphere.h"
#include "CCollisionManager.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CDebugLineHelper.h"

CSphere::CSphere(LPDIRECT3DDEVICE9 pGraphicDev)
	:CColliderBase(pGraphicDev), m_fRadius(0.5f)
{
	m_eColliderType = COLLIDER_SPHERE;
	m_vScale = { 1.f,1.f,1.f };
	m_vOffset = { 0.f, 0.f, 0.f };
}

CSphere::~CSphere()
{

}

_int CSphere::Update_Component(const _float& iTimeDelta)
{
	// 디버그 박스 그리기

	return 0;
}

void CSphere::LateUpdate_Component(const _float& iTimeDelta)
{
	ForceSyncFromOwner();
	CDebugLineHelper::GetInstance()->DrawAABBBox(m_vWorldMin, m_vWorldMax, { 1,0,0,1 });
}

HRESULT CSphere::Ready_CCollider()
{

	return S_OK;
}


float CSphere::Get_Radius()
{
	float sx = m_vScale.x, sy = m_vScale.y, sz = m_vScale.z;

	// 만약 음수 스케일(미러링) 가능성이 있으면 주석 해제
	// sx = fabsf(sx); sy = fabsf(sy); sz = fabsf(sz);

	float s = sx;
	if (sy > s) s = sy;
	if (sz > s) s = sz;

	return s * m_fRadius;
}

bool CSphere::RayIntersect(const _vec3& rayPos, const _vec3& rayDir, float* outDist, _vec3* outPos)
{
	_vec3 sphereCenter = m_vPos;
	float radius = Get_Radius(); //

	_vec3 m = rayPos - sphereCenter;
	float b = D3DXVec3Dot(&m, &rayDir);
	float c = D3DXVec3Dot(&m, &m) - radius * radius;

	if (c > 0.f && b > 0.f) return false;
	float discr = b * b - c;
	if (discr < 0.f) return false;

	float t = -b - sqrtf(discr);
	if (t < 0.f) t = 0.f;

	if (outDist) *outDist = t;
	if (outPos)  *outPos = rayPos + rayDir * t;
	return true;
}

bool CSphere::Collide(CColliderBase* others)
{
	return CCollisionManager::GetInstance()->CallCollision(this, others);
}

void CSphere::Free()
{

}

void CSphere::ForceSyncFromOwner()
{
	auto* tr = Get_MyOwner()->Get_Transform();

	_vec3 m_vRight, m_vUp, m_vLook;

	m_vOwnerPos = *(Get_MyOwner()->Get_Transform()->GetInfoPos());
	tr->Get_Info(INFO_RIGHT, &m_vRight);
	tr->Get_Info(INFO_UP, &m_vUp);
	tr->Get_Info(INFO_LOOK, &m_vLook);

	// 축은 단위벡터로 정규화 → 스케일은 따로 곱해줌(중복 스케일 방지)
	D3DXVec3Normalize(&m_vRight, &m_vRight);
	D3DXVec3Normalize(&m_vUp, &m_vUp);
	D3DXVec3Normalize(&m_vLook, &m_vLook);

	_vec3 worldOffset =
		m_vRight * (m_vOffset.x) +
		m_vUp * (m_vOffset.y) +
		m_vLook * (m_vOffset.z);
	 
	m_vPos = m_vOwnerPos + worldOffset; 

	// 스피어 디버그용 AABB는 "반지름 r"로 생성
	float r = Get_Radius();
	_vec3 ext{ r, r, r };
	m_vWorldMin = m_vPos - ext;
	m_vWorldMax = m_vPos + ext;
}


CColliderBase* CSphere::Clone()
{
	return new CSphere(*this);
}

CColliderBase* CSphere::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CColliderBase* pCollider = new CSphere(pGraphicDev);

	if (FAILED(pCollider->Ready_CCollider()))
	{
		Safe_Release(pCollider);
		MSG_BOX("Transform Create Failed");
		return nullptr;
	}

	return pCollider;
}