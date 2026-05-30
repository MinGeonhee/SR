#include "pch.h"
#include "CSetting_UI.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CInputManager.h"
#include "CUI_Number.h"
#include "CUI_Image.h"
#include "CPlayer.h"
#include "CObjectManager.h"
#include "CEventbus.h"
#include "CUIManager.h"
#include "CSoundManager.h"

CSetting_UI::CSetting_UI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUI(pGraphicDev)
{

}

CSetting_UI::CSetting_UI(const CSetting_UI& rhs)
	:CUI(rhs)
{

}

CSetting_UI::~CSetting_UI()
{

}

HRESULT	CSetting_UI::Ready_GameObject()
{
	CUI::Ready_GameObject();

	m_bActive = true;
	m_tHandle.m_eType = OBJ_UI;

	m_audioVals = { 100, 100, 100, 100 };
	m_dragging = false;
	m_dragIndex = -1;

	D3DXCreateLine(m_pGraphicDev, &m_pLine);

	if (auto sm = CSoundManager::GetInstance())
	{
		CSoundManager::AudioVolumes v = sm->GetVol();
		m_audioVals[0] = int(v.master * 100.0f + 0.5f);
		m_audioVals[1] = int(v.sfx * 100.0f + 0.5f);
		m_audioVals[2] = int(v.music * 100.0f + 0.5f);
		m_audioVals[3] = int(v.ambient * 100.0f + 0.5f);
	}

	return S_OK;
}

_int CSetting_UI::Update_GameObject(const _float& fTimeDelta)
{
	CUI::Update_GameObject(fTimeDelta);

	if (m_bActive && !m_prevActive)
	{
		if (auto sm = CSoundManager::GetInstance())
		{
			CSoundManager::AudioVolumes v = sm->GetVol();
			m_audioVals[0] = int(v.master * 100.0f + 0.5f);
			m_audioVals[1] = int(v.sfx * 100.0f + 0.5f);
			m_audioVals[2] = int(v.music * 100.0f + 0.5f);
			m_audioVals[3] = int(v.ambient * 100.0f + 0.5f);
		}
		m_dragging = false;
		m_dragIndex = -1;
	}

	m_prevActive = m_bActive;
	if (!m_bActive) return 0;
	Key_Input();

	return 0;
}

void CSetting_UI::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CUI::LateUpdate_GameObject(fTimeDelta);
	for (auto& pIcon : m_vecUIObjects)
		pIcon->LateUpdate_GameObject(fTimeDelta);
}

void CSetting_UI::Render_GameObject()
{
	if (!m_bActive)
		return;

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	const D3DXMATRIX parent = *m_pTransform->Get_World();

	D3DXMATRIX T, S, W;
	D3DXMatrixScaling(&S, 2.f, 2.f, 1.0f);
	D3DXMatrixTranslation(&T, 0.f, 0.f, 1.f);
	W = S * T;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &W);
	m_pBufferCom->Change_UV(1, 1, 1, 1);
	CResourceManager::GetInstance()->Get<CTexture>(L"Back")->Set_Texture();
	m_pBufferCom->Render_Buffer();

	Render_Text();
	Render_ChildTex();

	if (m_CategoryIndex == 2) {
		Render_AudioSlider();
	}

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CSetting_UI* CSetting_UI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSetting_UI* pSettingUI = new CSetting_UI(pGraphicDev);

	if (FAILED(pSettingUI->Ready_GameObject()))
	{
		// 만약에 제대로 생성이 되지 않았다면 바로 삭제.
		Safe_Release(pSettingUI);
		MSG_BOX("pSettingUI Create Failed");
		return nullptr;
	}

	return pSettingUI;
}

void CSetting_UI::Free()
{
	Safe_Release(m_pLine);
	for (auto& pIcon : m_vecUIObjects)
		Safe_Release(pIcon);
	m_vecUIObjects.clear();

	CUI::Free();
}

