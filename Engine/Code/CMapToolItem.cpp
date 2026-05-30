#include "CMapToolItem.h"
#include "CMapObjectTex.h"
#include "CTransform.h"
#include "CResourceManager.h"

CMapToolItem::CMapToolItem(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMapToolMapObject(pGraphicDev)  , m_eItemType(ITEMTYPE::ITEM_NONE)
{
	m_tHandle.m_eType = OBJTYPE::OBJ_ITEM;
	m_eCategory = MAPOBJECT_CATEGORY::ITEM;
	m_ePlacement = MAPOBJECT_PLACEMENT::GROUND;
	m_eEventPath = EVENT_PATH::UNKNOWN;
}

CMapToolItem::CMapToolItem(const CMapToolItem& rhs)
	:CMapToolMapObject(rhs) , m_eItemType(rhs.m_eItemType)
{
	m_tHandle.m_eType = OBJTYPE::OBJ_ITEM;
	m_eCategory = MAPOBJECT_CATEGORY::ITEM;
	m_ePlacement = MAPOBJECT_PLACEMENT::GROUND;
	m_eEventPath = EVENT_PATH::UNKNOWN;
}

CMapToolItem::~CMapToolItem()
{
}


_int CMapToolItem::Update_GameObject(const _float& fTimeDelta)
{
	if (m_IsEvent == true)
		CGameObject::Update_GameObject(fTimeDelta);

	return 0;
}

void CMapToolItem::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (m_IsEvent == true)
		CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CMapToolItem::Render_GameObject()
{
	CResourceManager::GetInstance()->Get<CTexture>(L"Item")->Set_Texture();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
	m_pBuffer->Render_Buffer();
}

HRESULT CMapToolItem::Ready_GameObject()
{
	if (FAILED(this->Add_Component()))
		return E_FAIL;

	return S_OK;
}

HRESULT	CMapToolItem::Add_Component()
{
	if (FAILED(CMapToolMapObject::Add_Component()))
		return E_FAIL;



	//뭐또 추가할거 없나?
}


CMapToolItem* CMapToolItem::Create(LPDIRECT3DDEVICE9 Device)
{
	CMapToolItem* pMapItem = new CMapToolItem(Device);

	if (FAILED(pMapItem->Ready_GameObject()))
	{
		MSG_BOX("CMapToolItem Create Is Failed");
		Safe_Release(pMapItem);
		return nullptr;
	}

	return pMapItem;
}


void	CMapToolItem::Free()
{
	CGameObject::Free();
}

