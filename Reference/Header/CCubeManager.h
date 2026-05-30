#pragma once

#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class json;
class CToolBufferCube;
class CGameObject;
class CRendererManager;
class CMapToolCube;


class ENGINE_DLL CCubeManager : public CBase
{

    DECLARE_SINGLETON(CCubeManager)

private:

    explicit CCubeManager();
    virtual ~CCubeManager() { Free(); };

public:
    void                 Init();
                         
    void                 CubeManager_Update(const _float& DT);
    void                 CubeManager_LateUpdate(const _float& DT);
    void                 CubeManger_Render(LPDIRECT3DDEVICE9 Device);
                         
    void                 KeyUpdate();
                         
    void                 SaveCubeToJSON(nlohmann::ordered_json& j,const string& sFileName);
    void                 LoadCube(const string& sFilePath);
                         
public:                  
    //층 세팅             
    void	             SetFloor(const FLOOR& eFloor);

public:

    void                 Add_Cube(CGameObject* CObj);
    void                 Add_Cube(const FLOOR& eFloor,CGameObject* CObj);
                         
    void                 Add_BufferCube(CToolBufferCube* pBufferRect) { m_pBufferCube = pBufferRect; }
    void                 Add_BufferInfoCube(CToolBufferCube* pBufferInfoRect) { m_pBufferInfoCube = pBufferInfoRect; }
    
    void                 Delete_BufferCube();
    void                 Delete_BufferInfoCube();

    const _vec3&         GetBufferCubePos();
    const _vec2&         GetCurCenterUV();
    const _ushort&       GetSettingRoomNum() { return m_sSettingRoomNum; }
    const FLOOR&         GetCurFloor() { return m_eCurFloor; }
    CGameObject*         GetTargetObject() { return m_pTargetObject; }
    const _vec3&         GetCubeScale() { return m_vCubeScale; }

    const _int&          GetCurIndex() { return m_iIndex; }
                         
    void                 SetTextureMode_CenterUV(const _vec2& vCenterUV) { m_vCurCenterUV = vCenterUV; }
                         
    void                 SetSettingRoomNum(const _ushort& sRoomNum) { m_sSettingRoomNum = sRoomNum; }
                         
    void                 SetTargetObject(CGameObject* Obj)
    {
        if (Obj == nullptr)
        {
            m_pTargetObject = nullptr;
            return;
        }

        m_pTargetObject = Obj;
    }
    void                 SetCubeScale(const _vec3& vScale) { m_vCubeScale = vScale; }
                         
                         
    void                 PlusIndex() { ++m_iIndex; }
    void                 MinusIndex() { --m_iIndex; }
                         
public:                  
    void                 Delete_Cube(CMapToolCube* pCube);
                         
    void                 Setting_HandleIndex_AterDelet(const _int& index);
public:                  
                         
    void                 SetActive(const ACTIVE_OBJECT& eType, const _bool& isActive);

    vector<CGameObject*>* GetCubeList() { return m_vCube; }

    bool            IsHaveLinkedEventHandle(const _int& index);

public:
    void            SetDefaultCubeUV(const _vec2& UV) { m_vDefaultCubeUV = UV; }
    const _vec2&    GetDefaultCubeUV() { return m_vDefaultCubeUV; }

protected:

    FLOOR                                       m_eCurFloor;
    _ushort                                     m_sSettingRoomNum;


    vector<CGameObject*>                        m_vCube[(int)FLOOR::FLOOR_END];

    _vec2                                       m_vCurCenterUV;
    _vec2                                       m_vDefaultCubeUV;
    _vec3                                       m_vCubeScale;

    // 단순 마우스 위치기반 버퍼 큐브
    CToolBufferCube*                            m_pBufferCube; 


    // 수정될 큐브에 맞게 그 위치에 그려질 버퍼큐브
    CToolBufferCube*                            m_pBufferInfoCube;
    CGameObject*                                m_pTargetObject;   


    CRendererManager*                           m_pRendererManager;


    _int			                            m_iIndex;


private:
    virtual void                        Free();
};


END