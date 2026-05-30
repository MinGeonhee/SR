#include "CUI.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CInputManager.h"
#include "CUIManager.h" 

bool	   CUI::s_bPaused = false;

CUI::CUI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev), m_strTag(L"")
{

}

CUI::CUI(const CUI& rhs)
	:CGameObject(rhs)
{

}

CUI::~CUI()
{

}

HRESULT	CUI::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;
	m_tHandle.m_eType = OBJ_UI;

	auto* uim = CUIManager::GetInstance();
	if (FAILED(uim->InitFonts(m_pGraphicDev))) return E_FAIL;

	m_big = uim->GetBig();
	m_mid = uim->GetMid();
	m_small = uim->GetSmall();
	m_mini = uim->GetMini();
	return S_OK;
}

_int CUI::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_bActive) return 0;
	CGameObject::Update_GameObject(fTimeDelta);
	CRendererManager::GetInstance()->Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CUI::Render_GameObject()
{
	if (!m_bActive)
		return;
	CGameObject::Render_GameObject();
}

HRESULT CUI::Add_Component()
{
	// ==================버퍼 ==============
	//Component
	CComponent* pComponent = NULL;
	pComponent = m_pBufferCom = CLONE_PROTOTYPE(PROTO_UITILETEX, CUITileTex);

	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });


	pComponent = m_pAniCom = CLONE_PROTOTYPE(PROTO_ANIMATION, CAnimation);

	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Animation",pComponent });

	// ===============트랜스폼=============
	pComponent = m_pTransform = CLONE_PROTOTYPE(PROTO_TRANS, CTransform);

	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}


CUI* CUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI* pUI = new CUI(pGraphicDev);

	if (FAILED(pUI->Ready_GameObject()))
	{
		Safe_Release(pUI);
		MSG_BOX("pUI Create Failed");
		return nullptr;
	}

	return pUI;
}

void CUI::Free()
{
	m_big = m_mid = m_small = nullptr;
	Engine::CGameObject::Free();
}

void CUI::DrawShadowedText(
	ID3DXFont* font, const wchar_t* text, const RECT& rc,
	DWORD colorMain, int shadowOfs, DWORD colorShadow, DWORD format)
{
	if (!font || !text) return;

	RECT rShadow = rc;
	OffsetRect(&rShadow, shadowOfs, shadowOfs);
	font->DrawTextW(nullptr, text, -1, &rShadow, format, colorShadow);
	font->DrawTextW(nullptr, text, -1, const_cast<RECT*>(&rc), format, colorMain);
}

void CUI::FormatTimeStr(float t, wchar_t* out, size_t cap) {
	if (t < 0.f) t = 0.f;
	int cs = static_cast<int>(lround(static_cast<double>(t) * 100.0));
	int mm = cs / 6000;  
		cs %= 6000;
	int ss = cs / 100;   
	int cc = cs % 100;
	_snwprintf_s(out, cap, _TRUNCATE, L"%02d:%02d.%02d", mm, ss, cc);
}