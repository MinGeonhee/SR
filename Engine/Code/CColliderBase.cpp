#include "CColliderBase.h"

CColliderBase::CColliderBase(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
{

}

CColliderBase::~CColliderBase()
{

}

void CColliderBase::Free()
{
	CComponent::Free();
}

HRESULT CColliderBase::Ready_CCollider()
{

	return S_OK;
}


