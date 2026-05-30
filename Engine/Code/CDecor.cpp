#include "CDecor.h"
#include "CProtoTypeManager.h"

CDecor::CDecor(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMapObject(pGraphicDev)
{
	m_tHandle.m_eType = OBJ_MAPDECOR;
	m_eCategory = MAPOBJECT_CATEGORY::DECOR;
}

CDecor::CDecor(const CDecor& rhs)
	: CMapObject(rhs)
{

}

CDecor::~CDecor()
{
}

