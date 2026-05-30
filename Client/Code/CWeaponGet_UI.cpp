#include "pch.h"
#include "CWeaponGet_UI.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CInputManager.h"
#include "CUI_Number.h"
#include "CUI_Image.h"
#include "CPlayer.h"
#include "CObjectManager.h"
#include "CEventbus.h"
#include "CSceneManager.h"
#include "CUIManager.h"

CWeaponGet_UI::CWeaponGet_UI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUI(pGraphicDev)
{

}

CWeaponGet_UI::CWeaponGet_UI(const CWeaponGet_UI& rhs)
	:CUI(rhs)
{

}

CWeaponGet_UI::~CWeaponGet_UI()
{

}

HRESULT	CWeaponGet_UI::Ready_GameObject()
{
	CUI::Ready_GameObject();

	m_bActive = false;
	bActive = true;
	m_tHandle.m_eType = OBJ_UI;
	m_pTransform->Set_Scale({ 2.f, 2.f, 1.f });
	m_pTransform->Set_Pos(0.f, 0.f, 0.f);

	m_pIcon = CUI_Image::Create(m_pGraphicDev, L"Shop_Weapon", 8);
	if (m_pIcon)
	{
		m_pIcon->SetSpriteIndex(2);
		m_pIcon->Set_Position(-150.f, 170.f, 0.f);
		m_pIcon->Set_Scale({ 0.25f, 0.32f, 1.f });
	}

	return S_OK;
}

_int CWeaponGet_UI::Update_GameObject(const _float& fTimeDelta)
{
	const bool prevActive = m_prevActive;

	if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::LBUTTON))
		m_bActive = false;

	if (m_bActive != prevActive) {
		bActive = m_bActive;
		Set_Toggle();
	}

	if (!m_bActive) {
		m_prevActive = false;
		return 0;
	}

	if (m_bActive && !prevActive) {
		const wstring& tag = CUIManager::GetInstance()->GetCategoryTag(L"WeaponGet_UI");
		m_cachedTag = tag;
		UpdateText(tag);
	}
	CUI::Update_GameObject(fTimeDelta);
	m_prevActive = m_bActive;
	if (m_pIcon) m_pIcon->Update_GameObject(fTimeDelta);
	return 0;
}

void CWeaponGet_UI::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CUI::LateUpdate_GameObject(fTimeDelta);
	if (m_pIcon) m_pIcon->LateUpdate_GameObject(fTimeDelta);
}

void CWeaponGet_UI::Render_GameObject()
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
	Render_ChildTex();
	Render_Text();
	if (m_pIcon) m_pIcon->Render_GameObject();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CWeaponGet_UI::Render_Text()
{
	D3DVIEWPORT9 vp;
	m_pGraphicDev->GetViewport(&vp);

	//이름
	RECT rcName = {
		LONG(vp.Width * 0.5f - 600 / 2),
		LONG(vp.Height * 0.22f),
		LONG(vp.Width * 0.5f + 600 / 2),
		LONG(vp.Height * 0.22f + 60)
	};
	const wchar_t* nameText = m_weaponName.empty() ? L"Unknown Weapon" : m_weaponName.c_str();
	DrawShadowedText(
		m_mid, nameText, rcName,
		D3DCOLOR_ARGB(255, 247, 180, 82),
		2, D3DCOLOR_ARGB(140, 0, 0, 0),
		DT_CENTER | DT_VCENTER | DT_SINGLELINE
	);

	//본문 범위계산학
	const int boxW = 900;
	const int cx = vp.Width / 2;
	int topY = int(vp.Height * 0.35f);

	const wchar_t* loreText = m_weaponLore.empty()
		? L" - "
		: m_weaponLore.c_str();

	RECT rcLore = { cx - boxW / 2, topY, cx + boxW / 2, topY + 1 };
	RECT rcCalc = rcLore;
	m_small->DrawTextW(nullptr, loreText, -1, &rcCalc,
		DT_CENTER | DT_WORDBREAK | DT_CALCRECT, 0);

	//본문 출력
	DrawShadowedText(
		m_small, loreText, rcCalc,
		D3DCOLOR_ARGB(255, 217, 165, 117),
		2, D3DCOLOR_ARGB(140, 0, 0, 0),
		DT_CENTER | DT_WORDBREAK
	);

	//요약
	const wchar_t* tagText = m_weaponSummary.empty() ? L" - " : m_weaponSummary.c_str();
	RECT rcTag = { rcLore.left, rcCalc.bottom + 16, rcLore.right, rcCalc.bottom + 16 + 80 };
	DrawShadowedText(
		m_small, tagText, rcTag,
		D3DCOLOR_ARGB(255, 255, 255, 255),
		2, D3DCOLOR_ARGB(140, 0, 0, 0),
		DT_CENTER | DT_WORDBREAK
	);
	//힌트
	RECT rcHint = {
			LONG(vp.Width * 0.5f - 600 / 2),
			LONG(vp.Height * 0.66f),
			LONG(vp.Width * 0.5f + 600 / 2),
			LONG(vp.Height * 0.66f + 40)
	};

	DrawShadowedText(
		m_mid,
		L"<Press \"Fire\" to Continue>",
		rcHint,
		D3DCOLOR_ARGB(255, 232, 205, 152),
		2,
		D3DCOLOR_ARGB(120, 0, 0, 0),
		DT_CENTER | DT_VCENTER | DT_SINGLELINE
	);
}



