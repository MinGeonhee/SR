#include "pch.h"
#include "CBackGround.h"
#include "CRendererManager.h"
#include "CResourceManager.h"

CBackGround::CBackGround(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CBackGround::CBackGround(const CGameObject& rhs)
	: CGameObject(rhs)
{
}

CBackGround::~CBackGround()
{
}

HRESULT CBackGround::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_tHandle.m_eType = OBJ_TILE;
	m_pTransform->Set_Pos(0.f, 0.f, 0.f);
	m_pTransform->Set_Scale(_vec3{1.83f, 1.6f, 1.f});

	return S_OK;
}

_int CBackGround::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);

	CRendererManager::GetInstance()->Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CBackGround::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CBackGround::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	CResourceManager::GetInstance()->Get<CTexture>(L"MainBackGround")->Set_Texture();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBackGround::Add_Component()
{
	CComponent* pComponent = NULL;

	pComponent = m_pBufferCom = CLONE_PROTOTYPE(PROTO_UIRECTEX, CUIRecTex);
	
	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer",pComponent });

	// =============== Æ®·£½ºÆû ÄÄÆ÷³ÍÆ® ==========
	pComponent = m_pTransform = CLONE_PROTOTYPE(PROTO_TRANS, CTransform);

	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}


CBackGround* CBackGround::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBackGround* pBackGround = new CBackGround(pGraphicDev);

	if (FAILED(pBackGround->Ready_GameObject()))
	{
		Safe_Release(pBackGround);
		MSG_BOX("BackGround Create Failed");
		return nullptr;
	}

	return pBackGround;
}

void CBackGround::Free()
{
	Engine::CGameObject::Free();
}
