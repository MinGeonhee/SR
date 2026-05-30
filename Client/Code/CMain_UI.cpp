#include "pch.h"
#include "CMain_UI.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CInputManager.h"
#include "CUI_Number.h"
#include "CUI_Image.h"
#include "CPlayer.h"
#include "CObjectManager.h"
#include "CEventbus.h"
#include "CSetting_UI.h"
#include "CSelect_UI.h"
#include "CUIManager.h"
#include "CSoundManager.h"
CMain_UI::CMain_UI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUI(pGraphicDev)
{

}

CMain_UI::CMain_UI(const CMain_UI& rhs)
	:CUI(rhs)
{

}

CMain_UI::~CMain_UI()
{

}

HRESULT	CMain_UI::Ready_GameObject()
{
	CUI::Ready_GameObject();

	while (ShowCursor(TRUE) < 0);
	m_bActive = true;
	m_prevHover = -1;
	m_tHandle.m_eType = OBJ_UI;
	m_pTransform->Set_Scale({ 2.f, 2.f, 1.f });
	return S_OK;
}

_int CMain_UI::Update_GameObject(const _float& fTimeDelta)
{
	CUI::Update_GameObject(fTimeDelta);
	if (!m_bActive) return 0;

	MainOpen = true; 
	for (auto* ui : m_vecUIObjects) {
		if (ui && ui->Is_UIActive()) {
			MainOpen = false;
			break;
		}
	}

	if (MainOpen == true)
	{
		POINT ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		int oldHover = m_hoverIndex;
		m_hoverIndex = -1;
		//텍스트 출력시랑 똑같은 범위의 사각형(선택범위)만들어서 클릭이벤트처리함 
		D3DVIEWPORT9 vp;
		m_pGraphicDev->GetViewport(&vp);

		for (int i = 0; i < 3; ++i)
		{
			LONG bannerHeight = 40;
			int  offset = 40;

			RECT rc = {
				LONG(vp.Width * 0.2f),
				LONG(vp.Height * 0.4f + offset * i),
				LONG(vp.Width * 0.8f),
				LONG(vp.Height * 0.4f + bannerHeight + offset * i)
			};

			EVENT event;

			if (PtInRect(&rc, ptMouse))
			{
				m_hoverIndex = i;
				// 클릭 처리
				if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::LBUTTON))
				{
					CSoundManager::GetInstance()->SoundPlay(L"ui-button-down.wav", SOUNDCHANNEL::UI_SOUND, CSoundManager::GetInstance()->GetVol().sfx);
					switch (i)
					{
					case 0:
						if (!m_pSelectUI) {
							m_pSelectUI = CSelect_UI::Create(m_pGraphicDev);
							if (m_pSelectUI) {
								m_vecUIObjects.push_back(m_pSelectUI);
							}
						}
						if (m_pSelectUI) {
							m_pSelectUI->Set_UIActive(true);
							m_pSelectUI->Set_Vanish(false);
						}
						break;
					case 1:
						CUIManager::GetInstance()->ActivateUI(L"Setting_UI");
						CUIManager::GetInstance()->DeactivateUI(L"Main_UI");
						break;
					case 2:
						DestroyWindow(g_hWnd);
						break;
					}
				}
				break;
			}
		}
		if (m_hoverIndex != m_prevHover && m_hoverIndex >= 0) {
			CSoundManager::GetInstance()->SoundPlay(L"ui-button-down.wav", SOUNDCHANNEL::UI_SOUND, CSoundManager::GetInstance()->GetVol().sfx);
		}
		m_prevHover = m_hoverIndex;
	}
	else {
		m_prevHover = -1;
		m_hoverIndex = -1;
	}
	
	for (auto& pPlayerUI : m_vecUIObjects)
		pPlayerUI->Update_GameObject(fTimeDelta);
	return 0;
}

void CMain_UI::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CUI::LateUpdate_GameObject(fTimeDelta);
	for (auto& pPlayerUI : m_vecUIObjects)
		pPlayerUI->LateUpdate_GameObject(fTimeDelta);
}

void CMain_UI::Render_GameObject()
{
	if (!m_bActive)
		return;

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	if (MainOpen == true)
	{
		const wchar_t* texts[3] = { L"Play", L"Settings", L"Resign" };

		if (m_mid)
		{
			D3DVIEWPORT9 vp;
			m_pGraphicDev->GetViewport(&vp);

			for (int i = 0; i < 3; i++)
			{
				LONG bannerHeight = 40;
				int offset = 40;
				RECT rc = {
					LONG(vp.Width * 0.2f),
					LONG(vp.Height * 0.4f + offset * i),
					LONG(vp.Width * 0.8f),
					LONG(vp.Height * 0.4f + bannerHeight + offset * i)
				};

				//선택된항목만색변경
				D3DCOLOR textCol = (i == m_hoverIndex)
					? D3DCOLOR_ARGB(255, 255, 255, 255)
					: D3DCOLOR_ARGB(255, 255, 198, 114);

				m_mid->DrawTextW(nullptr, texts[i], -1, &rc,
					DT_CENTER | DT_VCENTER | DT_SINGLELINE, textCol);
			}
		}
	}
	
	for (auto& pPlayerUI : m_vecUIObjects)
		pPlayerUI->Render_GameObject();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CMain_UI* CMain_UI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMain_UI* pMainUI = new CMain_UI(pGraphicDev);

	if (FAILED(pMainUI->Ready_GameObject()))
	{
		// 만약에 제대로 생성이 되지 않았다면 바로 삭제.
		Safe_Release(pMainUI);
		MSG_BOX("pMainUI Create Failed");
		return nullptr;
	}

	return pMainUI;
}

void CMain_UI::Free()
{
	for (auto& pPlayerUI : m_vecUIObjects)
		Safe_Release(pPlayerUI);

	CUI::Free();
}