#include "pch.h"
#include "CPlane.h"
#include "CRendererManager.h"
#include "CResourceManager.h"
#include "CInputManager.h"

CPlane::CPlane(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{

}

CPlane::CPlane(const CPlane& rhs)
	:CGameObject(rhs)
{

}

CPlane::~CPlane()
{

}

HRESULT	CPlane::Ready_GameObject()
{
	// 이니셜라이즈 부분에서 컴포넌트 추가를 한다.
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_tHandle.m_eType = OBJ_UI;
	m_pTransform->Set_Scale({ 1.7f, 1.5f, 1.f });

	return S_OK;
}

_int CPlane::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);

	CRendererManager::GetInstance()->Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CPlane::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
	Key_Input(fTimeDelta);
}

void CPlane::Render_GameObject()
{
	// 컬링 꺼주고
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphicDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	CResourceManager::GetInstance()->Get<CTexture>(L"MapUI")->Set_Texture();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
	m_pBufferCom->Render_Buffer();
	// 다시 켜줌.
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CPlane::Add_Component()
{
	//Component
	CComponent* pComponent = NULL;
	pComponent = m_pBufferCom = CLONE_PROTOTYPE(PROTO_UIRECTEX, CUIRecTex);

	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	// transform

	pComponent = m_pTransform = CLONE_PROTOTYPE(PROTO_TRANS, CTransform);

	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}

CPlane* CPlane::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlane* pMap = new CPlane(pGraphicDev);

	if (FAILED(pMap->Ready_GameObject()))
	{
		// 만약에 제대로 생성이 되지 않았다면 바로 삭제.
		Safe_Release(pMap);
		MSG_BOX("pMap Create Failed");
		return nullptr;
	}

	return pMap;
}

void CPlane::Free()
{
	Engine::CGameObject::Free();
}


void CPlane::Key_Input(const _float& fTimeDelta)
{
	static bool bActivated = false;

	static _vec3 vTargetPos = { 150.f, -150.f, 0.f };
	static _vec3 vStartPos = { 150.f, -800.f, 0.f };
	static _vec3 vCurrentPos = vStartPos;

	if (CInputManager::GetInstance()->GetButtonDown(static_cast<KEY_TYPE>(VK_TAB)))
	{
		bActivated = !bActivated;
	}

	_vec3 vDest;
	if (bActivated)
		vDest = vTargetPos;
	else
		vDest = vStartPos;

	D3DXVec3Lerp(&vCurrentPos, &vCurrentPos, &vDest, 5.f * fTimeDelta);

	m_pTransform->Set_Pos(vCurrentPos.x, vCurrentPos.y, vCurrentPos.z);
}
