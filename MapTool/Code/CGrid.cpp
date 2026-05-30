#include "pch.h"
#include "CGrid.h"
#include "CProtoTypeManager.h"
#include "CRendererManager.h"
#include "CGridManager.h"

CGrid::CGrid(const CGrid& rhs)
    : CGameObject(rhs) 
{
	m_tHandle.m_eType = OBJ_GRID;
}

CGrid::CGrid(LPDIRECT3DDEVICE9 pGraphicDev)
    : CGameObject(pGraphicDev), m_pBufferCom(nullptr) , m_pCenterBufferCom(nullptr)
{
	m_tHandle.m_eType = OBJ_GRID;
}

CGrid::~CGrid()
{
}

void CGrid::Free()
{
    CGameObject::Free();
}

HRESULT CGrid::Ready_GameObject(const _int& iFloor)
{
	if (FAILED(Add_Component()))
		return E_FAIL;


	m_pTransform->Set_Pos(0.f, float(iFloor), 0.f);
	
    return S_OK;
}

_int CGrid::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);	
    return 0;
}

void CGrid::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CGrid::Render_GameObject()
{
	m_pGraphicDev->SetTexture(0, nullptr);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
	m_pBufferCom->Render_Buffer();
	//m_pCenterBufferCom->Render_Buffer();
}

CGrid* CGrid::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _int& iFloor)
{
	CGrid* Obj = new CGrid(pGraphicDev);

	if (FAILED(Obj->Ready_GameObject(iFloor)))
	{
		// 만약에 제대로 생성이 되지 않았다면 바로 삭제.
		//Safe_Release(Obj);
		MSG_BOX("Player Create Failed");
		return nullptr;
	}

	return Obj;
}
HRESULT CGrid::Add_Component()
{
	CComponent* pCom = nullptr;

	pCom = m_pBufferCom =
		static_cast<CGridCol*>(CProtoTypeManager::GetInstance()->Clone_Prototype(PROTO_GRIDCOL));

	if (pCom == nullptr)
		return E_FAIL;

	m_mapComponent->insert({ L"Com_Buffer", pCom });


	pCom = m_pCenterBufferCom =
		static_cast<CCenterLineCol*>(CProtoTypeManager::GetInstance()->Clone_Prototype(PROTO_CENTERLINECOL));

	if (pCom == nullptr)
		return E_FAIL;

	m_mapComponent->insert({ L"Com_Buffer2", pCom });


	pCom = m_pTransform = CLONE_PROTOTYPE(PROTO_TRANS,CTransform);
	if (pCom == nullptr)
		return E_FAIL;

	m_mapComponent->insert({ L"Com_Trnasform", pCom });


    return S_OK;
}