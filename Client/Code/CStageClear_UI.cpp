#include "pch.h"
#include "CStageClear_UI.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CInputManager.h"
#include "CUI_Number.h"
#include "CUI_Image.h"
#include "CPlayer.h"
#include "CObjectManager.h"
#include "CEventbus.h"
#include "CSceneManager.h"
#include "CScoreBoard.h"
#include "CMonsterSystem.h"
#include "CSoundManager.h"
CStageClear_UI::CStageClear_UI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUI(pGraphicDev)
{

}

CStageClear_UI::CStageClear_UI(const CStageClear_UI& rhs)
	:CUI(rhs)
{

}

CStageClear_UI::~CStageClear_UI()
{

}

HRESULT	CStageClear_UI::Ready_GameObject()
{
	CUI::Ready_GameObject();

	m_bActive = false;
	bActive = true;
	PrintOn = false;

	m_tHandle.m_eType = OBJ_UI;

	if (FAILED(D3DXCreateSprite(m_pGraphicDev, &m_pTextSprite)))
		return E_FAIL;

	for (int i = 0; i < 12; ++i) m_lineTime[i] = -1.f;
	m_revealTimer = 0.f;
	m_revealedCount = 0;

	for (int i = 0; i < 3; ++i)
	{
		auto icon = CUI_Image::Create(m_pGraphicDev, L"ClearIcon", 3);
		icon->SetSpriteIndex(i);
		icon->Set_Position(-220.f, 170.f - 40.f * i, 0.f);
		icon->Set_Scale({ 0.f, 0.f, 1.f });
		m_vecUIObjects.push_back(icon);
		m_lineIcons[i] = icon;
	}

	Set_Ani();
	return S_OK;
}

_int CStageClear_UI::Update_GameObject(const _float& fTimeDelta)
{
	bActive = m_bActive;
	if (!m_bActive) { m_prevActive = false; return 0; }
	if (m_bActive && !m_prevActive) {
		for (int i = 0; i < 12; ++i) m_lineTime[i] = -1.f;
		m_revealTimer = 0.f;
		m_revealedCount = 0;
		m_overallTime = -1.f;
		PrintOn = false;

		m_bPulled = false;
		PullStageResult();
		m_bPulled = true;

		CSoundManager::GetInstance()->PlayBGM(L"Victory.wav", CSoundManager::GetInstance()->GetVol().music);
		iKillCount = CMonsterSystem::GetInstance()->GetMonsterKill();
		Set_Ani();
	}
	m_prevActive = m_bActive;

	const float uiDt = (fTimeDelta > 0.f) ? fTimeDelta : (1.f / 300.f);
	CUI::Update_GameObject(uiDt);

	if (m_pAniCom->Get_IsAnimationEnd())
		PrintOn = true;

	CInputManager::GetInstance()->Set_KeyLock(true);

	if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::LBUTTON))
	{
		bActive = false;
		m_bActive = false;
		EVENT event
		{
			PHASE::PHASE_BEGIN,
			TOPIC::Open_Shop,
			CHANNEL::UI,
			nullptr,
			nullptr,
			{},
			{}
		};
		CEventBus::GetInstance()->publish(event);
	}

	Set_Toggle();
	for (auto& pIcon : m_vecUIObjects)
		pIcon->Update_GameObject(uiDt);

	if (PrintOn) {
		const wchar_t* Achive[12] = { nullptr, nullptr, nullptr, nullptr, L"TREASURE",  nullptr, L"YOUR TIME", L"PAR TIME", L"BEST TIME" ,  nullptr, L"LEVEL SCORE", L"BEST SCORE" };
		const int      ScoreNum[12] = { iKillCount, 1000, 1000, -1, m_treasure, -1, -1, -1, -1, -1, m_totalScore, m_totalScore };

		m_revealTimer += uiDt;
		while (m_revealTimer >= interval && m_revealedCount < 12) {
			int i = m_revealedCount;

			if (Achive[i] || ScoreNum[i] != -1) {
				if (m_lineTime[i] < 0.f) m_lineTime[i] = 0.f;
			}
			++m_revealedCount;
			m_revealTimer -= interval;
		}

		for (int i = 0; i < 12; ++i) {
			if (m_lineTime[i] >= 0.f)
				m_lineTime[i] += uiDt;
		}

		if (m_overallTime < 0.f) {
			bool all_done = true;
			for (int i = 0; i < 12; ++i) {
				if (!(Achive[i] || ScoreNum[i] != -1)) continue;

				if (m_lineTime[i] < 0.f || m_lineTime[i] <= pause_time) {
					all_done = false;
					break;
				}
			}
			if (all_done)
				m_overallTime = 0.f;
		}

		if (m_overallTime >= 0.f)
		{
			m_overallTime += uiDt;
			if (m_overallTime > pause_time)
				m_overallTime = pause_time;
		}
		for (int i = 0; i < 3; ++i)
		{
			float age = m_lineTime[i];
			float factor = 0.f;

			if (age >= 0.f) {
				factor = 1.f;
				if (age <= pause_time) {
					float t = age / pause_time;
					factor = 1.5f + 0.15f * sinf(t * 3.1415926f);
				}
			}
			else {
				factor = 0.f;
			}

			if (m_lineIcons[i]) {
				_vec3 s = m_iconBaseScale[i];
				m_lineIcons[i]->Set_Scale({ s.x * factor, s.y * factor, 1.f });
			}
		}
	}

	return 0;
}

