#include "CPartitionManager.h"
#include "PartitionHelper.h"

using namespace std;

IMPLEMENT_SINGLETON(CPartitionManager)

CPartitionManager::CPartitionManager() {}

HRESULT CPartitionManager::Start()
{
    if (m_hThread) return S_FALSE;

    InitializeCriticalSection(&m_OutCrt);
    InitializeCriticalSection(&m_InCrt);
    m_quit = false;
    m_hWorkEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (!m_hWorkEvent) return E_FAIL;

    m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);
    return (m_hThread ? S_OK : E_FAIL);
}

void CPartitionManager::Stop() {
    if (m_hThread) {
        m_quit = true;
        SetEvent(m_hWorkEvent);
        WaitForSingleObject(m_hThread, INFINITE);
        CloseHandle(m_hThread);
        m_hThread = nullptr;
    }
    if (m_hWorkEvent) {
        CloseHandle(m_hWorkEvent);
        m_hWorkEvent = nullptr;
    }
    DeleteCriticalSection(&m_InCrt);
    DeleteCriticalSection(&m_OutCrt);

    // 내부 버퍼 정리
    for (auto& v : m_aPartitionList) v.clear();
    m_vObjectIDList.clear();
    queue<int> empty; swap(m_qPartitionIndex, empty);
    m_outBack.clear(); m_outFront.clear();
}

void CPartitionManager::Free() { Stop(); }

void CPartitionManager::Submit(InputFrame&& in)
{
    // 입력 더블버퍼 back에 이동
    EnterCriticalSection(&m_InCrt);
    m_inBack = std::move(in);
    m_hasWork = true;
    LeaveCriticalSection(&m_InCrt);
    SetEvent(m_hWorkEvent);
}

bool CPartitionManager::TryConsume(vector<ObjectID>& outIDs)
{
    EnterCriticalSection(&m_OutCrt);
    if (m_lastConsumedVersion == m_version) {
        LeaveCriticalSection(&m_OutCrt);
        return false;
    }
    outIDs = m_outFront; // 복사 
    m_lastConsumedVersion = m_version;
    LeaveCriticalSection(&m_OutCrt);
    return true;
}

unsigned int __stdcall CPartitionManager::Thread_Main(void* pArg)
{
    auto* self = reinterpret_cast<CPartitionManager*>(pArg);

    while (true) 
    {
        WaitForSingleObject(self->m_hWorkEvent, INFINITE);
        if (self->m_quit) break;

        bool doWork = false;
        {
            EnterCriticalSection(&self->m_InCrt);
            if (self->m_hasWork) {
                self->m_inLocal = std::move(self->m_inBack);
                self->m_hasWork = false;
                doWork = true;
            }
            LeaveCriticalSection(&self->m_InCrt);
        }
        if (!doWork) continue;

        // === 계산 ===
        self->BuildPartitions(self->m_inLocal);

        // === 결과 공개 (짧게 보호) ===
        EnterCriticalSection(&self->m_OutCrt);
        self->m_outFront.swap(self->m_outBack);
        self->m_version++;
        LeaveCriticalSection(&self->m_OutCrt);

        // 로컬 입력 클리어
        self->m_inLocal.snaps.clear();
    }

    _endthreadex(0);
    return 0;
}
void CPartitionManager::BuildPartitions(const InputFrame& in) 
{
    // 프레임 누적 제거
    for (int i = 0; i < PARTX * PARTZ; ++i) m_aPartitionList[i].clear();
    m_vObjectIDList.clear();

    // 파티션 원형 큐 만들기
    PartitionUpdate_WithEye(in.eye);

    // 스냅샷 순회하며 각 오브젝트를 파티션에 배치
    if (!in.snaps.empty()) 
    {
        for (const SNAPSHOT& snap : in.snaps)
        {
            const OBJTYPE t = snap.objectHandle.m_eType;

            PartitionObjectsByRegion(snap.objectPos, snap);
        }
    }

    Partition_ObjectGather();

    // outBack에 결과 저장 
    m_outBack.swap(m_vObjectIDList);  
}

void CPartitionManager::PartitionUpdate_WithEye(const _vec3& eye) 
{
    int xi = static_cast<int>(eye.x / PARTXSIZE);
    int zi = static_cast<int>(eye.z / PARTZSIZE);
    if (xi < 0) xi = 0; else if (xi >= PARTX) xi = PARTX - 1;
    if (zi < 0) zi = 0; else if (zi >= PARTZ) zi = PARTZ - 1;

    const int idx = xi + zi * PARTX;

    // 큐 리셋 후 원형 인덱스 채우기
    queue<int> empty; swap(m_qPartitionIndex, empty);
    PartitionHelper::CircleCalculate(idx, PARTX, PARTZ, 15, m_qPartitionIndex);
}

void CPartitionManager::PartitionObjectsByRegion(const _vec3& pos, const SNAPSHOT& snap)
{
    const int idx = ComputePartIdx(pos);
    if (idx < 0 || idx >= PARTX * PARTZ) return; // <-- && → || 로 수정
    MovePartIfNeeded(snap, idx);
}

void CPartitionManager::Partition_ObjectGather() 
{
    m_vObjectIDList.clear();

    // 전체 길이 미리 합산해 1회 reserve (불필요한 재할당 방지)
    size_t total = 0;
    {
        queue<int> tmp = m_qPartitionIndex;
        while (!tmp.empty()) {
            int p = tmp.front(); tmp.pop();
            if (p >= 0 && p < PARTX * PARTZ) total += m_aPartitionList[p].size();
        }
    }

    m_vObjectIDList.reserve(total);

    while (!m_qPartitionIndex.empty()) {
        const int partIdx = m_qPartitionIndex.front(); m_qPartitionIndex.pop();
        if (partIdx < 0 || partIdx >= PARTX * PARTZ) continue;

        auto& src = m_aPartitionList[partIdx];
        m_vObjectIDList.insert(m_vObjectIDList.end(), src.begin(), src.end());
    }
}

int CPartitionManager::ComputePartIdx(const _vec3& pos) const 
{
    if (pos.x < 0 || pos.z < 0) return -1;
    int xi = static_cast<int>(pos.x / PARTXSIZE);
    int zi = static_cast<int>(pos.z / PARTZSIZE);
    if (xi < 0) xi = 0; else if (xi >= PARTX) xi = PARTX - 1;
    if (zi < 0) zi = 0; else if (zi >= PARTZ) zi = PARTZ - 1;
    return xi + zi * PARTX;
}

void CPartitionManager::MovePartIfNeeded(const SNAPSHOT& snap, int newIdx) 
{
    if (newIdx < 0 || newIdx >= PARTX * PARTZ) return;
    // 스냅 objectID 저장
    m_aPartitionList[newIdx].push_back(snap.objectID);
}
