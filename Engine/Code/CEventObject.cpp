#include "CEventObject.h"


CEventObject::CEventObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMapObject(pGraphicDev)
{
	m_tHandle.m_eType = OBJ_MAPEVENT;
	m_eCategory = MAPOBJECT_CATEGORY::EVENT;
	m_ePlacement = MAPOBJECT_PLACEMENT::_END;
	m_eObject_Event_Type = OBJECT_EVENT_TYPE::NONE;
}

CEventObject::CEventObject(const CEventObject& rhs)
	: CMapObject(rhs) , m_eObject_Event_Type(rhs.m_eObject_Event_Type)
{

}

CEventObject::~CEventObject()
{
}
