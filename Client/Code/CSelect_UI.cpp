#include "pch.h"
#include "CSelect_UI.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CInputManager.h"
#include "CSelect_Achive_UI.h"
#include "CSelect_Stage_UI.h"
#include "CUI_Image.h"
#include "CScoreBoard.h" 
#include "CSoundManager.h"

static inline SCENETYPE StageNum_Change(int stageNum)
{
	switch (stageNum) {
	case 0: return SCENETYPE::STAGE1;
	case 1: return SCENETYPE::STAGE2;
	case 2: return SCENETYPE::BOSS;
	default: return (stageNum < 0) ? SCENETYPE::STAGE1 : SCENETYPE::BOSS;
	}
}

CSelect_UI::CSelect_UI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUI(pGraphicDev)
{

}

CSelect_UI::CSelect_UI(const CSelect_UI& rhs)
	:CUI(rhs)
{

}

CSelect_UI::~CSelect_UI()
{

}

HRESULT	CSelect_UI::Ready_GameObject()
{
	CUI::Ready_GameObject();

	m_tHandle.m_eType = OBJ_UI;
	m_bActive = true;
	m_bVanish = true;

	auto pAchive_UI = CSelect_Achive_UI::Create(m_pGraphicDev);
	m_vecUIObjects.push_back(pAchive_UI);

	auto pStage_UI = CSelect_Stage_UI::Create(m_pGraphicDev);
	m_vecUIObjects.push_back(pStage_UI);

	auto pLeft_Button = CUI_Image::Create(m_pGraphicDev, L"StageArrow_Left", 3);
	pLeft_Button->SetSpriteIndex(0);
	pLeft_Button->Set_Position(-520.f, 0.f, 0.f);
	pLeft_Button->Set_Scale({ 0.1f, 0.15f, 1.f });
	m_vecUIObjects.push_back(pLeft_Button);

	auto pRight_Button = CUI_Image::Create(m_pGraphicDev, L"StageArrow_Right", 3);
	pRight_Button->SetSpriteIndex(0);
	pRight_Button->Set_Position(520.f, 0.f, 0.f);
	pRight_Button->Set_Scale({ 0.1f, 0.15f, 1.f });
	m_vecUIObjects.push_back(pRight_Button);

	Set_Ani();

	return S_OK;
}

_int CSelect_UI::Update_GameObject(const _float& fTimeDelta)
{
	Set_Toggle(fTimeDelta);

	CUI::Update_GameObject(fTimeDelta);
	if (m_pAniCom->Get_IsAnimationEnd())
		m_bMove = false;

	const int cur = ((m_iStageNum % 3) + 3) % 3;

	if (!m_bMove && m_bDown)
	{
		for (auto& pSelect_UI : m_vecUIObjects)
		{
			pSelect_UI->Update_GameObject(fTimeDelta);
			pSelect_UI->Set_UIActive(m_bActive);
		}

		if (auto* pStage_UI = dynamic_cast<CSelect_Stage_UI*>(m_vecUIObjects[1]))
			pStage_UI->Set_StNum(cur + 1);

		const SCENETYPE st = StageNum_Change(cur);

		const auto sc = CScoreBoard::GetInstance()->GetStage(st);
		const bool parOK = (sc.parTime > 0.f && sc.lastTime > 0.f && sc.parTime >= sc.lastTime);

		if (auto* pAchive = dynamic_cast<CSelect_Achive_UI*>(m_vecUIObjects[0]))
			pAchive->SetCheck(0, parOK);
	}

	auto pLeft_Button = dynamic_cast<CUI_Image*>(m_vecUIObjects[2]);
	if (cur == 0)
		pLeft_Button->SetSpriteIndex(2);
	else if (cur != 0 && m_StageIndex == 0)
		pLeft_Button->SetSpriteIndex(1);
	else
		pLeft_Button->SetSpriteIndex(0);

	auto pRight_Button = dynamic_cast<CUI_Image*>(m_vecUIObjects[3]);
	if (cur == 2)
		pRight_Button->SetSpriteIndex(2);
	else if (cur != 2 && m_StageIndex == 1)
		pRight_Button->SetSpriteIndex(1);
	else
		pRight_Button->SetSpriteIndex(0);

	return 0;
}

void CSelect_UI::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CUI::LateUpdate_GameObject(fTimeDelta);
	Key_Input();

	if (!m_bMove && m_bDown)
	{
		for (auto& pSelect_UI : m_vecUIObjects)
			pSelect_UI->LateUpdate_GameObject(fTimeDelta);
		Key_StageChange();
	}
}

void CSelect_UI::Render_GameObject()
{
	if (!m_bActive)
		return;

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
	CResourceManager::GetInstance()->Get<CTexture>(L"SelectBook")->Set_Texture();
	m_pAniCom->Render_Buffer();

	Select_Stage();

	if (!m_bMove && m_bDown)
	{
		for (auto& pSelect_UI : m_vecUIObjects)
			pSelect_UI->Render_GameObject();
	}
	
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CSelect_UI* CSelect_UI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSelect_UI* pSelect_UI = new CSelect_UI(pGraphicDev);

	if (FAILED(pSelect_UI->Ready_GameObject()))
	{
		// 만약에 제대로 생성이 되지 않았다면 바로 삭제.
		Safe_Release(pSelect_UI);
		MSG_BOX("pSelect_UI Create Failed");
		return nullptr;
	}

	return pSelect_UI;
}

