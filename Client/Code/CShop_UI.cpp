#include "pch.h"
#include "CShop_UI.h"
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
namespace {
	const wchar_t* WeaponItems[8] = { L"Critical Chance", L"Breath of Fire", L"???", L"Ice Shatter", L"???", L"???", L"???", L"???" };
	const int WeaponPrice[8] = { 8000, 10000, 12000, 12000, 15000, 15000, 15000, 15000 };

	const wchar_t* SpellItems[4] = { L"Blink", L"Frost Breath", L"Arcane Shield", L"Spellhaste" };
	const int SpellPrice[4] = { 10000, 12000, 15000, 20000 };

	const wchar_t* CharItems[5] = { L"Bag of Holding", L"Runic Rosary", L"Arcane Arsenal", L"???", L"Moonlight Chart" };
	const int CharPrice[5] = { 15000, 10000, 15000, 20000, 30000 };
}


CShop_UI::CShop_UI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUI(pGraphicDev)
{

}

CShop_UI::CShop_UI(const CShop_UI& rhs)
	:CUI(rhs)
{

}

CShop_UI::~CShop_UI()
{

}

HRESULT	CShop_UI::Ready_GameObject()
{
	CUI::Ready_GameObject();

	m_bActive = true;

	m_tHandle.m_eType = OBJ_UI;
	m_pTransform->Set_Scale({ 2.f, 2.f, 1.f });

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObjectManager::GetInstance()->Get(OBJTYPE::OBJ_PLAYER));
	m_Money = pPlayer->Get_MoneyCnt();

	for (int i = 0; i < 3; i++)
	{
		auto pIcon = CUI_Image::Create(m_pGraphicDev, L"Shop_Hud", 8);
		if (i != m_CategoryIndex)
			pIcon->SetSpriteIndex(i + 3);
		else
			pIcon->SetSpriteIndex(i);
		pIcon->Set_Position(-105 + 102 * i, 265, 0.f);
		pIcon->Set_Scale({ 0.22f, 0.32f, 1.f });
		m_vecUIObjects.push_back(pIcon);
	}

	auto pMoneyIcon = CUI_Image::Create(m_pGraphicDev, L"Money", 1);
	pMoneyIcon->SetSpriteIndex(0);
	pMoneyIcon->Set_Position(-500.f, 270.f, 0.f);
	pMoneyIcon->Set_Scale({ 0.15f, 0.22f, 1.f });
	m_vecUIObjects.push_back(pMoneyIcon);


	//카테고리별 아이콘
	for (int i = 0; i < 8; i++)
	{
		auto pWeapon = CUI_Image::Create(m_pGraphicDev, L"Shop_Weapon", 8);
		float w = i; if (i >= 5) w = i - 5;
		float h = 1.f; if (i >= 5) h = - 1.7f;

		pWeapon->SetSpriteIndex(i);
		pWeapon->Set_Position(-490.f + 245 * w, 60.f * h, 0.f);
		pWeapon->Set_Scale({ 0.19f, 0.25f, 1.f });
		m_WeaponIcon.push_back(pWeapon);
	}

	for (int i = 0; i < 4; i++)
	{
		auto pSpell = CUI_Image::Create(m_pGraphicDev, L"Shop_Spell", 8);
		pSpell->SetSpriteIndex(i);
		pSpell->Set_Position(-370.f + 240 * i, 0.f, 0.f);
		pSpell->Set_Scale({ 0.19f, 0.25f, 1.f });
		m_SpellIcon.push_back(pSpell);
	}

	for (int i = 0; i < 5; i++)
	{
		auto pChar = CUI_Image::Create(m_pGraphicDev, L"Shop_Char", 8);
		pChar->SetSpriteIndex(i);
		pChar->Set_Position(-490.f + 243 * i, -10, 0.f);
		pChar->Set_Scale({ 0.19f, 0.25f, 1.f });
		m_CharIcon.push_back(pChar);
	}

	return S_OK;
}

