#include "CObjectManager.h"
#include "CTile.h"
#include "CCube.h"
#include "CEventCube.h"
#include "CTransform.h"
#include "CColliderBase.h"
#include "JsonFunction.h"
#include "CEnumStringFunction.h"
#include "CWallDecor.h"
#include "CGraphicDev.h"
#include "CGroundDecor.h"
#include "PartitionHelper.h"
#include "CCameraManager.h"
#include "CInteractable.h"
#include "CItem.h"
#include "CObjectIDMaker.h"
#include "CPartitionManager.h"
#include "CreatureCantGoCube.h"
#include "CLightObject.h"
#include "CEyeOfChaos.h"

using namespace std;

IMPLEMENT_SINGLETON(CObjectManager)

CObjectManager::CObjectManager() {}
CObjectManager::~CObjectManager() { Free(); }

// 팩토리 등록
void CObjectManager::RegisterFactory(OBJTYPE type, ObjectFactory factory)
{
    m_FactoryMap[type] = factory;
}

void CObjectManager::RegisterEventCubeFactory(const CUBE_EVENT_TYPE& type, const  EventCubeFactory& factory)
{
    m_EventCubeFactoryMap[type] = factory;
}

void CObjectManager::RegisterLightObjectFactory(const LIGHT_OBJECT_TYPE& type, const LightObjectFactory& factory)
{
    m_LigtObjectFactoryMap[type] = factory;
}

void CObjectManager::ObjectBillboard_OnOff(OBJTYPE type, bool b)
{
    auto objectlist = m_mObjectList[type];
    for (auto obj : objectlist)
    {
        if (obj)
            static_cast<CBillboard*>(obj->Get_Component(L"Com_Billboard", ID_DYNAMIC))->Set_Active(b);
    }

    
}


CGameObject* CObjectManager::Get(OBJTYPE type)
{
    auto& object = m_mObjectList[type];
    for (auto& obj : object)
        if (obj)
            return obj;
    return nullptr;
}


void CObjectManager::Preload(OBJTYPE type, int ObjectID,
    float xPos, float yPos, float zPos,
    float xAngle, float yAngle, float zAngle)
{
    auto& object = m_mObjectList[type];

    CGameObject* pObj = nullptr;
    auto it = m_FactoryMap.find(type);
    if (it != m_FactoryMap.end())
    {
        pObj = it->second();
        if (pObj)
        {
            pObj->Set_OBJTYPE(type);
            pObj->Set_Active(true);
            pObj->Get_Transform()->Set_Pos(xPos, yPos, zPos);
            pObj->Get_Transform()->Set_Angle(xAngle, yAngle, zAngle);
            pObj->Set_OriginType(OBJORIGINTYPE::ORIGIN_STATIC);
            pObj->Set_HandleIndex(ObjectID);
        }
    }

    if (pObj)
    {
        object.push_back(pObj);

        // 파티션
        if (type != OBJ_SKYCUBE)
            RegisterObject(pObj);
    }
}

