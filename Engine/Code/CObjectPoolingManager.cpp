#include "CObjectPoolingManager.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CObjectManager.h"
#include "CColliderBase.h"
#include "CSphere.h"
#include "CBillboard.h"

IMPLEMENT_SINGLETON(CObjectPoolingManager)

CObjectPoolingManager::CObjectPoolingManager()
    : m_fElapsedtime(0.f)
{
}

CObjectPoolingManager::~CObjectPoolingManager()
{
    Free();
}

void CObjectPoolingManager::RegisterFactory(OBJTYPE type, ObjectFactory factory)
{
    m_FactoryMap[type] = factory;
}


void CObjectPoolingManager::ObjectBillboard_OnOff(OBJTYPE type, bool b)
{
    auto objectlist = m_active[type];
    for (auto obj : objectlist)
    {
        if (obj)
            static_cast<CBillboard*>(obj->Get_Component(L"Com_Billboard", ID_DYNAMIC))->Set_Active(b);
    }


}

_int CObjectPoolingManager::Object_Update(float fTimeDelta)
{
    // 활성 객체만 순회
    for (int i = 0; i < OBJ_END; ++i)
    {
        auto& act = m_active[(OBJTYPE)i];
        for (CGameObject* obj : act)
        {
            if (obj && obj->Get_Active())
                obj->Update_GameObject(fTimeDelta);
         }
    }

    return 0;
}

_int CObjectPoolingManager::Object_LateUpdate(float fTimeDelta)
{
    for (int i = 0; i < OBJ_END; ++i)
    {
        auto& act = m_active[(OBJTYPE)i];
        for (CGameObject* obj : act)
        {
            if (!obj || !obj->Get_Active())
                continue;

            obj->LateUpdate_GameObject(fTimeDelta);
        }
    }
    return 0;
}
CGameObject* CObjectPoolingManager::Get(OBJTYPE type)
{
    // 오브젝트 타입의 pool을 가져오고
    POOL& tp = m_pools[type];

    int idx = -1;

    // 1) freeIdx로 O(1) 슬롯 재사용
    if (!tp.freeIdx.empty())
    {
        idx = static_cast<int>(tp.freeIdx.back());
        tp.freeIdx.pop_back();
    }
    else
    {
        // 없으면 Inactive 슬롯 탐색
        for (int i = 0; i < static_cast<int>(tp.slots.size()); ++i)
        {
            if (tp.slots[i].State == 0)
            {
                idx = i;
                break; 
            }
        }

        if (idx == -1) 
            return nullptr; // 가용 슬롯이 없으면 nullptr로 반납한다.
    }

    SLOT& s = tp.slots[idx];
    CGameObject* obj = s.pGameObject;

    if (!obj) 
        return nullptr;

    // 활성 캐시에 등록 (swap-erase를 위해 activeIdx 저장)

    s.State = 1;    // 활성화 상태로 등록한다.
    s.activeIdx = static_cast<int>(m_active[type].size());  
    m_active[type].push_back(obj);

    // 객체 활성화하고 오브젝트의 Generation을 최신화 한다.

    // ★ 슬롯 세대 증가 (핸들 유효성의 진실)
    s.generation++;     // 어 너 풀링했네 generation 증가해야겠지. (EventBus에서 잘못 가져오면 안되니까 방지 코드)

    obj->Set_Active(true);
    obj->Set_Generation(s.generation);  

    return obj;
}

