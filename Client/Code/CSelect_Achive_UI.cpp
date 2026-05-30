#include "pch.h"
#include "CSelect_Achive_UI.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CInputManager.h"
#include "CUI_Image.h"
#include "CSceneManager.h"
#include "CStage.h"
#include "CScoreBoard.h"

CSelect_Achive_UI::CSelect_Achive_UI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUI(pGraphicDev)
{

}

CSelect_Achive_UI::CSelect_Achive_UI(const CSelect_Achive_UI& rhs)
	:CUI(rhs)
{

}

CSelect_Achive_UI::~CSelect_Achive_UI()
{

}

HRESULT	CSelect_Achive_UI::Ready_GameObject()
{
	CUI::Ready_GameObject();

	m_tHandle.m_eType = OBJ_UI;
	m_bActive = true;

	m_pTransform->Set_Pos(250.f, 0.f, 0.f);

	for (int i = 0; i < 9; ++i)
	{
		if (i == 5 || i == 6) continue;

		auto* pCheck = CUI_Image::Create(m_pGraphicDev, L"Checking", 8);
		pCheck->Set_Position(350.f, 150.f - 30.f * i, 0.f);
		pCheck->Set_Scale({ 0.1f, 0.1f, 1.f });
		pCheck->SetSpriteCount(2);
		pCheck->SetSpriteIndex(0);
		m_vecUIObjects.push_back(pCheck);
	}

	return S_OK;
}

_int CSelect_Achive_UI::Update_GameObject(const _float& fTimeDelta)
{
	CUI::Update_GameObject(fTimeDelta);

	m_bMove = false;
	if (CInputManager::GetInstance()->GetButtonDown(static_cast<KEY_TYPE>(VK_SPACE)))
	{
		m_bMove = !m_bMove;
	}

	for (size_t s = 0; s < m_vecUIObjects.size(); ++s) {
		if (auto* img = dynamic_cast<CUI_Image*>(m_vecUIObjects[s])) {
			const int logic = (s < 5) ? static_cast<int>(s)
				: static_cast<int>(s) + 2;

			const bool checked = find(m_AchiveNum.begin(), m_AchiveNum.end(), logic) != m_AchiveNum.end();
			img->SetSpriteIndex(checked ? 1 : 0);
		}
	}

	for (auto& pIcon : m_vecUIObjects)
	{
		pIcon->Set_UIActive(m_bActive);
		pIcon->Update_GameObject(fTimeDelta);
	}

	return 0;
}

void CSelect_Achive_UI::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CUI::LateUpdate_GameObject(fTimeDelta);
	for (auto& pIcon : m_vecUIObjects)
		pIcon->LateUpdate_GameObject(fTimeDelta);
}

