#include "pch.h"
#include "CUI_NoticeBanner.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CInputManager.h"
#include "CUIManager.h"

CUI_NoticeBanner::CUI_NoticeBanner(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUI(pGraphicDev)
{

}

CUI_NoticeBanner::CUI_NoticeBanner(const CUI_NoticeBanner& rhs)
	:CUI(rhs)
{

}

CUI_NoticeBanner::~CUI_NoticeBanner()
{

}

HRESULT	CUI_NoticeBanner::Ready_GameObject()
{
	CUI::Ready_GameObject();

	m_bActive = false;
	m_tHandle.m_eType = OBJ_UI;
	m_pTransform->Set_Scale({ 0.5f, 0.06f, 1.f });
	m_pTransform->Set_Pos(0.f, 250.f, 1.f);

	return S_OK;
}

_int CUI_NoticeBanner::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bActive && !m_prevActive) {
		m_visibleTimer = (m_flashDuration > 0.f ? m_flashDuration : 3.f);	}

	if (m_bActive) {
		m_visibleTimer -= fTimeDelta;
		if (m_visibleTimer <= 0.f) {
			m_bActive = false;
			m_visibleTimer = 0.f;
		}
	}

	const wstring& tag = CUIManager::GetInstance()->GetCategoryTag(L"UI_NoticeBanner");
	if (!tag.empty() && tag != m_cachedTag) {
		m_cachedTag = tag;
		DialogueFromTag(tag);
	}

	m_prevActive = m_bActive;

	if (!m_bActive) return 0;
	CUI::Update_GameObject(fTimeDelta);
	return 0;
}

void CUI_NoticeBanner::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (!m_bActive) return;
	CUI::LateUpdate_GameObject(fTimeDelta);
}

void CUI_NoticeBanner::Render_GameObject()
{
	if (!m_bActive) return;
	CUI::Render_GameObject();

	const float dur = (m_flashDuration > 0.f ? m_flashDuration : 3.f);//얼마나보일건지
	float t = (dur > 0.f ? (m_visibleTimer / dur) : 0.f); //0~1로정규화.1에서 시작~0으로가면사라짐
	t = t < 0.f ? 0.f : (t > 1.f ? 1.f : t);//계싼오차방지클램프
	float a = powf(t, 1.4f);//초반 밝기 감소를 조금 늦추고 후반이 되면 감소속도를 올림
	DWORD tfactor = D3DCOLOR_COLORVALUE(1.f, 1.f, 1.f, a);
	const BYTE alpha8 = (BYTE)(a * 255.0f);//이건폰트용투명도

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

	CResourceManager::GetInstance()->Get<CTexture>(L"NotifiBcg")->Set_Texture();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
	m_pBufferCom->Change_UV(1, 1, 1, 1);
	m_pBufferCom->Render_Buffer();

	if (m_mini && !m_strText.empty())
	{
		D3DVIEWPORT9 vp;
		m_pGraphicDev->GetViewport(&vp);

		LONG bannerHeight = 36;
		RECT rc = {
			LONG(vp.Width * 0.2f),
			LONG(vp.Height * 0.13f),
			LONG(vp.Width * 0.8f),
			LONG(vp.Height * 0.13f + bannerHeight)
		};
		m_mini->DrawTextW(nullptr, m_strText.c_str(), -1, &rc,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE,
			D3DCOLOR_ARGB(alpha8, 255, 255, 255));
	}

	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}


CUI_NoticeBanner* CUI_NoticeBanner::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_NoticeBanner* pBanner = new CUI_NoticeBanner(pGraphicDev);

	if (FAILED(pBanner->Ready_GameObject()))
	{
		Safe_Release(pBanner);
		MSG_BOX("pBanner Create Failed");
		return nullptr;
	}

	return pBanner;
}

void CUI_NoticeBanner::Free()
{
	CUI::Free();
}

void CUI_NoticeBanner::DialogueFromTag(const wstring& tag)
{
	if (tag == L"Over Heal") {
		Set_Key(L"Health Is Full");
		return;
	}
	if (tag == L"Door_Open") {
		Set_Key(L"Something moved in the distance");
		return;
	}
}
