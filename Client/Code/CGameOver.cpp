#include "pch.h"
#include "CGameOver.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CInputManager.h"
#include "CSelect_Achive_UI.h"
#include "CSelect_Stage_UI.h"
#include "CUI_Image.h"
#include "CUIManager.h"
#include "CEventbus.h"
#include "CSoundManager.h"
CGameOver::CGameOver(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUI(pGraphicDev)
{

}

CGameOver::CGameOver(const CGameOver& rhs)
	:CUI(rhs)
{

}

CGameOver::~CGameOver()
{

}

HRESULT	CGameOver::Ready_GameObject()
{
	CUI::Ready_GameObject();

	m_tHandle.m_eType = OBJ_UI;
	m_bActive = false;
	m_bVanish = false;
	m_playedDeathSfx = false;

	m_pTransform->Set_Scale({ 2.f, 2.4f, 1.f });

	return S_OK;
}

_int CGameOver::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bActive) return 0;
	Set_Toggle(fTimeDelta);	

	if (!m_bRestartUI && m_bDown && CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::LBUTTON))
	{
		m_bRestartUI = true;
		Set_Time();
		CSoundManager::GetInstance()->SoundPlay(L"player-die.wav", PLAYER_SOUND, CSoundManager::GetInstance()->GetVol().sfx * 0.2f, false);
		CUI::Update_GameObject(fTimeDelta);
		return 0;
	}

	CUI::Update_GameObject(fTimeDelta);

	return 0;
}

void CGameOver::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (!m_bActive)
		return;

	CInputManager::GetInstance()->Set_KeyLock(true);

	if (m_bDown && m_bRestartUI)
	{
		POINT ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		D3DVIEWPORT9 vp;
		m_pGraphicDev->GetViewport(&vp);
		m_hoverIndex = -1;
		for (int i = 0; i < 2; ++i)
		{
			LONG bannerHeight = 50;
			int offset = 40;
			RECT rc = {
				LONG(vp.Width * 0.4f),
				LONG(vp.Height * 0.37f + offset * i),
				LONG(vp.Width * 0.6f),
				LONG(vp.Height * 0.37f + bannerHeight + offset * i)
			};

			EVENT event;

			if (PtInRect(&rc, ptMouse))
			{
				m_hoverIndex = i;
				// 클릭 처리
				if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::LBUTTON))
				{
					CInputManager::GetInstance()->Set_KeyLock(false);
					CUIManager::GetInstance()->DeactivateUI(L"GameOver");
					switch (i)
					{
					case 0:
						event = { PHASE::PHASE_BEGIN, TOPIC::Stage_Restart, CHANNEL::UI,
							nullptr, nullptr, {}, {} };

						CEventBus::GetInstance()->publish(event);
						break;
					case 1:
						event = { PHASE::PHASE_BEGIN, TOPIC::Go_Main, CHANNEL::UI,
							nullptr, nullptr, {}, {} };

						CEventBus::GetInstance()->publish(event);
						break;
					}
				}
				break;
			}
		}
	}
	CUI::LateUpdate_GameObject(fTimeDelta);
}