void CSelect_Achive_UI::Render_GameObject()
{
	if (!m_bActive)
		return;

	// 컬링 꺼주고
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	Select_Stage();
	Render_ChildTex();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

CSelect_Achive_UI* CSelect_Achive_UI::Create(LPDIRECT3DDEVICE9 pGraphicDev/*, int StageNum*/)
{
	CSelect_Achive_UI* pSelect_Achive_UI = new CSelect_Achive_UI(pGraphicDev);
	//pSelect_Archive_UI->Set_ClearNum(StageNum);
	if (FAILED(pSelect_Achive_UI->Ready_GameObject()))
	{
		// 만약에 제대로 생성이 되지 않았다면 바로 삭제.
		Safe_Release(pSelect_Achive_UI);
		MSG_BOX("pSelect_Achive_UI Create Failed");
		return nullptr;
	}

	return pSelect_Achive_UI;
}

void CSelect_Achive_UI::Free()
{

	for (auto& pIcon : m_vecUIObjects)
		Safe_Release(pIcon);
	m_vecUIObjects.clear();

	CUI::Free();
}

void CSelect_Achive_UI::Select_Stage()
{
	D3DVIEWPORT9 vp;
	m_pGraphicDev->GetViewport(&vp);
	_vec3 pos = *m_pTransform->GetInfoPos();

	RECT rc;

	if (m_big)
	{
		LONG bannerHeight = 45;
		int offset = 40;
		rc = {
			LONG(vp.Width * 0.38f + pos.x),
			LONG(vp.Height * 0.16f),
			LONG(vp.Width * 0.58f + pos.x),
			LONG(vp.Height * 0.16f + bannerHeight)
		};
		m_big->DrawTextW(nullptr, L" · Challenges · ", -1, &rc,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE, D3DCOLOR_ARGB(255, 41, 90, 124));
	}

	if (m_small)
	{
		LONG bannerHeight = 45;
		int offset = 40;
		rc = {
			LONG(vp.Width * 0.38f + pos.x),
			LONG(vp.Height * 0.23f),
			LONG(vp.Width * 0.58f + pos.x),
			LONG(vp.Height * 0.23f + bannerHeight)
		};
		m_small->DrawTextW(nullptr, L"Achievements", -1, &rc,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE, D3DCOLOR_ARGB(255, 255, 238, 208));
	}

	if (m_small)
	{
		LONG bannerHeight = 45;
		int offset = 40;
		rc = {
			LONG(vp.Width * 0.38f + pos.x),
			LONG(vp.Height * 0.5f),
			LONG(vp.Width * 0.58f + pos.x),
			LONG(vp.Height * 0.5f + bannerHeight)
		};
		m_small->DrawTextW(nullptr, L"Difficulties Beaten", -1, &rc,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE, D3DCOLOR_ARGB(255, 255, 238, 208));
	}

	const wchar_t* SettingAchive[5] = { L"Par Time Reached",  L"All Enemies Vanquished",  L"All Treasure Collected",
		L"All Secrets Found",  L"All Lore Scrolls Found" };
	const wchar_t* SettingDifficulty[2] = { L"Easy",  L"Difficult" };
	if (m_small)
	{
		for (int i = 0; i < 5; i++)
		{
			D3DCOLOR textCol = (i == m_iClearNum)
				? D3DCOLOR_ARGB(255, 184, 141, 103)
				: D3DCOLOR_ARGB(255, 182, 162, 147);

			LONG bannerHeight = 40;
			int offset = 28;
			rc = {
				LONG(vp.Width * 0.36f + pos.x),
				LONG(vp.Height * 0.275f + offset * i),
				LONG(vp.Width * 0.64f + pos.x),
				LONG(vp.Height * 0.275f + bannerHeight + offset * i)
			};
			m_small->DrawTextW(nullptr, SettingAchive[i], -1, &rc,
				DT_LEFT | DT_VCENTER | DT_SINGLELINE, textCol);
		}
		for (int i = 0; i < 2; i++)
		{
			D3DCOLOR textCol = (i == m_iClearNum + 5)
				? D3DCOLOR_ARGB(255, 184, 141, 103)
				: D3DCOLOR_ARGB(255, 182, 162, 147);

			LONG bannerHeight = 36;
			int offset = 28;
			rc = {
				LONG(vp.Width * 0.36f + pos.x),
				LONG(vp.Height * 0.56f + offset * i),
				LONG(vp.Width * 0.64f + pos.x),
				LONG(vp.Height * 0.56f + bannerHeight + offset * i)
			};
			m_small->DrawTextW(nullptr, SettingDifficulty[i], -1, &rc,
				DT_LEFT | DT_VCENTER | DT_SINGLELINE, textCol);
		}
	}
}

void CSelect_Achive_UI::Render_ChildTex()
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

	const D3DXMATRIX parent = *m_pTransform->Get_World();

	{
		D3DXMATRIX T1, S1, W1;
		D3DXMatrixScaling(&S1, 0.55f, 0.015f, 1.0f);
		D3DXMatrixTranslation(&T1, -20.f, -155.f, 1.f);
		W1 = (S1 * T1) * parent;
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &W1);
		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(100, 172, 134, 96));
		m_pBufferCom->Change_UV(1, 1, 1, 1);
		CResourceManager::GetInstance()->Get<CTexture>(L"Line_UI")->Set_Texture();
		m_pBufferCom->Render_Buffer();
	}

	{
		D3DXMATRIX T2, S2, W2;
		D3DXMatrixScaling(&S2, 0.55f, 0.015f, 1.0f);
		D3DXMatrixTranslation(&T2, -20.f, 5.f, 1.f);
		W2 = (S2 * T2) * parent;
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &W2);
		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(100, 172, 134, 96));
		m_pBufferCom->Change_UV(1, 1, 1, 1);
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
