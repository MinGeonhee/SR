#include "pch.h"
#include "CUIBackGround.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CInputManager.h"

CUIBackGround::CUIBackGround(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUI(pGraphicDev)
{

}

CUIBackGround::CUIBackGround(const CUIBackGround& rhs)
	:CUI(rhs)
{

}

CUIBackGround::~CUIBackGround()
{

}

HRESULT	CUIBackGround::Ready_GameObject()
{
	CUI::Ready_GameObject();

	m_bActive = true;
	m_tHandle.m_eType = OBJ_UI;
	m_pTransform->Set_Scale({ 2.f, 2.f, 1.f });
	m_pTransform->Set_Pos(0.f, 0.f, 0.f);

	return S_OK;
}

_int CUIBackGround::Update_GameObject(const _float& fTimeDelta)
{
	CUI::Update_GameObject(fTimeDelta);

	return 0;
}

void CUIBackGround::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CUI::LateUpdate_GameObject(fTimeDelta);
}

void CUIBackGround::Render_GameObject()
{
	// 컬링 꺼주고
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	CResourceManager::GetInstance()->Get<CTexture>(m_strTextureKey)->Set_Texture();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
	m_pBufferCom->Change_UV(1, 1, 1, 1);
	m_pBufferCom->Render_Buffer();

	// 다시 켜줌.
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CUIBackGround* CUIBackGround::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring strTextureKey)
{
	CUIBackGround* pBack = new CUIBackGround(pGraphicDev);
	pBack->Set_TextureKey(strTextureKey);
	if (FAILED(pBack->Ready_GameObject()))
	{
		// 만약에 제대로 생성이 되지 않았다면 바로 삭제.
		Safe_Release(pBack);
		MSG_BOX("pBack Create Failed");
		return nullptr;
	}

	return pBack;
}

void CUIBackGround::Free()
{
	CUI::Free();
}
