#include "CMapToolMonster.h"
#include "CResourceManager.h"
#include "CMapObjectTex.h"
#include "CTransform.h"
#include "CMapObjectManager.h"


CMapToolMonster::CMapToolMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMapToolMapObject(pGraphicDev), m_eMonsterType(MONSTER_END)
{
	m_tHandle.m_eType = OBJTYPE::OBJ_DOOR;
}

CMapToolMonster::CMapToolMonster(const CMapToolMonster& rhs)
	:CMapToolMapObject(rhs), m_eMonsterType(rhs.m_eMonsterType)
{
	m_tHandle.m_eType = OBJTYPE::OBJ_DOOR;
}

CMapToolMonster::~CMapToolMonster()
{
}



_int CMapToolMonster::Update_GameObject(const _float& fTimeDelta)
{
	if (m_IsEvent == true)
		CGameObject::Update_GameObject(fTimeDelta);

	return 0;
}

void CMapToolMonster::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (m_IsEvent == true)
		CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CMapToolMonster::Render_GameObject()
{
	Set_Texture();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
	m_pBuffer->Render_Buffer();
}

HRESULT CMapToolMonster::Ready_GameObject()
{
	if (FAILED(CMapToolMapObject::Add_Component()))
		return E_FAIL;


	m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"Monster");

	return S_OK;
}

HRESULT	CMapToolMonster::Add_Component()
{
	if (FAILED(CMapToolMapObject::Add_Component()))
		return E_FAIL;
	//뭐또 추가할거 없나?
}

void CMapToolMonster::Set_Texture()
{
	switch (m_eMonsterType)
	{
	case Engine::MONSTER_GOBLIN_HAMMER:
	case Engine::MONSTER_GOBLIN_FIREBALL:
	case Engine::MONSTER_ORGE_AXE:
	case Engine::MONSTER_RAT:
		CResourceManager::GetInstance()->Get<CTexture>(L"Monster")->Set_Texture();
		break;
	case Engine::MONSTER_SKELETON_SWORD:
	case Engine::MONSTER_WIZARD:
		CResourceManager::GetInstance()->Get<CTexture>(L"Monster2")->Set_Texture();
		break;
	case Engine::MONSTER_END:
		break;
	default:
		break;
	}
}


CMapToolMonster* CMapToolMonster::Create(LPDIRECT3DDEVICE9 Device)
{
	CMapToolMonster* pMapItem = new CMapToolMonster(Device);

	if (FAILED(pMapItem->Ready_GameObject()))
	{
		MSG_BOX("CMapToolItem Create Is Failed");
		Safe_Release(pMapItem);
		return nullptr;
	}

	return pMapItem;
}

void	CMapToolMonster::Free()
{
    CGameObject::Free();
}