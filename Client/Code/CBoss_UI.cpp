#include "pch.h"
#include "CBoss_UI.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CSceneManager.h"
#include "CLoading.h"
#include "CMonster_Boss.h"
#include "CObjectManager.h"

CBoss_UI::CBoss_UI(LPDIRECT3DDEVICE9 pGraphicDev)
    : CUI(pGraphicDev)
{
}

CBoss_UI::CBoss_UI(const CBoss_UI& rhs)
    : CUI(rhs)
{
}

CBoss_UI::~CBoss_UI()
{
}

HRESULT CBoss_UI::Ready_GameObject()
{
    CUI::Ready_GameObject();

    m_tHandle.m_eType = OBJ_UI;
    m_bActive = true;

    m_pTransform->Set_Pos(0.f, 300.f, 0.f);
    m_pTransform->Set_Scale({ 0.55f, 0.09f, 1.f });

    return S_OK;
}

_int CBoss_UI::Update_GameObject(const _float& fTimeDelta)
{
    CUI::Update_GameObject(fTimeDelta);
    auto pBoss = dynamic_cast<CMonster_Boss*>(
        CObjectManager::GetInstance()->Get(OBJTYPE::OBJ_MONSTER_BOSS));

    if (pBoss)
    {
        m_fBossHP = pBoss->Get_Status()->Get_CurrentHp();
        m_fMaxBossHP = pBoss->Get_Status()->Get_Status().fMaxHp;
    }

    return 0;
}

void CBoss_UI::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CUI::LateUpdate_GameObject(fTimeDelta);
}

void CBoss_UI::Render_GameObject()
{
    if (!m_bActive) return;

    static constexpr float CAP_L = 0.29f;
    static constexpr float CAP_R = 0.27f;

    D3DVIEWPORT9 vp;
    m_pGraphicDev->GetViewport(&vp);

    RECT rcFIRE = {
    LONG(vp.Width * 0.5f - (400 / 2)),
    LONG(vp.Height * 0.1f - 70 / 2),
    LONG(vp.Width * 0.5f + (400 / 2)),
    LONG(vp.Height * 0.1f + 70 / 2)
    };
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
    m_pBufferCom->Change_UV(1, 1, 1, 1);
    CResourceManager::GetInstance()->Get<CTexture>(L"LoadingGage")->Set_Texture();
    m_pBufferCom->Render_Buffer();

    float ratio = 0.f;
    if (m_fMaxBossHP > 0.f) 
        ratio = m_fBossHP / m_fMaxBossHP;
    ratio = max(0.f, min(1.f, ratio));

    if (ratio > 0.f)
    {
        // 뷰포트/바 위치/폭 계산은 기존대로
        const LONG cx = LONG(vp.X + vp.Width * 0.5f);
        const LONG barW = LONG(vp.Width * m_pTransform->Get_Scale().x);
        LONG barLeft = cx - barW / 2;
        LONG barRight = cx + barW / 2;

        const LONG innerLeft = barLeft + LONG(barW * CAP_L);
        const LONG innerRight = barLeft + LONG(barW * (1.f - CAP_R));

        LONG fillRight = innerLeft + LONG((innerRight - innerLeft) * ratio);

        const LONG vpLeft = (LONG)vp.X;
        const LONG vpRight = (LONG)(vp.X + vp.Width);

        const LONG scLeft = max(innerLeft, vpLeft);
        const LONG scRight = min(fillRight, vpRight);

        if (scRight > scLeft)
        {
            RECT sc{};
            sc.left = scLeft;
            sc.right = scRight;
            sc.top = vp.Y;
            sc.bottom = vp.Y + vp.Height;

            m_pGraphicDev->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
            m_pGraphicDev->SetScissorRect(&sc);

            m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
            CResourceManager::GetInstance()->Get<CTexture>(L"BossHP")->Set_Texture();
            m_pBufferCom->Change_UV(1, 1, 1, 1);
            m_pBufferCom->Render_Buffer();

            m_pGraphicDev->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        }
    }
    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    m_small->DrawTextW(nullptr, L"Baron Stormkeep", -1, &rcFIRE,
        DT_CENTER | DT_BOTTOM| DT_SINGLELINE, D3DCOLOR_ARGB(255, 255, 255, 255));
}


CBoss_UI* CBoss_UI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CBoss_UI* pObj = new CBoss_UI(pGraphicDev);
    if (FAILED(pObj->Ready_GameObject()))
    {
        Safe_Release(pObj);
        MSG_BOX("CBoss_UI Create Failed");
        return nullptr;
    }
    return pObj;
}

void CBoss_UI::Free()
{
    CUI::Free();
}