void CObjectPoolingManager::Return(CGameObject* obj)
{
    if (obj == NULL) // obj Return 할 때 null이면 return;
        return;     // 거니거니 바니바니

    OBJTYPE type = obj->Get_OBJTYPE();  // ObjectType 일단 들고와

    POOL& tp = m_pools[type];      // ObjectType의 vector 배열을 일단 들고와
                                    // 벡터 배열안에 들어있는 Pool 안에 있는 slot 꺼내오기.

    int idx = obj->Get_HandleIndex();     // obj의 Get_Index해온다.

    if (idx < 0 || idx >= static_cast<int>(tp.slots.size()))    // 내 인덱스가 0보다 작거나, 내 인덱스보다 사이즈가 작으면
        return;                                                 // 거니거니 바니바니

    SLOT& s = tp.slots[idx];        // 꺼내온 인덱스에 맞는 slot 들고오기.

    // 활성 캐시에서 제거 (swap-erase O(1))
    if (s.activeIdx != -1)      // 이 슬롯이 활성화 슬롯이라면 Active (True)라면.
    {
        int last = static_cast<int>(m_active[type].size()) - 1;     // 현재 인덱스 위치에 있는 자기 자신과 마지막에 있는 놈과 Swap하고
        std::swap(m_active[type][s.activeIdx], m_active[type][last]);   // 삭제해주는 코드를 짠다.

        // 스왑된 포인터의 activeIdx 갱신
        CGameObject* swapped = m_active[type][s.activeIdx];

        if (swapped)
        {
            int swappedIdx = swapped->Get_HandleIndex();
            if (swappedIdx >= 0 && swappedIdx < static_cast<int>(tp.slots.size()))
            {
                tp.slots[swappedIdx].activeIdx = s.activeIdx;
            }
        }

        if (auto* col = static_cast<CSphere*>(obj->Get_Component(L"Com_Collider", ID_DYNAMIC)))
        {
            if (auto* sph = dynamic_cast<CSphere*>(col)) {
                sph->Reset(); // m_vPos, m_vWorldMin/Max 초기화
            }
            else {
                col->Set_Collide_CenterPos({ _float(0.f), _float(-100.f), _float(0.f) });
            }
        }

        m_active[type].pop_back();  // 뒤를 날려버림
        s.activeIdx = -1;       // 활성화 아닌데용 -1
    }

    // 객체 초기화
    obj->Set_Active(false);         // Active false로 두고 비활성 객체 FreeIdx에 넣을 준비하자.
    obj->Set_Target(nullptr);       // Set_target Null로 초기화 해둔다.
    obj->Set_ObjectOwner(nullptr);

    if (auto* tr = obj->Get_Transform())        // 위치 좌표 초기화 저 바닥ㄴ으로 날라가버려
    {
        tr->Set_Pos(0.f, -100.f, 0.f);
        tr->Set_Angle(0.f, 0.f, 0.f);
        tr->Set_Scale({ 1.f,1.f,1.f });
    }

    if (auto* tr = obj->Get_Component(L"Com_Collider", ID_DYNAMIC))
    {
        static_cast<CColliderBase*>(tr)->Set_Collide_CenterPos({ 0.f, -100.f, 0.f });
    }

    s.State = 0;        // 빈 슬롯으로 등록
    tp.freeIdx.push_back(static_cast<uint32_t>(idx));   // 비활성화 목록 리스트에 현재 index를 넣는다.
}

void CObjectPoolingManager::Preload(OBJTYPE type, int count)
{
    auto& vec = m_pools[type];

    POOL& tp = vec;

    for (int i = 0; i < count; ++i)
    {
        CGameObject* obj = nullptr;

        auto it = m_FactoryMap.find(type);      // 등록해둔 생성 함수를 find해서 가져온다.

        if (it != m_FactoryMap.end())           // 서칭했을 때 마지막 순번이 아니면 찾았다는거니까... 그 함수 실행해서 생성한다.
            obj = it->second();                 // 생성한 Object를 obj에 담아둔다.

        if (obj == NULL)                        // 오브젝트 생성 안됐으면 밑에 다 스킵..
            continue;                           // 거니거니 바니 바니

        obj->Set_OriginType(OBJORIGINTYPE::ORIGIN_DYNAMIC);     // 너 어디 매니저에서 만들었어. 어 저 Pooling에서 만든건데요.
        obj->Set_OBJTYPE(type);                                 // ObjType 설정해주기

        int idx = static_cast<int>(tp.slots.size());    
        tp.slots.push_back({});      // default Slot
        SLOT& s = tp.slots.back();

        s.pGameObject = obj;
        s.generation = 1;           // 최초 세대는 1
        s.State = 0;           // Inactive
        s.activeIdx = -1;       // 

        obj->Set_HandleIndex(idx);        // idx는 slots.size();
        obj->Set_Generation(1);      // (객체 내부 캐시)
        obj->Set_Active(false);     // Set_Active false로 초기 세팅

        // 바로 비활성 풀로 넣기 → Get이 O(1)
        tp.freeIdx.push_back(static_cast<uint32_t>(idx));   // 비활성화 객체만 넣을 freeIdx 배열에 본인 Index 번호를 push_back한다.
    }
}

// GPT 고맙다
// Resolve를 쓰는 이유 -> 내가 들고 있는 숫자표(핸들)가 진짜로 살아있는 그 객체를 가리키는지 확인하고, 맞으면 포인터를 준다

