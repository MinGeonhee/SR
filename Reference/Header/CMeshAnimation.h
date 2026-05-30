#pragma once
#include "CVIBuffer.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CMeshAnimation : public CVIBuffer
{
public:
    using Super = CVIBuffer;

    // 추가 더 해도 되는데 더 있을까 싶은데
    enum class PlayMode { Loop, Once, OnceHold }; 

public:
    explicit CMeshAnimation(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CMeshAnimation(const CMeshAnimation& rhs);
    virtual ~CMeshAnimation();

public:
    HRESULT                 Ready_Animation();
    HRESULT                 AniMation_Init(int spriteCount, float delayTime);
    virtual _int            Update_Component(const _float& fTimeDelta) override;
    virtual void            LateUpdate_Component(const _float& fTimeDelta) override;

public:
    void                    Set_CenterUV(_vec2 center) { m_vCenterUV = center; }

    // x값만 옮기는 것.
    void                    Set_OffsetUV(float halfU) { m_vHalfUV.x = halfU; }
    // x만이 아니라 y 좌표 수동으로 옮기려면 사용.
    void                    Set_OffsetUV(float halfU, float halfV) { m_vHalfUV = { halfU, halfV }; }

    // 플레이 제어(기존 네이밍 유지)
    void                    PlayAnimation();      // 애니메이션 재생할래요
    void                    PauseAnimation();   // 애니메이션 현시점에서 멈출래요
    void                    ResumeAnimation();  // 애니메이션 다시 재생할래요
    void                    PauseOnEnd();         // 애니메이션 출력 다 되면 멈출게요

public:
    //      ==========  GPT 님께서 추가해주신겁니다. ==============

    // 열 행 
    void SetGrid(int cols, int rows) {
        m_iCols = (cols > 0 ? cols : 1);
        m_iRows = (rows > 0 ? rows : 1);
    }

    void SetRow(int row) {
        m_iRow = (row >= 0 ? row : 0);
    }

    void SetCol(int startCol, int endCol)
    {
        m_iStartCol = startCol;
        m_iEndCol = endCol;
        m_iCurrentFrame = 0;
        m_fElapsedTime = 0.f;
        m_bUseColSetting = true;
    }

    void ClearCol()
    {
        m_iStartCol = 0;
        m_iEndCol = 0;
        m_iCurrentFrame = 0;
        m_fElapsedTime = 0.f;
        m_bUseColSetting = false;
    }


    void Rewind() {
        m_iCurrentFrame = 0;
        m_fElapsedTime = 0.f;

    }

    //      =====================================================

    // 작게 추가: 모드로 제어(선택) — 안 쓰면 기존 PauseOnEnd로도 충분
    void                    SetPlayMode(PlayMode m) { m_eMode = m; }

public:
    CComponent* Clone();
    static CMeshAnimation* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    void                    ApplyUVToVB();        // 현재 m_vCenterUV, m_vHalfUV를 VB에 반영
    void                    OnReachEnd();         // 마지막 프레임 도달 시 처리

private:
    // UV 세팅값(외부에서 프레임 변경마다 바꿔줄 수도 있고, 내부에서 x만 덮어쓸 수도 있음) -> 근데 이제 안쓸듯. 그리드 기반이 있어서.
    _vec2                   m_vCenterUV = { 0.f, 0.f };  // 프레임 중심 UV
    _vec2                   m_vHalfUV = { 0.5f, 0.5f }; // halfU, halfV

    int                     m_iSpriteCount = 1;   // 전체 프레임
    int                     m_iCurrentFrame = 0;   // 현재 프레임
    float                   m_fDelayTime = 0.1f;
    float                   m_fElapsedTime = 0.f;

    bool                    m_bTimePause = false;       // The World
    bool                    m_bHoldLastFrame = false; // PauseOnEnd 전용. 마지막 프레임에 멈추기

private:
    
    // =============== GPT가 만들어준 그리드 기반으로 애니메이션 짜르는 것 =====================.
    int m_iCols = 0;
    int m_iRows = 0;
    int m_iRow = 0; 

    int m_iStartCol = 0;
    int m_iEndCol = 0;

    bool m_bUseColSetting = false;

    //  병준햄이 알려준 상태값에 따른 플레이모드 설정.
    //  사운드 매니저처럼 사용할 수 있게 해보겠습니다.

    PlayMode                m_eMode = PlayMode::Loop;

private:
    bool m_bIsAnimationEnd = false;
public:
    bool Get_IsAnimationEnd() { return m_bIsAnimationEnd; }
    void Set_IsAnimationEnd(bool _b) { m_bIsAnimationEnd = _b; }

public:
    int Get_CurrFrame() { return m_iCurrentFrame; }

private:
    virtual void            Free() override;
};

END