CWeaponGet_UI* CWeaponGet_UI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWeaponGet_UI* pStageClear = new CWeaponGet_UI(pGraphicDev);

	if (FAILED(pStageClear->Ready_GameObject()))
	{
		Safe_Release(pStageClear);
		MSG_BOX("pStageClear Create Failed");
		return nullptr;
	}

	return pStageClear;
}

void CWeaponGet_UI::Free()
{
	Safe_Release(m_pIcon);
	CUI::Free();
}

void CWeaponGet_UI::Set_Toggle()
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

void CWeaponGet_UI::UpdateText(const wstring& tag)
{
	if (tag == L"Spellstriker") {
		m_weaponName = L"Spellstriker";
		m_weaponLore = L"A nimble arcane blade that channels swift spell slashes.\n"
			L"Favored by battlemages who weave steel with sorcery.";
		m_weaponSummary = L"Fast hybrid casting, close-range burst.";
		if (m_pIcon) m_pIcon->SetSpriteIndex(2);
	}
	else if (tag == L"FrostWeaver") {
		m_weaponName = L"FrostWeaver";
		m_weaponLore = L"A staff that braids chilling winds into woven ice.\n"
			L"Slows foes and sets up precise strikes.";
		m_weaponSummary = L"Control-oriented, applies chill/slow.";
		if (m_pIcon) m_pIcon->SetSpriteIndex(3);
	}
	else if (tag == L"Fire Rings") {
		m_weaponName = L"Fire Rings";
		m_weaponLore = L"Magical Artifact used by novice mages to help them conjure fire spells.\n"
			L"Conjured spells might not be the strongest in the world of magic, "
			L"but the rings have proven a great ally to every mage again and again.";
		m_weaponSummary = L"All around good magical weapon for casting fireballs.";
		if (m_pIcon) m_pIcon->SetSpriteIndex(4);
	}
	else {
		m_weaponName.clear();
		m_weaponLore.clear();
		m_weaponSummary.clear();
		if (m_pIcon) m_pIcon->SetSpriteIndex(2);
	}
}

void CWeaponGet_UI::Render_ChildTex()
{
	//복원을위해서 미리 저장함
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
		D3DXMatrixScaling(&S, 2.5f, 0.3f, 1.0f);
		D3DXMatrixTranslation(&T, 0.f, 173.f, 1.f);
		W = S * T;
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &W);

		D3DCOLOR lineTint = D3DCOLOR_ARGB(220, 180, 85, 162);
		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, lineTint);

		CResourceManager::GetInstance()->Get<CTexture>(L"Line_UI")->Set_Texture();
		m_pBufferCom->Render_Buffer();
	}
	{
		D3DXMATRIX T, S, W;
		D3DXMatrixScaling(&S, 2.5f, 0.03f, 1.0f);
		D3DXMatrixTranslation(&T, 0.f, -80.f, 1.f);
		W = S * T;
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &W);

		D3DCOLOR lineTint = D3DCOLOR_ARGB(225, 232, 205, 152);
		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, lineTint);

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
