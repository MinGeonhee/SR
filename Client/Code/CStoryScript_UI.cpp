#include "pch.h"
#include "CStoryScript_UI.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CInputManager.h"
#include "CSelect_Achive_UI.h"
#include "CSelect_Stage_UI.h"
#include "CUI_Image.h"
#include "CUIManager.h"
#include "CEventbus.h"
#include "CSoundManager.h"

const wchar_t* CStoryScript_UI::PrologueTexts[] = {
	L"한때 평화로웠던 테라브루마의 왕국에서 고대의 봉인이 산산이 부서지며, "
	L"땅 전역으로 길들일 수 없는 어둠이 분출했다. 서쪽에서 온 용병들은 "
	L"혼돈에 물든 마음으로 경계 없는 마을들을 습격하여, 자비 없이 약탈과 파괴를 자행했다.",

	L"험준한 북부 산맥에서 불어온 어둠은 남쪽으로 서서히 스며들었고, "
	L"그 영향은 물과 흙, 그리고 공기마저 오염시켜 땅의 숨결 자체를 뒤틀어 놓았다.",

	L"절망이 짙어지자, 테라브루마의 사람들은 마지막 희망을 모아 절박한 탄원, "
	L"클라리온 콜을 왕국 전역으로 울려 퍼지게 했다. 그 목소리는 혼돈에 맞설 수 있는 자들-"
	L"비술의 피난처이자 지식의 성채인 위조덤의 마도사들-의 귀에까지 닿았다.",

	L"긴급한 소집에 응해 위조덤은 각 마법 학파의 정예 주술사들을 파견했다. "
	L"그들은 혼돈의 근원을 추적하고 결박하기 위해 뜻을 모았으나, "
	L"용맹한 그들의 노력에도 불구하고 누구 하나 돌아오지 못했다. "
	L"그들의 행방은 끝내 수수께끼로 남았다.",

	L"그리고 이제, 너-젊고 유망한 마도사 한 명만이 남은 희미한 등불처럼 "
	L"위조덤을 대표해 어둠과 마주 서 있다.",

	L"네 앞에는 한때 상업과 삶의 활기로 들끓던 그림브룩의 변두리가 펼쳐져 있다. "
	L"지금은 그림자만 드리운 채 초라한 껍데기만 남았고, "
	L"혼돈의 파고가 휩쓸고 지나간 상흔이 골목마다 깊게 패여 있다."
};
const int CStoryScript_UI::PrologueTextCount =
sizeof(CStoryScript_UI::PrologueTexts) / sizeof(CStoryScript_UI::PrologueTexts[0]);

const wchar_t* CStoryScript_UI::EpilogueTexts[] = {
	L"천둥을 가르던 번개가 꺼지고, 칼날 같던 비가 그쳤다. "
	L"암흑의 주문이 풀리자 성채를 둘러싸던 폭풍이 허물어지고, "
	L"발밑의 돌바닥은 얼음의 굴레에서 해방되어 서서히 본래의 숨을 되찾았다.",

	L"너는 쓰러진 마도사의 홀을 밟아 무너지는 제단을 넘었다. "
	L"심연에서 올라오던 귓속말이 침묵으로 잠기고, "
	L"성벽 곳곳에 박힌 룬은 마지막 반짝임을 남기고 소멸했다.",

	L"그림브룩의 하늘은 오랜만에 구름을 걷어내고 새벽빛을 품었다. "
	L"사람들은 조심스레 문을 열고 나와 서로의 얼굴을 확인했고, "
	L"어제와 다른 오늘을 믿기 위해 떨리는 손을 맞잡았다.",

	L"오염되었던 샘은 다시 맑은 물을 토해냈고, "
	L"말라붙었던 밭고랑에는 이슬이 맺혔다. "
	L"폐허가 된 시장에는 아이들의 웃음이 조심스레 돌아왔다.",

	L"너는 위조덤의 기록관에게 전투의 전말과 사라졌던 동료들의 흔적을 보고했다. "
	L"누군가는 돌아오지 못했으나, 그들의 이름은 성소의 벽에 새겨져 "
	L"새로운 세대의 등불이 되리라.",

	L"왕국은 잿더미 속에서도 다시 일어섰다. "
	L"각 길드와 학파는 협력하여 무너진 마을을 세우고, "
	L"흩어진 지식과 봉인을 정비하며, 혼돈의 흔적을 추적했다.",

	L"너는 마지막으로, 어둠의 성채가 있던 곳을 돌아보았다. "
	L"완전히 사라지지 않은 서늘한 기운 속에서, "
	L"부서진 봉인의 파편이 희미하게 반짝였다-경고처럼, 혹은 예고처럼.",

	L"그날 밤, 별들은 유난히 맑았다. "
	L"오랜 폭풍이 남긴 상처 위로 빛은 고르게 내려앉았고, "
	L"왕국은 오랜만의 깊은 잠에 들었다. "
	L"그리고 너는-다음 여정을 위해, 다시 지팡이를 세웠다."
};
const int CStoryScript_UI::EpilogueTextCount =
sizeof(CStoryScript_UI::EpilogueTexts) / sizeof(CStoryScript_UI::EpilogueTexts[0]);