void CSetting_UI::Render_Text()
{
	const wchar_t* SettingCategories[4] = { L"Gameplay", L"Video", L"Audio", L"Input" };

	const wchar_t* Settingtexts[4][4] =
	{
		{ L"Autoswitch Weapons", L"Show Score", L"Show Gameplay Time" },
		{ L"Window Mode", L"Field of View", L"Lighting", L"Show Crosshair" },
		{ L"Master Volume", L"Sound Volume", L"Music Volume", L"Ambient Volume" },
		{ L"Gamepad Sensitivity", L"Autorun"}
	};
	const wchar_t* Exit[2] = { L"Apply", L"Back" };

	if (!m_small) return;

	D3DVIEWPORT9 vp;
	m_pGraphicDev->GetViewport(&vp);

	for (int i = 0; i < 4; i++)
	{
		LONG bannerWidth;

		LONG bannerHeight = 36;
		RECT rc;
		if (i == 0)
		{
			bannerWidth = 120;
			rc = {
				LONG(vp.Width * 0.37f + (bannerWidth)*i),
				LONG(vp.Height * 0.3f),
				LONG(vp.Width * 0.37f + (bannerWidth) * (i + 1)),
				LONG(vp.Height * 0.3f + bannerHeight)
			};
		}
		else
		{
			bannerWidth = 80;
			rc = {
				LONG(vp.Width * 0.37f + 120 + (bannerWidth) * (i - 1)),
				LONG(vp.Height * 0.3f),
				LONG(vp.Width * 0.37f + 120 + (bannerWidth)*i),
				LONG(vp.Height * 0.3f + bannerHeight)
			};
		}

		//선택된항목만색변경
		D3DCOLOR textCol = (i == m_hoverIndex || i == m_CategoryIndex)
			? D3DCOLOR_ARGB(255, 255, 255, 255)
			: D3DCOLOR_ARGB(255, 255, 96, 171);

		m_small->DrawTextW(nullptr, SettingCategories[i], -1, &rc,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE, textCol);
	}

	int count = kCategorySize[m_CategoryIndex];

	for (int i = 0; i < count; i++)
	{
		LONG bannerHeight = 36;
		int offset = 30;
		RECT rc = {
			LONG(vp.Width * 0.3f),
			LONG(vp.Height * 0.38f + offset * i),
			LONG(vp.Width * 0.7f),
			LONG(vp.Height * 0.38f + bannerHeight + offset * i)
		};

		//선택된항목만색변경
		D3DCOLOR textCol = (i == m_SelectedIndex)
			? D3DCOLOR_ARGB(255, 255, 255, 255)
			: D3DCOLOR_ARGB(255, 255, 198, 114);

		m_small->DrawTextW(nullptr, Settingtexts[m_CategoryIndex][i], -1, &rc,
			DT_LEFT | DT_VCENTER | DT_SINGLELINE, textCol);
	}

	for (int i = 0; i < 2; i++)
	{
		LONG bannerHeight = 36;
		int offset = 40;
		RECT rc = {
			LONG(vp.Width * 0.2f),
			LONG(vp.Height * 0.8f + offset * i),
			LONG(vp.Width * 0.8f),
			LONG(vp.Height * 0.8f + bannerHeight + offset * i)
		};

		//선택된항목만색변경
		D3DCOLOR textCol = (m_hoverIndex == 5 + i)
			? D3DCOLOR_ARGB(255, 255, 255, 255)
			: D3DCOLOR_ARGB(255, 255, 198, 114);

		m_small->DrawTextW(nullptr, Exit[i], -1, &rc,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE, textCol);
	}
}

void CSetting_UI::Render_ChildTex()
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
	{
		D3DXMATRIX T, S, W;
		D3DXMatrixScaling(&S, 1.5f, 0.03f, 1.0f);
		D3DXMatrixTranslation(&T, 0.f, 95.f, 1.f);
		W = S * T;
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &W);

		D3DCOLOR lineTint = D3DCOLOR_ARGB(220, 255, 168, 108);
		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, lineTint);
		m_pBufferCom->Change_UV(1, 1, 1, 1);
		CResourceManager::GetInstance()->Get<CTexture>(L"Line_UI")->Set_Texture();
		m_pBufferCom->Render_Buffer();
	}

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, oldTFactor);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, oldColorOp);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, oldColorArg1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, oldColorArg2);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, oldAlphaOp);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, oldAlphaArg1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, oldAlphaArg2);
}

