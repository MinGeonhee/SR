#include "pch.h"
#include "CLoadingBar_UI.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CSceneManager.h"
#include "CLoading.h"

CLoadingBar_UI::CLoadingBar_UI(LPDIRECT3DDEVICE9 pGraphicDev)
    : CUI(pGraphicDev)
{
}

CLoadingBar_UI::CLoadingBar_UI(const CLoadingBar_UI& rhs)
    : CUI(rhs)
{
}

CLoadingBar_UI::~CLoadingBar_UI()
{
}

HRESULT CLoadingBar_UI::Ready_GameObject()
{
    CUI::Ready_GameObject();

    m_tHandle.m_eType = OBJ_UI;
    m_bActive = true;

    m_pTransform->Set_Pos(0.f, 0.f, 0.f);
    m_pTransform->Set_Scale({ 0.55f, 0.09f, 1.f });

    return S_OK;
}

_int CLoadingBar_UI::Update_GameObject(const _float& fTimeDelta)
{
    CUI::Update_GameObject(fTimeDelta);
    if (m_pLoading) {
        m_fProgress = m_pLoading->Get_LoadingPercent();
    }
    return 0;
}

void CLoadingBar_UI::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CUI::LateUpdate_GameObject(fTimeDelta);
}

void CLoadingBar_UI::Render_GameObject()
{
    if (!m_bActive && !m_mid) return;

    D3DVIEWPORT9 vp;
    m_pGraphicDev->GetViewport(&vp);

    RECT rcFIRE = {
    LONG(vp.Width * 0.5f - (400 / 2)),
    LONG(vp.Height * 0.5f - 150 / 2),
    LONG(vp.Width * 0.5f + (400 / 2)),
    LONG(vp.Height * 0.5f + 150 / 2)
    };
    if (!m_pLoading->Get_Finish())
    {
        m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
        m_pBufferCom->Change_UV(1, 1, 1, 1);
        CResourceManager::GetInstance()->Get<CTexture>(L"LoadingGage")->Set_Texture();
        m_pBufferCom->Render_Buffer();

        m_pGraphicDev->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);

        float clampedProgress = max(0.0f, min(100.0f, m_fProgress));

        RECT scissorRect;
        scissorRect.left = vp.X;
        scissorRect.top = vp.Y;
        scissorRect.right = vp.X + (LONG)(vp.Width * clampedProgress / 100.0f);
        scissorRect.bottom = vp.Y + vp.Height;

        m_pGraphicDev->SetScissorRect(&scissorRect);

        CResourceManager::GetInstance()->Get<CTexture>(L"LoadingBar")->Set_Texture();
        m_pBufferCom->Render_Buffer();

        m_pGraphicDev->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

        m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

            m_mid->DrawTextW(nullptr, L"Loading", -1, &rcFIRE,
                DT_CENTER | DT_TOP | DT_SINGLELINE, D3DCOLOR_ARGB(255, 255, 255, 255));
    }
    else
    {
        m_mid->DrawTextW(nullptr, L"<Press \"Fire\" to Continue>", -1, &rcFIRE,
            DT_CENTER | DT_VCENTER | DT_SINGLELINE, D3DCOLOR_ARGB(255, 237, 207, 149));
    }

}


CLoadingBar_UI* CLoadingBar_UI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CLoadingBar_UI* pObj = new CLoadingBar_UI(pGraphicDev);
    if (FAILED(pObj->Ready_GameObject()))
    {
        Safe_Release(pObj);
        MSG_BOX("CLoadingBar_UI Create Failed");
        return nullptr;
    }
    return pObj;
}

void CLoadingBar_UI::Free()
{
    CUI::Free();
}
