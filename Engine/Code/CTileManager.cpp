#include "CTileManager.h"
#include "CResourceManager.h"
#include "CRendererManager.h"
#include "CGraphicDev.h"
#include "CGameObject.h"
#include "CCollisionManager.h"
#include "CTransform.h"
#include "JsonFunction.h"
#include "CInputManager.h"
#include "CMapToolTile.h"


IMPLEMENT_SINGLETON(CTileManager);

CTileManager::CTileManager()
	: m_eCurFloor(FLOOR::FLOOR_END) , m_pRendererManager(nullptr) , m_pBufferRect(nullptr) , m_fTileOffset(0), m_iTileCntX(0)
	, m_iTileCntZ(0) , m_pTargetObject(nullptr) , m_sSettingRoomNum(0) , m_pBufferInfoRect(nullptr)
	, m_iIndex(0) , m_iTileSize(0) , m_iMakeIndex(0) , m_vTileScale() , m_isAllRender(false)
{
}

void CTileManager::Init()
{
	m_pBufferRect = nullptr;

	m_iTileSize = 1;
	m_iTileCntX = 100;
	m_iTileCntZ = 100;

	// 
	m_fTileOffset = (float)m_vTileScale.x / 2;

	MakeTileFloor();

	m_pRendererManager = CRendererManager::GetInstance();

	//기본 세팅값 1 1 1 

}



void CTileManager::MakeTileFloor()
{
	for (int f = 0; f < FLOOR::FLOOR_END; ++f) {
		m_vTile[f].clear(); // 필요시 타일 delete/Release 포함
		vector<CGameObject*>().swap(m_vTile[f]);
	}

	CMapToolTile* pTile = nullptr;

	for (int Floor = 0; Floor < FLOOR::FLOOR_END; ++Floor)
	{
		for (int z = 0 ; z < m_iTileCntZ; ++z)
		{
			for (int x = 0; x < m_iTileCntX; ++x)
			{
				pTile = CMapToolTile::Create(CGraphicDev::GetInstance()->m_pGraphicDev, (FLOOR)Floor, 0 ,{
					(float)x * m_vTileScale.x + m_fTileOffset ,
					(float)Floor * m_vTileScale.y ,
					(float)z * m_vTileScale.z  + m_fTileOffset });
				//만든 순서를 번호를 넣어준다.
				pTile->Set_MakeIndex(m_iMakeIndex++);

				m_vTile[Floor].push_back(pTile);
				pTile->SemiUpdate();
				pTile = nullptr;
			}
		}

		m_iMakeIndex = 0;
	}

}

void CTileManager::TileManger_Update(const _float& DT)
{

	KeyUpdate();


	for (auto& TileVec : m_vTile[m_eCurFloor])
	{
		if (static_cast<CMapToolTile*>(TileVec)->IsCubeOnTop())
		{
			//확인용이요~
			int a = 0;
		}

		TileVec->Update_GameObject(DT);
	}

	if (m_pBufferRect != nullptr)
	{
		m_pBufferRect->Update_GameObject(DT);
	}


	if (m_pBufferInfoRect != nullptr)
	{
		m_pBufferInfoRect->Update_GameObject(DT);
	}
}

void CTileManager::TileManger_LateUpdate(const _float& DT)
{
	for (auto& TileVec : m_vTile[m_eCurFloor])
	{
		TileVec->LateUpdate_GameObject(DT);
	}

	if (m_pBufferRect != nullptr)
		m_pBufferRect->LateUpdate_GameObject(DT);

	if (m_pBufferInfoRect != nullptr)
	{
		m_pBufferInfoRect->LateUpdate_GameObject(DT);
	}
}

