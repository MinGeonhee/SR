#include "pch.h"
#include "CDialogue_UI.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CInputManager.h"
#include "CUIManager.h"

CDialogue_UI::CDialogue_UI(LPDIRECT3DDEVICE9 pGraphicDev)
    : CUI(pGraphicDev) 
{

}

CDialogue_UI::CDialogue_UI(const CDialogue_UI& rhs)
    : CUI(rhs) 
{

}

CDialogue_UI::~CDialogue_UI() 
{

}

HRESULT CDialogue_UI::Ready_GameObject()
{
    CUI::Ready_GameObject();
    m_tHandle.m_eType = OBJ_UI;

    m_bActive = false;

    D3DVIEWPORT9 vp; m_pGraphicDev->GetViewport(&vp);
    m_rcBase = { LONG(vp.Width * 0.13f), LONG(vp.Height * 0.70f),
                 LONG(vp.Width * 0.87f), LONG(vp.Height * 0.90f) };
    m_rcName = { m_rcBase.left + 16, m_rcBase.top - 28,
                 m_rcBase.left + 220,  m_rcBase.top - 4 };
    m_rcText = { m_rcBase.left + 22, m_rcBase.top + 22,
                 m_rcBase.right - 22, m_rcBase.bottom - 22 };

    m_useTypewriter = true;
    return S_OK;
}

_int CDialogue_UI::Update_GameObject(const _float& fTimeDelta)
{
    //태그 새로들어오면 안끝났어도 강제로 대사바꿈 근데 대사진행중에 플레이어 이동 잠글거면 상관없을듯
    const wstring& tag = CUIManager::GetInstance()->GetCategoryTag(L"Dialogue_UI");
    if (!tag.empty() && tag != m_cachedTag) {
        m_cachedTag = tag;
        Start(tag, true, (m_cps > 0.f ? m_cps : 5.f));
    }
    if (!m_bActive)
        return 0;

	CInputManager::GetInstance()->Set_KeyLock(true);

    CRendererManager::GetInstance()->Add_RenderGroup(RENDER_UI, this);

    //창크기바뀔때 글자 위치도 맞춰서 같이 바뀜 
    D3DVIEWPORT9 vp; m_pGraphicDev->GetViewport(&vp);
    m_rcBase = { LONG(vp.Width * 0.13f), LONG(vp.Height * 0.70f),
                 LONG(vp.Width * 0.87f), LONG(vp.Height * 0.90f) };
    m_rcName = { m_rcBase.left + 16, m_rcBase.top - 28,
                 m_rcBase.left + 220,  m_rcBase.top - 4 };
    m_rcText = { m_rcBase.left + 22, m_rcBase.top + 22,
                 m_rcBase.right - 22, m_rcBase.bottom - 22 };

    //한글자씩 나오는 효과 구현하기는 했는데 2번째 창부터는 인풋매니저 문제인지 한번에출력됨...
    if (m_useTypewriter)
        Typewriter(fTimeDelta);
    Key_Input(fTimeDelta);

    m_blink += fTimeDelta * 0.8f;
    return 0;
}

void CDialogue_UI::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (!m_bActive) return;
    CUI::LateUpdate_GameObject(fTimeDelta);
}

void CDialogue_UI::Render_GameObject()
{
    if (!m_bActive) return;

    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

    // 배경 박스
    {
        D3DXMATRIX S, T, W;
        D3DXMatrixScaling(&S, 1.5f, 1.f, 1.f);
        D3DXMatrixTranslation(&T, 0.f, -200.f, 0.f);
        W = S * T;

        CResourceManager::GetInstance()->Get<CTexture>(L"Dialogue_Box")->Set_Texture();
        m_pGraphicDev->SetTransform(D3DTS_WORLD, &W);
        m_pBufferCom->Change_UV(1, 1, 1, 1);
        m_pBufferCom->Render_Buffer();
    }

    DrawTexts();

    // 다음 화살표
    if (!m_lines.empty()) {
        bool show = !m_useTypewriter || (m_charVisible >= (int)m_lines[m_cur].text.size());
        if (show && fmodf(m_blink, 1.0f) < 0.6f) {
            D3DXMATRIX S, T, W;
            D3DXMatrixScaling(&S, 0.02f, 0.05f, 1.f);
            D3DXMatrixTranslation(&T, 450.f, -280.f, 0.f);
            W = S * T;

            CResourceManager::GetInstance()->Get<CTexture>(L"ArrowRight")->Set_Texture();
            m_pGraphicDev->SetTransform(D3DTS_WORLD, &W);
            m_pBufferCom->Change_UV(1, 1, 1, 1);
            m_pBufferCom->Render_Buffer();
        }
    }

    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CDialogue_UI::DrawTexts()
{
    if (m_lines.empty()) return;
    const auto& L = m_lines[m_cur];

    // 이름
    if (m_mid) {
        DrawShadowedText(
            m_mid, L.speaker.c_str(), m_rcName,
            D3DCOLOR_ARGB(255, 255, 220, 120),
            2, D3DCOLOR_ARGB(160, 0, 0, 0),
            DT_LEFT | DT_VCENTER | DT_SINGLELINE
        );
    }
    // 본문
    if (m_small) {
        wstring text = L.text;
        if (m_useTypewriter) {
            int n = min<int>(m_charVisible, (int)text.size());
            text.resize(n);
        }//지금 보여줘도되느늑글자수 이상 다 짜름
        DrawShadowedText(
            m_small, text.c_str(), m_rcText,
            D3DCOLOR_ARGB(255, 255, 255, 255),
            2, D3DCOLOR_ARGB(160, 0, 0, 0),
            DT_WORDBREAK | DT_LEFT | DT_TOP
        );
    }
}

void CDialogue_UI::Key_Input(float)
{
    //한글자씩 나오는 모드일때 클릭하면 효과 스킵하고 바로나옴. 
    //이미 다 나왔으면 다음줄 출력함
    //대사 다 끝난상태면 창꺼버리기
    bool next = CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::LBUTTON) ||
        CInputManager::GetInstance()->GetButtonDown(static_cast<KEY_TYPE>(VK_RETURN));
    if (!next || m_lines.empty()) return;

    if (m_useTypewriter) {
        const auto& L = m_lines[m_cur];
        if (m_charVisible < (int)L.text.size()) {
            m_charVisible = (int)L.text.size();
            return;
        }
    }

    if (m_cur + 1 < (int)m_lines.size()) {
        ++m_cur;
        m_charVisible = 0;
        m_acc = 0.f;
    }
    else {
        m_bActive = false;
        m_lines.clear();
		CInputManager::GetInstance()->Set_KeyLock(false);
    }
}