CStoryScript_UI::CStoryScript_UI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUI(pGraphicDev)
{

}

CStoryScript_UI::CStoryScript_UI(const CStoryScript_UI& rhs)
	:CUI(rhs)
{

}

CStoryScript_UI::~CStoryScript_UI()
{

}

HRESULT	CStoryScript_UI::Ready_GameObject()
{
	CUI::Ready_GameObject();

	m_tHandle.m_eType = OBJ_UI;

	m_pTransform->Set_Scale({ 2.f, 1.7f, 1.f });

	m_currTexts = PrologueTexts;
	m_currCount = PrologueTextCount;
	m_currTitle = L"~ Prologue ~";
	m_currBgTex = L"Prologue";
	return S_OK;
}

_int CStoryScript_UI::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bActive && !m_prevActive) {
		D3DVIEWPORT9 vp; m_pGraphicDev->GetViewport(&vp);
		m_scrollOfs = vp.Height * 0.8f;
		m_bTextUp = false;
		m_bMove = true;
	}
	m_prevActive = m_bActive;

	const wstring& tag = CUIManager::GetInstance()->GetCategoryTag(L"StoryScript_UI");
	if (!tag.empty() && tag != m_cachedTag) {
		m_cachedTag = tag;
		ApplyTag(tag);
	}
	if (!m_bActive) return 0;

	if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::LBUTTON))
		m_bTextUp = true;

	if (tag == L"Epilogue") {
		if (CInputManager::GetInstance()->GetButtonDown(static_cast<KEY_TYPE>(VK_RETURN)))
			CUIManager::GetInstance()->ActivateUI(L"EndingCredit");
	}
	else if (tag == L"Prologue")
	{
		if (CInputManager::GetInstance()->GetButtonDown(static_cast<KEY_TYPE>(VK_RETURN)))
		{
			EVENT event;
			event = { PHASE::PHASE_BEGIN, TOPIC::Go_1st, CHANNEL::UI,
					  nullptr, nullptr, {}, {} };
			CEventBus::GetInstance()->publish(event);
		}
	}

	if (m_bTextUp) {
		m_scrollOfs = 0.f;
	}
	else {
		m_scrollOfs = max(0.f, m_scrollOfs - m_scrollSpeed * fTimeDelta);
	}

	if (m_bMove && m_scrollOfs <= 0.001f) {
		m_scrollOfs = 0.f;
		m_bMove = false;
	}

	CUI::Update_GameObject(fTimeDelta);
	return 0;
}

void CStoryScript_UI::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (!m_bActive)
		return;
	CUI::LateUpdate_GameObject(fTimeDelta);
}