void CObjectManager::PreLoad(const string& sFilePath)
{
    string FilePath = sFilePath;
    nlohmann::json jLoad;

    ifstream is(sFilePath);
    if (!is.is_open()) return;
    is >> jLoad;

    uint32_t index = 0;

    auto Tiles = jLoad.value("TILE", nlohmann::json::array());
    auto Cubes = jLoad.value("CUBE", nlohmann::json::array());

    auto Decors = jLoad.value("DECOR", nlohmann::json::array());
    auto Obstacles = jLoad.value("OBSTACLE", nlohmann::json::array());
    auto Events = jLoad.value("EVENT", nlohmann::json::array());

    auto Items = jLoad.value("ITEM", nlohmann::json::array());
    auto Monsters = jLoad.value("MONSTER", nlohmann::json::array());
    auto Doors = jLoad.value("DOOR", nlohmann::json::array());
    auto Lights = jLoad.value("LIGHT", nlohmann::json::array());

    auto& object_TILE = m_mObjectList[OBJ_TILE];
    auto& object_CUBE = m_mObjectList[OBJ_CUBE];
    auto& object_EVENTCUBE = m_mObjectList[OBJ_EVENTCUBE];
    auto& object_CreatureCantGo = m_mObjectList[OBJ_CREATURECANTGO];
    auto& object_MAPDECOR = m_mObjectList[OBJ_MAPDECOR];
    auto& object_MAPOBSTACLE = m_mObjectList[OBJ_MAPOBSTACLE];
    auto& object_MAPEVENT = m_mObjectList[OBJ_MAPEVENT];

    //맵 오브젝트로 찍혀서 들어오는애들
    auto& object_ITEM = m_mObjectList[OBJ_ITEM];
    auto& object_DOOR = m_mObjectList[OBJ_DOOR];
    auto& object_LIGHT = m_mObjectList[OBJ_LIGHT];

    auto& object_MONSTER_GOBLIN_HAMMER = m_mObjectList[OBJ_MONSTER_GOBLIN_HAMMER];
    auto& object_MONSTER_GOBLIN_FIREBALL = m_mObjectList[OBJ_MONSTER_GOBLIN_FIREBALL];
    auto& object_MONSTER_ORGE_AXE = m_mObjectList[OBJ_MONSTER_ORGE_AXE];
    auto& object_MONSTER_RAT = m_mObjectList[OBJ_MONSTER_RAT];
    auto& object_MONSTER_SKELETON_SWORD = m_mObjectList[OBJ_MONSTER_SKELETON_SWORD];
    auto& object_MONSTER_WIZARD = m_mObjectList[OBJ_MONSTER_WIZARD];

    //타일 로드
    for (const auto& Tile : Tiles)
    {
        auto it = m_FactoryMap.find(OBJ_TILE);
        if (it != m_FactoryMap.end())
        {

            CTile* pTile = static_cast<CTile*>(it->second());

            ObjectHandle tHandle = JsonFunction::Load_ObjectHandle(Tile);
            _vec3 vPos = JsonFunction::Load_Info(Tile, INFO_TYPE::INFO_TYPE_POS);
            _vec3 vRot = JsonFunction::Load_Info(Tile, INFO_TYPE::INFO_TYPE_ROT);
            _vec3 vScale = JsonFunction::Load_Info(Tile, INFO_TYPE::INFO_TYPE_SCALE);

            _vec2	 vCenterUV = JsonFunction::Load_CenterUV(Tile);

            TILETYPE eTileType = JsonFunction::Load_TileType(Tile);
            MAPTOOLTILE_INFO tTileInfo = JsonFunction::Load_MapToolTile_Info(Tile);

            index++;
            tHandle.m_iIndex = index;

            pTile->Setting_GameObject(tHandle, vPos, vRot, vScale);

            //타일 타입 천장타입 <-> 바닥 타입
            pTile->Set_TileType(eTileType);
            pTile->Change_TextureUV(vCenterUV);
            //UV좌표바꾸기
            if (pTile->Get_TileType() == TILETYPE::TILE_CEILING)
                pTile->Change_IndexBuffer_To_CCW();

            pTile->Get_Collide()->Set_ColliderScale(pTile->Get_Transform()->Get_Scale());

            //이거 지우면 큰일 미리 업데이트 돌리고 모든 업데이트 끄기때문에 조심 이거 없애면 0 0 0 으로 다모임
            pTile->SemiUpdate();

            object_TILE.push_back(pTile);

            // 파티션
            RegisterObject(pTile);
        }
    }

    //큐브 로드
    for (const auto& Cube : Cubes)
    {
        ObjectHandle tHandle = JsonFunction::Load_ObjectHandle(Cube);
        _vec3 vPos = JsonFunction::Load_Info(Cube, INFO_TYPE::INFO_TYPE_POS);
        _vec3 vRot = JsonFunction::Load_Info(Cube, INFO_TYPE::INFO_TYPE_ROT);
        _vec3 vScale = JsonFunction::Load_Info(Cube, INFO_TYPE::INFO_TYPE_SCALE);

        CUBE_TYPE eCubeType = JsonFunction::Load_Cube_Type(Cube);

        _bool HorFilpArr[CUBE_FACE_END];
        JsonFunction::Load_Cube_HorizontalFilp(Cube, HorFilpArr);

        _bool VerFilpArr[CUBE_FACE_END];
        JsonFunction::Load_Cube_VerticalFilp(Cube, VerFilpArr);

        _vec2     vCenterUV[CUBE_FACE_END];
        JsonFunction::Load_Cube_CenterUV(Cube, vCenterUV);

        index++;
        tHandle.m_iIndex = index;

        if (eCubeType == CUBE_TYPE::CUBE_MAPTOOL)
            eCubeType = CUBE_TYPE::CUBE_NONE;			// 혹시나 큐브타입이 맵툴로 안찍혀잇겠지만 맵툴용이라면 NONE으로 변경시켜버리기

        //이벤트 큐브면 이벤트큐브 OBJTYPE 으로 변경해줘야함
        if (eCubeType == CUBE_TYPE::CUBE_EVENT)
            tHandle.m_eType = OBJTYPE::OBJ_EVENTCUBE;

        if (eCubeType == CUBE_TYPE::CUBE_NONE)
        {
            //일반 큐브 생성
            auto it = m_FactoryMap.find(OBJ_CUBE);
            if (it != m_FactoryMap.end())
            {
                CCube* pCube = static_cast<CCube*>(it->second());

                pCube->SetCubeHorizontalFilpArr(HorFilpArr);
                pCube->SetCubeVerticalFilpArr(VerFilpArr);

                pCube->Setting_GameObject(tHandle, vPos, vRot, vScale);

                for (int i = 0; i < CUBE_FACE_END; ++i)
                {
                    pCube->Change_CubeTextureFaceUV((CUBE_FACE)i, vCenterUV[i]);
                }

                pCube->Get_Collide()->Set_ColliderScale(pCube->Get_Transform()->Get_Scale());
                pCube->Get_Collide()->Set_Offset(
                    _vec3(
                        0, pCube->Get_Transform()->Get_Scale().y * 0.5f, 0
                    ));

                pCube->SemiUpdate();
                object_CUBE.push_back(pCube);

                // 파티션
                RegisterObject(pCube);
            }
        }
        else if (eCubeType == CUBE_TYPE::CUBE_EVENT)
        {
            CUBE_EVENT_TYPE Type = CEnumStringFunction::Handle_Event_Index_To_CUBE_EVENT_TYPE(tHandle.m_sEventIndex);

            if (Type != CUBE_EVENT_TYPE::_END)
            {

                auto it = m_EventCubeFactoryMap.find(Type);

                //이벤트 큐브 생성
                if (it != m_EventCubeFactoryMap.end())
                {
                    CEventCube* pEventCube = static_cast<CEventCube*>(it->second());

                    if (Type == CUBE_EVENT_TYPE::POSITION)
                    {
                        pEventCube->Setting_GameObject(tHandle, vPos, vRot, vScale);
                        pEventCube->Set_Active(false);

                        m_vPositionCubelist.push_back(pEventCube);
                        //포지션값을 저장하는 리스트를 따로 저장
                    }
                    else
                    {
                        //나머지들은 움직이거나 , 부셔지거나 , 포탈관련 큐브애들이다 따라서 렌더 업데이트 등등 다 중요하다.
                        pEventCube->Setting_GameObject(tHandle, vPos, vRot, vScale);

                        pEventCube->SetHorFilpArr(HorFilpArr);
                        pEventCube->SetVerFilpArr(VerFilpArr);

                        for (int i = 0; i < CUBE_FACE_END; ++i)
                        {
                            pEventCube->Change_CubeTextureFaceUV((CUBE_FACE)i, vCenterUV[i]);
                        }

                        pEventCube->Get_Collide()->Set_ColliderScale(pEventCube->Get_Transform()->Get_Scale());
                        pEventCube->Get_Collide()->Set_Offset(
                            _vec3(
                                0, pEventCube->Get_Transform()->Get_Scale().y * 0.5f, 0
                            ));

                        pEventCube->SemiUpdate();
                        object_EVENTCUBE.push_back(pEventCube);

                        // 파티션
                        RegisterObject(pEventCube);
                    }
                }
            }
        }
        else if (eCubeType == CUBE_TYPE::CUBE_CREATURECANTGO)
        {
            auto it = m_FactoryMap.find(OBJTYPE::OBJ_CREATURECANTGO);

            if (it == m_FactoryMap.end()) continue;
            
            CGameObject* pCube = it->second();
            if (pCube == nullptr) continue;

            pCube->Setting_GameObject(tHandle, vPos, vRot, vScale);

            pCube->Set_OBJTYPE(OBJTYPE::OBJ_CREATURECANTGO);

            pCube->Get_Collide()->Set_ColliderScale(pCube->Get_Transform()->Get_Scale());
            pCube->Get_Collide()->Set_Offset(
                _vec3(
                    0, pCube->Get_Transform()->Get_Scale().y * 0.5f, 0
                ));

            pCube->SemiUpdate();
            
            object_CreatureCantGo.push_back(pCube);


            // 파티션
            RegisterObject(pCube);
        }
    }
        //맵 데코 로드
    for (const auto& Decor : Decors)
    {
            CMapObject* pMapObject = nullptr;

            ObjectHandle tHandle = JsonFunction::Load_ObjectHandle(Decor);
            _vec3 vPos =   JsonFunction::Load_Info(Decor, INFO_TYPE::INFO_TYPE_POS);
            _vec3 vRot =   JsonFunction::Load_Info(Decor, INFO_TYPE::INFO_TYPE_ROT);
            _vec3 vScale = JsonFunction::Load_Info(Decor, INFO_TYPE::INFO_TYPE_SCALE);

            index++;
            tHandle.m_iIndex = index;

            MAPOBJECT_CATEGORY  Cat = CEnumStringFunction::MAPOBJECT_CATEGORI_StringToEnum(Decor["CATEGORI"].get<string>());
            MAPOBJECT_PLACEMENT Pl = CEnumStringFunction::MAPOBJECT_PLACEMENT_StringToEnum(Decor["PLACEMENT"].get<string>());

            SpriteRect vSprite = JsonFunction::Load_SpriteRect(Decor);

            if (Pl == MAPOBJECT_PLACEMENT::GROUND)
                pMapObject = CGroundDecor::Create(CGraphicDev::GetInstance()->m_pGraphicDev);
            else if (Pl == MAPOBJECT_PLACEMENT::WALL)
                pMapObject = CWallDecor::Create(CGraphicDev::GetInstance()->m_pGraphicDev);
            else
                continue;

            static_cast<CMapObject*>(pMapObject)->Setting(tHandle, vPos, vRot, vScale, Cat, Pl, vSprite);
            object_MAPDECOR.push_back(pMapObject);

            // 파티션
            RegisterObject(pMapObject);
   
    }
    //맵 방어물(거의 데코임)
    for (const auto& Obstacle : Obstacles)
        {

            ObjectHandle tHandle = JsonFunction::Load_ObjectHandle(Obstacle);
            CGameObject* pObstacle = nullptr;

            index++;
            tHandle.m_iIndex = index;

            auto it = m_FactoryMap.find(tHandle.m_eType);
            if (it == m_FactoryMap.end()) continue;

            pObstacle = it->second();
            if (!pObstacle) continue;

            _vec3 vPos = JsonFunction::Load_Info(Obstacle, INFO_TYPE::INFO_TYPE_POS);
            _vec3 vRot = JsonFunction::Load_Info(Obstacle, INFO_TYPE::INFO_TYPE_ROT);
            _vec3 vScale = JsonFunction::Load_Info(Obstacle, INFO_TYPE::INFO_TYPE_SCALE);

            MAPOBJECT_CATEGORY  Cat = CEnumStringFunction::MAPOBJECT_CATEGORI_StringToEnum(Obstacle["CATEGORI"].get<string>());
            MAPOBJECT_PLACEMENT Pl = CEnumStringFunction::MAPOBJECT_PLACEMENT_StringToEnum(Obstacle["PLACEMENT"].get<string>());

            SpriteRect vSprite = JsonFunction::Load_SpriteRect(Obstacle);

            static_cast<CMapObject*>(pObstacle)->Setting(tHandle, vPos, vRot, vScale, Cat, Pl, vSprite);

            object_MAPOBSTACLE.push_back(pObstacle);

            // 파티션
            RegisterObject(pObstacle);

        }
    //맵 이벤트 관련 모든 오브젝트
    for (const auto& Event : Events)
    {
        CEventObject* pEventObject = nullptr;
    
        EVENT_PATH Event_Path = CEnumStringFunction::EVENT_PATH_StringToEnum(Event["EVENTPATH"].get<string>());
    
        //이벤트오브젝트 패스에 따라서 타입이 지정된다
        if (EVENT_PATH::INTERECT_START < Event_Path && Event_Path < EVENT_PATH::INTERECT_END)
        {
            pEventObject = CInteractable::Create(CGraphicDev::GetInstance()->m_pGraphicDev);
            pEventObject->SetObject_Event_Type(OBJECT_EVENT_TYPE::INTERECT);
        }
        else if (Event_Path ==  EVENT_PATH::CHAOS )
        {
            pEventObject = CEyeOfChaos::Create(CGraphicDev::GetInstance()->m_pGraphicDev);
            pEventObject->SetObject_Event_Type(OBJECT_EVENT_TYPE::CHAOSOFEYE);
        }
        else continue;
    
        ObjectHandle tHandle = JsonFunction::Load_ObjectHandle(Event);
    
        index++;
        tHandle.m_iIndex = index;
    
        // 이벤트관련 오브젝트 저장할때는 Sprite 좌표가 필요없다.
        _vec3 vPos = JsonFunction::Load_Info(Event, INFO_TYPE::INFO_TYPE_POS);
        _vec3 vRot = JsonFunction::Load_Info(Event, INFO_TYPE::INFO_TYPE_ROT);
        _vec3 vScale = JsonFunction::Load_Info(Event, INFO_TYPE::INFO_TYPE_SCALE);
    
        MAPOBJECT_CATEGORY Cat = CEnumStringFunction::MAPOBJECT_CATEGORI_StringToEnum(Event["CATEGORI"].get<string>());
        MAPOBJECT_PLACEMENT Placement = CEnumStringFunction::MAPOBJECT_PLACEMENT_StringToEnum(Event["PLACEMENT"].get<string>());
    
        //이벤트 오브젝트 세팅
        pEventObject->Setting_EventObject(tHandle, vPos, vRot, vScale, Cat, Placement, Event_Path);
        
        if (Event_Path == EVENT_PATH::ORBFIELD)
        {
            if (Event.contains("BUFFERINDEX") == true)
            {
                static_cast<CInteractable*>(pEventObject)->Set_OrbCount(Event["BUFFERINDEX"].get<int>());
            }
        }


        object_MAPEVENT.push_back(pEventObject);
    
        // 파티션
        RegisterObject(pEventObject);
    }
    
    //아이템
    for (const auto& Item : Items)
    {
        if (!Item.contains("ITEMTYPE"))
            continue;
    
        CItem* pItem = CItem::Create(CGraphicDev::GetInstance()->Get_GraphicDev());
    
        ObjectHandle tHandle = JsonFunction::Load_ObjectHandle(Item);
    
        index++;
        tHandle.m_iIndex = index;
    
        _vec3 vPos = JsonFunction::Load_Info(Item, INFO_TYPE::INFO_TYPE_POS);
        _vec3 vRot = JsonFunction::Load_Info(Item, INFO_TYPE::INFO_TYPE_ROT);
        _vec3 vScale = JsonFunction::Load_Info(Item, INFO_TYPE::INFO_TYPE_SCALE);
    
        vPos.y += 0.2f;

        pItem->Set_ItemType(
            CEnumStringFunction::ITEMTYPE_String_To_Enum(Item["ITEMTYPE"].get<string>()));
        
        pItem->Setting_GameObject(tHandle, vPos, vRot, vScale);
    
        object_ITEM.push_back(pItem);
    
        // 파티션
        RegisterObject(pItem);
    }
    
    for (const auto& Monster : Monsters)
    {
        if (!Monster.contains("MONSTERTYPE"))
            continue;
    
    
        ObjectHandle tHandle = JsonFunction::Load_ObjectHandle(Monster);
        MONSTER_TYPE eMsType = CEnumStringFunction::MONSTER_TYPE_String_To_Enum(Monster["MONSTERTYPE"].get<string>());
        OBJTYPE ObjType = CEnumStringFunction::MONSTER_TYPE_TO_OBJ_TYPE(eMsType);
        tHandle.m_eType = ObjType;
    
        index++;
        tHandle.m_iIndex = index;
    
        auto iter = m_FactoryMap.find(ObjType);
        CGameObject* pMonster = iter->second();
    
        _vec3	     vPos = JsonFunction::Load_Info(Monster, INFO_TYPE_POS);
        _vec3	     vRot = JsonFunction::Load_Info(Monster, INFO_TYPE_ROT);
        _vec3	     vScale = JsonFunction::Load_Info(Monster, INFO_TYPE_SCALE);
        tHandle.m_eType = ObjType;
    
    
        if (iter == m_FactoryMap.end())
            continue;
    
    
        pMonster->Setting_GameObject(tHandle, vPos, vRot, vScale);
    
        switch (eMsType)
        {
        case Engine::MONSTER_GOBLIN_HAMMER:
            object_MONSTER_GOBLIN_HAMMER.push_back(pMonster);
            break;
        case Engine::MONSTER_GOBLIN_FIREBALL:
            object_MONSTER_GOBLIN_FIREBALL.push_back(pMonster);
            break;
        case Engine::MONSTER_ORGE_AXE:
            object_MONSTER_ORGE_AXE.push_back(pMonster);
            break;
        case Engine::MONSTER_RAT:
            object_MONSTER_RAT.push_back(pMonster);
            break;
        case Engine::MONSTER_SKELETON_SWORD:
            object_MONSTER_SKELETON_SWORD.push_back(pMonster);
            break;
        case Engine::MONSTER_WIZARD:
            object_MONSTER_WIZARD.push_back(pMonster);
            break;
        case Engine::MONSTER_END:
        default:
            Safe_Release(pMonster);
            return;
        }
    
        // 파티션
        RegisterObject(pMonster);
    }
    
    for (const auto& Door : Doors)
    {
        if (!Door.contains("DOORTYPE"))
            continue;

        ObjectHandle tHandle    = JsonFunction::Load_ObjectHandle(Door);
        _vec3	     vPos       = JsonFunction::Load_Info(Door, INFO_TYPE_POS);
        _vec3	     vRot       = JsonFunction::Load_Info(Door, INFO_TYPE_ROT);
        _vec3	     vScale     = JsonFunction::Load_Info(Door, INFO_TYPE_SCALE);

        DOORTYPE     eDoorType  = CEnumStringFunction::DOORTTYPE_Stirng_To_Enum(Door["DOORTYPE"].get<string>());
        _int         iSwitchCount = 1;

        if (Door.contains("SWITCH_COUNT"))
            iSwitchCount = Door["SWITCH_COUNT"].get<_int>();
        

        index++;
        tHandle.m_iIndex = index;

        auto iter = m_FactoryMap.find(tHandle.m_eType);

        CGameObject* pDoor = iter->second();

        //타입 값 지정..일단내가만든거 될지안될지는모름~
        pDoor->Set_SubType(&eDoorType);

        CDoor_Component* Com = 
            static_cast<CDoor_Component*>(pDoor->Get_Component(COMPONENTID::ID_DYNAMIC, L"Com_Door"));

        Com->Set_SwitchCount(iSwitchCount);

        if (iter == m_FactoryMap.end())
            continue;

        pDoor->Setting_GameObject(tHandle, vPos, vRot, vScale);
        
        pDoor->Get_Collide()->Set_ColliderScale(vScale * 0.5f);

        object_DOOR.push_back(pDoor);

        RegisterObject(pDoor);
    }

    for (const auto& Light : Lights)
    {
        CLightObject* pLightObject = nullptr;

        ObjectHandle tHandle = JsonFunction::Load_ObjectHandle(Light);
        _vec3	     vPos = JsonFunction::Load_Info(Light, INFO_TYPE_POS);
        _vec3	     vRot = JsonFunction::Load_Info(Light, INFO_TYPE_ROT);
        _vec3	     vScale = JsonFunction::Load_Info(Light, INFO_TYPE_SCALE);

        LIGHT_OBJECT_TYPE Light_Type = LIGHT_OBJECT_TYPE::UNKNOWN;
        MAPOBJECT_PLACEMENT Placement_Type = MAPOBJECT_PLACEMENT::_END;
        
        if (Light.contains("LIGHT_OBJECT_TYPE")) Light_Type = CEnumStringFunction::LIGHT_OBJECT_TYPE_String_To_Enum(Light["LIGHT_OBJECT_TYPE"].get<string>());
        else continue;

        if (Light.contains("PLACEMENT")) Placement_Type = CEnumStringFunction::MAPOBJECT_PLACEMENT_StringToEnum(Light["PLACEMENT"].get<string>());
        else continue;


        index++;
        tHandle.m_iIndex = index;

        pLightObject = m_LigtObjectFactoryMap.find(Light_Type)->second();

        if (pLightObject == nullptr)
            continue;

        pLightObject->Setting(tHandle, vPos , vRot , vScale , Placement_Type);

        pLightObject->SemiUpdate();

        object_LIGHT.push_back(pLightObject);
        RegisterObject(pLightObject);
    }

}

