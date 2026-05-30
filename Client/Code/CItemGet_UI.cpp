#include "pch.h"
#include "CItemGet_UI.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CInputManager.h"
#include "CUIManager.h"

CItemGet_UI::CItemGet_UI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUI(pGraphicDev)
{

}

CItemGet_UI::CItemGet_UI(const CItemGet_UI& rhs)
	:CUI(rhs)
{

}

CItemGet_UI::~CItemGet_UI()
{

}

HRESULT	CItemGet_UI::Ready_GameObject()
{
	CUI::Ready_GameObject();

	m_bActive = false;
	m_tHandle.m_eType = OBJ_UI;

	return S_OK;
}

_int CItemGet_UI::Update_GameObject(const _float& fTimeDelta)
{
	wstring tag;
	while (CUIManager::GetInstance()->PopTag(tag)) {
		BannerSlot slot;
		BuildBannerFromTag(tag, slot);
		slot.t = (m_flashDuration > 0.f ? m_flashDuration : 5.f);

		m_slots.emplace_front(move(slot));
		if ((int)m_slots.size() > 5)
			m_slots.pop_back(); //맨위에 젤최근먹은거 올리고, 5개 넘어가면 하나 없앰
	}

	for (auto& s : m_slots) s.t -= fTimeDelta;
	while (!m_slots.empty() && m_slots.back().t <= 0.f) m_slots.pop_back();

	m_bActive = !m_slots.empty();
	m_prevActive = m_bActive;

	if (!m_bActive) return 0;

	CUI::Update_GameObject(fTimeDelta);
	return 0;
}

void CItemGet_UI::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (!m_bActive) return;
	CUI::LateUpdate_GameObject(fTimeDelta);
}

void CItemGet_UI::Render_GameObject()
{
	if (!m_bActive) return;
	CUI::Render_GameObject();

	D3DVIEWPORT9 vp; m_pGraphicDev->GetViewport(&vp);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	DWORD prevZ; m_pGraphicDev->GetRenderState(D3DRS_ZENABLE, &prevZ);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);

	const float lineH = 20.f; //한줄당간경ㄱ
	const float iconPadPx = 18.f; //아이콘가로
	const float textWidth = vp.Width * 0.20f;//텍스트 가로

	for (size_t i = 0; i < m_slots.size(); ++i)
	{
		const BannerSlot& s = m_slots[i];
		//알파값조절
		const float dur = (m_flashDuration > 0.f ? m_flashDuration : 5.f);
		float t = dur > 0.f ? (s.t / dur) : 0.f;
		t = t < 0.f ? 0.f : (t > 1.f ? 1.f : t);
		const float a = powf(t, 1.4f);
		const DWORD tfactor = D3DCOLOR_COLORVALUE(1.f, 1.f, 1.f, a);
		const BYTE  Textalpha = (BYTE)(a * 255.0f);

		const float lineTopPx = vp.Height * 0.23f + i * lineH;
		const float lineCenterPx = lineTopPx + lineH * 0.5f;

		const float worldX = vp.Width * 0.03f - (float)vp.Width * 0.5f;
		const float worldY = (float)vp.Height * 0.5f - lineCenterPx;

		D3DXMATRIX S, T, W;
		D3DXMatrixScaling(&S, 0.04f, 0.06f, 1.f);
		D3DXMatrixTranslation(&T, worldX, worldY, 0.f);
		W = S * T;

		CResourceManager::GetInstance()->Get<CTexture>(L"Item_Icon")->Set_Texture();

		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, tfactor);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &W);
		m_pBufferCom->Change_UV(s.uvx, 8, s.uvy, 16);
		m_pBufferCom->Render_Buffer();

		RECT rc = {
			LONG(vp.Width * 0.03f + iconPadPx),
			LONG(vp.Height * 0.23f + i * lineH),
			LONG(vp.Width * 0.03f + iconPadPx + textWidth),
			LONG(vp.Height * 0.23f + (i + 1) * lineH)
		};

		DrawShadowedText( m_mini, s.text.c_str(), rc, D3DCOLOR_ARGB(Textalpha, 255, 244, 218), 1,
			D3DCOLOR_ARGB((BYTE)(Textalpha * 0.85f), 0, 0, 0), DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, prevZ);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CItemGet_UI* CItemGet_UI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CItemGet_UI* pItemGet_UI = new CItemGet_UI(pGraphicDev);

	if (FAILED(pItemGet_UI->Ready_GameObject()))
	{
		Safe_Release(pItemGet_UI);
		MSG_BOX("pItemGet_UI Create Failed");
		return nullptr;
	}

	return pItemGet_UI;
}

void CItemGet_UI::BuildBannerFromTag(const wstring& tag, BannerSlot& out)
{
	out.text = tag;

	auto it = kIconMap.find(tag);
	if (it != kIconMap.end()) {
		out.uvx = it->second.first;
		out.uvy = it->second.second;
	}
	else {
		out.uvx = 7;
		out.uvy = 15;
	}
}

void CItemGet_UI::Free()
{
	CUI::Free();
}
