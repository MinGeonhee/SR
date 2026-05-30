#pragma once
#include <deque>
#include <functional>
#include <algorithm>
#include "CScene.h"
#include "Engine_Define.h"

class CLoadingBar_UI;

class CLoading : public CScene
{
private:
    explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CLoading();

public:
    // 상태 조회
    LOADINGID         Get_LoadingID() { return m_eLoadingID; }
    _bool             Get_Finish() { return m_bFinish; }
    const _tchar* Get_String() { return m_szLoading; }
    int               Get_LoadingPercent() const { return static_cast<int>(m_fLoadCnt); }

public:
    // 초기화
    HRESULT           Ready_Loading(LOADINGID eID);

    // 로딩 시작(작업 큐 생성)
    void              Loading_Scene(LOADINGID eID);

    // 프레임 루프
    virtual _int      Update_Scene(const _float& fTimeDelta) override;
    virtual void      LateUpdate_Scene(const _float& fTimeDelta) override;
    virtual void      Render_Scene() override;

private:
    // (레거시: 즉시 로딩형 ? 필요시 참고/보존)
    _uint             Loading_ForLogo();
    _uint             Loading_ForStage();
    _uint             Loading_ForStage2();
    _uint             Loading_ForBossStage();
    _uint             Loading_ForBossStage2();
    _uint             Loading_ForTutorialStage();

    // 작업 큐 구성 (코옵 로딩)
    void              BuildJobs_ForLogo();
    void              BuildJobs_ForStage();
    void              BuildJobs_ForStage2();
    void              BuildJobs_ForBoss();
    void              BuildJobs_ForBoss2();
    void              BuildJobs_ForTutorial();

    // 등록/프리로드 유틸
    void              RegisterClientPoolingFactories();
    void              PreloadClientPoolingFactories();
    void              RegisterClientFactories();
    void              PreloadClientFactories();

    // 내부 유틸
    void              SpawnUIIfNeeded();
    void              StepJobsPerFrame(int stepsPerFrame);

public:
    static  CLoading* Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID);

private:
    virtual void      Free();

private:
    // 상태
    _bool             m_bFinish = false;
    _bool             m_bLoading = false;
    _tchar            m_szLoading[128] = { 0 };
    LOADINGID         m_eLoadingID = LOADING_END;
    float             m_fLoadCnt = 0.f;

    // UI
    CLoadingBar_UI* m_pLoadingUI = nullptr;
    bool              m_uiSpawned = false;

    // 코옵 로딩용 작업 큐
    std::deque<std::function<void()>> m_jobs;
    size_t           m_totalJobs = 0;
    bool             m_jobsBuilt = false;
};
