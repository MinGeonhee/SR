#include "CMapToolMapObject.h"
#include "CProtoTypeManager.h"


CMapToolMapObject::CMapToolMapObject(LPDIRECT3DDEVICE9 Device)
	: CMapObject(Device) , m_tSpriteRect() ,  m_eEventPath(EVENT_PATH::UNKNOWN)
{
	m_tHandle.m_eType = OBJ_NONE;
}

CMapToolMapObject::CMapToolMapObject(const CMapToolMapObject& cpy)
	: CMapObject(cpy) , m_tSpriteRect(cpy.m_tSpriteRect) , m_eEventPath(cpy.m_eEventPath)
{
	m_tHandle.m_eType = OBJ_NONE;
}

CMapToolMapObject::~CMapToolMapObject()
{
}



void CMapToolMapObject::Setting(const ObjectHandle& tHandle, const _vec3& vPos, const _vec3& vRot, const _vec3 vScale,
	const MAPOBJECT_CATEGORY& eCat, const MAPOBJECT_PLACEMENT& ePlacement, const SpriteRect& tSpriteRect)
{

	m_tSpriteRect = tSpriteRect;


	this->CMapObject::Setting(tHandle, vPos, vRot, vScale, eCat, ePlacement, tSpriteRect);
	
}

HRESULT CMapToolMapObject::Add_Component()
{
	if (FAILED(CMapObject::Add_Component()))
		return E_FAIL;

	CComponent* newCom = nullptr;

	newCom = nullptr;
	newCom = m_pColliderCom = CLONE_PROTOTYPE(PROTO_COLLIDER_AAABBB, CAAABBB);
	m_mapComponent->insert({ L"Com_BCollider" , newCom });

	m_pColliderCom->Set_MyOwner(this);


	return S_OK;
}









