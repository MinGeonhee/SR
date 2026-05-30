#pragma once
#include "CGameObject.h"
#include "CProtoTypeManager.h"

class CBossEffect :
    public CGameObject
{
public:
    explicit CBossEffect(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CBossEffect(const CGameObject& rhs);
    virtual ~CBossEffect();

public:
    virtual HRESULT Ready_GameObject() override;
    virtual _int    Update_GameObject(const _float& fTimeDelta) override;
    virtual void    LateUpdate_GameObject(const _float& fTimeDelta) override;
    virtual void    Render_GameObject() override;

private:
    HRESULT Add_Component();

private:
    // === 추가: 회전 제어/빔 길이/굵기 ===
    float m_fSpinSpeedRad = 0.f;   // 초당 라디안
    float m_fAngleYRad = 0.f;   // 누적 각도(라디안)
    float m_fBeamLen = 8.f;   // 빔 길이(ZBeam의 zLen)
    float m_fBeamWidth = 3.f;  // 물줄기 두께(월드 X 스케일로 적용)
    static const int kStreamCount = 1;

private:
    float m_fPhaseOffset = 0.f; // 라디안. 객체별 시작 각도
public:
    void  SetPhaseOffset(float rad) { m_fPhaseOffset = rad; }

private:
    // 컴포넌트들
    Engine::CBossAppearMeshTex* m_pBufferCom = nullptr;
    Engine::CBossEffectAnimation* m_pMeshAnimCom = nullptr;

public:
    static CBossEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void            Free();
};
