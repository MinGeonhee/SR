#include "pch.h"
#include "CStageName_UI.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CInputManager.h"
#include "CUIManager.h"
#include "CSceneManager.h"
CStageName_UI::CStageName_UI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUI(pGraphicDev)
{

}

CStageName_UI::CStageName_UI(const CStageName_UI& rhs)
	:CUI(rhs)
{

}

CStageName_UI::~CStageName_UI()
{

}

HRESULT	CStageName_UI::Ready_GameObject()
{
	CUI::Ready_GameObject();

	m_bActive = false;
	m_tHandle.m_eType = OBJ_UI;
	m_pTransform->Set_Scale({ 2.f, 2.f, 1.f });
	m_pTransform->Set_Pos(0.f, 0.f, 0.f);

	return S_OK;
}

_int CStageName_UI::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bActive && !m_prevActive) {
		m_visibleTimer = (m_flashDuration > 0.f ? m_flashDuration : 2.5f);
		SCENETYPE stage = CSceneManager::GetInstance()->Get_CurrentSceneType();
		UpdateText(stage);
	}

	if (m_bActive) {
		m_visibleTimer -= fTimeDelta;
		if (m_visibleTimer <= 0.f) {
			m_bActive = false;
			m_visibleTimer = 0.f;
		}
	}

	m_prevActive = m_bActive;

	if (!m_bActive) return 0;
	CUI::Update_GameObject(fTimeDelta);
	return 0;
}

void CStageName_UI::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (!m_bActive) return;
	CUI::LateUpdate_GameObject(fTimeDelta);
}

void CStageName_UI::Render_GameObject()
{
	if (!m_bActive) return;

	const float dur = (m_flashDuration > 0.f ? m_flashDuration : 2.5f);

	float t = (dur > 0.f ? (m_visibleTimer / dur) : 0.f);
	t = (t < 0.f ? 0.f : (t > 1.f ? 1.f : t));
	float u = 1.f - t;
	float a = sinf(D3DX_PI * u);

	a = powf(max(0.f, a), 1.2f);
	DWORD tfactor = D3DCOLOR_COLORVALUE(1.f, 1.f, 1.f, a);
	const BYTE alpha8 = (BYTE)(a * 255.0f);


	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, tfactor);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	const D3DXMATRIX parent = *m_pTransform->Get_World();

	{
		D3DXMATRIX TR, SR, WR;
		D3DXMatrixScaling(&SR, 0.1f, 0.02f, 1.f);
		D3DXMatrixTranslation(&TR, 70.f, 117.f, 1.f);
		WR = (SR * TR) * parent;
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &WR);
		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(alpha8, 231, 199, 141));
		m_pBufferCom->Change_UV(1, 1, 1, 1);
		CResourceManager::GetInstance()->Get<CTexture>(L"Banner_Right")->Set_Texture();
		m_pBufferCom->Render_Buffer();
	}
	{
		D3DXMATRIX TL, SL, WL;
		D3DXMatrixScaling(&SL, 0.1f, 0.02f, 1.f);
		D3DXMatrixTranslation(&TL, -70.f, 117.f, 1.f);
		WL = (SL * TL) * parent;
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &WL);
		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(alpha8, 231, 199, 141));
		m_pBufferCom->Change_UV(1, 1, 1, 1);
		CResourceManager::GetInstance()->Get<CTexture>(L"Banner_Left")->Set_Texture();
		m_pBufferCom->Render_Buffer();
	}

	D3DVIEWPORT9 vp;
	m_pGraphicDev->GetViewport(&vp);

	LONG bannerHeight = 60;
	RECT rc = {
		LONG(vp.Width * 0.2f),
		LONG(vp.Height * 0.13f),
		LONG(vp.Width * 0.8f),
		LONG(vp.Height * 0.13f + bannerHeight)
	};

	DrawShadowedText(m_mid, m_strTextKey.c_str(), rc, D3DCOLOR_ARGB(alpha8, 231, 199, 141), 2, D3DCOLOR_ARGB((BYTE)(alpha8 * 0.8f), 20, 20, 20), DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}


CStageName_UI* CStageName_UI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStageName_UI* pStageName_UI = new CStageName_UI(pGraphicDev);
	if (FAILED(pStageName_UI->Ready_GameObject()))
	{
		// 만약에 제대로 생성이 되지 않았다면 바로 삭제.
		Safe_Release(pStageName_UI);
		MSG_BOX("pStageName_UI Create Failed");
		return nullptr;
	}

	return pStageName_UI;
}

void CStageName_UI::Free()
{
	CUI::Free();
}

void CStageName_UI::UpdateText(SCENETYPE st)
{
	switch (st)
	{
	case SCENETYPE::STAGE1:
		m_strTextKey = L"·Canal Streets·";
		break;
	case SCENETYPE::STAGE2:
		m_strTextKey = L"·Stage 2·";
		break;
	case SCENETYPE::BOSS:
		m_strTextKey = L"·Altar of the Baron·";
		break;
	case SCENETYPE::BOSS2:
		m_strTextKey = L"·dark citadel·";
		break;
	default:
		m_strTextKey = L"None";
		break;
	}

}
