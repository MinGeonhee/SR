#include "CMapObjectManager.h"
#include "CRendererManager.h"
#include "CCollisionManager.h"
#include "CGraphicDev.h"
#include "CMapToolMapObject.h"
#include "CMapToolDecor.h"
#include "CMapToolEventObject.h"
#include "CMapToolObstacle.h"
#include "CMapToolItem.h"
#include "CMapToolMonster.h"
#include "CEnumStringFunction.h"
#include "CMapToolDoor.h"
#include "CMapToolLight.h"
#include "CTransform.h"
#include "JsonFunction.h"
#include "CResourceManager.h"
#include "CMapObjectManager.h"

IMPLEMENT_SINGLETON(CMapObjectManager)


CMapObjectManager::CMapObjectManager()
	: m_eCurFloor(FLOOR::FLOOR_3F) , m_sSettingRoomNum(0) , m_pBufferMapObject(nullptr)
	 , m_eCurCategory(MAPOBJECT_CATEGORY::_END), m_eCurPlacement(MAPOBJECT_PLACEMENT::_END) , m_pInfoMapObject(nullptr) ,  m_eCurItemType(ITEMTYPE::ITEM_NONE)
	, m_eCurEventPath(EVENT_PATH::UNKNOWN) , m_isAllRender(false) , m_eCurMonsterType(MONSTER_TYPE::MONSTER_END) , m_eCurDoorType(DOORTYPE::NONE_DOOR)
{ 
	//인덱스 번호 초기화
	for (auto i = 0; i < (int)MAPOBJECT_CATEGORY::_END; ++i)
		m_iIndex[i] = 0;
}

void CMapObjectManager::Init()
{
	for (auto& arr : m_arrayMapObject)
	{
		for (auto& Vec : arr)
		{
			Vec.clear();
		}
	}

	m_eCurFloor = FLOOR::FLOOR_3F;
	//인덱스 번호 초기화
	for (auto i = 0; i < (int)MAPOBJECT_CATEGORY::_END; ++i)
		m_iIndex[i] = 0;	m_sSettingRoomNum = 0;

	m_pBufferMapObject = nullptr;

}

void CMapObjectManager::MapObjectManager_Update(const _float& DT)
{
	for (auto& arr : m_arrayMapObject)
	{
		for (auto& Vec : arr)
		{
			for (auto MapObject : Vec)
			{
				MapObject->Update_GameObject(DT);
			}
		}
	}

	if (m_pBufferMapObject != nullptr)
		m_pBufferMapObject->Update_GameObject(DT);
}

void CMapObjectManager::MapObjectManager_LateUpdate(const _float& DT)
{
	for (auto& arr : m_arrayMapObject)
	{
		for (auto& Vec : arr)
		{
			for (auto MapObject : Vec)
			{
				MapObject->LateUpdate_GameObject(DT);
			}
		}
	}

	if( m_pBufferMapObject != nullptr)
		m_pBufferMapObject->LateUpdate_GameObject(DT);
	else if (m_pBufferMapObject == nullptr)
		int i = 0;
}