void CDialogue_UI::Typewriter(float fTimeDelta) {
    if (m_lines.empty()) return;
    const auto& L = m_lines[m_cur];
    if (m_charVisible >= (int)L.text.size()) return;

    m_acc += fTimeDelta * m_cps;
    while (m_acc >= 1.f && m_charVisible < (int)L.text.size()) {
        m_acc -= 1.f;
        ++m_charVisible;
    }
}

void CDialogue_UI::Start(const wstring& tag, bool typewriter, float cps)
{
    vector<Line> lines;
    DialogueFromTag(tag, lines);
    m_lines.swap(lines);

    m_useTypewriter = typewriter;

    m_charVisible = 0;
    m_cur = 0;
    m_acc = 0.f;
    m_blink = 0.f;
    m_bActive = !m_lines.empty();
}

void CDialogue_UI::DialogueFromTag(const wstring& tag, vector<Line>& out)
{
    out.clear();

    if (tag == L"Prologue") {
        out.push_back({ L"나레이션", L"대사대사대사..." });
        out.push_back({ L"나레이션", L"대사대사대사...대사대사대사...대사대사대사..." });
        return;
    }
    if (tag == L"Scream") {
        out.push_back({ L"경비병", L"대사대사대사..." });
        out.push_back({ L"플레이어", L"대사대사대사...대사대사대사...대사대사대사...대사대사대사..." });
        return;
    }
    if (tag == L"BossIntro_Zoom") {
        out.push_back({ L"Baron", L"...또 하나의 어린 양이 폭풍 속으로 걸어들어왔군. 감히 나의 어둠의 성채 앞에 서겠다는 건가?" });
        m_cps = 15.f;
        return;
    }
    if (tag == L"BossIntro_Left") {
        out.push_back({ L"Baron", L"일어나라, 나의 사역마들이여! 번개와 불길 사이의 공허에서 솟아올라, 이 침입자를 갈갈이 찢어라!" });
        m_cps = 15.f;
        return;
    }
    if (tag == L"BossIntro_Right") {
        out.push_back({ L"Baron", L"폭풍은 오직 나에게만 응답한다! 번개가 너의 눈을 멀게 하고, 비가 너를 삼키며, 얼음이 너의 비참한 영혼을 얼려버리리라!" });
        m_cps = 15.f;
        return;
    }
    if (tag == L"BossIntro_Origin") {
        out.push_back({ L"Baron", L"원위치 복귀 대사..." });
        m_cps = 15.f;
        return;
    }
    if (tag == L"BossPhase2_Intro") {
        out.push_back({ L"Baron", L"말도 안 돼... 네가 나의 폭풍을 밀어내다니? 좋아... 이제 별조차 삼켜버리는 진정한 심연을 보여주마..." });
        m_cps = 15.f;
        return;
    }
    if (tag == L"BossPhase2_IntroZoom") {
        out.push_back({ L"Baron", L"그만! 이제 속박에서 풀려난 폭풍을 목도하라! 하늘을 가르는 천둥을 느끼고, 대지를 씻어내리는 비를 맞으며, 영원의 서리가 모든 생명을 거두리라! 나는 종말이다, 그리고 너의 저항은 지금 끝난다!" });
        m_cps = 28.f;
        return;
    }
    out.push_back({ L"시스템", wstring(L"대사없음: ") + tag });
}

CDialogue_UI* CDialogue_UI::Create(LPDIRECT3DDEVICE9 pGraphicDev) {
    auto* p = new CDialogue_UI(pGraphicDev);
    if (FAILED(p->Ready_GameObject())) {
        Safe_Release(p);
        MSG_BOX("CDialogue_UI Create Failed");
        return nullptr;
    }
    return p;
}

void CDialogue_UI::Free() {
    CUI::Free();
}