_int CShop_UI::Update_GameObject(const _float& fTimeDelta)
{
	Set_Toggle();
	if (!m_bActive) return 0;

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObjectManager::GetInstance()->Get(OBJTYPE::OBJ_PLAYER));
	CUI::Update_GameObject(fTimeDelta);

	for (int k = 0; k < 3; ++k) {
		if (auto img = dynamic_cast<CUI_Image*>(m_vecUIObjects[k]))
		{
			if (k != m_CategoryIndex)
			{
				img->SetSpriteIndex(k + 3);
				img->Set_Scale({ 0.22f, 0.32f, 1.f });
			}

			else
			{
				img->SetSpriteIndex(k);
				img->Set_Scale({ 0.25f, 0.35f, 1.f });
			}
		}
	}

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	D3DVIEWPORT9 vp;
	m_pGraphicDev->GetViewport(&vp);

	LONG bannerWidth;
	LONG bannerHeight;
	LONG gapX;
	RECT rc;

	//카테고리변경
	for (int i = 0; i < 3; ++i)
	{
		bannerWidth = 70;
		bannerHeight = 70;
		gapX = 32;
		LONG left = LONG(vp.Width * 0.39f) + (bannerWidth + gapX) * i;
		LONG top = LONG(vp.Height * 0.08f);

		rc = { left, top, left + bannerWidth, top + bannerHeight };

		if (PtInRect(&rc, ptMouse))
		{
			m_hoverIndex = i;

			auto pIcon = dynamic_cast<CUI_Image*>(m_vecUIObjects[i]);
			pIcon->Set_Scale({ 0.25f, 0.35f, 1.f });

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
				}
			}
			break;
		}
	}
	switch (m_CategoryIndex)
	{
	case 0:
		Set_WeaponInput();
		for (auto& pIcon : m_WeaponIcon)
			pIcon->Update_GameObject(fTimeDelta);
		break;
	case 1:
		Set_SpellInput();
		for (auto& pIcon : m_SpellIcon)
			pIcon->Update_GameObject(fTimeDelta);
		break;
	case 2:
		Set_CharacterInput();
		for (auto& pIcon : m_CharIcon)
			pIcon->Update_GameObject(fTimeDelta);
		break;
	}

	
	for (auto& pIcon : m_vecUIObjects)
		pIcon->Update_GameObject(fTimeDelta);

	pPlayer->Set_MoneyCnt(m_Money);
	return 0;
}

void CShop_UI::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CUI::LateUpdate_GameObject(fTimeDelta);
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	D3DVIEWPORT9 vp;
	m_pGraphicDev->GetViewport(&vp);
	//exit버튼
	{
		RECT rc = {
		LONG(vp.Width * 0.9f - (100 / 2)),
		LONG(vp.Height * 0.1f),
		LONG(vp.Width * 0.9f + (100 / 2)),
		LONG(vp.Height * 0.1f + 36)
		};
		if (PtInRect(&rc, ptMouse))
		{
			m_hoverIndex = 100;
			EVENT event;
			if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::LBUTTON))
			{
				CUIManager::GetInstance()->SetCleared(false);
				m_bActive = false;
				event = { PHASE::PHASE_BEGIN, TOPIC::Go_Next, CHANNEL::UI,
					nullptr, nullptr, {}, {} };

				CEventBus::GetInstance()->publish(event);
				CUIManager::GetInstance()->DeactivateUI(L"StageClearUI");
				CInputManager::GetInstance()->Set_KeyLock(false);
				while (ShowCursor(FALSE) >= 0);
			}
			
		}
	}
	for (auto& pIcon : m_vecUIObjects)
		pIcon->LateUpdate_GameObject(fTimeDelta);
}