void CStoryScript_UI::Render_GameObject()
{
	if (!m_bActive)
		return;

	D3DVIEWPORT9 vp;
	m_pGraphicDev->GetViewport(&vp);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
	CResourceManager::GetInstance()->Get<CTexture>(m_currBgTex)->Set_Texture();
	m_pBufferCom->Change_UV(1, 1, 1, 1);
	m_pBufferCom->Render_Buffer();

	RECT rcTitle = {
	  LONG(vp.Width * 0.5f - (400 / 2)),
	  LONG(vp.Height * 0.1f - 80 / 2),
	  LONG(vp.Width * 0.5f + (400 / 2)),
	  LONG(vp.Height * 0.1f + 80 / 2)
	};
	m_mid->DrawTextW(nullptr, m_currTitle, -1, &rcTitle,
		DT_CENTER | DT_TOP | DT_SINGLELINE, D3DCOLOR_ARGB(255, 255, 255, 255));

	// 프롤로그 텍스트
	const LONG left = LONG(vp.Width * 0.08f);
	const LONG right = LONG(vp.Width * 0.92f);
	LONG curY = LONG(vp.Height * 0.14f) + LONG(m_scrollOfs);
	const int paraGap = 14;

	for (int i = 0; i < m_currCount; ++i) {
		RECT rcCalc = { left, curY, right, LONG(vp.Height * 0.95f) };
		DWORD fmtCalc = DT_WORDBREAK | DT_TOP | DT_CENTER | DT_CALCRECT;
		m_small->DrawTextW(nullptr, m_currTexts[i], -1, &rcCalc, fmtCalc, 0);
		const int paraH = rcCalc.bottom - rcCalc.top;

		RECT rcDraw = { left, curY, right, curY + paraH };
		DrawShadowedText(
			m_small, m_currTexts[i], rcDraw,
			D3DCOLOR_ARGB(255, 237, 206, 157),
			2, D3DCOLOR_ARGB(160, 0, 0, 0),
			DT_WORDBREAK | DT_TOP | DT_CENTER
		);

		curY += paraH + paraGap;
	}

	if (!m_bMove)
	{
		RECT rcTitle = {
		  LONG(vp.Width * 0.5f - (400 / 2)),
		  LONG(vp.Height * 0.92f - 80 / 2),
		  LONG(vp.Width * 0.5f + (400 / 2)),
		  LONG(vp.Height * 0.92f + 80 / 2)
		};
		m_mid->DrawTextW(nullptr, L"<press Enter to continue>", -1, &rcTitle,
			DT_CENTER | DT_TOP | DT_SINGLELINE, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CStoryScript_UI* CStoryScript_UI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStoryScript_UI* pSelect_UI = new CStoryScript_UI(pGraphicDev);

	if (FAILED(pSelect_UI->Ready_GameObject()))
	{
		// 만약에 제대로 생성이 되지 않았다면 바로 삭제.
		Safe_Release(pSelect_UI);
		MSG_BOX("pSelect_UI Create Failed");
		return nullptr;
	}

	return pSelect_UI;
}

void CStoryScript_UI::Free()
{
	CUI::Free();
}

void CStoryScript_UI::ApplyTag(const wstring& tag)
{
	if (tag == L"Prologue") {
		m_currTexts = PrologueTexts;
		m_currCount = PrologueTextCount;
		m_currTitle = L"~ Prologue ~";
		m_currBgTex = L"Prologue";
		CSoundManager::GetInstance()->PlayBGM(L"Story_Telling_Loop.wav", CSoundManager::GetInstance()->GetVol().music);
		CSoundManager::GetInstance()->SoundPlay(L"fire-strong-loop.wav", EMBIENT_SOUND, CSoundManager::GetInstance()->GetVol().ambient);
		//CSoundManager::GetInstance()->SoundPlay(L"story-prologue.wav", EVENT_SOUND, CSoundManager::GetInstance()->GetVol().ambient);
	}
	else if (tag == L"Epilogue") {
		m_currTexts = EpilogueTexts;
		m_currCount = EpilogueTextCount;
		m_currTitle = L"~ Epilogue ~";
		m_currBgTex = L"Epilogue";
		CSoundManager::GetInstance()->PlayBGM(L"story-epilogoue-loop.wav", CSoundManager::GetInstance()->GetVol().music);
		CSoundManager::GetInstance()->SoundPlay(L"ambient-city.wav", EMBIENT_SOUND, CSoundManager::GetInstance()->GetVol().ambient);
		//CSoundManager::GetInstance()->SoundPlay(L"story-epilogue.wav", EVENT_SOUND, CSoundManager::GetInstance()->GetVol().ambient);
	}
	else {

		m_currTexts = PrologueTexts;
		m_currCount = PrologueTextCount;
		m_currTitle = L"~ Prologue ~";
		m_currBgTex = L"Prologue";
	}

	D3DVIEWPORT9 vp; m_pGraphicDev->GetViewport(&vp);
	m_scrollOfs = vp.Height * 0.8f;
	m_bTextUp = false;
	m_bMove = true;
}
