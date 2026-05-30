#pragma once
#include "Engine_Define.h"
#include "CMapToolMapObject.h"


BEGIN(Engine)

class CRendererManager;
class CGameObject;

class ENGINE_DLL CMapObjectManager
{
    DECLARE_SINGLETON(CMapObjectManager)

private:

    explicit CMapObjectManager();
    virtual ~CMapObjectManager() { Free(); };

public:

    void        Init();

    void        MapObjectManager_Update(const _float& DT);
    void        MapObjectManager_LateUpdate(const _float& DT);
    void        MapObjectManager_Render(LPDIRECT3DDEVICE9 Device);

public:

    void        Add_BufferMapObject(const _vec3& vPos, const _vec3& vRot);
    void        Add_MapObject(const _vec3& vPos, const _vec3& vRot);
    void        Add_MapObject(CGameObject* pObj);


    void        Delete_MapObject(CGameObject* pMapObject);
    void        Delete_BufferMapObject() { Safe_Release(m_pBufferMapObject); m_pBufferMapObject = nullptr; }
    
public:


    const _int& SetDoorEventHandleIndex(const DOORTYPE& eType);


public:


    const _short& GetSettingRoomNum() { return m_sSettingRoomNum; }
    const FLOOR& GetCurFloor() { return m_eCurFloor; }
    const _int  GetCurIndex(const MAPOBJECT_CATEGORY& eType) {return m_iIndex[(int)eType];}
    const MAPOBJECT_CATEGORY& GetCurCategory() { return m_eCurCategory; }
    const MAPOBJECT_PLACEMENT& GetCurPlacement() { return m_eCurPlacement; }
    const SpriteRect& GetCurSpriteRect()         { return m_tCurSpriteRect; }
    const EVENT_PATH& GetCurEventPath()     { return m_eCurEventPath; }
    const ITEMTYPE& GetCurItemType()        { return m_eCurItemType; }
    const MONSTER_TYPE& GetCurMonsterType() { return m_eCurMonsterType; }
    const DOORTYPE& GetDoorType()           { return m_eCurDoorType; }
    const LIGHT_OBJECT_TYPE GetLightType() { return m_eCurLightType; }

    CMapToolMapObject* GetBufferMapObject() { return m_pBufferMapObject; }

    CMapToolMapObject* GetInfoMapObject() { return m_pInfoMapObject; }
    
    const _bool& IsAllRender() { return m_isAllRender; }

    vector<CGameObject*>* GetMapObjectList(const MAPOBJECT_CATEGORY& eType) { return m_arrayMapObject[(int)eType]; }

private:

    CMapToolMapObject* CreateMapObject_ByCategori(const MAPOBJECT_CATEGORY& eType , EVENT_PATH eEventPath = EVENT_PATH::UNKNOWN , ITEMTYPE  eItemType =ITEMTYPE::ITEM_NONE , MONSTER_TYPE eMonsterType = MONSTER_TYPE::MONSTER_END ,DOORTYPE eDoorType = DOORTYPE::NONE_DOOR );
    CMapToolMapObject* CreateMapObject_ByCategori(const MAPOBJECT_CATEGORY& eType, void* eMapObj_Type);

public:

    void	    SetFloor(const FLOOR& eFloor);
    void        SetCurCategory(const MAPOBJECT_CATEGORY& eType) { 
        int a = 0;
        m_eCurCategory = eType; }
    void        SetCurPlacement(const MAPOBJECT_PLACEMENT& eType)    { m_eCurPlacement = eType; }
    void        SetSettingRoomNum(const _ushort& sRoomNum)           { m_sSettingRoomNum = sRoomNum; }
    void        SetCurSpriteRect(const SpriteRect& tSpriteRect)      { m_tCurSpriteRect = tSpriteRect;}
    void        SetIndex(const MAPOBJECT_CATEGORY& eType, int index) { m_iIndex[(int)eType] = index; }
    void        SetCurEventPath(const EVENT_PATH& eType)     { m_eCurEventPath = eType; }
    void        SetCurItemType(const ITEMTYPE& eType)        { m_eCurItemType = eType; }
    void        SetCurMonsterType(const MONSTER_TYPE& eType) { m_eCurMonsterType = eType; }
    void        SetDoorType(const DOORTYPE& eType)           { m_eCurDoorType = eType; }
    void        SetLightTYpe(const LIGHT_OBJECT_TYPE eType) { m_eCurLightType = eType; }

    void        SetInfoMapObject(CMapToolMapObject* pInfo) { m_pInfoMapObject = pInfo; }
    void        SetIsAllRender(const _bool& isAllRender) { m_isAllRender = isAllRender; }
    void        SetMonsterIndex(const _int& index) { m_MonsterIndex = index; }
    int         GetMonsterIndex() { return m_MonsterIndex; }

public:

    void        Setting_HandleIndex_AterDelete(const MAPOBJECT_CATEGORY eType, const _int& index);
   
public:
    void        SaveMapObjectToJson(nlohmann::ordered_json& j, const string& sFileName);
    void        LoadMapObjectFromJson(const string& sFilePath);

public:
    void        SetActive(const ACTIVE_OBJECT& eType, const _bool& isActive);

protected:

    bool                                                        m_isAllRender;

    MAPOBJECT_CATEGORY                  m_eCurCategory;
    MAPOBJECT_PLACEMENT                 m_eCurPlacement;

    SpriteRect                          m_tCurSpriteRect;

    EVENT_PATH                          m_eCurEventPath;

    ITEMTYPE                            m_eCurItemType;
    MONSTER_TYPE                        m_eCurMonsterType;
    DOORTYPE                            m_eCurDoorType;
    LIGHT_OBJECT_TYPE                   m_eCurLightType;

    int                                 m_MonsterIndex;

    _int                                m_iIndex[(int)MAPOBJECT_CATEGORY::_END];

    FLOOR                               m_eCurFloor; // 현재 층 번호

    _ushort                             m_sSettingRoomNum; //방번호

    array<vector<CGameObject*>[FLOOR::FLOOR_END], (int)MAPOBJECT_CATEGORY::_END> m_arrayMapObject;

    CMapToolMapObject*                  m_pBufferMapObject;

    CMapToolMapObject*                  m_pInfoMapObject;

private:
    virtual void                        Free();
};

END