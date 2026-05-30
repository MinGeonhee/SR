#include "pch.h"
#include "CRendererManager.h"
#include "CGridManager.h"
#include "CGameObject.h"
#include "CGraphicDev.h"
#include "CGrid.h"
#include "CProtoTypeManager.h"


IMPLEMENT_SINGLETON(CGridManager)

CGridManager::CGridManager()
	: m_eCurFloor(FLOOR::FLOOR_B3) , m_pRendererManager(nullptr) 
{
	for (size_t i = 0; i < FLOOR::FLOOR_END; ++i)
	{
		m_pGrid[i] = nullptr;
	}
}

CGridManager::~CGridManager()
{
	Free();
}

void CGridManager::Init()
{
	for (size_t i = 0; i < FLOOR::FLOOR_END; ++i)
	{
		m_pGrid[i] = nullptr;
	}

	m_pRendererManager = CRendererManager::GetInstance();

	//그리드는 따로생성해줘야한다
	CProtoTypeManager::GetInstance()->Ready_Prototype(PROTO_GRIDCOL, CGridCol::Create(CGraphicDev::GetInstance()->m_pGraphicDev
		, 100 , 100 , 0, m_vGridScale.x));



	this->MakeGrid_OnAllFloor();
}

void CGridManager::GridManager_Update(const _float& DT)
{
	m_pGrid[m_eCurFloor]->Update_GameObject(DT);
}

void CGridManager::GridManager_LateUpdate(const _float& DT)
{
	m_pGrid[m_eCurFloor]->LateUpdate_GameObject(DT);
}

void CGridManager::GridManager_Render(LPDIRECT3DDEVICE9 device)
{
	CGraphicDev::GetInstance()->m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	CGraphicDev::GetInstance()->m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	m_pGrid[m_eCurFloor]->Render_GameObject();

	CGraphicDev::GetInstance()->m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CGridManager::SetFloor(const FLOOR& eFloor)
{
	m_eCurFloor = eFloor;
}

void CGridManager::MakeGrid_OnAllFloor()
{
	CGrid* newGrid = nullptr;

	for (int i = 0; i < FLOOR_END; ++i)
	{
		newGrid = CGrid::Create( CGraphicDev::GetInstance()->Get_GraphicDev(), i * m_vGridScale.y);
		m_pGrid[i] = newGrid;
		newGrid = nullptr;
	}
}


void CGridManager::Free()
{
	for (size_t i = 0; i < FLOOR::FLOOR_END; ++i)
	{
		Safe_Release(m_pGrid[i]);
	}
}
