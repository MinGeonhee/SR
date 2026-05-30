#include "pch.h"
#include "CToolManager.h"
#include "CGridManager.h"
#include "CTileManager.h"
#include "CCubeManager.h"
#include "CMapObjectManager.h"
#include "CCollisionManager.h"
#include "CInputManager.h"


IMPLEMENT_SINGLETON(CToolManager)






CToolManager::CToolManager()
	:  m_eCurFloor(FLOOR::FLOOR_B3) , m_eBuildType(BUILD_NONE), m_eShapeType(SHAPE_NONE)
	, m_vCurCenterUV({0.f,0.f}), m_eTileType(TILETYPE::TILE_NONE) , m_pCubeManager(nullptr)
	, m_pGridManager(nullptr), m_pTileManager(nullptr) ,  m_sSettingRoomNum(0) ,  m_eToolType(TOOLTYPE::TOOL_MAP)
	, m_pMapObjectManager(nullptr)
{
	//모두 true로 초기화 해버리기
	m_tActivelist.isActiveCube = true;
	m_tActivelist.isActiveEventCube = true;

	m_tActivelist.isActiveTileCeiling = true;
	m_tActivelist.isActiveFloorTile = true;

	m_tActivelist.isActiveDecor = true;
	m_tActivelist.isActiveObstacle = true;
	m_tActivelist.isActiveEventObject = true;

	m_tActivelist.isActiveMonster = true;
	m_tActivelist.isActiveItem = true;
	m_tActivelist.isActiveCreatureCantGoCube = true;
}

CToolManager::~CToolManager()
{
}

void CToolManager::ToolManger_Update(const _float& DT)
{
	//버퍼로 생성된 버퍼 오브젝트삭제 매프레임마다
	m_pTileManager->Delete_BufferRect();
	m_pCubeManager->Delete_BufferCube();
	m_pMapObjectManager->Delete_BufferMapObject();
	

	KeyUpdate();

	MousePickingNoneClick();

	if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::LBUTTON))
		MousePickingClick();

	if (CInputManager::GetInstance()->GetButtonPress(KEY_TYPE::RBUTTON))
		MousePickingClick();


	m_pGridManager->GridManager_Update(DT);
	m_pTileManager->TileManger_Update(DT);
	m_pCubeManager->CubeManager_Update(DT);
	m_pMapObjectManager->MapObjectManager_Update(DT);
}

void CToolManager::ToolManager_LateUpdate(const _float& DT)
{
	m_pGridManager->GridManager_LateUpdate(DT);
	m_pTileManager->TileManger_LateUpdate(DT);
	m_pCubeManager->CubeManager_LateUpdate(DT);
	m_pMapObjectManager->MapObjectManager_LateUpdate(DT);
}

void CToolManager::ToolManger_Render(LPDIRECT3DDEVICE9 device)
{
	m_pGridManager->GridManager_Render(device);
	m_pTileManager->TileManger_Render(device);
	m_pCubeManager->CubeManger_Render(device);
	m_pMapObjectManager->MapObjectManager_Render(device);
}

void CToolManager::UpFloor()
{
	if (m_eCurFloor == FLOOR::FLOOR_3F)
		return;

	m_eCurFloor = static_cast<FLOOR>(static_cast<int>(m_eCurFloor) + 1);
	m_pGridManager->SetFloor(m_eCurFloor);
	m_pTileManager->SetFloor(m_eCurFloor);
	m_pCubeManager->SetFloor(m_eCurFloor);
	m_pMapObjectManager->SetFloor(m_eCurFloor);
}

void CToolManager::DownFloor()
{
	if (m_eCurFloor == FLOOR::FLOOR_B3)
		return;

	m_eCurFloor = static_cast<FLOOR>(static_cast<int>(m_eCurFloor) - 1);

	m_pGridManager->SetFloor(m_eCurFloor);
	m_pTileManager->SetFloor(m_eCurFloor);
	m_pCubeManager->SetFloor(m_eCurFloor);
	m_pMapObjectManager->SetFloor(m_eCurFloor);
}

void CToolManager::MousePickingNoneClick()
{
	if (m_eToolType == TOOLTYPE::TOOL_MAP)
		MapNonePicking();

	else if (m_eToolType == TOOLTYPE::TOOL_OBJECT)
		ObjectNonePicking();
}

void CToolManager::ObjectNonePicking()
{
	CCollisionManager::GetInstance()->FireRaycast(MAPTOOL_OBJECT_NONE_CLICK_RAY);
}

