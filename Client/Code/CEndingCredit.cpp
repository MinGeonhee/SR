#include "pch.h"
#include "CEndingCredit.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CInputManager.h"
#include "CUIManager.h"
#include "CEventbus.h"
#include "CSoundManager.h"

// 크레딧 텍스트
const wchar_t* CEndingCredit::Texts[] = {
    L"~ CREDITS ~",

    L"",
    L"쥬신아카데미 156기",
    L"SR 팀 포트폴리오",

    L"",
    L"개발",
    L"팀장 최정우",
    L"팀원 민건희",
    L"팀원 박상은",
    L"팀원 김채원",

    L"",
    L"모작 작품",
    L"Wizordum",

    L"",
    L"A암거나",
    L"쓰기",
    L"쓰기",

    L"쓰",
    L"기",
    L"쓰",
    L"기",
    L"쓰",
    L"기",
    //여기부턴비워놔야됨
    L"",
    L"",
    L"",
    L"",
    L"",
    L"",    
    L"",
    L"",
    L"Thank you for playing.",
};
const int CEndingCredit::TextCount =
sizeof(CEndingCredit::Texts) / sizeof(CEndingCredit::Texts[0]);

CEndingCredit::CEndingCredit(LPDIRECT3DDEVICE9 pGraphicDev)
    : CUI(pGraphicDev)
{
    m_bMove = true;
    m_scrollOfs = 0.f;
    m_scrollSpeed = 40.f;
    m_prevActive = false;
    m_bTextUp = false;
}

CEndingCredit::CEndingCredit(const CEndingCredit& rhs)
    : CUI(rhs)
{
}

CEndingCredit::~CEndingCredit()
{
}

HRESULT CEndingCredit::Ready_GameObject()
{
    CUI::Ready_GameObject();

    m_bActive = false;
    m_tHandle.m_eType = OBJ_UI;

    m_pTransform->Set_Scale({ 2.f, 1.7f, 1.f });
    m_pTransform->Set_Pos(0.f, 0.f, 0.f);

    return S_OK;
}

_int CEndingCredit::Update_GameObject(const _float& fTimeDelta)
{
    if (m_bActive && !m_prevActive)
    {
        D3DVIEWPORT9 vp; 
        m_pGraphicDev->GetViewport(&vp);
        m_scrollOfs = vp.Height * 1.f;
        m_bTextUp = false;
        m_bMove = true;

        CSoundManager::GetInstance()->PlayBGM(L"Ep2 - Stormkeep - Layer2.wav",
            CSoundManager::GetInstance()->GetVol().music);
    }
    m_prevActive = m_bActive;

    if (!m_bActive) return 0;

    if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::LBUTTON))
        m_bTextUp = true;

    if (m_bTextUp) {
        m_scrollOfs = -1300.f;
    }
    else {
        m_scrollOfs = max(-1300.f, m_scrollOfs - m_scrollSpeed * fTimeDelta);
    }

    if (m_bMove && m_scrollOfs <= -1300.f) {
        m_scrollOfs = -1300.f;
        m_bMove = false;
    }

    if (!m_bMove &&
        CInputManager::GetInstance()->GetButtonDown(static_cast<KEY_TYPE>(VK_RETURN)))
    {
        EVENT event = { PHASE::PHASE_BEGIN, TOPIC::Go_Main, CHANNEL::UI,
                        nullptr, nullptr, {}, {} };
        CEventBus::GetInstance()->publish(event);
    }

    CUI::Update_GameObject(fTimeDelta);
    return 0;
}

void CEndingCredit::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (!m_bActive) return;
    CUI::LateUpdate_GameObject(fTimeDelta);
}

void CEndingCredit::Render_GameObject()
{
    if (!m_bActive) return;

    D3DVIEWPORT9 vp; m_pGraphicDev->GetViewport(&vp);

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

    m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
    if (auto pTex = CResourceManager::GetInstance()->Get<CTexture>(L"BlackBack"))
    {
        pTex->Set_Texture();
        if (m_pBufferCom) {
            m_pBufferCom->Change_UV(1, 1, 1, 1);
            m_pBufferCom->Render_Buffer();
        }
    }

    const LONG left = LONG(vp.Width * 0.12f);
    const LONG right = LONG(vp.Width * 0.88f);
    LONG curY = LONG(vp.Height * 0.16f) + LONG(m_scrollOfs);
    const int paraGap = 12;

    // 본문
    for (int i = 0; i < TextCount; ++i)
    {
        RECT rcCalc = { left, curY, right, LONG(vp.Height * 0.95f) };
        DWORD fmt = DT_WORDBREAK | DT_TOP | DT_CENTER | DT_CALCRECT;

        //빈줄만들ㄷ기
        if (Texts[i][0] == L'\0') {
            curY += paraGap * 3;
            continue;
        }

        m_mid->DrawTextW(nullptr, Texts[i], -1, &rcCalc, fmt, 0);
        const int paraH = rcCalc.bottom - rcCalc.top;

        RECT rcDraw = { left, curY, right, curY + paraH };
        m_mid->DrawTextW(nullptr, Texts[i], -1, &rcDraw,
            DT_WORDBREAK | DT_TOP | DT_CENTER, D3DCOLOR_ARGB(255, 237, 206, 157));

        curY += paraH + paraGap;
    }

    // 완료 하단 안내
    if (!m_bMove)
    {
        RECT rcHint = {
            LONG(vp.Width * 0.5f - 260),
            LONG(vp.Height * 0.92f - 20),
            LONG(vp.Width * 0.5f + 260),
            LONG(vp.Height * 0.92f + 20)
        };
        m_mid->DrawTextW(nullptr, L"<press Enter to continue>", -1, &rcHint,
            DT_CENTER | DT_VCENTER | DT_SINGLELINE, D3DCOLOR_ARGB(255, 255, 255, 255));
    }

    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CEndingCredit* CEndingCredit::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CEndingCredit* pUI = new CEndingCredit(pGraphicDev);
    if (FAILED(pUI->Ready_GameObject()))
    {
        Safe_Release(pUI);
        MSG_BOX("CEndingCredit Create Failed");
        return nullptr;
    }
    return pUI;
}

void CEndingCredit::Free()
{
    CUI::Free();
}