void CGameOver::Render_GameObject()
{
	if (!m_bActive)
		return;

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
	CResourceManager::GetInstance()->Get<CTexture>(L"DeadScene")->Set_Texture();
	m_pBufferCom->Change_UV(1, 1, 1, 1);
	m_pBufferCom->Render_Buffer();

	if (m_bDown && !m_bRestartUI)
	{
		D3DVIEWPORT9 vp;
		m_pGraphicDev->GetViewport(&vp);

		RECT rcFIRE = {
		  LONG(vp.Width * 0.5f - (400 / 2)),
		  LONG(vp.Height * 0.5f - 80 / 2),
		  LONG(vp.Width * 0.5f + (400 / 2)),
		  LONG(vp.Height * 0.5f + 80 / 2)
		};
		m_big->DrawTextW(nullptr, L"You Have failed...", -1, &rcFIRE,
			DT_CENTER | DT_TOP | DT_SINGLELINE, D3DCOLOR_ARGB(255, 237, 206, 157));
		m_mid->DrawTextW(nullptr, L"<Press Fire key to continue>", -1, &rcFIRE,
			DT_CENTER | DT_BOTTOM | DT_SINGLELINE, D3DCOLOR_ARGB(255, 185, 111, 118));
	}
	else if (m_bDown && m_bRestartUI)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
		CResourceManager::GetInstance()->Get<CTexture>(L"Fade_Ground")->Set_Texture();
		m_pBufferCom->Change_UV(1, 1, 1, 1);
		m_pBufferCom->Render_Buffer();

		const wchar_t* texts[2] = { L"Restart", L"Main Menu" };
		D3DVIEWPORT9 vp;
		m_pGraphicDev->GetViewport(&vp);

		for (int i = 0; i < 2; i++)
		{
			LONG bannerHeight = 50;
			int offset = 40;
			RECT rc = {
				LONG(vp.Width * 0.4f),
				LONG(vp.Height * 0.37f + offset * i),
				LONG(vp.Width * 0.6f),
				LONG(vp.Height * 0.37f + bannerHeight + offset * i)
			};

			//선택된항목만색변경
			D3DCOLOR textCol = (i == m_hoverIndex)
				? D3DCOLOR_ARGB(255, 255, 255, 255)
				: D3DCOLOR_ARGB(255, 255, 198, 114);

			m_mid->DrawTextW(nullptr, texts[i], -1, &rc,
				DT_CENTER | DT_VCENTER | DT_SINGLELINE, textCol);
		}
	}
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CGameOver* CGameOver::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGameOver* pSelect_UI = new CGameOver(pGraphicDev);

	if (FAILED(pSelect_UI->Ready_GameObject()))
	{
		// 만약에 제대로 생성이 되지 않았다면 바로 삭제.
		Safe_Release(pSelect_UI);
		MSG_BOX("pSelect_UI Create Failed");
		return nullptr;
	}

	return pSelect_UI;
}

void CGameOver::Free()
{
	CUI::Free();
}

void CGameOver::Set_Toggle(const _float& fTimeDelta)
{
	static _vec3 vTargetPos = { 0.f, 0.f, 0.f };
	static _vec3 vStartPos = { 0.f, 1000.f, 0.f };
	static _vec3 vCurrentPos = vStartPos;

	if (m_bVanish) {
		vCurrentPos = vStartPos;
		m_bDown = false;
		m_bActive = false;
		m_playedDeathSfx = false;
		m_pTransform->Set_Pos(vCurrentPos.x, vCurrentPos.y, vCurrentPos.z);
		return;
	}

	const float a = (fTimeDelta > 0.f) ? min(1.f, 1.f * fTimeDelta) : 0.f;
	D3DXVec3Lerp(&vCurrentPos, &vCurrentPos, &vTargetPos, a);

	const float eps = 3.f;
	const bool arrived =
		fabsf(vCurrentPos.x - vTargetPos.x) < eps &&
		fabsf(vCurrentPos.y - vTargetPos.y) < eps &&
		fabsf(vCurrentPos.z - vTargetPos.z) < eps;

	if (arrived) {
		vCurrentPos = vTargetPos;
		m_bDown = true;
		m_bActive = true;

		if (!m_playedDeathSfx) {
			auto sm = CSoundManager::GetInstance();
			sm->SoundPlay(L"player-die.wav", PLAYER_SOUND, sm->GetVol().sfx, false);
			m_playedDeathSfx = true;
		}
	}
	else {
		m_bDown = false;
		m_bActive = true;
	}

	m_pTransform->Set_Pos(vCurrentPos.x, vCurrentPos.y, vCurrentPos.z);
}

void CGameOver::Set_Time()
{
	s_bPaused = m_bActive;

	if (m_bActive) {
		while (ShowCursor(TRUE) < 0);
	}
	else {
		while (ShowCursor(FALSE) >= 0);
	}
}
