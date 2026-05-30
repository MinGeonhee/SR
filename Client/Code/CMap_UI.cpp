#include "pch.h"
#include "CMap_UI.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CInputManager.h"
#include "CUIManager.h"
CMap_UI::CMap_UI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CUI(pGraphicDev)
{

}

CMap_UI::CMap_UI(const CMap_UI& rhs)
	:CUI(rhs)
{

}

CMap_UI::~CMap_UI()
{

}

HRESULT	CMap_UI::Ready_GameObject()
{
	CUI::Ready_GameObject();

	m_tHandle.m_eType = OBJ_UI;
	m_pTransform->Set_Scale({ 1.7f, 1.7f, 1.f });
	m_bActive = true;
	return S_OK;
}

_int CMap_UI::Update_GameObject(const _float& fTimeDelta)
{
	CUI::Update_GameObject(fTimeDelta);

	return 0;
}

void CMap_UI::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CUI::LateUpdate_GameObject(fTimeDelta);
	Set_Toggle(fTimeDelta);
}

void CMap_UI::Render_GameObject()
{
	if (!m_bActive)
		return;

	// 컬링 꺼주고
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	CResourceManager::GetInstance()->Get<CTexture>(L"MapUI")->Set_Texture();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
	m_pBufferCom->Change_UV(0, 1, 0, 1);
	m_pBufferCom->Render_Buffer();
	// 다시 켜줌.
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

CMap_UI* CMap_UI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMap_UI* pMap = new CMap_UI(pGraphicDev);

	if (FAILED(pMap->Ready_GameObject()))
	{
		// 만약에 제대로 생성이 되지 않았다면 바로 삭제.
		Safe_Release(pMap);
		MSG_BOX("pMap Create Failed");
		return nullptr;
	}

	return pMap;
}

void CMap_UI::Free()
{
	CUI::Free();
}


void CMap_UI::Set_Toggle(const _float& fTimeDelta)
{
	static _vec3 vTargetPos = { 0.f, -50.f, 0.f };
	static _vec3 vStartPos = { 0.f, -800.f, 0.f };
	static _vec3 vCurrentPos = vStartPos;

	if (CInputManager::GetInstance()->Get_KeyLock())
		return;

	if (CInputManager::GetInstance()->GetButtonDown(static_cast<KEY_TYPE>(VK_TAB)))
	{
		m_bVanish = !m_bVanish;

		if (m_bVanish)
			CUIManager::GetInstance()->DeactivateUI(L"PlayerTex");
		else
			CUIManager::GetInstance()->ActivateUI(L"PlayerTex");
	}

	const _vec3 vDest = m_bVanish ? vTargetPos : vStartPos;
	float t = 5.f * fTimeDelta;
	if (t > 1.f) t = 1.f;
	D3DXVec3Lerp(&vCurrentPos, &vCurrentPos, &vDest, t);
	m_pTransform->Set_Pos(vCurrentPos.x, vCurrentPos.y, vCurrentPos.z);
}