void CTileManager::TileManger_Render(LPDIRECT3DDEVICE9 Device)
{
	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	// 비교 함수 설정 (예: 128 이상이면 출력)
	Device->SetRenderState(D3DRS_ALPHAREF, 128); // 기준값 (0~255)
	Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

	CResourceManager::GetInstance()->Get<CTexture>(L"Tile")->Set_Texture();

	if (m_isAllRender == true) // 전체 렌더모드일때
	{
		for (int i = 0; i < FLOOR::FLOOR_END; ++i)
		{
			for (auto& TileVec : m_vTile[i])
			{
				if (TileVec->Get_Active() != false)
					TileVec->Render_GameObject();
			}
		}
	}
	else  //FLoor 렌더 모드일때
	{
		for (auto& TileVec : m_vTile[m_eCurFloor])
		{
			if (TileVec->Get_Active() != false)
				TileVec->Render_GameObject();
		}
	}

	if (m_pBufferRect != nullptr)
		m_pBufferRect->Render_GameObject();

	if (m_pBufferInfoRect != nullptr)
	{
		m_pBufferInfoRect->Render_GameObject();
	}

	CGraphicDev::GetInstance()->m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}



void CTileManager::SaveTileToJSON(nlohmann::ordered_json& j,const string& sFileName)
{
	for (int i = 0; i < FLOOR::FLOOR_END; ++i)
	{
		for (auto& Tile : m_vTile[i])
		{
			//타일타입이 NONE인건 걸러준다 맵툴용 바닥에 깔린 타일을 저장할 순 없으니까.
			CMapToolTile* pTile = static_cast<CMapToolTile*>(Tile);

			if (pTile->Get_TileType() != TILETYPE::TILE_NONE)
			{
				ObjectHandle tHandle = pTile->Get_Handle();
				MAPTOOLTILE_INFO tTileInfo = pTile->Get_MapToolTile_Info();
				
				auto Transform = pTile->Get_Transform();
				_vec3 vPos =	*Transform->GetInfoPos();
				_vec3 vRot =	Transform->Get_Angle();
				_vec3 vScale =  Transform->Get_Scale();
				TILETYPE eTileType = pTile->Get_TileType();
				_vec2 vCenterUV = pTile->Get_CenterUV();



				nlohmann::ordered_json t;

				JsonFunction::Save_ObjectHandle(t, tHandle);
				JsonFunction::Save_Info(t,vPos, vRot,vScale);
				
				JsonFunction::Save_CenterUV(t,vCenterUV);
				JsonFunction::Save_Tile_Type(t,eTileType);
				JsonFunction::Save_TileInfo_ForMapTool(t, tTileInfo);

				j["TILE"].push_back(t);
			}
		}
	}
}

void CTileManager::LoadTile(const string& sFilePath)
{
	//싹다 지우고 다시 깔기
	Free();
	MakeTileFloor();

	string FilePath = sFilePath;


	ifstream is(sFilePath);

	nlohmann::json jLoad;
	
	//파일 열지못했다면 리턴
	if (!is.is_open())
		return;

	is >> jLoad;

	const nlohmann::json Tiles = jLoad.value("TILE",nlohmann::json::array());
	
	if (!Tiles.is_array())
		return;

	int LastIndex = 0;

	for (const auto& Tile : Tiles)
	{
		//기본 정보
		ObjectHandle vHandle = JsonFunction::Load_ObjectHandle(Tile);
		MAPTOOLTILE_INFO vTileInfo = JsonFunction::Load_MapToolTile_Info(Tile);

		_vec3    vPos = JsonFunction::Load_Info(Tile, INFO_TYPE_POS);
		_vec3    vRot = JsonFunction::Load_Info(Tile, INFO_TYPE_ROT);
		_vec3    vScale = JsonFunction::Load_Info(Tile, INFO_TYPE_SCALE);

		//타일정보
		TILETYPE TileType = JsonFunction::Load_TileType(Tile);
		_vec2	 CenterUV = JsonFunction::Load_CenterUV(Tile);

		//타일생성
		CMapToolTile* pTile = nullptr;

		if( TileType == TILE_FLOOR )
			pTile = static_cast<CMapToolTile*>(m_vTile[vHandle.m_eFloor][vTileInfo.iMakeIndex]);	//아마 게임오브젝트로 들어가있어서 그럼
		else
			pTile = CMapToolTile::Create(CGraphicDev::GetInstance()->m_pGraphicDev);
		
		//기본 게임 오브젝트 설정 세팅
		pTile->Setting_GameObject(vHandle , vPos, vRot , vScale);

		//타일 정보세팅 들어간당
		pTile->Set_TileType(TileType);
		pTile->Set_MapToolTile_Info(vTileInfo);
		pTile->Change_TextureUV(CenterUV);

		if (pTile->Get_TileType() == TILETYPE::TILE_CEILING)
		{
			Add_Tile(pTile->GetFloor(), pTile);
			this->SetIsHaveCeiling_By_MakeIndex(vHandle.m_eFloor , pTile->GetTargetMakeIndex());
		}

		LastIndex = pTile->Get_HandleIndex();
		
		pTile->SemiUpdate();
	}


	m_iIndex = LastIndex + 1;


	//Load할거 다하고 콜리전 매니저 추가해주기.
	CCollisionManager::GetInstance()->Add_ObjectVector(OBJ_TILE, &m_vTile[m_eCurFloor]);

}

