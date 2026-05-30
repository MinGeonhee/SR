#pragma once
#include "CVIBuffer.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CBossEffectAnimation : public CVIBuffer
{
public:
    using Super = CVIBuffer;

    enum class PlayMode { Loop, Once, OnceHold };

public:
    explicit CBossEffectAnimation(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CBossEffectAnimation(const CBossEffectAnimation& rhs);
    virtual ~CBossEffectAnimation();

public:
    HRESULT                 Ready_Animation();
    HRESULT                 AniMation_Init(int spriteCount, float delayTime);
    virtual _int            Update_Component(const _float& fTimeDelta) override;
    virtual void            LateUpdate_Component(const _float& fTimeDelta) override;

public:
    // UV 수동 세팅 (필요 시)
    void                    Set_CenterUV(_vec2 center) { m_vCenterUV = center; }
    void                    Set_OffsetUV(float halfU) { m_vHalfUV.x = halfU; }
    void                    Set_OffsetUV(float halfU, float halfV) { m_vHalfUV = { halfU, halfV }; }

    // 재생 제어
    void                    PlayAnimation();
    void                    PauseAnimation();
    void                    ResumeAnimation();
    void                    PauseOnEnd();

    // 그리드/행/열 설정
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

    // 플레이 모드
    void                    SetPlayMode(PlayMode m) { m_eMode = m; }

    // ====== 추가: 메쉬 모양 토글/파라미터 ======
    // 기본은 false(일직선). true로 켜면 sin 굽힘 또는 Z 빔 모드 적용
    void EnableMeshBend(bool on) { m_bBendMesh = on; }
    void EnableZBeam(bool on) { m_bMakeZBeam = on; } // 둘 다 true면 ZBeam이 우선

    // 파라미터 튜닝 (원하면 사용)
    void SetBendParams(float amplitude, float maxRad) { m_fAmplitude = amplitude; m_fMaxRad = maxRad; }
    void SetBeamLength(float zLen) { m_fZLen = zLen; }
    void SetZBase(float zBase) { m_fZBase = zBase; }

public:
    CComponent* Clone();
    static CBossEffectAnimation* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    void                    ApplyUVToVB();        // UV를 VB에 반영
    void                    OnReachEnd();         // 마지막 프레임 도달 시 처리

private:
    // UV 상태
    _vec2                   m_vCenterUV = { 0.f, 0.f };
    _vec2                   m_vHalfUV = { 0.5f, 0.5f };

    // 애니메이션 상태
    int                     m_iSpriteCount = 1;
    int                     m_iCurrentFrame = 0;
    float                   m_fDelayTime = 0.1f;
    float                   m_fElapsedTime = 0.f;
    bool                    m_bTimePause = false;
    bool                    m_bHoldLastFrame = false;

    // 그리드 기반 제어
    int m_iCols = 0;
    int m_iRows = 0;
    int m_iRow = 0;
    int m_iStartCol = 0;
    int m_iEndCol = 0;
    bool m_bUseColSetting = false;

    // 플레이 모드
    PlayMode                m_eMode = PlayMode::Loop;

    // 종료 플래그
    bool m_bIsAnimationEnd = false;

    // ======  추가: 모양 토글/파라미터 ======
    bool  m_bBendMesh = false;   // sin 굽힘 on/off
    bool  m_bMakeZBeam = false;   // Z 빔 on/off (우선순위 높음)
    float m_fAmplitude = 3.f;     // 굽힘 Y 진폭
    float m_fMaxRad = 3.1415926535f * 4.9f / 6.f; // 굽힘 각도
    float m_fZBase = 0.f;     // 시작 Z
    float m_fZLen = 8.f;     // Z 진행 길이

public:
    bool Get_IsAnimationEnd() { return m_bIsAnimationEnd; }
    void Set_IsAnimationEnd(bool _b) { m_bIsAnimationEnd = _b; }
    int  Get_CurrFrame() { return m_iCurrentFrame; }
    void RebuildShape();

private:
    virtual void            Free() override;
};

END