CGameObject* CObjectPoolingManager::Resolve(const ObjectHandle& handle)       // 구독자가 EventBus로부터 받은 핸들을 받았을 때 이 함수로 실제 GameObject를 가져온다.
{
    if (handle.m_eOriginType != OBJORIGINTYPE::ORIGIN_DYNAMIC)      // ObjectPoolingManager에서 만든 object가 아니네
        return nullptr;     // 거니거니 바니바니

    if ((int)handle.m_eType < 0 || handle.m_eType >= OBJ_END)           // 만약에 Obj_Type이 내가 지정한 값이 아니면 
        return nullptr;     // 거니거니 바니바니

    OBJTYPE type = handle.m_eType;

    POOL& tp = m_pools[handle.m_eType];        // 타입에 맞는 벡터 배열을 들고 온다

    if (handle.m_iIndex < 0 || handle.m_iIndex >= static_cast<int>(tp.slots.size()))        // 해당 슬롯의 사이즈보다 object Index가 크면 무언가 잘못되었다.
        return nullptr;     // 그거

    SLOT& s = tp.slots[handle.m_iIndex];    // 내 Index의 슬롯 가져오기.

    if (s.generation != handle.m_iGeneration)   // 만약에 세대가 안맞으면 
        return nullptr;     // 그거

    return s.pGameObject;   // 이 모든 당근당근을 통과했다면 GameObject 반환한다.
}

vector<CGameObject*> CObjectPoolingManager::GetMany(OBJTYPE type, int count)
{
    // 파티클 시스템에 보내야 할 벡터 배열을 선언한다.
    vector<CGameObject*>    vOut;
    vOut.reserve(count);  // 카운트 갯수만큼 미리 공간 확보

    POOL& pool = m_pools[type]; // 풀 가져오기

    // 현재 내가 pool 안에 가지고 있는 비활성화 목록만 넘겨준다. 추가 생산 x
    for (int k = 0; k < count; ++k)
    {
        if (pool.freeIdx.empty())   // 전부 활성화 상태라면
        {
            break;
        }
        // 비활성 목록에 있는 가장 뒤에 있는 인덱스 번호를 뽑아오고
        // 비활성 목록에서 제거
        int idx = static_cast<int>(pool.freeIdx.back()); 
        pool.freeIdx.pop_back();

        // 비활성 슬롯에 접근해서 슬롯에 존재하는 CGameObject를 가지고 온다.

        SLOT& slot = pool.slots[idx];
        CGameObject* object = slot.pGameObject;

        // 만약에 object가 nullptr이라면 스킵 때린다.
        if (object == NULL)
            continue;

        // 해당 슬롯을 활성화 처리를 하고 활성화 되어있는 배열에 집어넣는다.
        slot.State = 1;
        slot.activeIdx = static_cast<int>(m_active[type].size());
        m_active[type].push_back(object);

        object->Set_Active(true);
        object->Set_Generation(slot.generation);

        // 파티클 시스템에 보내야 할 벡터에 오브젝트 드디어 담는다.
        vOut.push_back(object);
    }

    return vOut;
}

void CObjectPoolingManager::ReturnMany(const vector<CGameObject*>& Objectlist)
{
    for (auto object : Objectlist)
        Return(object);
}


void CObjectPoolingManager::AllActiveObject_Return()
{
    for (auto& active : m_active) {
        vector<CGameObject*> snapshot(active.begin(), active.end());
        ReturnMany(snapshot); // ReturnMany는 지금처럼 const ref여도 됨
    }
}

void CObjectPoolingManager::PurgeBucket()
{
    // 1) 활성 캐시 먼저 언레지스터
    for (int t = 0; t < OBJ_END; ++t) {

        std::vector<CGameObject*>().swap(m_active[(OBJTYPE)t]); // clear + shrink
    }

    // 2) 풀 내부 완전 파괴
    for (auto& [type, pool] : m_pools) 
    {
        for (SLOT& s : pool.slots)
        {
            if (s.pGameObject) 
            {
                Safe_Release(s.pGameObject);
                s.pGameObject = nullptr;
            }
        }
        std::vector<SLOT>().swap(pool.slots);
        std::vector<uint32_t>().swap(pool.freeIdx);
    }

    m_pools.clear();
}

void CObjectPoolingManager::Free()
{
    PurgeBucket();
}