void CObjectManager::Make_SnapShotList(CGameObject* object)
{
    if (!object) return;

    auto objectHandle = object->Get_Handle();
    auto objtrans = object->Get_Transform();
    
    if (!objtrans) return;

    _vec3 pos = *(objtrans->GetInfoPos());

    ObjectID ID = CObjectIDMaker::Make_ObjectID(objectHandle);
    SNAPSHOT snap = { ID, objectHandle, pos };

    m_vSnapShotList.push_back(snap);
}



vector<CGameObject*> CObjectManager::Get_EventIndex_ObjectVec(OBJTYPE type, int eventIndex)
{
    if (m_mObjectList[type].empty() == true)
        return vector<CGameObject*>();


    vector<CGameObject*> ObjVector;

    for (auto ObjCube : m_mObjectList[type])
    {
        if (ObjCube->Get_HandleEventIndex() == eventIndex)
        {
            ObjVector.push_back(ObjCube);
        }
    }

    return ObjVector;
}

_int CObjectManager::Object_Update(float fTimeDelta)
{
    m_vSnapShotList.clear();

    for (int i = 0; i < OBJ_END; ++i)
    {
        auto& objectlist = m_mObjectList[(OBJTYPE)i];
        for (auto& object : objectlist)
        {
            if (object->Get_Active() == false) continue;
            Make_SnapShotList(object);  // 스냅샷 생성.
        }
    }

    // 카메라 위치 값 보내주기. 
    _matrix view = CCameraManager::GetInstance()->Get_MainViewMatrix();
    _matrix inv; D3DXMatrixInverse(&inv, nullptr, &view);
    _vec3 eye(inv._41, inv._42, inv._43);

    // 워커에 제출
    CPartitionManager::InputFrame in;
    in.eye = eye;
    in.snaps = m_vSnapShotList;              
    CPartitionManager::GetInstance()->Submit(std::move(in));

    // 최신 결과 ID 배열 받음. ㅅㅄㅄㅄㅄㅄㅂ
    vector<ObjectID> ids;
    if (CPartitionManager::GetInstance()->TryConsume(ids)) {
        for (int i = 0; i < OBJ_END; ++i) m_visibleList[(OBJTYPE)i].clear();

        unordered_set<ObjectID> seen;
        seen.reserve(ids.size());

        for (auto id : ids) 
        {
            if (!seen.insert(id).second) continue;          // 중복 필터
            if (auto* obj = FindByID(id); obj && obj->Get_Active())
                m_visibleList[obj->Get_OBJTYPE()].push_back(obj);
        }
    }

 
    for (int i = 0; i < OBJ_END; ++i)
    {
        auto& vis = m_visibleList[(OBJTYPE)i];
        for (auto* object : vis) 
            if (object && object->Get_Active())
                object->Update_GameObject(fTimeDelta);
    }

    m_mObjectList[OBJ_SKYCUBE].front()->Update_GameObject(fTimeDelta);

    return 0;
}

