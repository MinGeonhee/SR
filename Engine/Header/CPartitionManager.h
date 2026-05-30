#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include "COBjectManager.h" 


BEGIN(Engine)

class ENGINE_DLL CPartitionManager : public CBase
{
    DECLARE_SINGLETON(CPartitionManager)

public:
    // 워커 입력 프레임 (메인에서 카메라 eye 스냅샷 포함해 넘김)
    struct InputFrame
    {
        vector<SNAPSHOT> snaps;
        _vec3            eye;
    };

    // ====== 라이프사이클 ======
    HRESULT Start();          // 스레드 1회 생성
    void    Stop();           // 종료 (Free()에서 호출)
    void    Free() override;

    // (LOADING 등에서 멈추고 싶으면 그냥 Submit 안 부르면 됨)
    void Submit(InputFrame&& in);    // move로 넘김, 내부 더블버퍼

    // ====== 메인 ← 워커: 최신 결과 수신 ======
    // 반환값: 새 버전 수신했는지
    bool TryConsume(vector<ObjectID>& outIDs);

private:
    explicit CPartitionManager();
    virtual ~CPartitionManager() { Stop(); }

    static unsigned int __stdcall Thread_Main(void* pArg);

    // === 내부 계산 ===
    void BuildPartitions(const InputFrame& in);

    // 기존 네 함수 최대한 유지 (내부에서만 사용)
    void PartitionUpdate_WithEye(const _vec3& eye);
    void PartitionObjectsByRegion(const _vec3& pos, const SNAPSHOT& snap);
    void Partition_ObjectGather();      // m_qPartitionIndex -> m_vObjectIDList
    int  ComputePartIdx(const _vec3& pos) const;
    void MovePartIfNeeded(const SNAPSHOT& snap, int newIdx);

private:
    // --- 스레드/동기화 ---
    HANDLE            m_hThread = nullptr;
    HANDLE            m_hWorkEvent = nullptr; // Submit 시 깨움
    atomic<bool>      m_quit{ false };

    // 출력 더블버퍼 보호용
    CRITICAL_SECTION  m_OutCrt;

    // 입력 더블버퍼 (메인/워커 간)
    CRITICAL_SECTION  m_InCrt;
    bool              m_hasWork = false;
    InputFrame        m_inBack;   // 메인이 채움
    InputFrame        m_inLocal;  // 워커가 사용 (swap 목적)

    // 출력 더블버퍼 (워커/메인 간)
    vector<ObjectID>  m_outBack;   // 워커가 씀
    vector<ObjectID>  m_outFront;  // 메인이 읽음
    uint64_t          m_version = 0;
    uint64_t          m_lastConsumedVersion = 0;

    // --- 파티션 내부 버퍼 (워커 전용) ---
    vector<ObjectID>  m_aPartitionList[PARTX * PARTZ];
    vector<ObjectID>  m_vObjectIDList;
    queue<int>        m_qPartitionIndex;
};

END