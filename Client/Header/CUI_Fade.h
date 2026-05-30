#pragma once
#include "CUI.h"
#include "CEventbus.h"

class CUI_Fade : public CUI
{
public:
    enum class Mode { None, FadeIn, FadeOut };

private:
    explicit CUI_Fade(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CUI_Fade(const CUI_Fade& rhs);
    virtual ~CUI_Fade();

public:
    static CUI_Fade* Create(LPDIRECT3DDEVICE9 pGraphicDev);

    void StartFadeIn(float duration, D3DCOLOR color = D3DCOLOR_ARGB(255, 0, 0, 0), bool blockInput = true, bool holdAfter = false);
    void StartFadeOut(float duration, D3DCOLOR color = D3DCOLOR_ARGB(255, 0, 0, 0), bool blockInput = true, bool holdAfter = true);
    void SetColor(D3DCOLOR color) { m_colorBase = color; }

    Mode  GetMode()       const { return m_mode; }
    float GetAlpha()      const { return m_alpha; }
    bool  IsBlocking()    const { return m_blockInput && m_bActive && m_alpha > 0.f; }

public:
    virtual HRESULT Ready_GameObject() override;
    virtual _int    Update_GameObject(const _float& fTimeDelta) override;
    virtual void    LateUpdate_GameObject(const _float& fTimeDelta) override;
    virtual void    Render_GameObject() override;
    virtual void    Free() override;

private:
    float    m_time = 0.f;
    float    m_duration = 1.5f;
    float    m_alpha = 0.f;
    D3DCOLOR m_colorBase = D3DCOLOR_ARGB(255, 0, 0, 0);
    bool     m_blockInput = true;
    bool     m_holdAfter = true;
    bool  m_prevActive = false;

    Mode    m_mode = Mode::None;
    Mode    m_nextMode = Mode::FadeIn;
};
