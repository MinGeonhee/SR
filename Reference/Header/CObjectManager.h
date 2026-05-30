#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "CObjectIDMaker.h"
#include "CGameObject.h"


BEGIN(Engine)

class CEventCube;
class CLightObject;

using ObjectID = uint32_t;

using PartitionObjectList = unordered_map<OBJTYPE, vector<CGameObject*>>;

typedef struct typeSnapshot
{
    ObjectID        objectID;
    ObjectHandle    objectHandle;
    _vec3           objectPos;
}SNAPSHOT;

using ObjectFactory = std::function<CGameObject* ()>;
using EventCubeFactory = std::function<CEventCube* ()>;
using LightObjectFactory = std::function<CLightObject* ()>;

class ENGINE_DLL CObjectManager : public CBase
{
    DECLARE_SINGLETON(CObjectManager)

private:
    explicit CObjectManager();
    virtual ~CObjectManager();

public:

    CGameObject* Get(OBJTYPE type);

    vector<CGameObject*>& Get_StaticObjectList(OBJTYPE pObjType)
    {
        return m_mObjectList[pObjType];
    }

    vector<CGameObject*>& Get_VisibleObjectList(OBJTYPE pObjType)
    {
        return m_visibleList[pObjType];
    }


    void                RegisterFactory(OBJTYPE type, ObjectFactory factory);
    void                RegisterEventCubeFactory(const  CUBE_EVENT_TYPE& type, const  EventCubeFactory& factory);
    void                RegisterLightObjectFactory(const  LIGHT_OBJECT_TYPE& type, const  LightObjectFactory& factory);

    void                Preload(OBJTYPE type, int ObjectID,
        float xPos, float yPos, float zPos,
        float xAngle, float yAngle, float zAngle);

    void                PreLoad(const string& sFilePath);

    CGameObject* Get_EventIndex_Object(OBJTYPE type, int eventIndex)
    {
        auto& OBJLIST = m_mObjectList[type];
        for (auto& obj : OBJLIST) {
            if (obj && obj->Get_HandleEventIndex() == eventIndex)
                return obj;
        }
        return nullptr;
    }

    vector<CGameObject*> Get_EventIndex_ObjectVec(OBJTYPE type, int eventIndex);

    // 매 프레임

public:
    _int                Object_Update(float fTimeDelta);
    _int                Object_LateUpdate(float fTimeDelta);

    // 씬 정리
    void                Delete_Scene();


    CGameObject*        GetPositionCube(const _int& EventIndex);
    void                ObjectBillboard_OnOff(OBJTYPE type, bool b);

private:

    // 스냅샷  만드는 곳.
    void                Make_SnapShotList(CGameObject* object);

private:

    // cpp에 만들기에는 너무 지저분해서 여기다가 만듬

    CGameObject* FindByID(ObjectID id) const 
    {
        auto it = m_mObjectIDlist.find(id);
        return (it == m_mObjectIDlist.end() ? nullptr : it->second);
    }

    void RegisterObject(CGameObject* object) 
    {
        if (!object) return;
        // 핸들 기반으로 ID 생성
        ObjectID id = CObjectIDMaker::Make_ObjectID(object->Get_Handle());
        object->Set_ObjectID(id);            
        m_mObjectIDlist[id] = object;
    }

    void UnregisterObject(CGameObject* object)
    {
        if (!object) return;
        m_mObjectIDlist.erase(object->Get_ObjectID());
    }

    

private:
    virtual void        Free();

private:

    unordered_map<OBJTYPE, ObjectFactory>                    m_FactoryMap;
    unordered_map<CUBE_EVENT_TYPE ,EventCubeFactory>         m_EventCubeFactoryMap;
    unordered_map<LIGHT_OBJECT_TYPE , LightObjectFactory>    m_LigtObjectFactoryMap;
    // 실제 객체들만 전부 모아둔곳
    unordered_map<OBJTYPE, vector<CGameObject*>>             m_mObjectList;

    // 실제로 화면에서 업데이트 되어야 할 가시 리스트.
    unordered_map<OBJTYPE, vector<CGameObject*>>             m_visibleList;   // ★ 추가

    // 스냅샷해서 PartitionManager에게 보낼 리스트.
    vector<SNAPSHOT>                                         m_vSnapShotList;

    // PartitionManager가 보낸 스냅샷에서 ObjectID를 통해 CGameObject*를 찾기 위함.
    unordered_map<ObjectID, CGameObject*>                    m_mObjectIDlist;

    // EventPath를 통해서 찾을 Pos를 담은 큐브를 담고 있을 Vector 배열
    vector<CGameObject*>                                     m_vPositionCubelist;
};

END