void CSetting_UI::Key_Input()
{
	//MOUSEInput
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	m_hoverIndex = -1;

	D3DVIEWPORT9 vp;
	m_pGraphicDev->GetViewport(&vp);

	LONG bannerWidth;
	LONG bannerHeight;
	LONG gapX;
	RECT rc;

	for (int i = 0; i < 4; i++)
	{
		bannerHeight = 36;
		if (i == 0)
		{
			bannerWidth = 120;
			rc = {
				LONG(vp.Width * 0.37f + (bannerWidth)*i),
				LONG(vp.Height * 0.3f),
				LONG(vp.Width * 0.37f + (bannerWidth) * (i + 1)),
				LONG(vp.Height * 0.3f + bannerHeight)
			};
		}
		else
		{
			bannerWidth = 80;
			rc = {
				LONG(vp.Width * 0.37f + 120 + (bannerWidth) * (i - 1)),
				LONG(vp.Height * 0.3f),
				LONG(vp.Width * 0.37f + 120 + (bannerWidth)*i),
				LONG(vp.Height * 0.3f + bannerHeight)
			};
		}

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
					m_CategoryIndex = 0;
					break;
				case 1:
					m_CategoryIndex = 1;
					break;
				case 2:
					m_CategoryIndex = 2;
					break;
				case 3:
					m_CategoryIndex = 3;
					break;
				}
			}
			break;
		}
	}

	for (int i = 0; i < 2; i++)
	{
		bannerHeight = 36;
		int offset = 40;
		RECT rc = {
			LONG(vp.Width * 0.3f),
			LONG(vp.Height * 0.8f + offset * i),
			LONG(vp.Width * 0.7f),
			LONG(vp.Height * 0.8f + bannerHeight + offset * i)
		};

		if (PtInRect(&rc, ptMouse))
		{
			m_hoverIndex = 5 + i;

			// 클릭 처리
			if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::LBUTTON))
			{
				switch (i)
				{
				case 0:
				{
					if (auto sm = CSoundManager::GetInstance())
					{
						float m = m_audioVals[0] / 100.0f; if (m < 0.f) m = 0.f; else if (m > 1.f) m = 1.f;
						float s = m_audioVals[1] / 100.0f; if (s < 0.f) s = 0.f; else if (s > 1.f) s = 1.f;
						float u = m_audioVals[2] / 100.0f; if (u < 0.f) u = 0.f; else if (u > 1.f) u = 1.f;
						float a = m_audioVals[3] / 100.0f; if (a < 0.f) a = 0.f; else if (a > 1.f) a = 1.f;
						CSoundManager::AudioVolumes av; av.master = m; av.sfx = s; av.music = u; av.ambient = a;
						sm->SetVol(av);

						sm->SetChannelVolume(SOUND_BGM, u);
					}
					CUIManager::GetInstance()->DeactivateUI(L"Setting_UI");
					CUIManager::GetInstance()->ActivateUI(L"Main_UI");
					CUIManager::GetInstance()->ActivateUI(L"PauseUI");
				}
				break;
				case 1:
					CUIManager::GetInstance()->DeactivateUI(L"Setting_UI");
					CUIManager::GetInstance()->ActivateUI(L"Main_UI");
					CUIManager::GetInstance()->ActivateUI(L"PauseUI");
					break;
				}
			}
			break;
		}
	}
	
	if (m_CategoryIndex == 2) {
		Update_AudioSlider_Input();
	}

	//KEYinput
	{
		if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::E))
		{
			m_CategoryIndex = (m_CategoryIndex + 1) % 4;
		}

		if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::Q))
		{
			m_CategoryIndex = (m_CategoryIndex + 3) % 4;
		}

		int count = kCategorySize[m_CategoryIndex];
		if (m_SelectedIndex >= count) m_SelectedIndex = 0;
		if (m_SelectedIndex < 0)      m_SelectedIndex = 0;

		if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::W))
		{
			m_SelectedIndex = (m_SelectedIndex - 1 + count) % count;
		}

		if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::S))
		{
			m_SelectedIndex = (m_SelectedIndex + 1) % count;
		}
	}
}

