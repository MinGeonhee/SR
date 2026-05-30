#include "CComponent.h"

CComponent::CComponent()
	:m_pGraphicDev(nullptr), m_bClone(false)
{
}

CComponent::CComponent(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev), m_bClone(false)
{
	m_pGraphicDev->AddRef();
	// 참조 카운트를 한개 늘린다.
}

CComponent::CComponent(const CComponent& rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev), m_bClone(true)
{
	m_pGraphicDev->AddRef();
	// 참조 카운트를 한개 늘린다.
}

CComponent::~CComponent()
{
}

void CComponent::Free()
{
	Safe_Release(m_pGraphicDev);
	// 컴포넌트를 삭제할 때 컴 객체 
}