#include "CScoreBoard.h"

IMPLEMENT_SINGLETON(CScoreBoard)

void CScoreBoard::ResetRun()
{
    m_scores.clear();
}

void CScoreBoard::CommitStageResult( SCENETYPE stage, int score, int treasure,
    float parSec, float clearSec)
{
    auto& s = m_scores[stage];

    s.lastScore = score;
    s.treasureSum = treasure;
    s.parTime = parSec;
    s.lastTime = clearSec;

    s.bestScore = max(s.bestScore, score);

    if (clearSec > 0.f) {
        if (s.bestTime <= 0.f || clearSec < s.bestTime)
            s.bestTime = clearSec;
    }
}

bool CScoreBoard::HasStage(SCENETYPE stage) const
{
    return m_scores.find(stage) != m_scores.end();
}

StageScore CScoreBoard::GetStage(SCENETYPE stage) const
{
    auto it = m_scores.find(stage);
    return (it == m_scores.end()) ? StageScore{} : it->second;
}

int CScoreBoard::GetStageBestScore(SCENETYPE stage) const
{
    auto it = m_scores.find(stage);
    return (it == m_scores.end()) ? 0 : it->second.bestScore;
}

float CScoreBoard::GetStageBestTime(SCENETYPE stage) const
{
    auto it = m_scores.find(stage);
    return (it == m_scores.end()) ? 0.f : it->second.bestTime;
}

int CScoreBoard::GetTotalScore() const
{
    int sum = 0;
    for (const auto& kv : m_scores)
        sum += kv.second.lastScore;
    return sum;
}

void CScoreBoard::Free()
{
    m_scores.clear();
}
