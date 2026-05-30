#include "CGrid.h"
#include "CRendererManager.h"

CGrid::CGrid(LPDIRECT3DDEVICE9 pGraphicDev)
    :CGameObject(pGraphicDev)
{
	
}

CGrid::CGrid(const CGameObject& rhs)
    :CGameObject(rhs)
{
}

CGrid::~CGrid()
{
}

HRESULT CGrid::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;


    return S_OK;
}

_int CGrid::Update_GameObject(const _float& fTimeDelta)
{
    CGameObject::Update_GameObject(fTimeDelta);
	//CRendererManager::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

    return 0;
}

void CGrid::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CGrid::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT CGrid::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = CLONE_PROTOTYPE(PROTO_TERRAINCOL, CTerrainCol);

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

CGrid* CGrid::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGrid* pGrid = new CGrid(pGraphicDev);

	if (FAILED(pGrid->Ready_GameObject()))
	{
		Safe_Release(pGrid);
		MSG_BOX("Grid Create Failed");
		return nullptr;
	}

	return pGrid;
}

void CGrid::Free()
{
	Engine::CGameObject::Free();
}
