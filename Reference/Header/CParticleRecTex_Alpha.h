#pragma once
#include "CVIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CParticleRecTex_Alpha :
    public CVIBuffer
{
private:
    explicit CParticleRecTex_Alpha();
    explicit CParticleRecTex_Alpha(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CParticleRecTex_Alpha(const CParticleRecTex_Alpha& rhs);
    virtual ~CParticleRecTex_Alpha();

public:
    virtual HRESULT Ready_Buffer();
    virtual void    Render_Buffer();

    HRESULT         Change_CenterUV(const _vec2 vCenterIndex);


private:
    float m_fAlpha = 1.0f;  
    float m_fFadeSpeed = 0.5f; // 초당 감소량
    float m_fRed = 1.f;
    float m_fGreen = 1.f;
    float m_fBlue = 1.f;
    

public:
    void SetAlpha(float a) { m_fAlpha = max(0.f, min(1.f, a)); }
    void FadeOut(float dt) { SetAlpha(m_fAlpha - m_fFadeSpeed * dt); }
    void SetColor(float r, float g, float b) { m_fRed = r; m_fGreen = g; m_fBlue = b; };


public:
    virtual CComponent* Clone() override;
    static CParticleRecTex_Alpha* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    _vec2       m_vUVCenter;
    _vec2       m_vUVCenterIndex;
    // 8 / 256 한거
    float       m_fUVOffset = 0.03125f;


private:
    virtual void Free() override;

};

END


