#include "pch.h"
#include "CUI_Weapon.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CInputManager.h"
#include "CUI_Number.h"
#include "CUI_Image.h"
#include "CPlayer.h"
#include "CObjectManager.h"
#include "CEventbus.h"
#include "CSetting_UI.h"

CUI_Weapon::CUI_Weapon(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUI(pGraphicDev)
{

}

CUI_Weapon::CUI_Weapon(const CUI_Weapon& rhs)
	:CUI(rhs)
{

}

CUI_Weapon::~CUI_Weapon()
{

}

HRESULT	CUI_Weapon::Ready_GameObject()
{
	CUI::Ready_GameObject();

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObjectManager::GetInstance()->Get(OBJTYPE::OBJ_PLAYER));
	m_pWeaponCon = pPlayer->Get_WeaponController();

	bActive = false;	
	m_bActive = true;
	m_tHandle.m_eType = OBJ_UI;
	m_pTransform->Set_Scale({ 2.f, 2.f, 1.f });

	for (int i = 0; i < 8; i++)
	{
		auto pWeapon = CUI_Image::Create(m_pGraphicDev, L"Shop_Weapon", 8);
		pWeapon->SetSpriteIndex(i);
		switch (i)
		{
		case 0:
			pWeapon->Set_Position(70.f, 180.f, 0.f);
			break;
		case 1:
			pWeapon->Set_Position(180.f, 70.f, 0.f);
			break;
		case 2:
			pWeapon->Set_Position(180.f, -70.f, 0.f);
			break;
		case 3:
			pWeapon->Set_Position(70.f, -180.f, 0.f);
			break;
		case 4:
			pWeapon->Set_Position(-70.f, -180.f, 0.f);
			break;
		case 5:
			pWeapon->Set_Position(-180.f, -70.f, 0.f);
			break;
		case 6:
			pWeapon->Set_Position(-180.f, 70.f, 0.f);
			break;
		case 7:
			pWeapon->Set_Position(-70.f, 180.f, 0.f);
			break;
		}
		pWeapon->Set_Scale({ 0.19f, 0.25f, 1.f });
		m_WeaponIcon.push_back(pWeapon);
	}

	return S_OK;
}

_int CUI_Weapon::Update_GameObject(const _float& fTimeDelta)
{
	CUI::Update_GameObject(fTimeDelta);
	if (!bActive) return 0;

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObjectManager::GetInstance()->Get(OBJTYPE::OBJ_PLAYER));
	m_pWeaponCon = pPlayer->Get_WeaponController();

	for (size_t k = 0; k < 8; ++k) {
		if (auto img = dynamic_cast<CUI_Image*>(m_WeaponIcon[k]))
		{
			if (k != m_hoverIndex)
			{
				img->Set_Scale({ 0.22f, 0.32f, 1.f });
			}
			else
			{
				img->Set_Scale({ 0.25f, 0.35f, 1.f });
			}
		}
	}

	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	D3DVIEWPORT9 vp;
	m_pGraphicDev->GetViewport(&vp);

	const int   slices = 8; // 몇등분? 
	const float cx = vp.Width * 0.5f; // 중심좌표 X
	const float cy = vp.Height * 0.5f; // 중심좌표 Y
	const float innerR = 100.0f; // 안쪽 원 반지름
	const float outerR = 300.0f; // 바같 원 반지름

	// 중심에서 마우스까지 벡터
	D3DXVECTOR2 v(ptMouse.x - cx, ptMouse.y - cy);
	float r2 = D3DXVec2LengthSq(&v);

	//중점에서 마우스까지의 거리가 안쪽원의 반지름보다 크고 바깥원의 반지름보단 작을때(도넛안에있을때)
	if (r2 >= innerR * innerR && r2 <= outerR * outerR)
	{
		// 화면 좌표계임 y가 위일수록 -!!!!!!!!!!!!!!!
		D3DXVECTOR2 ref(0.0f, -1.0f);

		float dot = ref.x * v.x + ref.y * v.y;
		float det = ref.x * v.y - ref.y * v.x;
		float ang = D3DXToDegree(atan2f(det, dot));	
		// atan으로 라디안 구하고, 라디안 D3DXToDegree로 도로바ㄱ구

		if (ang < 0)	// 음수나오면 360도더해서 정상화
			ang += 360.0f;

		int idx = (int)(ang * (slices / 360.0f));	// 내가정한등분값으로나눠서조각조각
		if (idx >= slices)	//오차 대비로 넣으라는데,,,?? 
			idx = slices - 1;

		m_hoverIndex = idx;

		switch (idx) {
		case 0:
			m_eWeaponState = WEAPON_FIREWAND;
			break;
		case 1:
			m_eWeaponState = WEAPON_MACE;
			break;
		case 2:
			m_eWeaponState = WEAPON_GUN;
			break;
		case 3:
			m_eWeaponState = WEAPON_ICEWAND;
			break;
		case 4:
			m_eWeaponState = WEAPON_FIRERING;
			break;
		case 5:
			//m_eWeaponState = WEAPON_ICEWAND;
			break;
		case 6:
			//m_eWeaponState = WEAPON_ICEWAND;
			break;
		case 7:
			//m_eWeaponState = WEAPON_ICEWAND;
			break;
		}
		if(CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::LBUTTON))
			m_pWeaponCon->Change_Weapon(m_eWeaponState);
	}

	for (auto& pIcon : m_WeaponIcon)
		pIcon->Update_GameObject(fTimeDelta);
	return 0;
}

