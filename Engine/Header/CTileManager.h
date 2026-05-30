#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "CToolBufferRect.h"

BEGIN(Engine)

class json;
class CGameObject;
class CRendererManager;
class CTile;
class CMapToolTile;

// 내 스테이지 타입이 외부냐, 던전이냐.
class ENGINE_DLL CTileManager :
    public CBase
{
    DECLARE_SINGLETON(CTileManager)

private:

    explicit CTileManager();
    virtual ~CTileManager() { Free(); };

public:
    void        Init();
    void        MakeTileFloor();

    void        TileManger_Update(const _float& DT);
    void        TileManger_LateUpdate(const _float& DT);
    void        TileManger_Render(LPDIRECT3DDEVICE9 Device);


    const _short& GetSettingRoomNum() { return m_sSettingRoomNum; }


    void        SaveTileToJSON(nlohmann::ordered_json& j,const string& sFileName);
    void        LoadTile(const string& sFilePath);

public:
    //층 세팅
    void	        SetFloor(const FLOOR& eFloor);
    
    //세팅해줄 방번호세팅
    void        SetSettingRoomNum(const _short& sRoomNum) {  m_sSettingRoomNum = sRoomNum; }

    void        SetTargetObject(CGameObject* Obj);
    void        SetTextureMode_CenterUV(const _vec2& vCenterUV) { m_vCurCenterUV = vCenterUV;}
    // 전체 타일 스케일을 조정해줄 친구
    void        SetTileScale(const _vec3& vScale) { m_vTileScale = vScale; }

    void        SetOnTopCube_By_MakeIndex(const FLOOR& eFloor , const _int& MakeIndex);
    void        SetIsHaveCeiling_By_MakeIndex(const FLOOR& eFloor , const _int& MakeIndex);

    void        SetIsAllRender(bool isAllRender) { m_isAllRender = isAllRender; }

public:

    void        Add_Tile(CGameObject* CObj);
    void        Add_Tile(const FLOOR& eFloor,CGameObject* CObj);
    void        Delete_Tile(CMapToolTile* pTile);

    void        Setting_HandleIndex_AfterDelete(const _int& index);

    void        Add_BufferRect(CToolBufferRect* pBufferRect) { m_pBufferRect = pBufferRect; }

    void        Add_BufferInfoRect(CToolBufferRect* pBufferInfoRect) { m_pBufferInfoRect = pBufferInfoRect; }

    void        Delete_BufferRect();
    void        Delete_BufferInfoRect();

    void        KeyUpdate();

    const _vec3& GetBuffeRectPos();
    const _vec2& GetCenterUV();
    const FLOOR& GetCurFloor() { return m_eCurFloor; }

    CGameObject* GetTargetObject() { return m_pTargetObject; }

    const _vec3& GetTileScale() { return m_vTileScale; }
   
    CMapToolTile* GetMapToolTile_ByMakeIndex(const FLOOR& eFloor , const int& MakeIndex);
    
    const _bool& IsAllRender() { return m_isAllRender; }

    const _vec2& GetDefaultTileUV() { return m_vDefaultTileUV; }
    void         SetDefaultTileUV(const _vec2& UV) { m_vDefaultTileUV = UV; }

    //Active 관련
public:

    void        SetActive(const ACTIVE_OBJECT& eType, const _bool& isActive);

public:
    //툴전용으로 일단 쓸예정
    
    const _int  GetCurIndex() { return m_iIndex; }



    void        PlusIndex() { ++m_iIndex; }
    void        MinusIndex() { --m_iIndex; }

protected:

    bool                                    m_isAllRender;


    FLOOR                               m_eCurFloor;

    _ushort                              m_sSettingRoomNum;

    //
    vector<CGameObject*>                m_vTile[(int)FLOOR::FLOOR_END];

    float                                                   m_fTileOffset;
    int                                                       m_iTileSize;
    int                                                       m_iTileCntX;
    int                                                       m_iTileCntZ;

    _int                                                    m_iIndex;

    _int                                                    m_iMakeIndex;
    _vec2                                                   m_vCurCenterUV;
    _vec2                                                   m_vDefaultTileUV;
    
    //단순 마우스 위치좌표에 계속 버퍼를 그려줄 버퍼렉트
    CToolBufferRect*                    m_pBufferRect;


    //수정되고 있는 TargetObject의 위치에 계속 버퍼를 그려줄 버퍼 렉트
    CToolBufferRect*                    m_pBufferInfoRect;
    CGameObject*                        m_pTargetObject;


    CRendererManager*                   m_pRendererManager;
       
    _vec3                               m_vTileScale;


    RENDER_TYPE                         m_eRenderType;

private:
    virtual void                        Free();
};

END