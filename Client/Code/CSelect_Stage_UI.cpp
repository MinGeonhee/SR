#include "pch.h"
#include "CSelect_Stage_UI.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CInputManager.h"
#include "CEventbus.h"
#include "CUIManager.h"
#include "CScoreBoard.h"
#include "CSceneManager.h"

static inline int StageIdx(int stNum) {
    int idx = (stNum - 1) % 3;
    return (idx < 0) ? idx + 3 : idx;
}

CSelect_Stage_UI::CSelect_Stage_UI(LPDIRECT3DDEVICE9 pGraphicDev)
    : CUI(pGraphicDev) {
}

CSelect_Stage_UI::CSelect_Stage_UI(const CSelect_Stage_UI& rhs)
    : CUI(rhs) {
}

CSelect_Stage_UI::~CSelect_Stage_UI() {}

HRESULT CSelect_Stage_UI::Ready_GameObject()
{
    CUI::Ready_GameObject();

    m_tHandle.m_eType = OBJ_UI;
    m_bActive = true;

    m_pTransform->Set_Pos(-250.f, 0.f, 0.f);

    return S_OK;
}

_int CSelect_Stage_UI::Update_GameObject(const _float& fTimeDelta)
{
    CUI::Update_GameObject(fTimeDelta);

    m_bMove = false;
    if (CInputManager::GetInstance()->GetButtonDown(static_cast<KEY_TYPE>(VK_SPACE))) {
        m_bMove = !m_bMove;
    }

    return 0;
}

void CSelect_Stage_UI::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CUI::LateUpdate_GameObject(fTimeDelta);
    Key_Input();
}

void CSelect_Stage_UI::Render_GameObject()
{
    if (!m_bActive) return;

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

    Select_Stage();

    CResourceManager::GetInstance()->Get<CTexture>(L"thumb")->Set_Texture();
    const D3DXMATRIX parent = *m_pTransform->Get_World();

    D3DXMATRIX S, T, W;
    D3DXMatrixScaling(&S, 0.5f, 0.38f, 1.f);
    D3DXMatrixTranslation(&T, 0.f, 80.f, 0.f);
    W = (S * T) * parent;
    m_pGraphicDev->SetTransform(D3DTS_WORLD, &W);

    const int idx = StageIdx(m_iStageNum);
    m_pBufferCom->Change_UV(idx + 1, 4, 0, 1);
    m_pBufferCom->Render_Buffer();

    Render_ChildTex();

    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CSelect_Stage_UI* CSelect_Stage_UI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CSelect_Stage_UI* pSelect_Stage_UI = new CSelect_Stage_UI(pGraphicDev);
    if (FAILED(pSelect_Stage_UI->Ready_GameObject())) {
        Safe_Release(pSelect_Stage_UI);
        MSG_BOX("pSelect_Stage_UI Create Failed");
        return nullptr;
    }
    return pSelect_Stage_UI;
}

void CSelect_Stage_UI::Free()
{
    CUI::Free();
}

