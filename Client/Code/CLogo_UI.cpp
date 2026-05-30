#include "pch.h"
#include "CLogo_UI.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CInputManager.h"

CLogo_UI::CLogo_UI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUI(pGraphicDev)
{

}

CLogo_UI::CLogo_UI(const CLogo_UI& rhs)
	:CUI(rhs)
{

}

CLogo_UI::~CLogo_UI()
{

}

HRESULT	CLogo_UI::Ready_GameObject()
{
	CUI::Ready_GameObject();

	m_tHandle.m_eType = OBJ_UI;
	m_pTransform->Set_Pos(0.f, 200.f, 0.f);
	m_pTransform->Set_Scale({ 0.9f, 0.4f, 1.f });
	m_bActive = true;
	return S_OK;
}

_int CLogo_UI::Update_GameObject(const _float& fTimeDelta)
{
	CUI::Update_GameObject(fTimeDelta);

	return 0;
}

void CLogo_UI::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CUI::LateUpdate_GameObject(fTimeDelta);
}

void CLogo_UI::Render_GameObject()
{
	if (!m_bActive)
		return;

	// 컬링 꺼주고
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	CResourceManager::GetInstance()->Get<CTexture>(L"Logo")->Set_Texture();
	m_pBufferCom->Change_UV(1, 1, 1, 1);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
	m_pBufferCom->Render_Buffer();
	// 다시 켜줌.
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CLogo_UI* CLogo_UI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo_UI* pLogo_UI = new CLogo_UI(pGraphicDev);

	if (FAILED(pLogo_UI->Ready_GameObject()))
	{
		// 만약에 제대로 생성이 되지 않았다면 바로 삭제.
		Safe_Release(pLogo_UI);
		MSG_BOX("pLogo_UI Create Failed");
		return nullptr;
	}

	return pLogo_UI;
}

void CLogo_UI::Free()
{
	CUI::Free();
}
