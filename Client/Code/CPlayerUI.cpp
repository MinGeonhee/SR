#include "pch.h"
#include "CPlayerUI.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CInputManager.h"
#include "CUI_Number.h"
#include "CUI_Image.h"
#include "CPlayer.h"
#include "CObjectManager.h"
#include "CWeaponController.h"
#include "CItemSystem.h"

CPlayerUI::CPlayerUI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUI(pGraphicDev)
{

}

CPlayerUI::CPlayerUI(const CPlayerUI& rhs)
	:CUI(rhs)
{

}

CPlayerUI::~CPlayerUI()
{

}

HRESULT	CPlayerUI::Ready_GameObject()
{
	CUI::Ready_GameObject();

	m_bActive = true;
	m_tHandle.m_eType = OBJ_UI;

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObjectManager::GetInstance()->Get(OBJTYPE::OBJ_PLAYER));
	m_pWeaponCon = pPlayer->Get_WeaponController();
	//HP
	auto pHpIcon = CUI_Image::Create(m_pGraphicDev, L"HpIcon", 3);
	pHpIcon->SetSpriteIndex(0);
	pHpIcon->Set_Position(-600.f, -320.f, 0.f);
	pHpIcon->Set_Scale({ 0.09f, 0.14f, 1.f });
	m_vecUIObjects.push_back(pHpIcon);

	auto pHpUI = CUI_Number::Create(m_pGraphicDev);
	pHpUI->Set_Position(-560.f, -320.f, 0.f);
	pHpUI->Set_Number(pPlayer->Get_Hp());
	m_vecUIObjects.push_back(pHpUI);

	//DEF
	auto pDefIcon = CUI_Image::Create(m_pGraphicDev, L"DefIcon", 1);
	pDefIcon->SetSpriteIndex(0);
	pDefIcon->Set_Position(-460.f, -320.f, 0.f);
	pDefIcon->Set_Scale({ 0.08f, 0.13f, 1.f });
	m_vecUIObjects.push_back(pDefIcon);

	auto pDefUI = CUI_Number::Create(m_pGraphicDev);
	pDefUI->Set_Position(-410.f, -320.f, 0.f);
	pDefUI->Set_Number(pPlayer->Get_Def());
	m_vecUIObjects.push_back(pDefUI);

	//ITEM
	auto pItemIcon = CUI_Image::Create(m_pGraphicDev, L"ItemIcon", 1);
	pItemIcon->SetSpriteIndex(0);
	pItemIcon->Set_Position(470.f, -320.f, 0.f);
	pItemIcon->Set_Scale({ 0.1f, 0.16f, 1.f });
	m_vecUIObjects.push_back(pItemIcon);

	auto pItemCnt = CUI_Number::Create(m_pGraphicDev);
	pItemCnt->Set_Position(390.f, -320.f, 0.f);
	pItemCnt->Set_Number(pPlayer->Get_ItemCnt());
	m_vecUIObjects.push_back(pItemCnt);

	//BUL
	auto pBulIcon = CUI_Image::Create(m_pGraphicDev, L"BulIcon", 10);
	pBulIcon->SetSpriteIndex(0);
	pBulIcon->Set_Position(600.f, -320.f, 0.f);
	pBulIcon->Set_Scale({ 0.1f, 0.16f, 1.f });
	m_vecUIObjects.push_back(pBulIcon);

	auto pBulCnt = CUI_Number::Create(m_pGraphicDev);
	pBulCnt->Set_Position(520.f, -320.f, 0.f);
	pBulCnt->Set_Number(m_pWeaponCon->Get_Bullet());
	m_vecUIObjects.push_back(pBulCnt);

	auto pMoneyIcon = CUI_Image::Create(m_pGraphicDev, L"Money", 1);
	pMoneyIcon->SetSpriteIndex(0);
	pMoneyIcon->Set_Position(-600.f, 330.f, 0.f);
	pMoneyIcon->Set_Scale({ 0.1f, 0.12f, 1.f });
	m_vecUIObjects.push_back(pMoneyIcon);

	auto pCross = CUI_Image::Create(m_pGraphicDev, L"Cross");
	pCross->Set_Position(0.f, 0.f, 0.f);
	pCross->Set_Scale({ 0.035f, 0.05f, 1.f });
	m_vecUIObjects.push_back(pCross);

	for (int i = 0; i < 4; i++)
	{
		auto pIcon = CUI_Image::Create(m_pGraphicDev, L"Key_Icon", 4);
		pIcon->SetSpriteIndex(i); 
		pIcon->Set_Position(450.f + 50 * i, 310.f, 0.f);
		pIcon->Set_Scale({ 0.1f, 0.2f, 1.f });
		pIcon->Set_UIActive(false);
		m_vecUIObjects.push_back(pIcon);
	}

	return S_OK;
}