void CToolManager::MapNonePicking()
{
	//아무 모드도 설정되어있지 않으면 리턴
//빌드 타입     ,  도형 타입이 선택이 안되어 있으면 리턴한다  둘다 적용은 되어있어야한다.
	if (m_eBuildType == MAPTOOL_BUILDTYPE::BUILD_NONE && m_eShapeType == MAPTOOL_SHAPETYPE::SHAPE_NONE)
		return;


	if (m_eBuildType == MAPTOOL_BUILDTYPE::BUILD_CREATE)
	{
		// 설치모드 일때 도형 종류 별로 나누기
		if (m_eShapeType == MAPTOOL_SHAPETYPE::SHAPE_TILE)
		{

			// 타일 타입일때 천장, 바닥으로 2개로 나뉜다
			if (m_eTileType == TILETYPE::TILE_FLOOR)
			{
				// 바닥 타일 타입일때
				CCollisionManager::GetInstance()->FireRaycast(MAPTOOL_TILE_FLOOR_NONE_CLICK_RAY);
			}
			else if (m_eTileType == TILETYPE::TILE_CEILING)
			{
				//  천장 타일 타입일때
				CCollisionManager::GetInstance()->FireRaycast(MAPTOOL_TILE_CEILING_NONE_CLICK_RAY);
			}
		}
		else if (m_eShapeType == MAPTOOL_SHAPETYPE::SHAPE_CUBE)
		{
			CCollisionManager::GetInstance()->FireRaycast(MAPTOOL_CUBE_NONE_CLICK_RAY);
		}
	}
	else if (m_eBuildType == MAPTOOL_BUILDTYPE::BUILD_TEXTURE)
	{
		// 텍스처 모드일때 도형 종류 별로 나누기
		if (m_eShapeType == MAPTOOL_SHAPETYPE::SHAPE_TILE)
		{
			// 타일 타입일때 천장, 바닥으로 2개로 나뉜다
			if (m_eTileType == TILETYPE::TILE_FLOOR)
			{
				// 바닥 타일 타입일때
				CCollisionManager::GetInstance()->FireRaycast(MAPTOOL_TILE_FLOOR_NONE_CLICK_TEX_RAY);
			}
			else if (m_eTileType == TILETYPE::TILE_CEILING)
			{
				//  천장 타일 타입일때
				CCollisionManager::GetInstance()->FireRaycast(MAPTOOL_TILE_CEILING_NONE_CLICK_TEX_RAY);
			}
		}
		else if (m_eShapeType == MAPTOOL_SHAPETYPE::SHAPE_CUBE)
		{
			CCollisionManager::GetInstance()->FireRaycast(MAPTOOL_CUBE_NONE_CLICK_TEX_RAY);
		}
	}
	else if (m_eBuildType == BUILD_DELETE)
		CCollisionManager::GetInstance()->FireRaycast(MAPTOOL_DELETE_NONE_CLICK_RAY);
	else if (m_eBuildType == BUILD_INFO)
	{
		CCollisionManager::GetInstance()->FireRaycast(MAPTOOL_NONE_CLICK_INFO_RAY);
	}
}

void CToolManager::MousePickingClick()
{
	if (m_eToolType == TOOLTYPE::TOOL_MAP)
		MapPicking();

	else if (m_eToolType == TOOLTYPE::TOOL_OBJECT)
		ObjectPicking();
}

void CToolManager::ObjectPicking()
{
	CCollisionManager::GetInstance()->FireRaycast(MAPTOOL_OBJECT_CLICK_RAY);
}