void CSelect_UI::Free()
{
	for (auto& pSelect_UI : m_vecUIObjects)
		Safe_Release(pSelect_UI);
	m_vecUIObjects.clear();

	CUI::Free();
}

void CSelect_UI::Set_Ani()
{
	if (m_bMove == false)
	{
		m_pAniCom->SetGrid(9, 1);
		m_pAniCom->SetRow(0);
		m_pAniCom->AniMation_Init(1, 0.25f);
		m_pAniCom->SetPlayMode(CAnimation::PlayMode::Once);
		m_pAniCom->PlayAnimation();
		m_pTransform->Set_Scale(_vec3{ 1000.f, 700.f, 1000.f });
	}
	else
	{
		m_pAniCom->SetGrid(9, 1);
		m_pAniCom->SetRow(0);
		m_pAniCom->AniMation_Init(9, 0.05f);
		m_pAniCom->SetPlayMode(CAnimation::PlayMode::Once);
		m_pAniCom->PlayAnimation();
		m_pTransform->Set_Scale(_vec3{ 1000.f, 700.f, 1000.f });
	}

}

void CSelect_UI::Select_Stage()
{
	if (m_mid)
	{
		D3DVIEWPORT9 vp;
		m_pGraphicDev->GetViewport(&vp);

		LONG bannerHeight = 36;
		int offset = 40;
		RECT rc = {
			LONG(vp.Width * 0.47f),
			LONG(vp.Height * 0.9f),
			LONG(vp.Width * 0.53f),
			LONG(vp.Height * 0.9f + bannerHeight)
		};

		D3DCOLOR textCol = (m_hoverIndex == 0)
			? D3DCOLOR_ARGB(255, 255, 255, 255)
			: D3DCOLOR_ARGB(255, 255, 198, 114);

		m_mid->DrawTextW(nullptr, L"Back", -1, &rc,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE, textCol);
	}
}

void CSelect_UI::Key_Input()
{
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	D3DVIEWPORT9 vp;
	m_pGraphicDev->GetViewport(&vp);

	m_hoverIndex = -1;

	LONG bannerHeight = 36;
	int offset = 40;
	RECT rc = {
		LONG(vp.Width * 0.47f),
		LONG(vp.Height * 0.9f),
		LONG(vp.Width * 0.53f),
		LONG(vp.Height * 0.9f + bannerHeight)
	};

	if (PtInRect(&rc, ptMouse))
	{
		m_hoverIndex = 0;
		if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::LBUTTON))
		{
			m_bVanish = true;
		}
	}
}

void CSelect_UI::Key_StageChange()
{
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	D3DVIEWPORT9 vp;
	m_pGraphicDev->GetViewport(&vp);

	m_StageIndex = -1;

	RECT rcLeftButton = {
		LONG(vp.Width * 0.08f),
		LONG(vp.Height * 0.47f),
		LONG(vp.Width * 0.1f),
		LONG(vp.Height * 0.53f)
	};

	RECT rcRightButton = {
	LONG(vp.Width * 0.9f),
	LONG(vp.Height * 0.47f),
	LONG(vp.Width * 0.92f),
	LONG(vp.Height * 0.53f)
	};

	const int cur = ((m_iStageNum % 3) + 3) % 3;

	if (PtInRect(&rcLeftButton, ptMouse)) {
		m_StageIndex = 0;
		if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::LBUTTON) && cur != 0) {
			CSoundManager::GetInstance()->SoundPlay(L"book-flip.wav", SOUNDCHANNEL::UI_SOUND, 0.5f);
			m_bMove = !m_bMove;
			m_iStageNum -= 1;
			Set_Ani();
		}
	}

	if (PtInRect(&rcRightButton, ptMouse)) {
		m_StageIndex = 1;
		if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::LBUTTON) && cur != 2) {
			CSoundManager::GetInstance()->SoundPlay(L"book-flip.wav", SOUNDCHANNEL::UI_SOUND, 0.5f);
			m_bMove = !m_bMove;
			m_iStageNum += 1;
			Set_Ani();
		}
	}
}

void CSelect_UI::Set_Toggle(const _float& fTimeDelta)
{
	static _vec3 vTargetPos = { 0.f, -300.f, 0.f };
	static _vec3 vStartPos = { 0.f, 1000.f, 0.f };
	static _vec3 vCurrentPos = vStartPos;

	if (m_bVanish) {
		vCurrentPos = vStartPos;
		m_bDown = false;
		m_bActive = false;
		m_pTransform->Set_Pos(vCurrentPos.x, vCurrentPos.y, vCurrentPos.z);
		return;
	}

	const float a = (fTimeDelta > 0.f) ? min(1.f, 15.f * fTimeDelta) : 0.f;
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
	}
	else {
		m_bDown = false;
		m_bActive = true;
	}

	m_pTransform->Set_Pos(vCurrentPos.x, vCurrentPos.y, vCurrentPos.z);
}
