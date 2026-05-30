#include "CHoverItem.h"
#include "CTransform.h"
#include "CGameObject.h"

CHoverItem::CHoverItem()
{

}

CHoverItem::CHoverItem(LPDIRECT3DDEVICE9 pGrapahicDev)
	:CComponent(pGrapahicDev)
{

}

CHoverItem::CHoverItem(const CHoverItem& rhs)
	:CComponent(rhs)
{

}

void CHoverItem::HoverItem_Initialize()
{
	auto owner = Get_MyOwner();
	if (owner)
		m_pOwnerTransform = owner->Get_Transform();
	m_vOriginPos = *(m_pOwnerTransform->GetInfoPos());
}

_int CHoverItem::Update_Component(const _float& fTimeDelta)
{

	return 0;
}

void CHoverItem::LateUpdate_Component(const _float& fTimeDelta)
{
	m_fElapsedtime += fTimeDelta;

	if (m_fElapsedtime >= m_fHoverTimeoffset && !m_bHoverStart) m_bHoverStart = true;

	if (m_bHoverStart) HoverItemEffect(fTimeDelta);
}

void CHoverItem::HoverItemEffect(const _float& fTimeDelta)
{
	if (!m_pOwnerTransform) return;

	auto OwnerPos = *(m_pOwnerTransform->GetInfoPos());

	auto OffsetDownPos = m_vOriginPos - m_vMoveOffset;
	auto OffsetUpPos = m_vOriginPos + m_vMoveOffset;

	// 초기 방향(둘 다 false면 위로 시작)
	if (!m_bHoverUp && !m_bHoverDown) m_bHoverUp = true;

	// 경계 도달 시 방향 토글
	if (m_bHoverUp && OwnerPos.y >= OffsetUpPos.y) { m_bHoverUp = false; m_bHoverDown = true; }
	if (m_bHoverDown && OwnerPos.y <= OffsetDownPos.y) { m_bHoverDown = false; m_bHoverUp = true; }


	if(m_bHoverDown && !m_bHoverUp)
	m_pOwnerTransform->Move_Pos(&m_vDownDir, m_fSpeed, fTimeDelta);

	else if(!m_bHoverDown && m_bHoverUp)
	m_pOwnerTransform->Move_Pos(&m_vUpdir, m_fSpeed, fTimeDelta);
}

HRESULT CHoverItem::Ready_HoverItem()
{
	return S_OK;
}

CComponent* CHoverItem::Clone()
{
	return new CHoverItem(*this);
}

CHoverItem* CHoverItem::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHoverItem* pHover = new CHoverItem(pGraphicDev);

	if (FAILED(pHover->Ready_HoverItem()))
	{
		Safe_Release(pHover);
		MSG_BOX("HoverItem Create Failed");
		return nullptr;
	}

	return pHover;
}

void CHoverItem::Free()
{
	CComponent::Free();
}