void CShop_UI::Render_GameObject()
{
	if (!m_bActive)
		return;

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	const D3DXMATRIX parent = *m_pTransform->Get_World();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &parent);

	CResourceManager::GetInstance()->Get<CTexture>(L"MainBackGround")->Set_Texture();
	m_pBufferCom->Change_UV(1, 1, 1, 1);
	m_pBufferCom->Render_Buffer();
	CResourceManager::GetInstance()->Get<CTexture>(L"Back")->Set_Texture();
	m_pBufferCom->Change_UV(1, 1, 1, 1);
	m_pBufferCom->Render_Buffer();

	Render_ChildTex();
	D3DVIEWPORT9 vp;

	m_pGraphicDev->GetViewport(&vp);

	RECT rc;
	LONG  bannerWidth;
	LONG  bannerHeight;
	LONG  gapX;
	D3DCOLOR textCol;

	for (int i = 0; i < 3; ++i)
	{
		bannerWidth = 70;
		bannerHeight = 70;
		gapX = 32;

		LONG left = LONG(vp.Width * 0.39f) + (bannerWidth + gapX) * i;
		LONG top = LONG(vp.Height * 0.08f);

		rc = { left, top, left + bannerWidth, top + bannerHeight };
	}

	rc = {
			LONG(vp.Width * 0.9f - (100 / 2)),
			LONG(vp.Height * 0.1f),
			LONG(vp.Width * 0.9f + (100 / 2)),
			LONG(vp.Height * 0.1f + 36)
	};

	textCol = (100 == m_hoverIndex)
		? D3DCOLOR_ARGB(255, 255, 255, 255)
		: D3DCOLOR_ARGB(255, 255, 198, 114);

	m_small->DrawTextW(nullptr, L"Exit Shop", -1, &rc,
		DT_CENTER | DT_VCENTER | DT_SINGLELINE, textCol);

	const wchar_t* Categories[3] = { L"Weapon Upgrades", L"Spell Upgrades", L"Character Upgrades" };

	bannerWidth = 300;
	bannerHeight = 36;

	rc = {
			LONG(vp.Width * 0.5f - (bannerWidth/2)),
			LONG(vp.Height * 0.2f),
			LONG(vp.Width * 0.5f + (bannerWidth/2)),
			LONG(vp.Height * 0.2f + bannerHeight)
	};

	switch (m_CategoryIndex)
	{
	case 0:
		textCol = D3DCOLOR_ARGB(255, 235, 131, 132);
		break;
	case 1:
		textCol = D3DCOLOR_ARGB(255, 145, 239, 247);
		break;
	case 2:
		textCol = D3DCOLOR_ARGB(255, 186, 255, 134);
		break;
	}

	m_mid->DrawTextW(nullptr, Categories[m_CategoryIndex], -1, &rc,
		DT_CENTER | DT_VCENTER | DT_SINGLELINE, textCol);

	switch (m_CategoryIndex)
	{
	case 0:
		Set_WeaponUI();
		break;
	case 1:
		Set_SpellUI();
		break;
	case 2:
		Set_CharacterUI();
		break;
	}

	if (m_mid)
	{
		D3DVIEWPORT9 vp;
		m_pGraphicDev->GetViewport(&vp);

		LONG bannerHeight = 36;
		RECT rc = {
			LONG(vp.Width * 0.14f),
			LONG(vp.Height * 0.1f),
			LONG(vp.Width * 0.24f),
			LONG(vp.Height * 0.1f + bannerHeight)
		};

		wchar_t buffer[32];
		swprintf(buffer, 32, L"%d", m_Money);

		m_mid->DrawTextW(nullptr, buffer, -1, &rc,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE,
			D3DCOLOR_ARGB(255, 255, 198, 114));
	}

	for (auto& pIcon : m_vecUIObjects)
		pIcon->Render_GameObject();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CShop_UI::Render_ChildTex()
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
		D3DXMatrixScaling(&S, 1.8f, 0.15f, 1.0f);
		D3DXMatrixTranslation(&T, 0.f, 200.f, 1.f);
		W = S * T;
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &W);

		D3DCOLOR bannerTint;
		switch (m_CategoryIndex)
		{
		case 0:
			bannerTint = D3DCOLOR_ARGB(255, 235, 131, 132);
			break;
		case 1:
			bannerTint = D3DCOLOR_ARGB(255, 145, 239, 247);
			break;
		case 2:
			bannerTint = D3DCOLOR_ARGB(255, 186, 255, 134);
			break;
		}

		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, bannerTint);

		CResourceManager::GetInstance()->Get<CTexture>(L"Banner_UI")->Set_Texture();
		m_pBufferCom->Render_Buffer();
	}

	{
		D3DXMATRIX T, S, W;
		D3DXMatrixScaling(&S, 2.f, 0.05f, 1.0f);
		D3DXMatrixTranslation(&T, 0.f, -220.f, 1.f);
		W = S * T;
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &W);

		D3DCOLOR lineTint = D3DCOLOR_ARGB(220, 255, 168, 108);
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

