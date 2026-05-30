#include "CResourceBase.h"

CResourceBase::CResourceBase(RESOURCETYPE _type, LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev), m_eType(_type), m_bClone(false)
{
}

CResourceBase::CResourceBase(const CResourceBase& rhs)
	:m_eType(rhs.m_eType), m_bClone(true)
{

};


CResourceBase::~CResourceBase()
{

}

void CResourceBase::Free()
{
	CComponent::Free();
}

