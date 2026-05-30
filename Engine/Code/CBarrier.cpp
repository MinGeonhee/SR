#include "CBarrier.h"
#include "CTransform.h"
#include "CGameObject.h"
#include "CPrototypeManager.h"
#include "CResourceManager.h"
#include "CRendererManager.h"
#include "CCameraManager.h"

CBarrier::CBarrier()
{

}

CBarrier::CBarrier(LPDIRECT3DDEVICE9 pGrapahicDev)
	:CComponent(pGrapahicDev)
{

}

CBarrier::CBarrier(const CBarrier& rhs)
	:CComponent(rhs)
{

}

HRESULT CBarrier::Ready_Barrier()
{
	return S_OK;
}


_int CBarrier::Update_Component(const _float& fTimeDelta)
{
	return 0;
}

void CBarrier::LateUpdate_Component(const _float& fTimeDelta)
{
}



_vec3 CBarrier::Get_ReflectDir(const _vec3& bulletpos, const _vec3& bulletDir)
{
	return -bulletDir;

	_vec3 ownerpos;
	auto ownertrans = Get_MyOwner()->Get_Transform();
	if (ownertrans)  ownerpos = *ownertrans->GetInfoPos();

	_vec3 dotvector;
	dotvector = bulletpos - ownerpos; D3DXVec3Normalize(&dotvector, &dotvector);

	// 반사벡터 공식 R = P + 2*n(-p * n); 여기서 n은 법선 벡터

	_vec3 reflectvector;
	reflectvector = bulletDir + 2 * dotvector * (-D3DXVec3Dot(&bulletDir, &dotvector));
	return reflectvector;
}

CComponent* CBarrier::Clone()
{
	return new CBarrier(*this);
}

CBarrier* CBarrier::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBarrier* pBarrier = new CBarrier(pGraphicDev);

	if (FAILED(pBarrier->Ready_Barrier()))
	{
		Safe_Release(pBarrier);
		MSG_BOX("HoverItem Create Failed");
		return nullptr;
	}

	return pBarrier;
}


void CBarrier::Free()
{
	CComponent::Free();
}