void CTileManager::SetFloor(const FLOOR& eFloor)
{
	m_eCurFloor = eFloor;
	Delete_BufferInfoRect();
	CCollisionManager::GetInstance()->Add_ObjectVector(OBJTYPE::OBJ_TILE, &m_vTile[m_eCurFloor]);
}

void CTileManager::SetTargetObject(CGameObject* Obj)
{
	if (Obj == nullptr)
	{
		m_pTargetObject = nullptr;
		return;
	}

	m_pTargetObject = Obj;
}

void CTileManager::SetOnTopCube_By_MakeIndex(const FLOOR& eFloor, const _int& MakeIndex)
{
	static_cast<CMapToolTile*>(m_vTile[(int)eFloor][MakeIndex])->Set_OnTopCube(true);
}

void CTileManager::SetIsHaveCeiling_By_MakeIndex(const FLOOR& eFloor, const _int& MakeIndex)
{
	static_cast<CMapToolTile*>(m_vTile[(int)eFloor][MakeIndex])->Set_IsHaveCeiling(true);
}

void CTileManager::Add_Tile(CGameObject* CObj)
{
	m_vTile[m_eCurFloor].push_back(static_cast<CMapToolTile*>(CObj));
	CCollisionManager::GetInstance()->Add_ObjectVector(OBJTYPE::OBJ_TILE, &m_vTile[m_eCurFloor]);
}

void CTileManager::Add_Tile(const FLOOR& eFloor, CGameObject* CObj)
{
	m_vTile[eFloor].push_back(static_cast<CMapToolTile*>(CObj));
	CCollisionManager::GetInstance()->Add_ObjectVector(OBJTYPE::OBJ_TILE, &m_vTile[m_eCurFloor]);
}

void CTileManager::Delete_Tile(CMapToolTile* pTile)
{
	vector<CGameObject*>::iterator iter = std::find(m_vTile[m_eCurFloor].begin(), m_vTile[m_eCurFloor].end(), pTile);

	if ( iter != m_vTile[m_eCurFloor].end())
	{
		if (m_pBufferInfoRect != nullptr && m_pBufferInfoRect->Get_Target() != nullptr)
		{
			// info 버퍼 타일이 들고있던 타켓 타일이 pTile과 일치할때 TargetObject도 비워준다.
			if( pTile  == m_pBufferInfoRect->Get_Target())
			{
				Safe_Release(m_pBufferInfoRect);
				m_pBufferInfoRect = nullptr;

				m_pTargetObject = nullptr;
			}
		}

		int index = pTile->Get_HandleIndex();

		Safe_Release(*iter);
		m_vTile[m_eCurFloor].erase(iter);

		Setting_HandleIndex_AfterDelete(index);
	}

	CCollisionManager::GetInstance()->Add_ObjectVector(OBJTYPE::OBJ_TILE, &m_vTile[m_eCurFloor]);
}

