#include "CCubeManager.h"
#include "CCollisionManager.h"
#include "CRendererManager.h"
#include "CToolBufferCube.h"
#include "CTransform.h"
#include "CMapToolCube.h"
#include "CEventCube.h"
#include "JsonFunction.h"
#include "CCube.h"
#include "CGraphicDev.h"
#include "CResourceManager.h"
#include "CInputManager.h"
#include "CMapToolTile.h"
#include "CTileManager.h"

CCubeManager::CCubeManager()
	: m_pBufferCube(nullptr), m_pRendererManager(nullptr), m_eCurFloor(FLOOR::FLOOR_B3)
	, m_pTargetObject(nullptr), m_pBufferInfoCube(nullptr), m_sSettingRoomNum(0)
	, m_iIndex(0) , m_vCubeScale()

{
}


IMPLEMENT_SINGLETON(CCubeManager)

void CCubeManager::Init()
{
	m_vCube->clear();
	//m_vCube->resize(100);

	m_pRendererManager = CRendererManager::GetInstance();

}

void CCubeManager::CubeManager_Update(const _float& DT)
{

	KeyUpdate();

	for (int i = 0; i < FLOOR::FLOOR_END; ++i)
	{
		for (auto& CubeVec : m_vCube[i])
		{
			if( CubeVec->GetFloor() == m_eCurFloor)
				CubeVec->Update_GameObject(DT);
			CRendererManager::GetInstance()->Add_RenderGroup(RENDER_TILETEX, CubeVec);
		}
	}

	if (m_pBufferCube != nullptr)
	{
		m_pBufferCube->Update_GameObject(DT);
	}

	if (m_pBufferInfoCube != nullptr)
		m_pBufferInfoCube->Update_GameObject(DT);
}

void CCubeManager::CubeManager_LateUpdate(const _float& DT)
{
	for (auto& CubeVec : m_vCube[m_eCurFloor])
	{
		CubeVec->LateUpdate_GameObject(DT);
		//_vec3 vPos;
		//CubeVec->Get_Transform()->Get_Info(INFO_POS,&vPos);
		//CubeVec->Compute_ViewZ(&vPos);
	}

	if (m_pBufferCube != nullptr)
		m_pBufferCube->LateUpdate_GameObject(DT);


	if (m_pBufferInfoCube != nullptr)
		m_pBufferInfoCube->LateUpdate_GameObject(DT);
}