_int CPlayerUI::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bActive) return 0;
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObjectManager::GetInstance()->Get(OBJTYPE::OBJ_PLAYER));
	CUI::Update_GameObject(fTimeDelta);

	m_pWeaponCon = pPlayer->Get_WeaponController();
	m_eWeaponState = m_pWeaponCon->Get_WeaponType();


	for (auto& pPlayerUI : m_vecUIObjects)
		pPlayerUI->Update_GameObject(fTimeDelta);

	auto pHpIcon = dynamic_cast<CUI_Image*>(m_vecUIObjects[0]);
	if (pPlayer->Get_Hp() > 60)
		pHpIcon->SetSpriteIndex(0);
	else if (pPlayer->Get_Hp() > 30)
		pHpIcon->SetSpriteIndex(1);
	else
		pHpIcon->SetSpriteIndex(2);

	auto pHpUI = dynamic_cast<CUI_Number*>(m_vecUIObjects[1]);
	if (pHpUI) pHpUI->Set_Number(pPlayer->Get_Hp());

	auto pDefUI = dynamic_cast<CUI_Number*>(m_vecUIObjects[3]);
	if (pDefUI) pDefUI->Set_Number(pPlayer->Get_Def());

	auto pItemCnt = dynamic_cast<CUI_Number*>(m_vecUIObjects[5]);
	if (pItemCnt) pItemCnt->Set_Number(pPlayer->Get_ItemCnt());

	auto pBulCnt = dynamic_cast<CUI_Number*>(m_vecUIObjects[7]);
	if (pBulCnt)
	{
		if (m_eWeaponState == WEAPON_MACE)
			pBulCnt->Set_UIActive(false);
		else {
			pBulCnt->Set_UIActive(true);
			pBulCnt->Set_Number(m_pWeaponCon->Get_Bullet());
		}
	}

	auto isys = CItemSystem::GetInstance();
	const bool hasBronze = isys->HasKey(ITEMTYPE::ITEM_BRONZE_KEY);
	const bool hasSilver = isys->HasKey(ITEMTYPE::ITEM_SILVER_KEY);
	const bool hasGold = isys->HasKey(ITEMTYPE::ITEM_GOLD_KEY);
	const bool hasTreasure = isys->HasKey(ITEMTYPE::ITEM_TREASURE_KEY);

	if (m_vecUIObjects.size() >= 14) {
		if (auto* goldIcon = dynamic_cast<CUI_Image*>(m_vecUIObjects[10]))
			goldIcon->Set_UIActive(hasGold);
		if (auto* silverIcon = dynamic_cast<CUI_Image*>(m_vecUIObjects[11]))
			silverIcon->Set_UIActive(hasSilver);
		if (auto* bronzeIcon = dynamic_cast<CUI_Image*>(m_vecUIObjects[12]))
			bronzeIcon->Set_UIActive(hasBronze);
		if (auto* TreasureIcon = dynamic_cast<CUI_Image*>(m_vecUIObjects[13]))
			TreasureIcon->Set_UIActive(hasTreasure);
	}

	m_pPlayerMoney = pPlayer->Get_MoneyCnt();

	return 0;
}

void CPlayerUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (!m_bActive) return;
	CUI::LateUpdate_GameObject(fTimeDelta);
	for (auto& pPlayerUI : m_vecUIObjects)
		pPlayerUI->LateUpdate_GameObject(fTimeDelta);
}

void CPlayerUI::Render_GameObject()
{
	if (!m_bActive) return;

	if (m_small)
	{
		D3DVIEWPORT9 vp;
		m_pGraphicDev->GetViewport(&vp);

		LONG bannerHeight = 36;
		RECT rc = {
			LONG(vp.Width * 0.03f),
			LONG(vp.Height * 0.015f),
			LONG(vp.Width * 0.13f),
			LONG(vp.Height * 0.015f + bannerHeight)
		};

		wchar_t buffer[32];
		swprintf(buffer, 32, L"%d", m_pPlayerMoney);

		m_small->DrawTextW(nullptr, buffer, -1, &rc,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE,
			D3DCOLOR_ARGB(255, 255, 198, 114));
	}

	for (auto& pPlayerUI : m_vecUIObjects)
		pPlayerUI->Render_GameObject();
}

CPlayerUI* CPlayerUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayerUI* pPlayerUI = new CPlayerUI(pGraphicDev);

	if (FAILED(pPlayerUI->Ready_GameObject()))
	{
		// 만약에 제대로 생성이 되지 않았다면 바로 삭제.
		Safe_Release(pPlayerUI);
		MSG_BOX("pPlayerUI Create Failed");
		return nullptr;
	}

	return pPlayerUI;
}

void CPlayerUI::Free()
{
	for (auto& pPlayerUI : m_vecUIObjects)
		Safe_Release(pPlayerUI);
	m_vecUIObjects.clear();

	CUI::Free();
}
