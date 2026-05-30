#include "CMapToolDoor.h"
#include "CResourceManager.h"
#include "CMapObjectTex.h"
#include "CTransform.h"



CMapToolDoor::CMapToolDoor(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMapToolMapObject(pGraphicDev), m_eDoorType(DOORTYPE::NONE_DOOR) , m_iDoorSwitchCount(1)
{
	m_eCategory = MAPOBJECT_CATEGORY::DOOR;
	m_tHandle.m_eType = OBJTYPE::OBJ_DOOR;
	m_ePlacement = MAPOBJECT_PLACEMENT::GROUND;
}

CMapToolDoor::CMapToolDoor(const CMapToolDoor& rhs)
	:CMapToolMapObject(rhs), m_eDoorType(rhs.m_eDoorType) , m_iDoorSwitchCount(rhs.m_iDoorSwitchCount)
{
	m_eCategory = MAPOBJECT_CATEGORY::DOOR;
	m_tHandle.m_eType = OBJTYPE::OBJ_DOOR;
	m_ePlacement = MAPOBJECT_PLACEMENT::GROUND;
}

CMapToolDoor::~CMapToolDoor()
{
}

_int CMapToolDoor::Update_GameObject(const _float& fTimeDelta)
{
	if (m_IsEvent == true)
		CGameObject::Update_GameObject(fTimeDelta);

	return 0;
}

void CMapToolDoor::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (m_IsEvent == true)
		CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CMapToolDoor::Render_GameObject()
{
	m_pTextureCom->Set_Texture();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
	m_pBuffer->Render_Buffer();
}

HRESULT CMapToolDoor::Ready_GameObject()
{
	if (FAILED(CMapToolMapObject::Add_Component()))
		return E_FAIL;


	m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"Door");

	return S_OK;
}

HRESULT	CMapToolDoor::Add_Component()
{
	if (FAILED(CMapToolMapObject::Add_Component()))
		return E_FAIL;
}


CMapToolDoor* CMapToolDoor::Create(LPDIRECT3DDEVICE9 Device)
{
	CMapToolDoor* pMapItem = new CMapToolDoor(Device);

	if (FAILED(pMapItem->Ready_GameObject()))
	{
		MSG_BOX("CMapToolItem Create Is Failed");
		Safe_Release(pMapItem);
		return nullptr;
	}

	return pMapItem;
}

void	CMapToolDoor::Free()
{
	CGameObject::Free();
}