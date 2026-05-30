#include "pch.h"
#include "CPauseUI.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CInputManager.h"
#include "CUI_Number.h"
#include "CUI_Image.h"
#include "CPlayer.h"
#include "CObjectManager.h"
#include "CEventbus.h"
#include "CSetting_UI.h"
#include "CUIManager.h"

CPauseUI::CPauseUI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUI(pGraphicDev)
{

}

CPauseUI::CPauseUI(const CPauseUI& rhs)
	:CUI(rhs)
{

}

CPauseUI::~CPauseUI()
{

}

HRESULT	CPauseUI::Ready_GameObject()
{
	CUI::Ready_GameObject();

	while (ShowCursor(FALSE) >= 0);
	bActive = false;
	m_bActive = false;
	m_tHandle.m_eType = OBJ_UI;
	m_pTransform->Set_Scale({ 2.f, 2.f, 1.f });

	return S_OK;
}

_int CPauseUI::Update_GameObject(const _float& fTimeDelta)
{
	CUI::Update_GameObject(fTimeDelta);

	if (!m_bActive) return 0;

	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	m_hoverIndex = -1;

	D3DVIEWPORT9 vp;
	m_pGraphicDev->GetViewport(&vp);
	if (bActive == true)
	{
		for (int i = 0; i < 4; ++i)
		{
			LONG bannerHeight = 36;
			int  offset = 40;

			RECT rc = {
				LONG(vp.Width * 0.2f),
				LONG(vp.Height * 0.39f + offset * i),
				LONG(vp.Width * 0.8f),
				LONG(vp.Height * 0.39f + bannerHeight + offset * i)
			};

			EVENT event;

			if (PtInRect(&rc, ptMouse))
			{
				m_hoverIndex = i;

				// 클릭 처리
				if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::LBUTTON))
				{
					switch (i)
					{
					case 0:
						Set_Toggle();
						break;
					case 1:
						event = { PHASE::PHASE_BEGIN, TOPIC::Stage_Restart, CHANNEL::UI,
							nullptr, nullptr, {}, {} };

						CEventBus::GetInstance()->publish(event);
						break;
					case 2:
						CUIManager::GetInstance()->ActivateUI(L"Setting_UI");
						CUIManager::GetInstance()->DeactivateUI(L"PauseUI");
						break;
					case 3:
						Set_Toggle();
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
	return 0;
}

void CPauseUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CUI::LateUpdate_GameObject(fTimeDelta);
	if (CInputManager::GetInstance()->GetButtonDown(static_cast<KEY_TYPE>(VK_SPACE)))
		Set_Toggle();
}

void CPauseUI::Render_GameObject()
{
	if (!m_bActive) return;

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	const D3DXMATRIX parent = *m_pTransform->Get_World();

	CResourceManager::GetInstance()->Get<CTexture>(L"Back")->Set_Texture();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &parent);
	m_pBufferCom->Change_UV(1, 1, 1, 1);
	m_pBufferCom->Render_Buffer();

	{
		_vec3 p = *m_pTransform->GetInfoPos();
		D3DXMATRIX T, Sfade, worldFade;

		D3DXMatrixScaling(&Sfade, 1.0f, 0.35f, 1.0f);
		D3DXMatrixTranslation(&T, p.x, p.y, p.z);
		worldFade = Sfade * T;

		CResourceManager::GetInstance()->Get<CTexture>(L"Fade")->Set_Texture();
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &worldFade);
		m_pBufferCom->Change_UV(1, 1, 1, 1);
		m_pBufferCom->Render_Buffer();
	}

	const wchar_t* texts[4] = { L"Resume", L"Restart Level", L"Settings", L"Main Menu" };

	if (m_small)
	{
		D3DVIEWPORT9 vp;
		m_pGraphicDev->GetViewport(&vp);

		for (int i = 0; i < 4; i++)
		{
			LONG bannerHeight = 36;
			int offset = 40;
			RECT rc = {
				LONG(vp.Width * 0.2f),
				LONG(vp.Height * 0.39f + offset * i),
				LONG(vp.Width * 0.8f),
				LONG(vp.Height * 0.39f + bannerHeight + offset * i)
			};

			//선택된항목만색변경
			D3DCOLOR textCol = (i == m_hoverIndex)
				? D3DCOLOR_ARGB(255, 255, 255, 255)
				: D3DCOLOR_ARGB(255, 255, 198, 114);

			m_small->DrawTextW(nullptr, texts[i], -1, &rc,
				DT_CENTER | DT_VCENTER | DT_SINGLELINE, textCol);
		}
	}

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CPauseUI* CPauseUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPauseUI* pPauseUI = new CPauseUI(pGraphicDev);

	if (FAILED(pPauseUI->Ready_GameObject()))
	{
		// 만약에 제대로 생성이 되지 않았다면 바로 삭제.
		Safe_Release(pPauseUI);
		MSG_BOX("pPauseUI Create Failed");
		return nullptr;
	}

	return pPauseUI;
}

void CPauseUI::Free()
{
	CUI::Free();
}

void CPauseUI::Set_Toggle()
{
	bActive = !bActive;
	m_bActive = bActive;
	s_bPaused = bActive;

	if (bActive) {
		while (ShowCursor(TRUE) < 0);
	}
	else {
		while (ShowCursor(FALSE) >= 0);
	}

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