void CStageClear_UI::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CUI::LateUpdate_GameObject(fTimeDelta);
	for (auto& pIcon : m_vecUIObjects)
		pIcon->LateUpdate_GameObject(fTimeDelta);
}

void CStageClear_UI::Render_GameObject()
{
	if (!m_bActive)
		return;
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	const D3DXMATRIX parent = *m_pTransform->Get_World();

	CResourceManager::GetInstance()->Get<CTexture>(L"Back")->Set_Texture();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
	m_pBufferCom->Change_UV(1, 1, 1, 1);
	m_pBufferCom->Render_Buffer();

	D3DVIEWPORT9 vp;
	m_pGraphicDev->GetViewport(&vp);

	RECT rcFIRE = {
		LONG(vp.Width * 0.5f - (400 / 2)),
		LONG(vp.Height * 0.92f),
		LONG(vp.Width * 0.5f + (400 / 2)),
		LONG(vp.Height * 0.92f + 36)
	};
	if (m_mid)
		m_mid->DrawTextW(nullptr, L"<Press \"Fire\" to Continue>", -1, &rcFIRE,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE, D3DCOLOR_ARGB(255, 237, 207, 149));

	D3DXMATRIX TR, TL, S, W, R;
	D3DXMatrixIdentity(&S);
	D3DXMatrixIdentity(&R);
	D3DXMatrixTranslation(&TL, -400.f, -250.f, 0.f);
	W = parent * (S * R * TL);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &W);
	CResourceManager::GetInstance()->Get<CTexture>(L"ClearFlag")->Set_Texture();
	m_pAniCom->Render_Buffer();

	D3DXMatrixTranslation(&TR, 400.f, -250.f, 0.f);
	W = parent * (S * R * TR);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &W);
	CResourceManager::GetInstance()->Get<CTexture>(L"ClearFlag")->Set_Texture();
	m_pAniCom->Render_Buffer();

	if(PrintOn == true)
	Render_Text();

	for (auto& pIcon : m_vecUIObjects)
		pIcon->Render_GameObject();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CStageClear_UI::Render_Text()
{
	if (!PrintOn || !m_pTextSprite) return;

	D3DVIEWPORT9 vp;
	m_pGraphicDev->GetViewport(&vp);

	const wchar_t* StageName[4] = { L"Tutorial", L"Canal Streets", L"Stage 2", L"Boss Field"};
	const wchar_t* Achive[12] = { nullptr, nullptr, nullptr, nullptr, L"TREASURE",  nullptr, L"YOUR TIME", L"PAR TIME", L"BEST TIME" ,  nullptr, L"LEVEL SCORE", L"BEST SCORE" };
	const int      ScoreNum[12] = { iKillCount, 1000, 1000, -1, m_treasure, -1, -1, -1, -1, -1, m_totalScore, m_totalScore };

	wchar_t rbuf[12][32] = {};
	const wchar_t* RightText[12] = { nullptr };

	auto put_time = [&](int idx, float t) {
		CUI::FormatTimeStr(t, rbuf[idx], _countof(rbuf[idx]));
		RightText[idx] = rbuf[idx];
	};

	put_time(6, m_clearTime);
	put_time(7, m_parTime);
	put_time(8, m_bestTime);

	{
		RECT rcTitle = {
			LONG(vp.Width * 0.5f - (300 / 2)),
			LONG(vp.Height * 0.05f),
			LONG(vp.Width * 0.5f + (300 / 2)),
			LONG(vp.Height * 0.05f + 80)
		};
		m_big->DrawTextW(nullptr, StageName[m_iStageNum], -1, &rcTitle,
			DT_CENTER | DT_TOP | DT_SINGLELINE, D3DCOLOR_ARGB(255, 236, 219, 147));
		m_small->DrawTextW(nullptr, L"Cleared", -1, &rcTitle,
			DT_CENTER | DT_BOTTOM | DT_SINGLELINE, D3DCOLOR_ARGB(255, 255, 236, 136));
	}

	auto Rect = [&](int row) {
		RECT r = {
			LONG(vp.Width * 0.5f - (500 / 2)),
			LONG(vp.Height * 0.25f + 30 * row),
			LONG(vp.Width * 0.5f + (500 / 2)),
			LONG(vp.Height * 0.25f + 30 * row + 36)
		};
		return r;
	};

	m_pTextSprite->Begin(D3DXSPRITE_ALPHABLEND);

	for (int i = 0; i < 12; ++i) {
		if (m_lineTime[i] < 0.f) continue;
		if (!Achive[i] && ScoreNum[i] == -1 && !RightText[i]) continue;

		float scale = 1.f;
		float age = m_lineTime[i];
		if (age <= pause_time) {
			float t = age / pause_time;
			scale = 1.5f + 0.15f * sinf(t * 3.1415926f);
		}

		auto TextbaseCol = D3DCOLOR_ARGB(255, 210, 191, 168);
		auto TexthiCol = D3DCOLOR_ARGB(255, 224, 210, 194);
		D3DCOLOR Textcol = (age <= pause_time) ? TexthiCol : TextbaseCol;

		auto NumbaseCol = D3DCOLOR_ARGB(255, 236, 219, 147);
		auto NumhiCol = D3DCOLOR_ARGB(255, 255, 240, 160);
		D3DCOLOR Numcol = (age <= pause_time) ? NumhiCol : NumbaseCol;

		// 왼쪽 라벨
		if (Achive[i] && m_small) {
			RECT rc = Rect(i);
			D3DXMATRIX mat;
			D3DXVECTOR2 center((float)rc.left, (float)(rc.top + rc.bottom) * 0.5f);
			D3DXVECTOR2 s(scale, scale);
			D3DXMatrixTransformation2D(&mat, &center, 0.f, &s, &center, 0.f, nullptr);
			m_pTextSprite->SetTransform(&mat);

			m_small->DrawTextW(m_pTextSprite, Achive[i], -1, &rc,
				DT_LEFT | DT_VCENTER | DT_SINGLELINE, Textcol);
		}

		// 오른쪽 값: 문자열(시간/임의 텍스트)이 우선
		RECT rc = Rect(i);
		D3DXMATRIX mat;
		D3DXVECTOR2 center((float)rc.right, (float)(rc.top + rc.bottom) * 0.5f);
		D3DXVECTOR2 s(scale, scale);
		D3DXMatrixTransformation2D(&mat, &center, 0.f, &s, &center, 0.f, nullptr);
		m_pTextSprite->SetTransform(&mat);

		if (RightText[i]) {
			m_small->DrawTextW(m_pTextSprite, RightText[i], -1, &rc,
				DT_RIGHT | DT_VCENTER | DT_SINGLELINE, Numcol);
		}
		else if (ScoreNum[i] != -1) {
			wchar_t buf[32];
			_snwprintf_s(buf, 32, L"%d", ScoreNum[i]);
			m_small->DrawTextW(m_pTextSprite, buf, -1, &rc,
				DT_RIGHT | DT_VCENTER | DT_SINGLELINE, Numcol);
		}
	}

	// 전체 스코어 블록 (OVERALL)
	if (m_overallTime >= 0.f) {
		RECT rcScore = {
			LONG(vp.Width * 0.5f - (400 / 2)),
			LONG(vp.Height * 0.8f),
			LONG(vp.Width * 0.5f + (400 / 2)),
			LONG(vp.Height * 0.8f + 72)
		};

		float t = m_overallTime / pause_time;
		float scale = 1.0f + 0.3f * sinf(t * 3.1415926f);

		D3DXVECTOR2 center((float)(rcScore.left + rcScore.right) * 0.5f,
			(float)(rcScore.top + rcScore.bottom) * 0.5f);
		D3DXVECTOR2 s(scale, scale);
		D3DXMATRIX mat;
		D3DXMatrixTransformation2D(&mat, &center, 0.f, &s, &center, 0.f, nullptr);
		m_pTextSprite->SetTransform(&mat);

		m_mid->DrawTextW(m_pTextSprite, L"~ Score OVERALL ~", -1, &rcScore,
			DT_CENTER | DT_TOP | DT_SINGLELINE, D3DCOLOR_ARGB(255, 245, 162, 80));

		wchar_t buf[32];
		_snwprintf_s(buf, 32, L"%d", m_totalScore);
		m_mid->DrawTextW(m_pTextSprite, buf, -1, &rcScore,
			DT_CENTER | DT_BOTTOM | DT_SINGLELINE, D3DCOLOR_ARGB(255, 255, 243, 203));
	}

	m_pTextSprite->End();

	D3DXMATRIX id; D3DXMatrixIdentity(&id);
	m_pTextSprite->SetTransform(&id);
}