void CCubeManager::CubeManger_Render(LPDIRECT3DDEVICE9 Device)
{
	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	Device->SetRenderState(D3DRS_ZENABLE, TRUE);


	CResourceManager::GetInstance()->Get<CTexture>(L"Tile")->Set_Texture();


	for (int i = 0; i < FLOOR::FLOOR_END; ++i)
	{
		for (auto& CubeVec : m_vCube[i])
		{
			if (CubeVec->Get_Active() != false)
				CubeVec->Render_GameObject();
		}
	}

	if (m_pBufferCube != nullptr)
	{
		m_pBufferCube->Render_GameObject();
	}

	if (m_pBufferInfoCube != nullptr)
		m_pBufferInfoCube->Render_GameObject();

	CGraphicDev::GetInstance()->m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CCubeManager::KeyUpdate()
{
	auto Mgr = CInputManager::GetInstance();
	if (Mgr->GetButtonDown(Engine::KEY_TYPE::TAB))
	{
		if (m_pTargetObject == nullptr)
			return;

		m_pTargetObject->Get_Transform()->Rotation(ROT_Y, D3DXToRadian(90.f));
	}
}


void CCubeManager::SaveCubeToJSON(nlohmann::ordered_json& j,const string& sFileName)
{
	for (int i = 0; i < FLOOR::FLOOR_END; ++i)
	{
		for (auto& pCube : m_vCube[i])
		{
			CMapToolCube* MTCube = static_cast<CMapToolCube*>(pCube);

			ObjectHandle tHandle = MTCube->Get_Handle();
			_vec3 vPos = *MTCube->Get_Transform()->GetInfoPos();
			_vec3 vRot = MTCube->Get_Transform()->Get_Angle();
			_vec3 vScale = MTCube->Get_Transform()->Get_Scale();
			_vec2* vCenterUV = MTCube->GetCenterUV();
			_bool* VerFilpArr = MTCube->GetCubeVerticalFilpArr();
			_bool* HorFilpArr = MTCube->GetCubeHorizontalFilpArr();

			CUBE_TYPE CubeType = MTCube->GetCubeType();
			//저장할때부터 Cube를 NONE으로해준다 이벤트 큐브가 아니라면
			if (CubeType == CUBE_MAPTOOL)
				CubeType = CUBE_NONE;

			nlohmann::ordered_json t; 
			
			JsonFunction::Save_ObjectHandle(t, tHandle);
			JsonFunction::Save_Info(t, vPos, vRot, vScale);
			JsonFunction::Save_Cube_Type(t, CubeType);

			JsonFunction::Save_Cube_HorizontalFilp(t, HorFilpArr);
			JsonFunction::Save_Cube_VerticalFilp(t, VerFilpArr);
			
			JsonFunction::Save_Cube_CenterUV(t,vCenterUV);



			t["TARGETMAKEINDEX"] = MTCube->GetTargetMakeIndex();


			j["CUBE"].push_back(t);
		}
	}
}

void CCubeManager::LoadCube(const string& sFilePath)
{
	Free();
	string FilePath = sFilePath;

	nlohmann::json jLoad;

	ifstream is(sFilePath);

	//파일 열지못했다면 리턴
	if (!is.is_open())
		return;

	//json 파일 파싱
	is >> jLoad;
	
	if (!jLoad.contains("CUBE"))
		return;

	const nlohmann::json& Cubes = jLoad.value("CUBE", nlohmann::json::array());

	if (!Cubes.is_array())
		return;

	int index = 0;


	for (const auto& Cube : Cubes)
	{
		CMapToolCube* pMapToolCube = CMapToolCube::Create(CGraphicDev::GetInstance()->m_pGraphicDev);

		ObjectHandle tHandle = JsonFunction::Load_ObjectHandle(Cube);

		//OBJ타입을 큐브로 강제조정 , 이벤트 큐브라고해도 맵툴안에선 CUBE로 지정
		tHandle.m_eType = OBJTYPE::OBJ_CUBE;

		_vec3 vPos = JsonFunction::Load_Info(Cube, INFO_TYPE_POS);
		_vec3 vRot = JsonFunction::Load_Info(Cube, INFO_TYPE_ROT);
		_vec3 vScale = JsonFunction::Load_Info(Cube, INFO_TYPE_SCALE);

		//큐브 타입 로드
		CUBE_TYPE CubeType = JsonFunction::Load_Cube_Type(Cube);

		//NONE일시 맵툴 큐브 타입으로 변경	EventCUBE는  EventCube 타입 그대로
		if (CubeType == CUBE_TYPE::CUBE_NONE)
			CubeType = CUBE_TYPE::CUBE_MAPTOOL;

		pMapToolCube->Setting_GameObject(tHandle, vPos, vRot, vScale);
		pMapToolCube->SetCubeType(CubeType);
		pMapToolCube->SetTargetMakeIndex(Cube.at("TARGETMAKEINDEX").get<int>());

		_bool HorFilpArr[CUBE_FACE_END];

		memset(HorFilpArr, false , sizeof(bool)*CUBE_FACE_END );

		JsonFunction::Load_Cube_HorizontalFilp(Cube, HorFilpArr);
		pMapToolCube->SetCubeHorizontalFilpArr(HorFilpArr);

		_bool VerFilpArr[CUBE_FACE_END];
		memset(VerFilpArr, false, sizeof(bool) * CUBE_FACE_END);
		JsonFunction::Load_Cube_VerticalFilp(Cube, VerFilpArr);
		pMapToolCube->SetCubeVerticalFilpArr(VerFilpArr);

		_vec2 vCenterUV[CUBE_FACE_END];
		JsonFunction::Load_Cube_CenterUV(Cube,vCenterUV);

		for (int i = 0; i < CUBE_FACE_END; ++i)
		{
			pMapToolCube->Change_CubeTextureFaceUV((CUBE_FACE)i, vCenterUV[i]);
		}

		pMapToolCube->SemiUpdate();

		CCubeManager::Add_Cube(pMapToolCube->GetFloor(), pMapToolCube);
	
		CTileManager::GetInstance()->SetOnTopCube_By_MakeIndex(tHandle.m_eFloor,
			pMapToolCube->GetTargetMakeIndex()
		);

		index = pMapToolCube->Get_HandleIndex();
	}

	m_iIndex = index +1 ;
}



void CCubeManager::SetFloor(const FLOOR& eFloor)
{
	m_eCurFloor = eFloor;

	Delete_BufferInfoCube();
	CCollisionManager::GetInstance()->Add_ObjectVector(OBJTYPE::OBJ_CUBE, &m_vCube[m_eCurFloor]);
}

void CCubeManager::Add_Cube(CGameObject* CObj)
{
	m_vCube[m_eCurFloor].push_back(CObj);

	CMapToolCube* Cube = static_cast<CMapToolCube*>(CObj);

	for (int i = 0; i < CUBE_FACE::CUBE_FACE_END; ++i)
	{
		Cube->Change_CubeTextureFaceUV((CUBE_FACE)i, m_vDefaultCubeUV);
	}

	CCollisionManager::GetInstance()->Add_ObjectVector(OBJTYPE::OBJ_CUBE, &m_vCube[m_eCurFloor]);
}

void CCubeManager::Add_Cube(const FLOOR& eFloor, CGameObject* CObj)
{
	m_vCube[CObj->GetFloor()].push_back(CObj);

	CCollisionManager::GetInstance()->Add_ObjectVector(OBJTYPE::OBJ_CUBE, &m_vCube[m_eCurFloor]);
}

void CCubeManager::Delete_BufferCube()
{
	//생성되었던 버퍼 렉트 지우기?
	if (m_pBufferCube != nullptr)
	{
		Safe_Release(m_pBufferCube);
		m_pBufferCube = nullptr;
	}
}

void CCubeManager::Delete_BufferInfoCube()
{
	//생성되었던 버퍼 렉트 지우기?
	if (m_pBufferInfoCube != nullptr)
	{
		Safe_Release(m_pBufferInfoCube);
		m_pTargetObject = nullptr;
		m_pBufferInfoCube = nullptr;
	}
}

const _vec3& CCubeManager::GetBufferCubePos()
{
	if (m_pBufferCube == nullptr)
		return _vec3({0.f,0.f,0.f});
	

	return *m_pBufferCube->Get_Transform()->GetInfoPos();
}

const _vec2& CCubeManager::GetCurCenterUV()
{
	return m_vCurCenterUV;
}

void CCubeManager::Delete_Cube(CMapToolCube* pCube)
{
	auto iter = find(m_vCube[m_eCurFloor].begin(), m_vCube[m_eCurFloor].end(), pCube);

	if (iter != m_vCube[m_eCurFloor].end())
	{
		if (m_pBufferInfoCube != nullptr && m_pBufferInfoCube->Get_Target() != nullptr)
		{
			// Info버퍼 큐브가 가지고 있던 타겟 큐브가 제거대상일때 
			if (pCube == m_pBufferInfoCube->Get_Target())
			{
				Delete_BufferInfoCube();
			
				m_pTargetObject = nullptr;
			}
		}

		
		Setting_HandleIndex_AterDelet(pCube->Get_HandleIndex());

		Safe_Release(*iter);
		m_vCube[m_eCurFloor].erase(iter);
	}

	CCollisionManager::GetInstance()->Add_ObjectVector(OBJTYPE::OBJ_CUBE, &m_vCube[m_eCurFloor]);
}

void CCubeManager::Setting_HandleIndex_AterDelet(const _int& index)
{
	for (int floor = 0; floor < FLOOR::FLOOR_END; ++floor)
	{
		for (size_t j = 0; j < m_vCube[floor].size(); ++j)
		{
			int Buffer_Index = m_vCube[floor][j]->Get_HandleIndex();

			if (Buffer_Index > index)
			{
				m_vCube[floor][j]->Set_HandleIndex(Buffer_Index - 1);
			}
		}
	}

	// 삭제로 전역 index 하나 줄이기
	--m_iIndex;
}

void CCubeManager::Free()
{
	for (size_t i = 0; i < FLOOR::FLOOR_END; ++i)
	{
		for (auto iter = m_vCube[i].begin(); iter != m_vCube[i].end(); )
		{
			if (*iter != nullptr)
			{
				Safe_Release(*iter);
				iter = m_vCube[i].erase(iter);
			}
			else
				++iter;
		}
	}

	if (m_pBufferInfoCube != nullptr)
	{
		Safe_Release(m_pBufferInfoCube);
	}


	if (m_pBufferCube != nullptr)
	{
		Safe_Release(m_pBufferInfoCube);
	}
}


void	CCubeManager::SetActive(const ACTIVE_OBJECT& eType, const _bool& isActive)
{
	CUBE_TYPE TargetType = CUBE_TYPE::CUBE_UNKNOWN;

	if (eType == ACTIVE_OBJECT::CUBE)
		TargetType = CUBE_TYPE::CUBE_MAPTOOL;
	else if (eType == ACTIVE_OBJECT::EVENTCUBE)
		TargetType = CUBE_TYPE::CUBE_EVENT;
	else if (eType == ACTIVE_OBJECT::CREATURECANTGOCUBE)
		TargetType = CUBE_TYPE::CUBE_CREATURECANTGO;
	else
		return;

	for (int i = 0; i < FLOOR::FLOOR_END; ++i)
	{
		for (auto Cube : m_vCube[i])
		{
			//타입검사를 진행해서 Active를 꺼준다 아마 렌더도안되고 다 안될듯?
			CUBE_TYPE Type = static_cast<CMapToolCube*>(Cube)->GetCubeType();
			if (Type == TargetType)
				Cube->Set_Active(isActive);
		}
	}

	CCollisionManager::GetInstance()->Add_ObjectVector(OBJTYPE::OBJ_CUBE, &m_vCube[m_eCurFloor]);
}

bool CCubeManager::IsHaveLinkedEventHandle(const _int& index)
{
	for (int i = 0; i < FLOOR::FLOOR_END; ++i)
	{
		for (auto Cube : m_vCube[i])
		{
			if (Cube->Get_HandleEventIndex() == index)
			{
				return true;
			}
		}
	}

	return false;
}
