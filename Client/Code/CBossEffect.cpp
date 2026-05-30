#include "pch.h"
#include "CBossEffect.h"
#include "CResourceManager.h"
#include "CRendererManager.h"

CBossEffect::CBossEffect(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{

}

CBossEffect::CBossEffect(const CGameObject& rhs)
	:CGameObject(rhs)
{

}

CBossEffect::~CBossEffect()
{

}

HRESULT CBossEffect::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	// 빔 길이/베이스 설정
	m_fBeamLen = 8.f; // 원하는 길이
	m_pMeshAnimCom->EnableZBeam(true);
	m_pMeshAnimCom->EnableMeshBend(false);
	m_pMeshAnimCom->SetBeamLength(m_fBeamLen);
	m_pMeshAnimCom->SetZBase(-m_fBeamLen * 0.5f);

	// 정점 재배치 (토글/파라미터 반영)
	m_pMeshAnimCom->RebuildShape();
	m_pTransform->Set_Scale({ m_fBeamWidth, 3.f, 3.f });
	m_pMeshAnimCom->PlayAnimation();


	// 초당 90도 회전
	m_fSpinSpeedRad = D3DXToRadian(90.f);
	m_fAngleYRad = 0.f;

	return S_OK;
}

_int CBossEffect::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);

	m_fAngleYRad += m_fSpinSpeedRad * fTimeDelta;
	if (m_fAngleYRad > D3DX_PI * 2.f) m_fAngleYRad -= D3DX_PI * 2.f;

	CRendererManager::GetInstance()->Add_RenderGroup(RENDERID::RENDER_ALPHABLEND, this);
	return 0;
}

void CBossEffect::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);

	_vec3 vPos;
	m_pTransform->Get_Info(INFO_POS, &vPos);

	CGameObject::Compute_ViewZ(&vPos);
}
void CBossEffect::Render_GameObject()
{
	const _matrix* pBaseWorld = m_pTransform->Get_World();
	CResourceManager::GetInstance()->Get<CTexture>(L"Effect_Thunder_Anim")->Set_Texture();

	// 한 번만! (한 가닥만 그린다)
	float angle = m_fAngleYRad + m_fPhaseOffset;

	D3DXMATRIXA16 R, W;
	D3DXMatrixRotationY(&R, angle);
	W = R * (*pBaseWorld); // 컨벤션에 따라 (*pBaseWorld) * R 가능

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &W);
	m_pMeshAnimCom->Render_Buffer();
}


HRESULT CBossEffect::Add_Component()
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

	pComponent = m_pMeshAnimCom = CLONE_PROTOTYPE(PROTO_BOSS_EFFECT_ANIMATION, CBossEffectAnimation);

	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_DYNAMIC].insert({ L"Com_MeshAnimation",pComponent });


	return S_OK;
}

CBossEffect* CBossEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBossEffect* pTerrain = new CBossEffect(pGraphicDev);

	if (FAILED(pTerrain->Ready_GameObject()))
	{
		Safe_Release(pTerrain);
		MSG_BOX("Terrain Create Failed");
		return nullptr;
	}

	return pTerrain;
}

void CBossEffect::Free()
{
	Engine::CGameObject::Free();
}