void CMapObjectManager::MapObjectManager_Render(LPDIRECT3DDEVICE9 Device)
{
	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for (auto& arr : m_arrayMapObject)
	{
		for (auto& Vec : arr)
		{
			for (auto MapObject : Vec)
			{
				if (MapObject->Get_Active() != false)
				{
					if(m_isAllRender == true || MapObject->GetFloor()  == m_eCurFloor )
						MapObject->Render_GameObject();
				}
			}
		}
	}

	if (m_pBufferMapObject != nullptr)
		m_pBufferMapObject->Render_GameObject();

	Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CMapObjectManager::Add_BufferMapObject(const _vec3& vPos, const _vec3& vRot)
{
	if (m_eCurCategory == MAPOBJECT_CATEGORY::DECOR)
	{
		m_pBufferMapObject = CMapToolDecor::Create(CGraphicDev::GetInstance()->m_pGraphicDev);
	}
	else if (m_eCurCategory == MAPOBJECT_CATEGORY::OBSTACLE)
	{
		m_pBufferMapObject = CMapToolObstacle::Create(CGraphicDev::GetInstance()->m_pGraphicDev);
	}
	else if (m_eCurCategory == MAPOBJECT_CATEGORY::EVENT)
	{
		m_pBufferMapObject = CMapToolEventObject::Create(CGraphicDev::GetInstance()->m_pGraphicDev);
	}
	else if (m_eCurCategory == MAPOBJECT_CATEGORY::ITEM)
	{
		CMapToolItem* pMapItem = CMapToolItem::Create(CGraphicDev::GetInstance()->m_pGraphicDev);
		pMapItem->Set_ItemType(m_eCurItemType);
		m_pBufferMapObject = pMapItem;
	}
	else if (m_eCurCategory == MAPOBJECT_CATEGORY::MONSTER)
	{
		CMapToolMonster* pMapMonster = CMapToolMonster::Create(CGraphicDev::GetInstance()->m_pGraphicDev);
		m_pBufferMapObject = pMapMonster;
	}
	else if (m_eCurCategory == MAPOBJECT_CATEGORY::DOOR)
	{
		CMapToolDoor* pMapDoor = CMapToolDoor::Create(CGraphicDev::GetInstance()->m_pGraphicDev);
		m_pBufferMapObject = pMapDoor;
	}
	else if (m_eCurCategory == MAPOBJECT_CATEGORY::LIGHT)
	{
		CMapToolLight* pMapLgiht = CMapToolLight::Create(CGraphicDev::GetInstance()->m_pGraphicDev);
		m_pBufferMapObject = pMapLgiht;
	}
	else
		return;

	if (m_pBufferMapObject == nullptr)
		return;


	ObjectHandle tHandle;
	tHandle.m_eFloor = m_eCurFloor;
	tHandle.m_sEventIndex = 0;
	tHandle.m_sRoomNum = m_sSettingRoomNum;
	_vec3 vScale = { 1.f,1.f,1.f };


	m_pBufferMapObject->CMapToolMapObject::Setting(tHandle, vPos , vRot, vScale ,m_eCurCategory ,m_eCurPlacement ,m_tCurSpriteRect);

}

void CMapObjectManager::Add_MapObject(const _vec3& vPos, const _vec3& vRot)
{
	CMapToolMapObject* newObj = nullptr;
	OBJTYPE eType;

	int EventHandleIndex = 0;

	if (m_eCurCategory == MAPOBJECT_CATEGORY::DECOR)
	{
		newObj = CMapToolDecor::Create(CGraphicDev::GetInstance()->m_pGraphicDev);
		eType = OBJTYPE::OBJ_MAPDECOR;
		
	}
	else if (m_eCurCategory == MAPOBJECT_CATEGORY::OBSTACLE)
	{
		newObj = CMapToolObstacle::Create(CGraphicDev::GetInstance()->m_pGraphicDev);
		eType = OBJTYPE::OBJ_MAPOBSTACLE;
	}
	else if (m_eCurCategory == MAPOBJECT_CATEGORY::EVENT)
	{
		newObj = CMapToolEventObject::Create(CGraphicDev::GetInstance()->m_pGraphicDev);
		eType = OBJTYPE::OBJ_MAPEVENT;
	}
	else if (m_eCurCategory == MAPOBJECT_CATEGORY::ITEM)
	{
		CMapToolItem* pMapItem = CMapToolItem::Create(CGraphicDev::GetInstance()->m_pGraphicDev);
		pMapItem->Set_ItemType(m_eCurItemType);
		newObj = pMapItem;
		eType = OBJTYPE::OBJ_ITEM;
	}
	else if (m_eCurCategory == MAPOBJECT_CATEGORY::MONSTER)
	{
		CMapToolMonster* pMapMonster = CMapToolMonster::Create(CGraphicDev::GetInstance()->m_pGraphicDev);
		pMapMonster->Set_MonsterType(m_eCurMonsterType);
		newObj = pMapMonster;
		eType = OBJTYPE::OBJ_MONSTER;
	}
	else if (m_eCurCategory == MAPOBJECT_CATEGORY::DOOR)
	{
		CMapToolDoor* pMapDoor = CMapToolDoor::Create(CGraphicDev::GetInstance()->m_pGraphicDev);
		pMapDoor->Set_DoorType(m_eCurDoorType);

		EventHandleIndex  = SetDoorEventHandleIndex(pMapDoor->Get_DoorType());

		newObj = pMapDoor;
		eType = OBJTYPE::OBJ_DOOR;
	}
	else  if (m_eCurCategory == MAPOBJECT_CATEGORY::LIGHT)
	{
		CMapToolLight* pMapLight = CMapToolLight::Create(CGraphicDev::GetInstance()->m_pGraphicDev);
		pMapLight->Set_LightType(m_eCurLightType);
		newObj = pMapLight;
		eType = OBJTYPE::OBJ_LIGHT;
	}

	else
		return;


	if (newObj == nullptr)
		return;

	ObjectHandle tHandle;

	tHandle.m_eType = eType;
	tHandle.m_iIndex = m_iIndex[(int)m_eCurCategory]++;
	tHandle.m_eFloor = m_eCurFloor;
	tHandle.m_sEventIndex = EventHandleIndex;
	tHandle.m_sRoomNum = m_sSettingRoomNum;

	_vec3 vScale = { 1.f,1.f,1.f };

	newObj->CMapToolMapObject::Setting(tHandle, vPos, vRot , vScale,m_eCurCategory, m_eCurPlacement ,m_tCurSpriteRect);
	newObj->Set_EventPath(m_eCurEventPath);
	m_arrayMapObject[(int)m_eCurCategory][m_eCurFloor].push_back(newObj);

	CCollisionManager::GetInstance()->Add_ObjectVector(newObj->Get_OBJTYPE(), &m_arrayMapObject[(int)m_eCurCategory][m_eCurFloor]);

}

void CMapObjectManager::Add_MapObject(CGameObject* pObj)
{
	auto Type = pObj->Get_OBJTYPE();

	if (Type != OBJTYPE::OBJ_MAPDECOR || Type != OBJTYPE::OBJ_MAPOBSTACLE || Type != OBJTYPE::OBJ_MAPEVENT)
		return;

	CMapObject* pMapObject = static_cast<CMapObject*>(pObj);

	m_arrayMapObject[(int)pMapObject->Get_Category()][pMapObject->GetFloor()].push_back(pObj);

	CCollisionManager::GetInstance()->Add_ObjectVector(Type, &m_arrayMapObject[(int)m_eCurCategory][m_eCurFloor]);

	return;
}

void CMapObjectManager::Delete_MapObject(CGameObject* pMapObject)
{
	if (pMapObject == m_pInfoMapObject)
		m_pInfoMapObject = nullptr;


	OBJTYPE Type = pMapObject->Get_OBJTYPE();

	auto MapObj = static_cast<CMapToolMapObject*>(pMapObject);

	auto Categori = MapObj->Get_Category();
	auto index = MapObj->Get_HandleIndex();

	auto& Vec = m_arrayMapObject[(int)Categori][m_eCurFloor];
	
	
	auto iter = find_if(Vec.begin(), Vec.end(), [&](CGameObject* MapObject){
		return(MapObject == pMapObject);
		});

	if (iter == Vec.end())
		return;

	Safe_Release(*iter);
	Vec.erase(iter);


	CCollisionManager::GetInstance()->Add_ObjectVector(Type, &m_arrayMapObject[(int)Categori][m_eCurFloor]);


	Setting_HandleIndex_AterDelete(Categori, index);
}

const _int& CMapObjectManager::SetDoorEventHandleIndex(const DOORTYPE& eType)
{
	//여기서 인덱스 값을 디폴트로 정해주는건 index값으로 생성을 하기위함이아닌
//이 인덱스 번호를 사용하여 이벤트를 주고받으니까 이이벤트 값 외에 값으로 벗어나게하지 않게하기 위함이다

	if (eType == DOORTYPE::BRONZE_DOOR) return 1;
	if (eType == DOORTYPE::SILVER_DOOR) return 2;
	if (eType == DOORTYPE::GOLD_DOOR)   return 3;
	if (eType == DOORTYPE::BOSS_DOOR)   return 4;

	if (eType == DOORTYPE::DEFAULT_WOOD_DOOR) return 5;
	if (eType == DOORTYPE::DEFAULT_IRON_DOOR) return 6;


	if (eType == DOORTYPE::SWITCH_IRON_DOOR || eType == DOORTYPE::SWITCH_WOOD_DOOR)
	{
		vector<_int> IndexVec;

		auto& DoorVec = m_arrayMapObject[(int)MAPOBJECT_CATEGORY::DOOR];

		for (int i = 0; i < FLOOR::FLOOR_END; ++i)
		{

			if (DoorVec[i].empty()) continue;

			for (auto Door : DoorVec[i])
			{
				auto eDoorType = static_cast<CMapToolDoor*>(Door)->Get_DoorType();

				if (eDoorType != DOORTYPE::SWITCH_IRON_DOOR && eDoorType != DOORTYPE::SWITCH_WOOD_DOOR) continue;

				IndexVec.push_back(Door->Get_HandleEventIndex());

			}
		}

		if (!IndexVec.empty())
		{
			std::sort(IndexVec.begin(), IndexVec.end());

			int Start = IndexVec.front();

			for (int i = 0; i < IndexVec.size(); ++i)
			{
				if (IndexVec[i] == Start)
					++Start;
				else if (i > Start)
					return Start;
			}
			return Start;
		}

		return 1501;
	}

	return 0;
}

CMapToolMapObject* CMapObjectManager::CreateMapObject_ByCategori(const MAPOBJECT_CATEGORY& eType, EVENT_PATH eEventPath, ITEMTYPE  eItemType , MONSTER_TYPE eMonsterType , DOORTYPE eDoorType)
{
	CMapToolMapObject* newObj = nullptr;

	switch (eType)
	{
	case Engine::MAPOBJECT_CATEGORY::DECOR:
		newObj = CMapToolDecor::Create(CGraphicDev::GetInstance()->m_pGraphicDev);
		break;
	case Engine::MAPOBJECT_CATEGORY::OBSTACLE:
		newObj = CMapToolObstacle::Create(CGraphicDev::GetInstance()->m_pGraphicDev);
		break;
	case Engine::MAPOBJECT_CATEGORY::EVENT:
		newObj = CMapToolEventObject::Create(CGraphicDev::GetInstance()->m_pGraphicDev);
		newObj->Set_EventPath(eEventPath);
		break;
	case Engine::MAPOBJECT_CATEGORY::ITEM:
		newObj = CMapToolItem::Create(CGraphicDev::GetInstance()->m_pGraphicDev);
		static_cast<CMapToolItem*>(newObj)->Set_ItemType(eItemType);
		break;
	case Engine::MAPOBJECT_CATEGORY::MONSTER:
		newObj = CMapToolMonster::Create(CGraphicDev::GetInstance()->m_pGraphicDev);
		static_cast<CMapToolMonster*>(newObj)->Set_MonsterType(eMonsterType);
		break;
	case Engine::MAPOBJECT_CATEGORY::DOOR:
		newObj = CMapToolDoor::Create(CGraphicDev::GetInstance()->m_pGraphicDev);
		static_cast<CMapToolDoor*>(newObj)->Set_DoorType(eDoorType);
	default:
		return nullptr;
	}

	return newObj;
}

CMapToolMapObject* CMapObjectManager::CreateMapObject_ByCategori(const MAPOBJECT_CATEGORY& eType, void* eMapObj_Type)
{
	CMapToolMapObject* newObj = nullptr;

	switch (eType)
	{
	case Engine::MAPOBJECT_CATEGORY::DECOR:				newObj = CMapToolDecor::Create(CGraphicDev::GetInstance()->m_pGraphicDev); break;
	case Engine::MAPOBJECT_CATEGORY::OBSTACLE:	 		newObj = CMapToolObstacle::Create(CGraphicDev::GetInstance()->m_pGraphicDev); break;
	case Engine::MAPOBJECT_CATEGORY::EVENT:
		newObj = CMapToolEventObject::Create(CGraphicDev::GetInstance()->m_pGraphicDev);
		newObj->Set_EventPath( *static_cast<EVENT_PATH*>(eMapObj_Type)  );
		break;
	case Engine::MAPOBJECT_CATEGORY::ITEM:
		newObj = CMapToolItem::Create(CGraphicDev::GetInstance()->m_pGraphicDev);
		static_cast<CMapToolItem*>(newObj)->Set_ItemType(*static_cast<ITEMTYPE*>(eMapObj_Type));
		break;
	case Engine::MAPOBJECT_CATEGORY::MONSTER:
		newObj = CMapToolMonster::Create(CGraphicDev::GetInstance()->m_pGraphicDev);
		static_cast<CMapToolMonster*>(newObj)->Set_MonsterType(*static_cast<MONSTER_TYPE*>(eMapObj_Type));
		break;
	case Engine::MAPOBJECT_CATEGORY::DOOR:
		newObj = CMapToolDoor::Create(CGraphicDev::GetInstance()->m_pGraphicDev);
		static_cast<CMapToolDoor*>(newObj)->Set_DoorType(*static_cast<DOORTYPE*>(eMapObj_Type));
		break;
	case Engine::MAPOBJECT_CATEGORY::LIGHT:
		newObj = CMapToolLight::Create(CGraphicDev::GetInstance()->m_pGraphicDev);
		static_cast<CMapToolLight*>(newObj)->Set_LightType(*static_cast<LIGHT_OBJECT_TYPE*>(eMapObj_Type));
		break;
	case Engine::MAPOBJECT_CATEGORY::_END:
		break;
	default:
		break;
	}

	return newObj;
}

void CMapObjectManager::SetFloor(const FLOOR& eFloor)
{
	m_eCurFloor = eFloor;

	Delete_BufferMapObject();
	
	CCollisionManager::GetInstance()->Add_ObjectVector(OBJTYPE::OBJ_MAPDECOR,    &m_arrayMapObject[(int)MAPOBJECT_CATEGORY::DECOR]	  [m_eCurFloor]);
	CCollisionManager::GetInstance()->Add_ObjectVector(OBJTYPE::OBJ_MAPOBSTACLE, &m_arrayMapObject[(int)MAPOBJECT_CATEGORY::OBSTACLE] [m_eCurFloor]);
	CCollisionManager::GetInstance()->Add_ObjectVector(OBJTYPE::OBJ_MAPEVENT,	 &m_arrayMapObject[(int)MAPOBJECT_CATEGORY::EVENT]	  [m_eCurFloor]);
	CCollisionManager::GetInstance()->Add_ObjectVector(OBJTYPE::OBJ_ITEM,		 &m_arrayMapObject[(int)MAPOBJECT_CATEGORY::ITEM]	  [m_eCurFloor]);
	CCollisionManager::GetInstance()->Add_ObjectVector(OBJTYPE::OBJ_MONSTER,	 &m_arrayMapObject[(int)MAPOBJECT_CATEGORY::MONSTER]  [m_eCurFloor]);
	CCollisionManager::GetInstance()->Add_ObjectVector(OBJTYPE::OBJ_DOOR,		 &m_arrayMapObject[(int)MAPOBJECT_CATEGORY::DOOR]	  [m_eCurFloor]);
	CCollisionManager::GetInstance()->Add_ObjectVector(OBJTYPE::OBJ_LIGHT,		 &m_arrayMapObject[(int)MAPOBJECT_CATEGORY::LIGHT]	  [m_eCurFloor]);
}

void CMapObjectManager::Setting_HandleIndex_AterDelete(const MAPOBJECT_CATEGORY eType, const _int& index)
{
	auto& Vec = m_arrayMapObject[(int)eType];

	for (int i = 0; i < FLOOR::FLOOR_END; ++i)          // 층 전체 순회
	{
		for (size_t j = 0; j < Vec[i].size(); ++j)      // 안전하게 0부터 끝까지
		{
			int Buffer_Index = Vec[i][j]->Get_HandleIndex();

			if (Buffer_Index > index)
				Vec[i][j]->Set_HandleIndex(Buffer_Index - 1);
		}
	}

	--m_iIndex[(int)eType];  // 다음 할당 index 감소
}

void CMapObjectManager::SaveMapObjectToJson(nlohmann::ordered_json& j, const string& sFileName)
{
	auto& DecorVec =	m_arrayMapObject[(int)MAPOBJECT_CATEGORY::DECOR];
	auto& ObstacleVec = m_arrayMapObject[(int)MAPOBJECT_CATEGORY::OBSTACLE];
	auto& EvnetVec =	m_arrayMapObject[(int)MAPOBJECT_CATEGORY::EVENT];
	auto& ItemVec =		m_arrayMapObject[(int)MAPOBJECT_CATEGORY::ITEM];
	auto& MonsterVec =	m_arrayMapObject[(int)MAPOBJECT_CATEGORY::MONSTER];
	auto& DoorVec =		m_arrayMapObject[(int)MAPOBJECT_CATEGORY::DOOR];
	auto& LightVec =	m_arrayMapObject[(int)MAPOBJECT_CATEGORY::LIGHT];

	for (int i = 0; i < FLOOR::FLOOR_END; ++i)
	{
		for (auto& GameDecor : DecorVec[i])
		{
			auto Decor = static_cast<CMapToolDecor*>(GameDecor);

			ObjectHandle tHandle = Decor->Get_Handle();
			auto vPos = *Decor->Get_Transform()->GetInfoPos();
			auto vRot = Decor->Get_Transform()->Get_Angle();
			auto vScale = Decor->Get_Transform()->Get_Scale();


			string str_Cat = CEnumStringFunction::MAPOBJECT_CATEGORI_EnumToString(Decor->Get_Category());
			string str_Placement = CEnumStringFunction::MAPOBJECT_PLACEMENT_EnumToString(Decor->Get_Placement());
			SpriteRect rect = Decor->Get_SpriteRect();

			nlohmann::ordered_json t;

			JsonFunction::Save_ObjectHandle(t, tHandle);
			JsonFunction::Save_Info(t, vPos, vRot, vScale);

			JsonFunction::Save_String(t, "CATEGORI", str_Cat);
			JsonFunction::Save_String(t, "PLACEMENT", str_Placement);
			JsonFunction::Save_SpriteRect(t, rect);


			j["DECOR"].push_back(t);
		}
	}

	for (int i = 0; i < FLOOR::FLOOR_END; ++i)
	{
		for (auto& GameObstacle : ObstacleVec[i])
		{
			auto Obstacle = static_cast<CMapToolObstacle*>(GameObstacle);

			ObjectHandle tHandle = Obstacle->Get_Handle();
			auto vPos = *Obstacle->Get_Transform()->GetInfoPos();
			auto vRot = Obstacle->Get_Transform()->Get_Angle();
			auto vScale = Obstacle->Get_Transform()->Get_Scale();


			string str_Cat = CEnumStringFunction::MAPOBJECT_CATEGORI_EnumToString(Obstacle->Get_Category());
			string str_Placement = CEnumStringFunction::MAPOBJECT_PLACEMENT_EnumToString(Obstacle->Get_Placement());
			SpriteRect rect = Obstacle->Get_SpriteRect();


			nlohmann::ordered_json t;

			JsonFunction::Save_ObjectHandle(t, tHandle);
			JsonFunction::Save_Info(t, vPos, vRot, vScale);

			JsonFunction::Save_String(t,"CATEGORI", str_Cat);
			JsonFunction::Save_String(t,"PLACEMENT", str_Placement);
			JsonFunction::Save_SpriteRect(t, rect);

			j["OBSTACLE"].push_back(t);
		}
	}

	for (int i = 0; i < FLOOR::FLOOR_END; ++i)
	{
		for (auto& GameEvent : EvnetVec[i])
		{
			auto Event = static_cast<CMapToolEventObject*>(GameEvent);

			ObjectHandle tHandle = Event->Get_Handle();
			auto vPos = *Event->Get_Transform()->GetInfoPos();
			auto vRot = Event->Get_Transform()->Get_Angle();
			auto vScale = Event->Get_Transform()->Get_Scale();
			

			string str_Cat = CEnumStringFunction::MAPOBJECT_CATEGORI_EnumToString(Event->Get_Category());
			string str_Placement = CEnumStringFunction::MAPOBJECT_PLACEMENT_EnumToString(Event->Get_Placement());
			SpriteRect rect = Event->Get_SpriteRect();
			_int index = Event->GetBufferInt();



			nlohmann::ordered_json t;

			JsonFunction::Save_ObjectHandle(t, tHandle);
			JsonFunction::Save_Info(t, vPos, vRot, vScale);

			JsonFunction::Save_String(t, "CATEGORI", str_Cat);
			JsonFunction::Save_String(t, "PLACEMENT", str_Placement);
			JsonFunction::Save_SpriteRect(t, rect);
			if (Event->Get_Category() == MAPOBJECT_CATEGORY::EVENT)
			{
				JsonFunction::Save_EventPath(t, Event->Get_EventPath());
				t["BUFFERINDEX"] = index;
			}
			

			

			j["EVENT"].push_back(t);
		}
	}

	for (int i = 0; i < FLOOR::FLOOR_END; ++i)
	{
		for (auto& GameItem : ItemVec[i])
		{
			auto Item = static_cast<CMapToolItem*>(GameItem);

			ObjectHandle tHandle = Item->Get_Handle();
			auto vPos = *Item->Get_Transform()->GetInfoPos();
			auto vRot = Item->Get_Transform()->Get_Angle();
			auto vScale = Item->Get_Transform()->Get_Scale();


			string str_Cat = CEnumStringFunction::MAPOBJECT_CATEGORI_EnumToString(Item->Get_Category());
			string str_Placement = CEnumStringFunction::MAPOBJECT_PLACEMENT_EnumToString(Item->Get_Placement());
			SpriteRect rect = Item->Get_SpriteRect();


			nlohmann::ordered_json t;

			JsonFunction::Save_ObjectHandle(t, tHandle);
			JsonFunction::Save_Info(t, vPos, vRot, vScale);

			JsonFunction::Save_String(t, "CATEGORI", str_Cat);
			JsonFunction::Save_String(t, "PLACEMENT", str_Placement);
			JsonFunction::Save_SpriteRect(t, rect);
		
			t["ITEMTYPE"] = CEnumStringFunction::ITEMTYPE_Enum_To_String(Item->Get_ItemType());

			j["ITEM"].push_back(t);
			
		}
	}

	for (int i = 0; i < FLOOR::FLOOR_END; ++i)
	{
		for (auto& GameMonster : MonsterVec[i])
		{
			auto pMonster = static_cast<CMapToolMonster*>(GameMonster);

			ObjectHandle tHandle = pMonster->Get_Handle();
			auto vPos = *pMonster->Get_Transform()->GetInfoPos();
			auto vRot = pMonster->Get_Transform()->Get_Angle();
			auto vScale = pMonster->Get_Transform()->Get_Scale();


			string str_Cat = CEnumStringFunction::MAPOBJECT_CATEGORI_EnumToString(pMonster->Get_Category());
			string str_Placement = CEnumStringFunction::MAPOBJECT_PLACEMENT_EnumToString(pMonster->Get_Placement());
			SpriteRect rect = pMonster->Get_SpriteRect();


			nlohmann::ordered_json t;

			JsonFunction::Save_ObjectHandle(t, tHandle);
			JsonFunction::Save_Info(t, vPos, vRot, vScale);

			JsonFunction::Save_String(t, "CATEGORI", str_Cat);
			JsonFunction::Save_String(t, "PLACEMENT", str_Placement);
			JsonFunction::Save_SpriteRect(t, rect);

			t["MONSTERTYPE"] = CEnumStringFunction::MONSTER_TYPE_Enum_To_String(pMonster->Get_MonsterType());

			j["MONSTER"].push_back(t);
		}
	}

	for (int i = 0; i < FLOOR::FLOOR_END; ++i)
	{
		for (auto& GameDoor : DoorVec[i])
		{
			auto pDoor = static_cast<CMapToolDoor*>(GameDoor);

			ObjectHandle tHandle = GameDoor->Get_Handle();
			auto vPos = *GameDoor->Get_Transform()->GetInfoPos();
			auto vRot = GameDoor->Get_Transform()->Get_Angle();
			auto vScale = GameDoor->Get_Transform()->Get_Scale();


			string str_Cat = CEnumStringFunction::MAPOBJECT_CATEGORI_EnumToString(pDoor->Get_Category());
			string str_Placement = CEnumStringFunction::MAPOBJECT_PLACEMENT_EnumToString(pDoor->Get_Placement());
			SpriteRect rect = pDoor->Get_SpriteRect();


			nlohmann::ordered_json t;

			JsonFunction::Save_ObjectHandle(t, tHandle);
			JsonFunction::Save_Info(t, vPos, vRot, vScale);

			JsonFunction::Save_String(t, "CATEGORI", str_Cat);
			JsonFunction::Save_String(t, "PLACEMENT", str_Placement);
			JsonFunction::Save_SpriteRect(t, rect);

			t["DOORTYPE"] = CEnumStringFunction::DOORTYPE_Enum_To_String(pDoor->Get_DoorType());
			t["SWITCH_COUNT"] = pDoor->Get_DoorSwitchCount();

			j["DOOR"].push_back(t);
		}
	}

	for (int i = 0; i < FLOOR::FLOOR_END; ++i)
	{
		for (auto& GameLight : LightVec[i])
		{
			auto pLight = static_cast<CMapToolLight*>(GameLight);

			ObjectHandle tHandle = GameLight->Get_Handle();
			auto vPos = *GameLight->Get_Transform()->GetInfoPos();
			auto vRot = GameLight->Get_Transform()->Get_Angle();
			auto vScale = GameLight->Get_Transform()->Get_Scale();


			string str_Cat = CEnumStringFunction::MAPOBJECT_CATEGORI_EnumToString(pLight->Get_Category());
			string str_Placement = CEnumStringFunction::MAPOBJECT_PLACEMENT_EnumToString(pLight->Get_Placement());
			SpriteRect rect = pLight->Get_SpriteRect();


			nlohmann::ordered_json t;

			JsonFunction::Save_ObjectHandle(t, tHandle);
			JsonFunction::Save_Info(t, vPos, vRot, vScale);

			JsonFunction::Save_String(t, "CATEGORI", str_Cat);
			JsonFunction::Save_String(t, "PLACEMENT", str_Placement);
			JsonFunction::Save_SpriteRect(t, rect);

			t["LIGHT_OBJECT_TYPE"] = CEnumStringFunction::LIGHT_OBJECT_TYPE_Enum_To_String(pLight->Get_LightType());

			j["LIGHT"].push_back(t);
		}
	}

}

void CMapObjectManager::LoadMapObjectFromJson(const string& sFilePath)
{
	//싹다 지우고 다시 깔기
	Free();

	string FilePath = sFilePath;


	ifstream is(sFilePath);

	nlohmann::json jLoad;

	//파일 열지못했다면 리턴
	if (!is.is_open())
		return;

	is >> jLoad;


	std::vector<std::pair<MAPOBJECT_CATEGORY, std::string>> MapObject = {
	{ MAPOBJECT_CATEGORY::DECOR,   "DECOR" },
	{ MAPOBJECT_CATEGORY::OBSTACLE,"OBSTACLE" },
	{ MAPOBJECT_CATEGORY::EVENT,   "EVENT" },
	{ MAPOBJECT_CATEGORY::ITEM,    "ITEM" },
	{ MAPOBJECT_CATEGORY::MONSTER, "MONSTER" },
	{ MAPOBJECT_CATEGORY::DOOR , "DOOR" },
	{ MAPOBJECT_CATEGORY::LIGHT , "LIGHT" }
	};


	for (auto& [Cat, Key] : MapObject)
	{
		auto& vec = m_arrayMapObject[(int)Cat];
		const nlohmann::json& MapObjectVec = jLoad.value(Key, nlohmann::json::array());

		int index = 0;

		for (auto& Object : MapObjectVec)
		{
			//먼저 생성을위해 Categori 들고온다
			ObjectHandle vHandle = JsonFunction::Load_ObjectHandle(Object);

			_vec3 vPos = JsonFunction::Load_Info(Object, INFO_TYPE::INFO_TYPE_POS);
			_vec3 vRot = JsonFunction::Load_Info(Object, INFO_TYPE::INFO_TYPE_ROT);
			_vec3 vScale = JsonFunction::Load_Info(Object, INFO_TYPE::INFO_TYPE_SCALE);

			MAPOBJECT_CATEGORY eCat = CEnumStringFunction::MAPOBJECT_CATEGORI_StringToEnum(Object["CATEGORI"].get<string>());
			MAPOBJECT_PLACEMENT ePlacement = CEnumStringFunction::MAPOBJECT_PLACEMENT_StringToEnum(Object["PLACEMENT"].get<string>());
			

			void* Sub_Type = nullptr;
			
			if (Object.contains("EVENTPATH"))
			{
				EVENT_PATH Type = CEnumStringFunction::EVENT_PATH_StringToEnum(Object["EVENTPATH"].get<string>());
				Sub_Type = &Type;
			}
			if (Object.contains("ITEMTYPE"))
			{
				ITEMTYPE Type = CEnumStringFunction::ITEMTYPE_String_To_Enum(Object["ITEMTYPE"].get<string>());
				Sub_Type = &Type;
			}
			if (Object.contains("MONSTERTYPE"))
			{
				MONSTER_TYPE Type = CEnumStringFunction::MONSTER_TYPE_String_To_Enum(Object["MONSTERTYPE"].get<string>());
				Sub_Type = &Type;
			}
			if (Object.contains("DOORTYPE")) // Door관련임
			{
				DOORTYPE Type = CEnumStringFunction::DOORTTYPE_Stirng_To_Enum(Object["DOORTYPE"].get<string>());
				Sub_Type = &Type;
			}
			if (Object.contains("LIGHT_OBJECT_TYPE")) // Door관련임
			{
				LIGHT_OBJECT_TYPE Type = CEnumStringFunction::LIGHT_OBJECT_TYPE_String_To_Enum(Object["LIGHT_OBJECT_TYPE"].get<string>());
				Sub_Type = &Type;
			}

			SpriteRect vSprite = JsonFunction::Load_SpriteRect(Object);
			

			CMapToolMapObject* pMapToolObj = CreateMapObject_ByCategori(eCat, Sub_Type);

			if (pMapToolObj->Get_Category() == MAPOBJECT_CATEGORY::DOOR)
				if (Object.contains("SWITCH_COUNT"))
					static_cast<CMapToolDoor*>(pMapToolObj)->Set_DoorSwitchCount(Object["SWITCH_COUNT"].get<int>());
			if (pMapToolObj->Get_Category() == MAPOBJECT_CATEGORY::EVENT)
			{
				if(Object.contains("BUFFERINDEX"))
					static_cast<CMapToolEventObject*>(pMapToolObj)->SetBufferInt(Object["BUFFERINDEX"].get<int>());
			}

			pMapToolObj->Setting(vHandle, vPos, vRot, vScale, Cat, ePlacement, vSprite);

			pMapToolObj->SemiUpdate();

			vec[pMapToolObj->GetFloor()].push_back(pMapToolObj);

			index = pMapToolObj->Get_HandleIndex();

		}

		m_iIndex[(int)Cat] = index+1;
	}
}


void CMapObjectManager::Free()
{
	for (int cat = 0; cat < (int)MAPOBJECT_CATEGORY::_END; ++cat)
	{
		for (int floor = 0; floor < (int)FLOOR::FLOOR_END; ++floor)
		{
			auto& vec = m_arrayMapObject[cat][floor];

			for (auto& obj : vec)
				Safe_Release(obj);

			// 완전히 비우기 (capacity 초기화)
			vector<CGameObject*>().swap(vec);
		}
	}

	if (m_pBufferMapObject != nullptr)
		Safe_Release(m_pBufferMapObject);
	if (m_pInfoMapObject != nullptr)
		Safe_Release(m_pBufferMapObject);
}



void	CMapObjectManager::SetActive(const ACTIVE_OBJECT& eType, const _bool& isActive)
{
	MAPOBJECT_CATEGORY TargetType = MAPOBJECT_CATEGORY::_END;

	switch (eType)
	{
	case Engine::ACTIVE_OBJECT::DECOR:
		TargetType = MAPOBJECT_CATEGORY::DECOR;
		break;
	case Engine::ACTIVE_OBJECT::OBSTACLE: 
		TargetType = MAPOBJECT_CATEGORY::OBSTACLE;
		break;
	case Engine::ACTIVE_OBJECT::EVENTOBJECT:
		TargetType = MAPOBJECT_CATEGORY::EVENT;
		break;
	case Engine::ACTIVE_OBJECT::ITEM:
		TargetType = MAPOBJECT_CATEGORY::ITEM;
		break;
	case Engine::ACTIVE_OBJECT::MONSTER:
		TargetType = MAPOBJECT_CATEGORY::MONSTER;
		break;
	default: return;
	}

	auto& ObjectVec = m_arrayMapObject[(int)TargetType];

	for (int i = 0; i < FLOOR::FLOOR_END; ++i)
	{
		for (auto& Obj : ObjectVec[i])
		{
			Obj->Set_Active(isActive);
		}
	}


	auto Obj_Type = CEnumStringFunction::CATEGORI_To_OBJTYPE(TargetType);


	CCollisionManager::GetInstance()->Add_ObjectVector(Obj_Type, &m_arrayMapObject[(int)TargetType][m_eCurFloor]);

}