void CSelect_Stage_UI::Select_Stage()
{
    PullStageResult();

    D3DVIEWPORT9 vp; m_pGraphicDev->GetViewport(&vp);
    _vec3 pos = *m_pTransform->GetInfoPos();

    const int idx = StageIdx(m_iStageNum);
    const wchar_t* StageName[3] = { L"Canal Streets",  L"dark citadel", L"Baron's Altar" };

    if (m_big && m_small)
    {
        // 스테이지명
        LONG bannerHeight = 45;

        RECT rc = {
            LONG(vp.Width * 0.4f + pos.x),
            LONG(vp.Height * 0.16f),
            LONG(vp.Width * 0.6f + pos.x),
            LONG(vp.Height * 0.16f + bannerHeight)
        };
        m_big->DrawTextW(nullptr, StageName[idx], -1, &rc,
            DT_CENTER | DT_VCENTER | DT_SINGLELINE, D3DCOLOR_ARGB(255, 133, 90, 79));

        // 부가텍스트
        wchar_t buf[64];
        swprintf_s(buf, L"Level %d", idx + 1);

        rc = {
            LONG(vp.Width * 0.4f + pos.x),
            LONG(vp.Height * 0.20f),
            LONG(vp.Width * 0.6f + pos.x),
            LONG(vp.Height * 0.20f + bannerHeight)
        };
        m_small->DrawTextW(nullptr, buf, -1, &rc,
            DT_CENTER | DT_VCENTER | DT_SINGLELINE, D3DCOLOR_ARGB(255, 182, 162, 147));

        // 구분 라벨
        rc = {
            LONG(vp.Width * 0.4f + pos.x),
            LONG(vp.Height * 0.50f),
            LONG(vp.Width * 0.6f + pos.x),
            LONG(vp.Height * 0.50f + bannerHeight)
        };
        m_small->DrawTextW(nullptr, L"Progress", -1, &rc,
            DT_CENTER | DT_VCENTER | DT_SINGLELINE, D3DCOLOR_ARGB(255, 255, 233, 210));

        wchar_t sBestScore[32];
        wchar_t sBestTime[32];
        wchar_t sParTime[32];
        swprintf_s(sBestScore, L"%d", m_BestScore);
        CUI::FormatTimeStr(m_bestTime, sBestTime, _countof(sBestTime));
        CUI::FormatTimeStr(m_parTime, sParTime, _countof(sParTime));

        const wchar_t* texts[3] = { L"Best Score", L"Best Time", L"Par Time" };
        const wchar_t* values[3] = { sBestScore,   sBestTime,     sParTime };

        const LONG rowH = 36;
        const int  step = 25;
        for (int i = 0; i < 3; i++)
        {
            RECT line = {
                LONG(vp.Width * 0.38f + pos.x),
                LONG(vp.Height * 0.55f + step * i),
                LONG(vp.Width * 0.62f + pos.x),
                LONG(vp.Height * 0.55f + rowH + step * i)
            };

            m_small->DrawTextW(nullptr, texts[i], -1, &line,
                DT_LEFT | DT_VCENTER | DT_SINGLELINE, D3DCOLOR_ARGB(255, 186, 141, 108));
            m_small->DrawTextW(nullptr, values[i], -1, &line,
                DT_RIGHT | DT_VCENTER | DT_SINGLELINE, D3DCOLOR_ARGB(255, 255, 245, 230));
        }

        // Select 버튼
        RECT rcBtn = {
            LONG(vp.Width * 0.4f + pos.x),
            LONG(vp.Height * 0.73f),
            LONG(vp.Width * 0.6f + pos.x),
            LONG(vp.Height * 0.73f + bannerHeight)
        };

        D3DCOLOR textCol = (0 == m_hoverIndex)
            ? D3DCOLOR_ARGB(255, 255, 255, 255)
            : D3DCOLOR_ARGB(255, 172, 134, 96);

        m_big->DrawTextW(nullptr, L"Select", -1, &rcBtn,
            DT_CENTER | DT_VCENTER | DT_SINGLELINE, textCol);
    }
}

void CSelect_Stage_UI::Key_Input()
{
    POINT ptMouse; GetCursorPos(&ptMouse);
    ScreenToClient(g_hWnd, &ptMouse);

    D3DVIEWPORT9 vp; m_pGraphicDev->GetViewport(&vp);
    _vec3 pos = *m_pTransform->GetInfoPos();

    m_hoverIndex = -1;

    LONG bannerHeight = 36;
    RECT rc = {
        LONG(vp.Width * 0.4f + pos.x),
        LONG(vp.Height * 0.73f),
        LONG(vp.Width * 0.6f + pos.x),
        LONG(vp.Height * 0.73f + bannerHeight)
    };

    EVENT event;

    if (PtInRect(&rc, ptMouse))
    {
        m_hoverIndex = 0;
        if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::LBUTTON))
        {
            switch (StageIdx(m_iStageNum))
            {
            case 0: // Stage1
                CUIManager::GetInstance()->ActivateUI(L"UI_FadeIn");
                CUIManager::GetInstance()->ActivateUI(L"StoryScript_UI");
                CUIManager::GetInstance()->SetCategoryTag(L"StoryScript_UI", L"Prologue");
                CUIManager::GetInstance()->DeactivateUI(L"Main_UI");
                break;
            case 1: // Stage2
                 event = { PHASE::PHASE_BEGIN, TOPIC::Go_2st, CHANNEL::UI,
                           nullptr, nullptr, {}, {} };
                 CEventBus::GetInstance()->publish(event);
                break;
            case 2: // Boss
                event = { PHASE::PHASE_BEGIN, TOPIC::Go_Boss, CHANNEL::UI,
                          nullptr, nullptr, {}, {} };
                CEventBus::GetInstance()->publish(event);
                break;
            }
        }
    }
}

