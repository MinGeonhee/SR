#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "CColliderBase.h"

BEGIN(Engine)

// 광선에게 맞은 오브젝트에 대한 struct.
typedef struct RayHitInfo
{
    CGameObject* pGameobject = nullptr;
    float        m_fDistance = 100.f;
    _vec3        m_vHitPos = {};

}RAYINFO;

// 함수객체 사용해서 충돌 묶음 지정한 것들만 충돌 시킴.
using CollisionFunction = bool(*)(CColliderBase*, CColliderBase*);

class ENGINE_DLL CCollisionManager :
    public CBase
{
    DECLARE_SINGLETON(CCollisionManager)
    // 충돌 함수 테이블

    CollisionFunction m_CollisionTable[COLLIDER_END][COLLIDER_END];

    explicit CCollisionManager();
    virtual ~CCollisionManager();

public:

    _uint Update_Collision();
    void CheckGroup(OBJTYPE _left, OBJTYPE _right);
    void CheckRaycastGroup(RAY_TYPE _left, OBJTYPE _right);

    void Update_AllObjectList(OBJTYPE type, 
        const vector<CGameObject*>& _list1, 
        const vector<CGameObject*>& _list2)
    {
        // 미리 공간 확보해두고 복사해두기

         // 벡터 자료형의 insert를 사용해서 _list1 과 _list2를 합친 vector를 Scene이나 Stage 변경시 계속 갱신해준다.

        m_vObjectList[type].clear();
        m_vObjectList[type].reserve(_list1.size() + _list2.size());
        // 범위만큼 복사할 것이기 때문에 insert를 쓰고 vector 컨테이너에 오버로딩된 함수중 하나인 for in-range 형태의 함수를 호출한다.
        m_vObjectList[type].insert(m_vObjectList[type].end(), _list1.begin(), _list1.end());
        m_vObjectList[type].insert(m_vObjectList[type].end(), _list2.begin(), _list2.end());
       
    }

    void Reset()
    {
        // 오브젝트 갯수만큼 들고 있어야한다.
        memset(m_arrCheck, 0, sizeof(_uint) * (_uint)OBJTYPE::OBJ_END);

        // 광선 자체가 RAY 개수만큼 있기 때문에 RAY 만큼만 들고 있는다.
        memset(m_RayCheck, 0, sizeof(_uint) * (_uint)RAY_TYPE::END_RAY);
    }

public:

    void       Add_ObjectVector(const OBJTYPE& eType, vector<CGameObject*>* pObjVector)
    {
        if (pObjVector == nullptr)
            return;

        m_vObjectList[eType] = *pObjVector;
    }

public:

    // 각 레이어에서 호출만 하면 땡. 레이어에서 Map으로 Add Object할 때 이곳으로 빼돌리면 된다.
    void Add_Object(OBJTYPE type, CGameObject* object) { m_vObjectList[type].push_back(object); }

    // 저 이놈이랑 충돌할건데 무슨 충돌해야하는지 검사 좀 할게요.
    bool CallCollision(CColliderBase* pLeft, CColliderBase* pRight);

    // 내가 지정한 광선 타입과 지정한 타입의 Object와의 충돌 검사를 진행하게 된다.
    // 이름 개멋있는데? FireRaycast...
    void FireRaycast(RAY_TYPE raytype);
    void FireRaycast(RAY_TYPE raytype, OBJTYPE objectType);

    void InitCollisionTable();
    void Delete_ObjectList();

private:

    void CollisionGroupUpdate(OBJTYPE _left, OBJTYPE _right); // 그룹간의 충돌 갱신.

private:
    virtual void    Free() override;

private:
    // 비트 플래그로 계산하려면 그룹 갯수만큼 _uint를 가져가야함.
    _uint                           m_arrCheck[(_uint)OBJTYPE::OBJ_END];
    _uint                           m_RayCheck[(_uint)RAY_TYPE::END_RAY];

    // _uint로 선언한 조합식은 32개까지 가능하다 라고 함.

    vector<CGameObject*>            m_vObjectList[OBJ_END];

};

END