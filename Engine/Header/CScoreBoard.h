#pragma once
#include "CBase.h"
#include "Engine_Define.h"
#include <map>      // std::map

BEGIN(Engine)

struct StageScore
{
    int   lastScore = 0;   // 클리어한 점수
    int   bestScore = 0;   // 해당 스테이지 베스트 점수
    int   treasureSum = 0;   // 해당 스테이지에서 먹은 보물 합
    float lastTime = 0.f; // 해당 스테이지 마지막 클리어 타임
    float parTime = 0.f; // 기준 타임
    float bestTime = 0.f; // 최고 기록
};

class ENGINE_DLL CScoreBoard : public CBase
{
    DECLARE_SINGLETON(CScoreBoard)

private:
    explicit CScoreBoard() {}
    virtual ~CScoreBoard() {}

public:
    void ResetRun();

public:
    void CommitStageResult(
        SCENETYPE stage,
        int score,
        int treasure = 0,
        float parSec = 0.f,
        float clearSec = 0.f);

    bool        HasStage(SCENETYPE stage) const;
    StageScore  GetStage(SCENETYPE stage) const;
    int         GetStageBestScore(SCENETYPE stage) const;
    float       GetStageBestTime(SCENETYPE stage) const;
    int         GetTotalScore() const;

    void     SetPendingBossTime(float t) { m_pendingBossTime = t; }
    float    ConsumePendingBossTime() { float t = m_pendingBossTime; m_pendingBossTime = 0.f; return t; }

private:
    void Free();
    map<SCENETYPE, StageScore> m_scores;
    float m_pendingBossTime = 0.f;
};

END