void CTileManager::Setting_HandleIndex_AfterDelete(const _int& index)
{
	for (int i = 0; i < FLOOR::FLOOR_END; ++i)          // 모든 층 순회
	{
		for (size_t j = 0; j < m_vTile[i].size(); ++j)  // 층 벡터 순회
		{
			CMapToolTile* pTile = static_cast<CMapToolTile*>(m_vTile[i][j]);
			if (pTile->Get_TileType() == TILETYPE::TILE_NONE)
				continue;

			int Buffer_Index = pTile->Get_HandleIndex();
			if (Buffer_Index > index)
			{
				pTile->Set_HandleIndex(Buffer_Index - 1);
			}
		}
	}

	--m_iIndex;  // 전역 다음 할당 인덱스 감소
}


void CTileManager::Delete_BufferRect()
{
	//생성되었던 버퍼 렉트 지우기?
	if (m_pBufferRect != nullptr)
	{
		Safe_Release(m_pBufferRect);
		m_pBufferRect = nullptr;
	}
}

void CTileManager::Delete_BufferInfoRect()
{	
	//생성되었던 버퍼 렉트 지우기?
	if (m_pBufferInfoRect != nullptr)
	{
		Safe_Release(m_pBufferInfoRect);
		m_pTargetObject = nullptr;
		m_pBufferInfoRect = nullptr;
	}
}

void CTileManager::KeyUpdate()
{
	auto InputMgr = CInputManager::GetInstance();

	if (InputMgr->GetButtonDown(KEY_TYPE::TAB))
	{
		if (m_pTargetObject == nullptr)
			return;            
		m_pTargetObject->Get_Transform()->Rotation(ROT_Y, D3DXToRadian(90));
	}
}


const _vec3& CTileManager::GetBuffeRectPos()
{
	if (m_pBufferRect == nullptr)
		return { 0.f , 0.f , 0.f };

	return *m_pBufferRect->Get_Transform()->GetInfoPos();
}

const _vec2& CTileManager::GetCenterUV()
{
	return m_vCurCenterUV;
}

CMapToolTile* CTileManager::GetMapToolTile_ByMakeIndex(const FLOOR& eFloor, const int& MakeIndex)
{
	return static_cast<CMapToolTile*>(m_vTile[m_eCurFloor][MakeIndex]);
}

void CTileManager::SetActive(const ACTIVE_OBJECT& eType, const _bool& isActive)
{
	TILETYPE TargetType = TILETYPE::TILE_NONE;

	if (eType == ACTIVE_OBJECT::TILE_FLOOR)
		TargetType = TILETYPE::TILE_FLOOR;
	else if (eType == ACTIVE_OBJECT::TILE_CELILING)
		TargetType = TILETYPE::TILE_CEILING;
	else
		return;

	for (int i = 0; i < FLOOR::FLOOR_END; ++i)
	{
		for (auto Tile : m_vTile[i])
		{
			//타입검사를 진행해서 Active를 꺼준다 아마 렌더도안되고 다 안될듯?
			TILETYPE Type = static_cast<CMapToolTile*>(Tile)->Get_TileType();
			if (Type == TargetType)
				Tile->Set_Active(isActive);
		}
	}


	CCollisionManager::GetInstance()->Add_ObjectVector(OBJTYPE::OBJ_TILE, &m_vTile[m_eCurFloor]);
}



void CTileManager::Free()
{
	for (size_t i = 0; i < FLOOR::FLOOR_END; ++i)
	{
		for (auto iter = m_vTile[i].begin(); iter != m_vTile[i].end(); )
		{
			if (*iter != nullptr)
			{
				Safe_Release(*iter);
				iter = m_vTile[i].erase(iter);
			}
			else
				++iter;
		}
	}

	if (m_pBufferInfoRect != nullptr)
	{
		Safe_Release(m_pBufferInfoRect);
	}
	if (m_pBufferRect != nullptr)
	{
		Safe_Release(m_pBufferRect);
	}

}