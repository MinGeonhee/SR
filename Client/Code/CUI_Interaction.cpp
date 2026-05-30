#include "pch.h"
#include "CUI_Interaction.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CInputManager.h"
#include "CCollisionManager.h"
#include "CUIManager.h"

CUI_Interaction::CUI_Interaction(LPDIRECT3DDEVICE9 pGraphicDev)
    :CUI(pGraphicDev)
{

}

CUI_Interaction::CUI_Interaction(const CUI_Interaction& rhs)
    :CUI(rhs)
{

}

CUI_Interaction::~CUI_Interaction()
{

}

HRESULT CUI_Interaction::Ready_GameObject()
{
    CUI::Ready_GameObject();

    m_tHandle.m_eType = OBJ_UI;
    m_pTransform->Set_Scale({ 0.5f, 0.06f, 1.f });
    m_pTransform->Set_Pos(0.f, 0.f, 1.f);

    m_bActive = true;
    return S_OK;
}

_int CUI_Interaction::Update_GameObject(const _float& fTimeDelta)
{
    const wstring& tag = CUIManager::GetInstance()->GetCategoryTag(L"UI_Interaction");
    if (!tag.empty() && tag != m_cachedTag) {
        m_cachedTag = tag;
        DialogueFromTag(tag);
    }

    if (!m_bActive)
        return 0;

    CUI::Update_GameObject(fTimeDelta);

    return 0;
}

void CUI_Interaction::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (!m_bActive)
        return;
    CUI::LateUpdate_GameObject(fTimeDelta);
}

void CUI_Interaction::Render_GameObject()
{
    if (!m_bActive)
        return;
    CUI::Render_GameObject();
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

    if (m_mini && !m_strOutput.empty())
    {
        D3DVIEWPORT9 vp;
        m_pGraphicDev->GetViewport(&vp);

        LONG bannerHeight = 36;
        RECT rc = {
            LONG(vp.Width * 0.2f),
            LONG(vp.Height * 0.8f),
            LONG(vp.Width * 0.8f),
            LONG(vp.Height * 0.8f + bannerHeight)
        };
        m_mini->DrawTextW(nullptr, m_strOutput.c_str(), -1, &rc,
            DT_CENTER | DT_VCENTER | DT_SINGLELINE,
            D3DCOLOR_ARGB(255, 255, 255, 255));
    }

    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CUI_Interaction* CUI_Interaction::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CUI_Interaction* pBanner = new CUI_Interaction(pGraphicDev);

    if (FAILED(pBanner->Ready_GameObject()))
    {
        Safe_Release(pBanner);
        MSG_BOX("pBanner Create Failed");
        return nullptr;
    }

    return pBanner;
}

void CUI_Interaction::Free()
{
    CUI::Free();
}

void CUI_Interaction::DialogueFromTag(const wstring& tag)
{
    if (tag == L"Door_In") {
        Set_Output(L"[E] Enter");
        return;
    }
    if (tag == L"Door_Open" || tag == L"Box_Open") {
        Set_Output(L"[E] Open");
        return;
    }
    if (tag == L"Door_NotOpen") {
        Set_Output(L"[E] Unlock");
        return;
    }
    if (tag == L"Heal") {
        Set_Output(L"[E] Eat");
        return;
    }
    if (tag == L"ClearObj") {
        Set_Output(L"[E] Exit Level");
        return;
    }
    if (tag == L"Dead Body") {
        Set_Output(L"[E] Search");
        return;
    }
    if (tag == L"Trigger") {
        Set_Output(L"[E] Use");
        return;
    }
}