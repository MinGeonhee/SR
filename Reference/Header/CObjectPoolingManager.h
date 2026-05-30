#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "CGameObject.h"

BEGIN(Engine)

typedef struct typeSlot
{
    CGameObject* pGameObject = nullptr; // 현재 슬롯이 가리키고 있는 포인터.
    uint16_t     generation = 1;    // 내가 현재 몇 번 이 슬롯을 초기화 했는지 의미하는 것. (풀링을 몇번 했는지.) 
                                    // 예전 포인터 쓰다가 잘못된 객체 건드리는 버그를 터치지 않게 하기 위해서 generation을 쓴다.
    uint8_t      State = 0;         // Active(1), Inactive(0) 활성 비활성을 의미하는 것.
    int          activeIdx = -1;    // 활성 리스트 내 위치 저장하는것.
}SLOT;

typedef struct typePool
{
    vector<SLOT>        slots;    // [index] -> Slot
    vector<uint32_t>    freeIdx; // 빈 슬롯 인덱스 스택 (TypePool당 1개)
}POOL;

// ㅋㅋ 풀링 쓰려고 하니까 얘가 Client쪽 Object들을 아예 모르네 ㅎ;

using ObjectFactory = std::function<CGameObject* ()>;

// 이펙트 파티클, 이펙트 효과, 무기나 아이템 같은 것들을
// 동적으로 생성되는 오브젝트를 관리하는 매니저로 만듦.
// 오브젝트에 내가 Dynamic_Object인지 Static_Object인지 명시하는게 좋아보인다.
// Collision Manager에서 분기로 나눌 수 있을 듯.

class ENGINE_DLL CObjectPoolingManager :
    public CBase
{
    DECLARE_SINGLETON(CObjectPoolingManager)
private:

    explicit CObjectPoolingManager();
    virtual ~CObjectPoolingManager();

public:

    // CollisionManager 전용
    vector<CGameObject*>& Get_DynamicObjectList(OBJTYPE pObjType)
    {
         return m_active[pObjType];
    }

public:

    // 미리 등록시킬 오브젝트들의 내가 지정한 개수만큼 Pooling한다.
    void                Preload(OBJTYPE type, int objectcount);

    // 다 쓰고 나면 초기 상태로 돌려놓는 것.
    void                Return(CGameObject* pObject);

    // Active시킬 오브젝트들을 가져온다.
    CGameObject*        Get(OBJTYPE type);

    // ObjectHandle을 가지고 구독자가 판단 했을 때 이녀석의 Generation을 보고 이벤트 적용대상이 맞는지 확인하는 절차.
    CGameObject*        Resolve(const ObjectHandle& handle);

    // Client에서 가지고 올 함수를 등록해두는 함수
    void                RegisterFactory(OBJTYPE type, ObjectFactory factory);


    //  ============= 파티클 전용 풀링 함수 ===============
    // 
    // count 개수만큼 가지고 올 수 있게 하자
    vector<CGameObject*>    GetMany(OBJTYPE type, int count);

    // 한번에 반납할 수 있도록 한다.
    void                    ReturnMany(const vector<CGameObject*>& objs);

    void                    AllActiveObject_Return();
    void                    ObjectBillboard_OnOff(OBJTYPE type, bool b); // 특정 이벤트 때 풀링 놈들 빌보드 on off

    //외부에서 풀링매니저 안에서 조작하고 외부로 vector 들고가지 않게 막기.
    //int                     Spawn(OBJTYPE type, int count, function<void(CGameObject*, int)> init);

public:

    _int                Object_Update(float fTimeDelta);
    _int                Object_LateUpdate(float fTimeDelta);

    void                PurgeBucket();

private:
    virtual void        Free();

    unordered_map<OBJTYPE, POOL>                        m_pools;    // vector 
    array<vector<CGameObject*>, OBJ_END>                m_active;   // 액티브 된 것들만 리스트에 넣어둔다

    unordered_map<OBJTYPE, ObjectFactory>               m_FactoryMap;

    // 풀링 실험용 변수
    float                                               m_fElapsedtime;
};

END