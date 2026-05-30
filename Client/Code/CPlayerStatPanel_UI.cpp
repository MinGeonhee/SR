#include "pch.h"
#include "CPlayerStatPanel_UI.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CInputManager.h"
#include "CUIManager.h"

CPlayerStatPanel_UI::CPlayerStatPanel_UI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUI(pGraphicDev)
{

}

CPlayerStatPanel_UI::CPlayerStatPanel_UI(const CPlayerStatPanel_UI& rhs)
	:CUI(rhs)
{

}

CPlayerStatPanel_UI::~CPlayerStatPanel_UI()
{

}

HRESULT	CPlayerStatPanel_UI::Ready_GameObject()
{
	CUI::Ready_GameObject();

	m_bActive = false;
	m_tHandle.m_eType = OBJ_UI;
	m_pTransform->Set_Scale({ 2.f, 2.f, 1.f });
	m_pTransform->Set_Pos(0.f, 0.f, 0.f);

	return S_OK;
}

_int CPlayerStatPanel_UI::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bActive && !m_prevActive) {
		m_visibleTimer = (m_flashDuration > 0.f ? m_flashDuration : 0.3f);
		const wstring& tag = CUIManager::GetInstance()->GetCategoryTag(L"PlayerStatPanel_UI");
		m_cachedTag = tag;
		UpdateText(tag);
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

void CPlayerStatPanel_UI::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (!m_bActive) return;
	CUI::LateUpdate_GameObject(fTimeDelta);
}

void CPlayerStatPanel_UI::Render_GameObject()
{
	if (!m_bActive) return;

	const float dur = (m_flashDuration > 0.f ? m_flashDuration : 0.3f);
	float t = (dur > 0.f ? (m_visibleTimer / dur) : 0.f);
	if (t < 0.f) 
		t = 0.f; 
	else if (t > 1.f) 
		t = 1.f;
	float a = powf(t, 1.4f);
	DWORD tfactor = D3DCOLOR_COLORVALUE(1.f, 1.f, 1.f, a);

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

	CResourceManager::GetInstance()->Get<CTexture>(m_strTextureKey)->Set_Texture();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
	m_pBufferCom->Change_UV(1, 1, 1, 1);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}


CPlayerStatPanel_UI* CPlayerStatPanel_UI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayerStatPanel_UI* pBack = new CPlayerStatPanel_UI(pGraphicDev);
	if (FAILED(pBack->Ready_GameObject()))
	{
		// 만약에 제대로 생성이 되지 않았다면 바로 삭제.
		Safe_Release(pBack);
		MSG_BOX("pBack Create Failed");
		return nullptr;
	}

	return pBack;
}

void CPlayerStatPanel_UI::Free()
{
	CUI::Free();
}

void CPlayerStatPanel_UI::UpdateText(const wstring& tag)
{
	if (tag == L"Eat_Effect") {
		m_strTextureKey = L"Eat";
	}
	else if (tag == L"Hit_Effect") {
		m_strTextureKey = L"Hit";
	}
	else if (tag == L"Get_Effect") {
		m_strTextureKey = L"Get";
	}
	else {
		m_strTextureKey = L"Get";
	}
}
