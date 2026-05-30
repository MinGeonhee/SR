#include "pch.h"
#include "CTerrain.h"
#include "CResourceManager.h"
#include "CRendererManager.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
	m_tHandle.m_eType = OBJTYPE::OBJ_TERRAIN;
}

CTerrain::CTerrain(const CGameObject& rhs)
	:CGameObject(rhs)
{

}

CTerrain::~CTerrain()
{

}

HRESULT CTerrain::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CTerrain::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);

	CRendererManager::GetInstance()->Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;
}

void CTerrain::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CTerrain::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	CResourceManager::GetInstance()->Get<CTexture>(L"Terrain")->Set_Texture();

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CTerrain::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = CLONE_PROTOTYPE(PROTO_TERRAINTEX, CTerrainTex);

	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer",pComponent });

	// Transform
	pComponent = m_pTransformCom = CLONE_PROTOTYPE(PROTO_TRANS, CTransform);

	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });



	return S_OK;
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrain* pTerrain = new CTerrain(pGraphicDev);

	if (FAILED(pTerrain->Ready_GameObject()))
	{
		Safe_Release(pTerrain);
		MSG_BOX("Terrain Create Failed");
		return nullptr;
	}

	return pTerrain;
}

void CTerrain::Free()
{
	Engine::CGameObject::Free();
}