void CToolManager::MapPicking()
{
	//아무 모드도 설정되어있지 않으면 리턴
//빌드 타입 , 도형 타입이 선택이 안되어 있으면 리턴한다  둘다 적용은 되어있어야한다.
	if (m_eBuildType == MAPTOOL_BUILDTYPE::BUILD_NONE && m_eShapeType == MAPTOOL_SHAPETYPE::SHAPE_NONE)
		return;

	if (m_eBuildType == MAPTOOL_BUILDTYPE::BUILD_CREATE)
	{
		// 설치모드 일때 도형 종류 별로 나누기
		if (m_eShapeType == MAPTOOL_SHAPETYPE::SHAPE_TILE)
		{
			// 타일 타입일때 천장, 바닥으로 2개로 나뉜다
			if (m_eTileType == TILETYPE::TILE_FLOOR)
			{
				// 바닥 타일 타입일때
				CCollisionManager::GetInstance()->FireRaycast(MAPTOOL_TILE_FLOOR_CLICK_RAY);
			}
			else if (m_eTileType == TILETYPE::TILE_CEILING)
			{
				//  천장 타일 타입일때
				CCollisionManager::GetInstance()->FireRaycast(MAPTOOL_TILE_CEILING_CLICK_RAY);
			}
		}
		else if (m_eShapeType == MAPTOOL_SHAPETYPE::SHAPE_CUBE)
		{
			//큐브 설치할때도 타일과만 충돌처리가되야만 설치가된다.
			CCollisionManager::GetInstance()->FireRaycast(MAPTOOL_CUBE_CLICK_RAY);
		}
	}
	else if (m_eBuildType == MAPTOOL_BUILDTYPE::BUILD_TEXTURE)
	{
		// 텍스처 모드일때 도형 종류 별로 나누기
		if (m_eShapeType == MAPTOOL_SHAPETYPE::SHAPE_TILE)
		{
			// 타일 타입일때 천장, 바닥으로 2개로 나뉜다
			if (m_eTileType == TILETYPE::TILE_FLOOR)
			{
				// 바닥 타일 타입일때
				CCollisionManager::GetInstance()->FireRaycast(MAPTOOL_TILE_FLOOR_CLICK_TEX_RAY);
			}
			else if (m_eTileType == TILETYPE::TILE_CEILING)
			{
				//  천장 타일 타입일때
				CCollisionManager::GetInstance()->FireRaycast(MAPTOOL_TILE_CEILING_CLICK_TEX_RAY);
			}
		}
		else if (m_eShapeType == MAPTOOL_SHAPETYPE::SHAPE_CUBE)
		{
			//큐브랑 충돌처리가 되어야하기때문에 큐브랑만 처리
			CCollisionManager::GetInstance()->FireRaycast(MAPTOOL_CUBE_CLICK_TEX_RAY);
		}
	}
	else if (m_eBuildType == BUILD_DELETE)
	{
		CCollisionManager::GetInstance()->FireRaycast(MAPTOOL_DELETE_CLICK_RAY);
	}
	else if (m_eBuildType == BUILD_INFO)
	{
		CCubeManager::GetInstance()->Delete_BufferCube();
		CTileManager::GetInstance()->Delete_BufferInfoRect();

		CCollisionManager::GetInstance()->FireRaycast(MAPTOOL_CLICK_INFO_RAY);
	}

}

void CToolManager::KeyUpdate()
{
	auto InputMgr = CInputManager::GetInstance();

	if (InputMgr->GetButtonDown(KEY_TYPE::NUM1))
		this->DownFloor();

	if (InputMgr->GetButtonDown(KEY_TYPE::NUM2))
		this->UpFloor();


	if (m_eToolType == TOOLTYPE::TOOL_MAP)
	{
		if (InputMgr->GetButtonDown(KEY_TYPE::F1))
			this->Set_BuildType(MAPTOOL_BUILDTYPE::BUILD_CREATE);
		if (InputMgr->GetButtonDown(KEY_TYPE::F2))
			this->Set_BuildType(MAPTOOL_BUILDTYPE::BUILD_TEXTURE);
		if (InputMgr->GetButtonDown(KEY_TYPE::F3))
			this->Set_BuildType(MAPTOOL_BUILDTYPE::BUILD_DELETE);
		if (InputMgr->GetButtonDown(KEY_TYPE::F4))
			this->Set_BuildType(MAPTOOL_BUILDTYPE::BUILD_INFO);


		if (InputMgr->GetButtonDown(KEY_TYPE::T))
			this->Set_ShapeType(MAPTOOL_SHAPETYPE::SHAPE_TILE);
		if (InputMgr->GetButtonDown(KEY_TYPE::C))
			this->Set_ShapeType(MAPTOOL_SHAPETYPE::SHAPE_CUBE);
	}


}

void CToolManager::SetTextureMode_CenterUV(const _vec2& vCenterUV)
{
	m_vCurCenterUV = vCenterUV;

	m_pCubeManager->SetTextureMode_CenterUV(vCenterUV);
	m_pTileManager->SetTextureMode_CenterUV(vCenterUV);
}

void CToolManager::Set_SettingRoomNum(const _ushort& sRoomNum)
{
	if (sRoomNum > 65535)
	{
		MSG_BOX("방 번호가 최대값을 넘어서서 입력되었습니다 \n 다시 설정해주세요 ");
		return;
	}

	//본인 방 번호 설정해 준뒤
	m_sSettingRoomNum = sRoomNum;

	//밑에 관리하는 매니저들의 방번호도 똑같이 설정
	m_pTileManager->SetSettingRoomNum(sRoomNum);
	m_pCubeManager->SetSettingRoomNum(sRoomNum);
}

void CToolManager::SetAllRender()
{
	m_pTileManager->SetIsAllRender(true);
	m_pMapObjectManager->SetIsAllRender(true);
}

void CToolManager::SetFloorRender()
{
	m_pTileManager->SetIsAllRender(false);
	m_pMapObjectManager->SetIsAllRender(false);
}

