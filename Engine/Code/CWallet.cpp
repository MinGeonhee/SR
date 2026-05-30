#include "CWallet.h"
#include "CTransform.h"
#include "CGameObject.h"

CWallet::CWallet()
{

}

CWallet::CWallet(LPDIRECT3DDEVICE9 pGrapahicDev)
	:CComponent(pGrapahicDev)
{

}

CWallet::CWallet(const CWallet& rhs)
	:CComponent(rhs)
{

}


_int CWallet::Update_Component(const _float& fTimeDelta)
{

	return 0;
}

void CWallet::LateUpdate_Component(const _float& fTimeDelta)
{
}

HRESULT CWallet::Ready_PlayerWallet()
{
	m_iWalletMoney = 1000;

	return S_OK;
}

CComponent* CWallet::Clone()
{
	return new CWallet(*this);
}

CWallet* CWallet::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWallet* pWallet = new CWallet(pGraphicDev);

	if (FAILED(pWallet->Ready_PlayerWallet()))
	{
		Safe_Release(pWallet);
		MSG_BOX("HoverItem Create Failed");
		return nullptr;
	}

	return pWallet;
}

void CWallet::Free()
{
	CComponent::Free();
}