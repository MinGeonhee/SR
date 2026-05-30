#include "CBossAppearEffect.h"
#include "CResourceManager.h"
#include "CRendererManager.h"

CBossAppearEffect::CBossAppearEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{

}

CBossAppearEffect::CBossAppearEffect(const CGameObject& rhs)
	:CGameObject(rhs)
{

}

CBossAppearEffect::~CBossAppearEffect()
{

}

HRESULT CBossAppearEffect::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_tHandle.m_eType = OBJ_EFFECT_BOSS_APPEAR;
	m_pTransform->Set_Scale({ 2.5f,2.5f,2.5f });

	// 
	m_pMeshAnimCom->SetGrid(4, 4); // 4?? 8??
	m_pMeshAnimCom->SetRow(0);      // ?? ?? ??
	m_pMeshAnimCom->AniMation_Init(4, 0.05f);	// ?? ????? ??? ??? ??????.
	m_pMeshAnimCom->SetPlayMode(CMeshAnimation::PlayMode::Loop);
	m_pMeshAnimCom->PlayAnimation();

	return S_OK;
}

_int CBossAppearEffect::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);

	CRendererManager::GetInstance()->Add_RenderGroup(RENDERID::RENDER_ALPHATESTING, this);

	return 0;
}

void CBossAppearEffect::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);

	_vec3 vPos;
	m_pTransform->Get_Info(INFO_POS, &vPos);

	CGameObject::Compute_ViewZ(&vPos);
}

void CBossAppearEffect::Render_GameObject()
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
	CResourceManager::GetInstance()->Get<CTexture>(L"Effect_Thunder_Anim")->Set_Texture();
	m_pMeshAnimCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CBossAppearEffect::Add_Component()
{
	CComponent* pComponent = NULL;

	//================ ???? ??????? ===============
	pComponent = m_pBufferCom = CLONE_PROTOTYPE(PROTO_BOSS_APPEAR_MESH_TEX, CBossAppearMeshTex);

	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer",pComponent });

	//================ ??????? ??????? ===============
	pComponent = m_pTransform = CLONE_PROTOTYPE(PROTO_TRANS, CTransform);

	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	//================ ??? ??????? ??????? ===============

	pComponent = m_pMeshAnimCom = CLONE_PROTOTYPE(PROTO_MESH_ANIMATION, CMeshAnimation);

	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_DYNAMIC].insert({ L"Com_MeshAnimation",pComponent });


	return S_OK;
}

CBossAppearEffect* CBossAppearEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBossAppearEffect* pTerrain = new CBossAppearEffect(pGraphicDev);

	if (FAILED(pTerrain->Ready_GameObject()))
	{
		Safe_Release(pTerrain);
		MSG_BOX("Terrain Create Failed");
		return nullptr;
	}

	return pTerrain;
}

void CBossAppearEffect::Free()
{
	Engine::CGameObject::Free();
}