void CToolManager::Add_MapObject(const OBJTYPE& eType, CGameObject* pObj)
{
	if (pObj->Get_OBJTYPE() != eType || pObj == nullptr)
		return;

	if ((int)m_eCurFloor < 0 || m_eCurFloor >= FLOOR_END)
		return;

	switch (eType)
	{
	case Engine::OBJ_TILE:
		m_pTileManager->Add_Tile(pObj);
		break;
	case Engine::OBJ_CUBE:
		m_pCubeManager->Add_Cube(pObj);
		break;
	default:
		break;
	}
}


void CToolManager::Init()
{
	//먼저 인스턴스 생성
	m_pGridManager = CGridManager::GetInstance();
	m_pTileManager = CTileManager::GetInstance();
	m_pCubeManager = CCubeManager::GetInstance();
	m_pMapObjectManager = CMapObjectManager::GetInstance();

	this->Set_Scale({1.f,1.f,1.f});


	m_pGridManager->Init();
	m_pGridManager->SetFloor(m_eCurFloor);

	m_pTileManager->Init();
	m_pTileManager->SetFloor(m_eCurFloor);
	m_pTileManager->SetSettingRoomNum(m_sSettingRoomNum);

	m_pCubeManager->Init();
	m_pCubeManager->SetFloor(m_eCurFloor);
	m_pCubeManager->SetSettingRoomNum(m_sSettingRoomNum);

	m_pMapObjectManager->Init();
	m_pMapObjectManager->SetFloor(m_eCurFloor);
	m_pMapObjectManager->SetSettingRoomNum(m_sSettingRoomNum);

}
void CToolManager::Set_Scale(const _vec3& vScale)
{
	m_vScale = vScale;

	m_pGridManager->SetGridScale(m_vScale);
	m_pTileManager->SetTileScale(m_vScale);
	m_pCubeManager->SetCubeScale(m_vScale);
}

void CToolManager::Set_DefaultTileUV(const _vec2 UV)
{
	m_vDefaultTileCenterUV = UV;

	m_pTileManager->SetDefaultTileUV(UV);
}

void CToolManager::Set_DefaultCubeUV(const _vec2 UV)
{
	m_vDefaultCubeCenterUV = UV;

	m_pCubeManager->SetDefaultCubeUV(UV);
}


void CToolManager::SetActive(const ACTIVE_OBJECT& eType, const _bool& isActive)
{
	switch (eType)
	{
	case Engine::ACTIVE_OBJECT::TILE_FLOOR:
		CTileManager::GetInstance()->SetActive(eType, isActive);
		m_tActivelist.isActiveFloorTile = isActive;
		break;
	case Engine::ACTIVE_OBJECT::TILE_CELILING:
		CTileManager::GetInstance()->SetActive(eType, isActive);
		m_tActivelist.isActiveTileCeiling = isActive;
		break;
	case Engine::ACTIVE_OBJECT::CUBE:
		CCubeManager::GetInstance()->SetActive(eType, isActive);
		m_tActivelist.isActiveCube = isActive;
		break;
	case Engine::ACTIVE_OBJECT::EVENTCUBE:
		CCubeManager::GetInstance()->SetActive(eType, isActive);
		m_tActivelist.isActiveEventCube = isActive;
		break;
	case Engine::ACTIVE_OBJECT::CREATURECANTGOCUBE:
		CCubeManager::GetInstance()->SetActive(eType, isActive);
		m_tActivelist.isActiveEventCube = isActive;
		break;
	case Engine::ACTIVE_OBJECT::DECOR:
		CMapObjectManager::GetInstance()->SetActive(eType, isActive);
		m_tActivelist.isActiveDecor = isActive;
		break;
	case Engine::ACTIVE_OBJECT::OBSTACLE:
		CMapObjectManager::GetInstance()->SetActive(eType, isActive);
		m_tActivelist.isActiveObstacle = isActive;
		break;
	case Engine::ACTIVE_OBJECT::EVENTOBJECT:
		CMapObjectManager::GetInstance()->SetActive(eType, isActive);
		m_tActivelist.isActiveEventObject = isActive;
		break;
	case Engine::ACTIVE_OBJECT::ITEM:
		CMapObjectManager::GetInstance()->SetActive(eType, isActive);
		m_tActivelist.isActiveItem = isActive;
		break;
	case Engine::ACTIVE_OBJECT::MONSTER:
		CMapObjectManager::GetInstance()->SetActive(eType, isActive);
		m_tActivelist.isActiveMonster = isActive;
		break;
	default:
		return;
	}
}



void CToolManager::Free()
{

}