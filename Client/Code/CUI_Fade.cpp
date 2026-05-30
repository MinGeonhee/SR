#include "pch.h"
#include "CUI_Fade.h"

CUI_Fade::CUI_Fade(LPDIRECT3DDEVICE9 pGraphicDev)
    : CUI(pGraphicDev)
{
}

CUI_Fade::CUI_Fade(const CUI_Fade& rhs)
    : CUI(rhs)
{
}

CUI_Fade::~CUI_Fade()
{
}

CUI_Fade* CUI_Fade::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CUI_Fade* pFade = new CUI_Fade(pGraphicDev);
    if (FAILED(pFade->Ready_GameObject()))
    {
        Safe_Release(pFade);
        MSG_BOX("CUI_Fade Create Failed");
        return nullptr;
    }
    return pFade;
}

HRESULT CUI_Fade::Ready_GameObject()
{
    CUI::Ready_GameObject();

    m_bActive = false;

    m_tHandle.m_eType = OBJ_UI;
    m_pTransform->Set_Scale({ 1.f, 1.f, 1.f });

    m_mode = Mode::None;
    m_time = 0.f;
    m_duration = 0.5f;
    m_alpha = 0.f;
    m_colorBase = D3DCOLOR_ARGB(255, 0, 0, 0);
    m_blockInput = true;
    m_holdAfter = true;

    return S_OK;
}

void CUI_Fade::StartFadeIn(float duration, D3DCOLOR color, bool blockInput, bool holdAfter)
{
    m_mode = Mode::FadeIn;
    m_time = 0.f;
    m_duration = (duration > 0.f ? duration : 0.001f);
    m_colorBase = color;
    m_blockInput = blockInput;
    m_holdAfter = holdAfter;
    m_alpha = 1.f;
    m_bActive = true;
}

void CUI_Fade::StartFadeOut(float duration, D3DCOLOR color, bool blockInput, bool holdAfter)
{
    m_mode = Mode::FadeOut;
    m_time = 0.f;
    m_duration = (duration > 0.f ? duration : 0.001f);
    m_colorBase = color;
    m_blockInput = blockInput;
    m_holdAfter = holdAfter;
    m_alpha = 0.f;
    m_bActive = true;
}

_int CUI_Fade::Update_GameObject(const _float& fTimeDelta)
{
    if (m_bActive && !m_prevActive)
    {
        m_mode = m_nextMode;
        m_time = 0.f;

        m_alpha = (m_mode == Mode::FadeOut) ? 0.f : 1.f;
    }
    m_prevActive = m_bActive;

    if (!m_bActive || m_mode == Mode::None)
        return 0;

    const float dt = (fTimeDelta > 0.f) ? fTimeDelta : (1.f / 600.f);
    CUI::Update_GameObject(dt);

    m_time += dt;
    float t = m_time / m_duration; if (t > 1.f) t = 1.f;
    const float e = t * t * (3.f - 2.f * t);

    if (m_mode == Mode::FadeOut) m_alpha = e;
    else                         m_alpha = 1.f - e;

    if (m_time >= m_duration)
    {
        m_alpha = (m_mode == Mode::FadeOut) ? 1.f : 0.f;
        m_mode = Mode::None;
        m_time = 0.f;
        m_bActive = false;
        m_blockInput = false;
    }
    return 0;
}

void CUI_Fade::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CUI::LateUpdate_GameObject(fTimeDelta);
}

void CUI_Fade::Render_GameObject()
{
    if (!m_bActive || m_alpha <= 0.f)
        return;

    D3DVIEWPORT9 vp;
    m_pGraphicDev->GetViewport(&vp);

    struct VTX { float x, y, z, rhw; D3DCOLOR c; };
    const BYTE a = static_cast<BYTE>(max(0.f, min(1.f, m_alpha)) * 255.f);
    const D3DCOLOR col = (m_colorBase & 0x00FFFFFF) | (a << 24);

    VTX v[4] = {
        { 0.f,                0.f,                 0.f, 1.f, col },
        { (float)vp.Width,    0.f,                 0.f, 1.f, col },
        { 0.f,                (float)vp.Height,    0.f, 1.f, col },
        { (float)vp.Width,    (float)vp.Height,    0.f, 1.f, col },
    };

    m_pGraphicDev->SetTexture(0, nullptr);
    m_pGraphicDev->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    m_pGraphicDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(VTX));

    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CUI_Fade::Free()
{
    CUI::Free();
}