CShop_UI* CShop_UI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShop_UI* pShop_UI = new CShop_UI(pGraphicDev);

	if (FAILED(pShop_UI->Ready_GameObject()))
	{
		Safe_Release(pShop_UI);
		MSG_BOX("pShop_UI Create Failed");
		return nullptr;
	}

	return pShop_UI;
}

void CShop_UI::Free()
{
	for (auto& pIcon : m_vecUIObjects)
		Safe_Release(pIcon);
	m_vecUIObjects.clear();

	for (auto& pIcon : m_WeaponIcon)
		Safe_Release(pIcon);
	m_WeaponIcon.clear();

	for (auto& pIcon : m_SpellIcon)
		Safe_Release(pIcon);
	m_SpellIcon.clear();

	for (auto& pIcon : m_CharIcon)
		Safe_Release(pIcon);
	m_CharIcon.clear();

	CUI::Free();
}

// 카테고리별 ui
void CShop_UI::Set_WeaponInput()
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObjectManager::GetInstance()->Get(OBJTYPE::OBJ_PLAYER));
	m_pWeaponCon = pPlayer->Get_WeaponController();

	for (size_t k = 0; k < m_WeaponIcon.size(); ++k) {
		if (auto img = dynamic_cast<CUI_Image*>(m_WeaponIcon[k]))
			img->Set_Scale({ 0.19f, 0.25f, 1.f });
	}

	m_hoverIndex = -1;

	D3DVIEWPORT9 vp;
	m_pGraphicDev->GetViewport(&vp);

	for (int i = 0; i < 8; ++i)
	{
		const LONG  bannerWidth = 230;
		const LONG  bannerHeight = 150;
		const LONG  gapX = 12;
		const LONG  gapY = 20;

		int row = (i >= 5) ? 1 : 0;
		int col = (row == 0) ? i : (i - 5);

		LONG left = LONG(vp.Width * 0.03f) + col * (bannerWidth + gapX);
		LONG top = LONG(vp.Height * 0.3f) + row * (bannerHeight + gapY);

		RECT rc = { left, top, left + bannerWidth, top + bannerHeight };

		EVENT event;

		if (PtInRect(&rc, ptMouse)) {

			bool owned = (find(m_BuyWeaponlist.begin(), m_BuyWeaponlist.end(), i)
				!= m_BuyWeaponlist.end());
			if (owned) { m_hoverIndex = -1; break; }

			m_hoverIndex = i;

			auto pIcon = dynamic_cast<CUI_Image*>(m_WeaponIcon[i]);
			pIcon->Set_Scale({ 0.23f, 0.3f, 1.f });

			if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::LBUTTON)) {
				const int price = WeaponPrice[i];

				if (m_Money >= price) 
				{
					m_Money -= price;
					m_BuyWeaponlist.push_back(i);
					switch (i)
					{
					case 0:
						m_pWeaponCon->LevelUp(WEAPON_FIREWAND);
						break;
					case 1:
						m_pWeaponCon->LevelUp(WEAPON_MACE);
						break;
					case 2:
						m_pWeaponCon->LevelUp(WEAPON_GUN);
						break;
					case 3:
						m_pWeaponCon->LevelUp(WEAPON_ICEWAND);
						break;
					case 4:
						m_pWeaponCon->LevelUp(WEAPON_FIRERING);
						break;
					case 5:
						break;
					case 6:
						break;
					case 7:
						break;
					}
				}
			}
			break;
		}
	}

}
void CShop_UI::Set_WeaponUI()
{
	if (m_small)
	{
		D3DVIEWPORT9 vp;
		m_pGraphicDev->GetViewport(&vp);

		const LONG  bannerWidth = 230;
		const LONG  bannerHeight = 150;
		const LONG  gapX = 12;
		const LONG  gapY = 20;

		for (int i = 0; i < 8; ++i)
		{
			int row = (i >= 5) ? 1 : 0;
			int col = (row == 0) ? i : (i - 5);

			LONG left = LONG(vp.Width * 0.03f) + col * (bannerWidth + gapX);
			LONG top = LONG(vp.Height * 0.3f) + row * (bannerHeight + gapY);

			RECT rc = { left, top, left + bannerWidth, top + bannerHeight };

			bool owned = (find(m_BuyWeaponlist.begin(), m_BuyWeaponlist.end(), i)
				!= m_BuyWeaponlist.end());

			const int price = WeaponPrice[i];
			D3DCOLOR textCol = D3DCOLOR_ARGB(255, 244, 175, 97);
			if (i == m_hoverIndex)	textCol = D3DCOLOR_ARGB(255, 255, 255, 255);
			if (m_Money < price) textCol = D3DCOLOR_ARGB(255, 232, 0, 0);
			if (owned) textCol = D3DCOLOR_ARGB(255, 191, 255, 36);

			m_mid->DrawTextW(nullptr, WeaponItems[i], -1, &rc,
				DT_CENTER | DT_TOP | DT_SINGLELINE, textCol);

			if (owned)
			{
				m_small->DrawTextW(nullptr, L"Purchased", -1, &rc,
					DT_CENTER | DT_BOTTOM | DT_SINGLELINE, textCol);
			}
			else
			{
				wchar_t priceBuf[16];
				_snwprintf_s(priceBuf, 16, L"%d", WeaponPrice[i]);

				m_small->DrawTextW(nullptr, priceBuf, -1, &rc,
					DT_CENTER | DT_BOTTOM | DT_SINGLELINE, textCol);
			}
		}
	}

	for (auto& pIcon : m_WeaponIcon)
		pIcon->Render_GameObject();
}