CStageClear_UI* CStageClear_UI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStageClear_UI* pStageClear = new CStageClear_UI(pGraphicDev);

	if (FAILED(pStageClear->Ready_GameObject()))
	{
		Safe_Release(pStageClear);
		MSG_BOX("pStageClear Create Failed");
		return nullptr;
	}

	return pStageClear;
}

void CStageClear_UI::Free()
{
	if (m_pTextSprite) { m_pTextSprite->Release(); m_pTextSprite = nullptr; }
	for (auto& pIcon : m_vecUIObjects)
		Safe_Release(pIcon);
	m_vecUIObjects.clear();

	CUI::Free();
}

void CStageClear_UI::Set_Ani()
{
	m_pAniCom->SetGrid(5, 1);
	m_pAniCom->SetRow(1);
	m_pAniCom->AniMation_Init(5, 0.1f);
	m_pAniCom->SetPlayMode(CAnimation::PlayMode::Once);
	m_pAniCom->PlayAnimation();
	m_pTransform->Set_Scale(_vec3{ 120.f, 600.f, 10.f });
}

void CStageClear_UI::Set_Toggle()
{
	s_bPaused = bActive;

	if (s_bPaused)
	{
		EVENT event
		{
			PHASE::PHASE_BEGIN,
			TOPIC::TIME_STOP,
			CHANNEL::Global,
			nullptr,
			nullptr,
			{},
			{}
		};

		CEventBus::GetInstance()->publish(event);
	}
	else
	{
		EVENT event
		{
			PHASE::PHASE_BEGIN,
			TOPIC::TIME_RESUME,
			CHANNEL::Global,
			nullptr,
			nullptr,
			{},
			{}
		};

		CEventBus::GetInstance()->publish(event);
	}
}

