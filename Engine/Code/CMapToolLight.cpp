#include "CMapToolLight.h"
#include "CResourceManager.h"
#include "CMapObjectTex.h"
#include "CTransform.h"



CMapToolLight::CMapToolLight(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMapToolMapObject(pGraphicDev), m_eLight_Object_Type(LIGHT_OBJECT_TYPE::NONE)
{
	m_eCategory = MAPOBJECT_CATEGORY::DOOR;
	m_tHandle.m_eType = OBJTYPE::OBJ_LIGHT;
	m_ePlacement = MAPOBJECT_PLACEMENT::GROUND;
}

CMapToolLight::CMapToolLight(const CMapToolLight& rhs)
	:CMapToolMapObject(rhs), m_eLight_Object_Type(rhs.m_eLight_Object_Type)
{
	m_eCategory = MAPOBJECT_CATEGORY::DOOR;
	m_tHandle.m_eType = OBJTYPE::OBJ_LIGHT;
	m_ePlacement = MAPOBJECT_PLACEMENT::GROUND;
}

CMapToolLight::~CMapToolLight()
{
}

_int CMapToolLight::Update_GameObject(const _float& fTimeDelta)
{
	if (m_IsEvent == true)
		CGameObject::Update_GameObject(fTimeDelta);

	return 0;
}

void CMapToolLight::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (m_IsEvent == true)
		CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CMapToolLight::Render_GameObject()
{
	m_pTextureCom->Set_Texture();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
	m_pBuffer->Render_Buffer();
}

HRESULT CMapToolLight::Ready_GameObject()
{
	if (FAILED(CMapToolMapObject::Add_Component()))
		return E_FAIL;


	m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"Light");

	return S_OK;
}

HRESULT	CMapToolLight::Add_Component()
{
	if (FAILED(CMapToolMapObject::Add_Component()))
		return E_FAIL;
}


CMapToolLight* CMapToolLight::Create(LPDIRECT3DDEVICE9 Device)
{
	CMapToolLight* pMapItem = new CMapToolLight(Device);

	if (FAILED(pMapItem->Ready_GameObject()))
	{
		MSG_BOX("CMapToolItem Create Is Failed");
		Safe_Release(pMapItem);
		return nullptr;
	}

	return pMapItem;
}

void	CMapToolLight::Free()
{
	CGameObject::Free();
}