void CSetting_UI::Render_AudioSlider()
{

	D3DVIEWPORT9 vp; m_pGraphicDev->GetViewport(&vp);
	const int bannerHeight = 36;
	const int offsetY = 30;
	const int itemCount = 4;

	for (int i = 0; i < itemCount; ++i)
	{
		RECT rcText = {
			LONG(vp.Width * 0.30f),
			LONG(vp.Height * 0.38f + offsetY * i),
			LONG(vp.Width * 0.70f),
			LONG(vp.Height * 0.38f + bannerHeight + offsetY * i)
		};

		const float left = vp.Width * 0.55f;
		const float right = vp.Width * 0.78f;
		const float cy = (rcText.top + rcText.bottom) * 0.5f + 2.0f;
		const float heightPx = 10.0f;

		m_sliderRects[i].left = LONG(left);
		m_sliderRects[i].right = LONG(right);
		m_sliderRects[i].top = LONG(cy - heightPx * 0.7f);
		m_sliderRects[i].bottom = LONG(cy + heightPx * 0.7f);

		//bg : 회색부분(풀 바) fill : 주황부분(채워진 바)
		D3DXVECTOR2 bg[2] = { {left, cy}, {right, cy} };
		m_pLine->SetWidth(heightPx);
		m_pLine->Draw(bg, 2, D3DCOLOR_ARGB(180, 70, 70, 70));

		int v = m_audioVals[i];
		if (v < 0) v = 0; 
		else if (v > 100) v = 100;
		float t = v / 100.0f;
		float fillX = left + (right - left) * t;

		D3DXVECTOR2 fill[2] = { {left, cy}, {fillX, cy} };
		m_pLine->SetWidth(heightPx);

		bool isSelected = (i == m_SelectedIndex);
		D3DCOLOR fillCol = isSelected
			? D3DCOLOR_ARGB(255, 255, 220, 150)
			: D3DCOLOR_ARGB(255, 255, 198, 114);
		m_pLine->Draw(fill, 2, fillCol);
		//수치
		wchar_t buf[16];
		swprintf(buf, 16, L"%d", m_audioVals[i]);
		RECT rcVal = { LONG(right + 8), LONG(cy - bannerHeight * 0.5f),
					   LONG(right + 8 + 60), LONG(cy + bannerHeight * 0.5f) };
		m_small->DrawTextW(nullptr, buf, -1, &rcVal,
			DT_LEFT | DT_VCENTER | DT_SINGLELINE,
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}
void CSetting_UI::Update_AudioSlider_Input()
{
	if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::A)) {
		m_audioVals[m_SelectedIndex] -= 1;
		if (m_audioVals[m_SelectedIndex] < 0) m_audioVals[m_SelectedIndex] = 0;
	}
	if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::D)) {
		m_audioVals[m_SelectedIndex] += 1;
		if (m_audioVals[m_SelectedIndex] > 100) m_audioVals[m_SelectedIndex] = 100;
	}

	POINT ptMouse; 
	GetCursorPos(&ptMouse); 
	ScreenToClient(g_hWnd, &ptMouse);

	if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::LBUTTON)) {
		m_dragging = false; m_dragIndex = -1;
		for (int i = 0; i < 4; ++i) {
			if (PtInRect(&m_sliderRects[i], ptMouse)) {
				m_dragging = true; m_dragIndex = i; break;
			}
		}
	}
	if (CInputManager::GetInstance()->GetButtonUp(KEY_TYPE::LBUTTON)) {
		m_dragging = false; 
		m_dragIndex = -1;
	}

	if (m_dragging && m_dragIndex >= 0 && m_dragIndex < 4) {
		float left = (float)m_sliderRects[m_dragIndex].left;
		float right = (float)m_sliderRects[m_dragIndex].right;
		float x = (float)ptMouse.x;
		float t = (x - left) / (right - left);
		if (t < 0.f) t = 0.f; else if (t > 1.f) t = 1.f;
		m_audioVals[m_dragIndex] = (int)floorf(t * 100.0f + 0.5f);
	}
	//바뀐값바로반영시킴
	static int prevVals[4] = { -1, -1, -1, -1 };
	bool changed = false;
	for (int i = 0; i < 4; ++i) {
		if (m_audioVals[i] != prevVals[i]) { changed = true; break; }
	}

	if (changed) {
		if (auto sm = CSoundManager::GetInstance())
		{
			float m = m_audioVals[0] / 100.0f; if (m < 0.f) m = 0.f; else if (m > 1.f) m = 1.f;
			float s = m_audioVals[1] / 100.0f; if (s < 0.f) s = 0.f; else if (s > 1.f) s = 1.f;
			float u = m_audioVals[2] / 100.0f; if (u < 0.f) u = 0.f; else if (u > 1.f) u = 1.f;
			float a = m_audioVals[3] / 100.0f; if (a < 0.f) a = 0.f; else if (a > 1.f) a = 1.f;

			CSoundManager::AudioVolumes av; av.master = m; av.sfx = s; av.music = u; av.ambient = a;
			sm->SetVol(av);

			sm->SetChannelVolume(SOUND_BGM, u);
		}
		// prev 갱신
		for (int i = 0; i < 4; ++i) prevVals[i] = m_audioVals[i];
	}

}