void CShop_UI::Set_SpellInput()
{
	for (size_t k = 0; k < m_SpellIcon.size(); ++k) {
		if (auto img = dynamic_cast<CUI_Image*>(m_SpellIcon[k]))
			img->Set_Scale({ 0.19f, 0.25f, 1.f });
	}

	m_hoverIndex = -1;

	D3DVIEWPORT9 vp;
	m_pGraphicDev->GetViewport(&vp);

	const LONG  bannerWidth = 230;
	const LONG  bannerHeight = 150;
	const LONG  gapX = 12;

	for (int i = 0; i < 4; ++i)
	{
		LONG left = LONG(vp.Width * 0.12f) + (bannerWidth + gapX) * i;
		LONG top = LONG(vp.Height * 0.4f);

		RECT rc = { left, top, left + bannerWidth, top + bannerHeight };

		EVENT event;

		if (PtInRect(&rc, ptMouse)) {

			bool owned = (find(m_BuySpelllist.begin(), m_BuySpelllist.end(), i)
				!= m_BuySpelllist.end());
			if (owned) { m_hoverIndex = -1; break; }

			m_hoverIndex = i;

			auto pIcon = dynamic_cast<CUI_Image*>(m_SpellIcon[i]);
			pIcon->Set_Scale({ 0.23f, 0.3f, 1.f });

			if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::LBUTTON)) {
				const int price = SpellPrice[i];
				m_BuySpelllist.push_back(i);
				if (m_Money >= price) {
					m_Money -= price;

					switch (i)
					{
					case 0:
						//event = { PHASE::PHASE_BEGIN,
						//TOPIC::Upgrade_Ice,
						//CHANNEL::Player,
						//nullptr,
						//nullptr,
						//{},
						//{} };
						//CEventBus::GetInstance()->publish(event);
						break;
					case 1:
						//event = { PHASE::PHASE_BEGIN,
						//TOPIC::Upgrade_Ice,
						//CHANNEL::Player,
						//nullptr,
						//nullptr,
						//{},
						//{} };
						//CEventBus::GetInstance()->publish(event);
						break;
					case 2:

						break;
					case 3:
						//event = { PHASE::PHASE_BEGIN,
						//TOPIC::Upgrade_Ice,
						//CHANNEL::Player,
						//nullptr,
						//nullptr,
						//{},
						//{} };
						//CEventBus::GetInstance()->publish(event);
						break;
					}
				}
				else {

				}
			}
			break;
		}
	}
}
void CShop_UI::Set_SpellUI()
{
	if (m_small)
	{
		D3DVIEWPORT9 vp;
		m_pGraphicDev->GetViewport(&vp);

		const LONG  bannerWidth = 230;
		const LONG  bannerHeight = 150;
		const LONG  gapX = 12;

		for (int i = 0; i < 4; ++i)
		{

			LONG left = LONG(vp.Width * 0.12f) + (bannerWidth + gapX) * i;
			LONG top = LONG(vp.Height * 0.4f);

			RECT rc = { left, top, left + bannerWidth, top + bannerHeight };

			bool owned = (find(m_BuySpelllist.begin(), m_BuySpelllist.end(), i)
				!= m_BuySpelllist.end());

			const int price = SpellPrice[i];
			D3DCOLOR textCol = D3DCOLOR_ARGB(255, 244, 175, 97);
			if (i == m_hoverIndex)	textCol = D3DCOLOR_ARGB(255, 255, 255, 255);
			if (m_Money < price) textCol = D3DCOLOR_ARGB(255, 232, 0, 0);
			if (owned) textCol = D3DCOLOR_ARGB(255, 191, 255, 36);

			m_mid->DrawTextW(nullptr, SpellItems[i], -1, &rc,
				DT_CENTER | DT_TOP | DT_SINGLELINE, textCol);

			if (owned)
			{
				m_small->DrawTextW(nullptr, L"Purchased", -1, &rc,
					DT_CENTER | DT_BOTTOM | DT_SINGLELINE, textCol);
			}
			else
			{
				wchar_t priceBuf[16];
				_snwprintf_s(priceBuf, 16, L"%d", SpellPrice[i]);

				m_small->DrawTextW(nullptr, priceBuf, -1, &rc,
					DT_CENTER | DT_BOTTOM | DT_SINGLELINE, textCol);
			}
		}
	}

	for (auto& pIcon : m_SpellIcon)
		pIcon->Render_GameObject();
}