_int CObjectManager::Object_LateUpdate(float fTimeDelta)
{
    for (int i = 0; i < OBJ_END; ++i)
    {
        auto& vis = m_visibleList[(OBJTYPE)i];
        for (auto* object : vis)
            if (object && object->Get_Active())
                object->LateUpdate_GameObject(fTimeDelta);
    }

    m_mObjectList[OBJ_SKYCUBE].front()->LateUpdate_GameObject(fTimeDelta);

    return 0;
}

// ==================== 씬 정리 ====================

void CObjectManager::Delete_Scene()
{
    for (int i = OBJ_PLAYER + 1; i < OBJ_END; ++i)
    {
        for (auto& pObj : m_mObjectList[(OBJTYPE)i]) 
        {
            if (pObj) UnregisterObject(pObj);     
            Safe_Release(pObj);
        }
        m_mObjectList[(OBJTYPE)i].clear();
        m_visibleList[(OBJTYPE)i].clear();
    }

    for (auto iter = m_vPositionCubelist.begin(); iter < m_vPositionCubelist.end(); ++iter)
    {
        Safe_Release(*iter);
    }
    vector<CGameObject*>().swap(m_vPositionCubelist);
}

CGameObject* CObjectManager::GetPositionCube(const _int& EventIndex)
{
    if (m_vPositionCubelist.empty())
        return nullptr;


    auto iter = find_if(m_vPositionCubelist.begin() , m_vPositionCubelist.end() ,  [&](CGameObject* PosCube) {
        return PosCube->Get_HandleEventIndex() == EventIndex + 1000;
        });


    if (iter == m_vPositionCubelist.end())
        return nullptr;

    return *iter;
}



void CObjectManager::Free()
{
    for (int i = 0; i < OBJ_END; ++i)
    {
        for (auto& pObj : m_mObjectList[(OBJTYPE)i]) {
            if (pObj) UnregisterObject(pObj);    
            Safe_Release(pObj);
        }
        m_mObjectList[(OBJTYPE)i].clear();
        m_visibleList[(OBJTYPE)i].clear();
    }
    m_mObjectIDlist.clear();                       


    for ( auto iter = m_vPositionCubelist.begin() ; iter < m_vPositionCubelist.end() ; ++iter)
    {
        Safe_Release(*iter);
    }
    vector<CGameObject*>().swap(m_vPositionCubelist);
}