void CUI_Weapon::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CUI::LateUpdate_GameObject(fTimeDelta);

	if (CInputManager::GetInstance()->GetButtonDown(static_cast<KEY_TYPE>(VK_CONTROL)))
		Set_Toggle();
	
	else if(CInputManager::GetInstance()->GetButtonUp(static_cast<KEY_TYPE>(VK_CONTROL)))
		Set_Toggle();

	for (auto& pPlayerUI : m_WeaponIcon)
		pPlayerUI->LateUpdate_GameObject(fTimeDelta);
}

void CUI_Weapon::Render_GameObject()
{
	if (!bActive)
		return;

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	const D3DXMATRIX parent = *m_pTransform->Get_World();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &parent);
	CResourceManager::GetInstance()->Get<CTexture>(L"Back")->Set_Texture();
	m_pBufferCom->Render_Buffer();
	//도넛
	{
		D3DXMATRIX T, S, W;
		D3DXMatrixScaling(&S, 1.f, 1.5f, 1.0f);
		D3DXMatrixTranslation(&T, 0.f, 0.f, 1.f);
		W = S * T;
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &W);
		m_pBufferCom->Change_UV(m_hoverIndex, 8, 1, 1);
		CResourceManager::GetInstance()->Get<CTexture>(L"donut")->Set_Texture();
		m_pBufferCom->Render_Buffer();
	}
	// 내부 문구
	if (m_small && m_big)
	{
		D3DVIEWPORT9 vp;
		m_pGraphicDev->GetViewport(&vp);
		const wchar_t* Weapons[8] = { L"Wand", L"Mace" , L"Spellstriker", L"FrostWeaver", L"Fire Rings", L"CrossBow", L"Gloves", L"Stick", };

		RECT rc = {
			LONG(vp.Width * 0.5f - (150 / 2)),
			LONG(vp.Height * 0.5f - 80),
			LONG(vp.Width * 0.5f + (150 / 2)),
			LONG(vp.Height * 0.5f - 40)
		};

		m_small->DrawTextW(nullptr, Weapons[m_hoverIndex], -1, &rc,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE, D3DCOLOR_ARGB(255, 255, 255, 255));

		rc = {
			LONG(vp.Width * 0.5f - (100 / 2)),
			LONG(vp.Height * 0.5f + 50),
			LONG(vp.Width * 0.5f + (100 / 2)),
			LONG(vp.Height * 0.5f + 100)
		};

		wchar_t buffer[32];
		swprintf(buffer, 32, L"%d", m_pWeaponCon->Get_Bullet(m_eWeaponState));

		m_big->DrawTextW(nullptr, buffer, -1, &rc,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE,
			D3DCOLOR_ARGB(255, 255, 198, 114));
	}
	// 총알 종류

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CUI_Weapon* CUI_Weapon::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_Weapon* pPauseUI = new CUI_Weapon(pGraphicDev);

	if (FAILED(pPauseUI->Ready_GameObject()))
	{
		// 만약에 제대로 생성이 되지 않았다면 바로 삭제.
		Safe_Release(pPauseUI);
		MSG_BOX("pPauseUI Create Failed");
		return nullptr;
	}

	return pPauseUI;
}

void CUI_Weapon::Free()
{
	for (auto& pPlayerUI : m_WeaponIcon)
		Safe_Release(pPlayerUI);
	m_WeaponIcon.clear();

	CUI::Free();
}

void CUI_Weapon::Set_Toggle()
{
	bActive = !bActive;
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