void CShop_UI::Set_CharacterInput()
{
	for (size_t k = 0; k < m_CharIcon.size(); ++k) {
		if (auto img = dynamic_cast<CUI_Image*>(m_CharIcon[k]))
			img->Set_Scale({ 0.19f, 0.25f, 1.f });
	}

	m_hoverIndex = -1;

	D3DVIEWPORT9 vp;
	m_pGraphicDev->GetViewport(&vp);

	const LONG  bannerWidth = 230;
	const LONG  bannerHeight = 150;
	const LONG  gapX = 12;

	for (int i = 0; i < 5; ++i)
	{
		LONG left = LONG(vp.Width * 0.03f) + (bannerWidth + gapX) * i;
		LONG top = LONG(vp.Height * 0.4f);

		RECT rc = { left, top, left + bannerWidth, top + bannerHeight };

		EVENT event;

		if (PtInRect(&rc, ptMouse)) {
			bool owned = (find(m_BuyCharlist.begin(), m_BuyCharlist.end(), i)
				!= m_BuyCharlist.end());
			if (owned) { m_hoverIndex = -1; break; }

			m_hoverIndex = i;

			auto pIcon = dynamic_cast<CUI_Image*>(m_CharIcon[i]);
			pIcon->Set_Scale({ 0.23f, 0.3f, 1.f });

			if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::LBUTTON)) {
				const int price = CharPrice[i];

				if (m_Money >= price) {
					m_Money -= price;
					m_BuyCharlist.push_back(i);
					switch (i)
					{
					case 0:
						//event = { PHASE::PHASE_BEGIN,
						//TOPIC::Upgrade_Ice,
						//CHANNEL::Player,
						//nullptr,
						//nullptr,
						//{},
						//{} };
						//CEventBus::GetInstance()->publish(event);
						break;
					case 1:
						//event = { PHASE::PHASE_BEGIN,
						//TOPIC::Upgrade_Ice,
						//CHANNEL::Player,
						//nullptr,
						//nullptr,
						//{},
						//{} };
						//CEventBus::GetInstance()->publish(event);
						break;
					case 2:

						break;
					case 3:
						//event = { PHASE::PHASE_BEGIN,
						//TOPIC::Upgrade_Ice,
						//CHANNEL::Player,
						//nullptr,
						//nullptr,
						//{},
						//{} };
						//CEventBus::GetInstance()->publish(event);
						break;
					case 4:

						break;
					}
				}
				else {

				}
			}
			break;
		}
	}
}
void CShop_UI::Set_CharacterUI()
{
	if (m_small)
	{
		D3DVIEWPORT9 vp;
		m_pGraphicDev->GetViewport(&vp);

		const LONG  bannerWidth = 230;
		const LONG  bannerHeight = 150;
		const LONG  gapX = 12;

		for (int i = 0; i < 5; ++i)
		{

			LONG left = LONG(vp.Width * 0.03f) + (bannerWidth + gapX) * i;
			LONG top = LONG(vp.Height * 0.4f);

			RECT rc = { left, top, left + bannerWidth, top + bannerHeight };

			bool owned = (find(m_BuyCharlist.begin(), m_BuyCharlist.end(), i)
				!= m_BuyCharlist.end());

			const int price = CharPrice[i];
			D3DCOLOR textCol = D3DCOLOR_ARGB(255, 244, 175, 97);
			if (i == m_hoverIndex)	textCol = D3DCOLOR_ARGB(255, 255, 255, 255);
			if (m_Money < price) textCol = D3DCOLOR_ARGB(255, 232, 0, 0);
			if (owned) textCol = D3DCOLOR_ARGB(255, 191, 255, 36);

			m_mid->DrawTextW(nullptr, CharItems[i], -1, &rc,
				DT_CENTER | DT_TOP | DT_SINGLELINE, textCol);

			if (owned)
			{
				m_small->DrawTextW(nullptr, L"Purchased", -1, &rc,
					DT_CENTER | DT_BOTTOM | DT_SINGLELINE, textCol);
			}
			else
			{
				wchar_t priceBuf[16];
				_snwprintf_s(priceBuf, 16, L"%d", CharPrice[i]);

				m_small->DrawTextW(nullptr, priceBuf, -1, &rc,
					DT_CENTER | DT_BOTTOM | DT_SINGLELINE, textCol);
			}
		}
	}

	for (auto& pIcon : m_CharIcon)
		pIcon->Render_GameObject();
}

void CShop_UI::Set_Toggle()
{
	if (m_bActive)
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
		while (ShowCursor(TRUE) < 0);
	}
}