void CStageClear_UI::PullStageResult()
{
	auto type = CSceneManager::GetInstance()->Get_CurrentSceneType();

	CScene* scene = CSceneManager::GetInstance()->Get_CurrentScene();
	if (!scene) return;

	switch (type) {
	case SCENETYPE::TUTORIAL: {
		m_clearTime = CScoreBoard::GetInstance()->GetStage(TUTORIAL).lastTime;
		m_parTime = CScoreBoard::GetInstance()->GetStage(TUTORIAL).parTime;
		m_bestTime = CScoreBoard::GetInstance()->GetStageBestTime(TUTORIAL);
		m_BestScore = CScoreBoard::GetInstance()->GetStage(TUTORIAL).bestScore;
		m_LevelScore = CScoreBoard::GetInstance()->GetStage(TUTORIAL).lastScore;
		m_totalScore = CScoreBoard::GetInstance()->GetTotalScore();
		m_iStageNum = 0;
		break;
	}
	case SCENETYPE::STAGE1: {
		m_clearTime = CScoreBoard::GetInstance()->GetStage(STAGE1).lastTime;
		m_parTime = CScoreBoard::GetInstance()->GetStage(STAGE1).parTime;
		m_bestTime = CScoreBoard::GetInstance()->GetStageBestTime(STAGE1);
		m_BestScore = CScoreBoard::GetInstance()->GetStage(STAGE1).bestScore;
		m_LevelScore = CScoreBoard::GetInstance()->GetStage(STAGE1).lastScore;
		m_totalScore = CScoreBoard::GetInstance()->GetTotalScore();
		m_iStageNum = 1;
		break;
	}
	case SCENETYPE::STAGE2:
		m_clearTime = CScoreBoard::GetInstance()->GetStage(STAGE2).lastTime;
		m_parTime = CScoreBoard::GetInstance()->GetStage(STAGE2).parTime;
		m_bestTime = CScoreBoard::GetInstance()->GetStageBestTime(STAGE2);
		m_BestScore = CScoreBoard::GetInstance()->GetStage(STAGE2).bestScore;
		m_LevelScore = CScoreBoard::GetInstance()->GetStage(STAGE2).lastScore;
		m_totalScore = CScoreBoard::GetInstance()->GetTotalScore();
		m_iStageNum = 2;
		break;
	case SCENETYPE::BOSS: {
		m_clearTime = CScoreBoard::GetInstance()->GetStage(BOSS).lastTime;
		m_parTime = CScoreBoard::GetInstance()->GetStage(BOSS).parTime;
		m_bestTime = CScoreBoard::GetInstance()->GetStageBestTime(BOSS);
		m_BestScore = CScoreBoard::GetInstance()->GetStage(BOSS).bestScore;
		m_LevelScore = CScoreBoard::GetInstance()->GetStage(BOSS).lastScore;
		m_totalScore = CScoreBoard::GetInstance()->GetTotalScore();
		m_iStageNum = 3;
		break;
	}
	default:
		break;
	}
}