void CSelect_Stage_UI::Render_ChildTex()
{
    DWORD oldColorOp, oldColorArg1, oldColorArg2;
    DWORD oldAlphaOp, oldAlphaArg1, oldAlphaArg2;
    DWORD oldTFactor;

    m_pGraphicDev->GetTextureStageState(0, D3DTSS_COLOROP, &oldColorOp);
    m_pGraphicDev->GetTextureStageState(0, D3DTSS_COLORARG1, &oldColorArg1);
    m_pGraphicDev->GetTextureStageState(0, D3DTSS_COLORARG2, &oldColorArg2);
    m_pGraphicDev->GetTextureStageState(0, D3DTSS_ALPHAOP, &oldAlphaOp);
    m_pGraphicDev->GetTextureStageState(0, D3DTSS_ALPHAARG1, &oldAlphaArg1);
    m_pGraphicDev->GetTextureStageState(0, D3DTSS_ALPHAARG2, &oldAlphaArg2);
    m_pGraphicDev->GetRenderState(D3DRS_TEXTUREFACTOR, &oldTFactor);

    m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

    const D3DXMATRIX parent = *m_pTransform->Get_World();

    const int idx = StageIdx(m_iStageNum);
    int iOffset = 0;
    switch (idx) {
    case 0: iOffset = 40; break;
    case 1: iOffset = 40; break;
    case 2: iOffset = 40; break;
    default: iOffset = 0;  break;
    }

    {
        D3DXMATRIX TR, SR, WR;
        D3DXMatrixScaling(&SR, 0.2f, 0.2f, 1.f);
        D3DXMatrixTranslation(&TR, 120.f + iOffset, 220.f, 1.f);
        WR = (SR * TR) * parent;
        m_pGraphicDev->SetTransform(D3DTS_WORLD, &WR);
        m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(100, 172, 134, 96));
        m_pBufferCom->Change_UV(1, 1, 1, 1);
        CResourceManager::GetInstance()->Get<CTexture>(L"Banner_Right")->Set_Texture();
        m_pBufferCom->Render_Buffer();
    }

    {
        D3DXMATRIX TL, SL, WL;
        D3DXMatrixScaling(&SL, 0.2f, 0.2f, 1.f);
        D3DXMatrixTranslation(&TL, -120.f - iOffset, 220.f, 1.f);
        WL = (SL * TL) * parent;
        m_pGraphicDev->SetTransform(D3DTS_WORLD, &WL);
        m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(100, 172, 134, 96));
        m_pBufferCom->Change_UV(1, 1, 1, 1);
        CResourceManager::GetInstance()->Get<CTexture>(L"Banner_Left")->Set_Texture();
        m_pBufferCom->Render_Buffer();
    }

    {
        D3DXMATRIX T1, S1, W1;
        D3DXMatrixScaling(&S1, 0.55f, 0.015f, 1.0f);
        D3DXMatrixTranslation(&T1, 5.f, -155.f, 1.f);
        W1 = (S1 * T1) * parent;
        m_pGraphicDev->SetTransform(D3DTS_WORLD, &W1);
        m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(100, 172, 134, 96));
        m_pBufferCom->Change_UV(1, 1, 1, 1);
        CResourceManager::GetInstance()->Get<CTexture>(L"Line_UI")->Set_Texture();
        m_pBufferCom->Render_Buffer();
    }

    // 상태 복원
    m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, oldTFactor);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, oldColorOp);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, oldColorArg1);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, oldColorArg2);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, oldAlphaOp);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, oldAlphaArg1);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, oldAlphaArg2);
}

void CSelect_Stage_UI::PullStageResult()
{
    const int idx = StageIdx(m_iStageNum);

    SCENETYPE stageID;
    switch (idx) {
    case 0: stageID = SCENETYPE::STAGE1; break;
    case 1: stageID = SCENETYPE::STAGE2;   break;
    case 2: stageID = SCENETYPE::BOSS;   break;
    default:     break;
    }

    m_parTime = CScoreBoard::GetInstance()->GetStage(stageID).parTime;
    m_BestScore = CScoreBoard::GetInstance()->GetStage(stageID).bestScore;
    m_bestTime = CScoreBoard::GetInstance()->GetStageBestTime(stageID);
}
