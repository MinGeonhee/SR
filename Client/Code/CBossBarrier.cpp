#include "pch.h"
#include "CBossBarrier.h"
#include "CTransform.h"
#include "CGameObject.h"
#include "CPrototypeManager.h"
#include "CResourceManager.h"
#include "CRendererManager.h"
#include "CCameraManager.h"
#include "CObjectManager.h"


CBossBarrier::CBossBarrier(LPDIRECT3DDEVICE9 pGrapahicDev)
	:CMonster(pGrapahicDev)
{

}

CBossBarrier::CBossBarrier(const CBossBarrier& rhs)
	:CMonster(rhs)
{

}


HRESULT CBossBarrier::Ready_GameObject()
{
	CMonster::Ready_GameObject();

	if (FAILED(Add_Component())) return E_FAIL;

	m_pBarrierTexture = CResourceManager::GetInstance()->Get<CTexture>(L"baron-shield");
	Set_Animation();
	m_pBossOwner = CObjectManager::GetInstance()->Get(OBJ_MONSTER_BOSS);
	m_pTransform->Set_Scale({ 2.f, 2.f, 2.f });
	m_pColliderCom->Set_ColliderScale({ 3.0f, 3.0f, 3.0f });
	m_pColliderCom->Set_Offset({0.f, 0.5f, 0.f});

	m_tHandle.m_eType = OBJ_BARRIER;

	return S_OK;
}

_int CBossBarrier::Update_GameObject(const _float& fTimeDelta)
{
	CMonster::Update_GameObject(fTimeDelta);
	CRendererManager::GetInstance()->Add_RenderGroup(RENDERID::RENDER_ALPHATESTING, this);
	SetMyPosition();
	return 0;
}

void CBossBarrier::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CMonster::LateUpdate_GameObject(fTimeDelta);
}

void CBossBarrier::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
	if (m_pBarrierTexture != NULL) m_pBarrierTexture->Set_Texture();
	m_pAnimationCom->Render_Buffer();
}

HRESULT CBossBarrier::Add_Component()
{
	CComponent* pComponent = NULL;

	// ============= 베리어 =========== 
	pComponent = m_pBarrier = CLONE_PROTOTYPE(PROTO_BARRIER, CBarrier);

	if (nullptr == pComponent)
		return E_FAIL;

	m_pBarrier->Set_MyOwner(this);
	m_pBarrier->Set_BarrierActive(true);
	m_pBarrier->Set_BarrierCount(4);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Barrier",pComponent });

	return S_OK; 
}

CBossBarrier* CBossBarrier::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBossBarrier* pBarrier = new CBossBarrier(pGraphicDev);

	if (FAILED(pBarrier->Ready_GameObject()))
	{
		Safe_Release(pBarrier);
		MSG_BOX("Barrier Create Failed");
		return nullptr;
	}

	return pBarrier;
}

void CBossBarrier::SetMyPosition()
{
	if (!m_pTransform || !m_pBossOwner) return;

	CGameObject* target = m_pBossOwner->Get_Target();
	if (!target) return; // 타깃 없으면 패스(원하면 보스 정면 등으로 대체)

	const _vec3 owner = *m_pBossOwner->Get_Transform()->GetInfoPos();
	const _vec3 tpos = *target->Get_Transform()->GetInfoPos();

	_vec3 dir = tpos - owner;

	if (dir.x == 0.f && dir.y == 0.f && dir.z == 0.f) return;

	const float distSq = dir.x * dir.x + dir.y * dir.y + dir.z * dir.z;
	const float dist = sqrtf(distSq);
	dir /= dist; 

	const float desired = 1.1f;  
	const float margin = 0.05f; 


	float upper = dist - margin;
	if (upper < 0.f) upper = 0.f;

	float t = desired;
	if (t > upper) t = upper;

	const _vec3 posoffset = { 0.f, 0.2f, 0.f };
	const _vec3 pos = owner + dir * t * 1.001f + posoffset;
	m_pTransform->Set_Pos(pos.x, pos.y, pos.z);
}


void CBossBarrier::Set_Animation()
{
	// 애니메이션의 행 열 지정.
	m_pAnimationCom->SetGrid(9, 1); // 
	m_pAnimationCom->SetRow(0);      // 맨 위 행
	m_pAnimationCom->SetCol(0, 8);
	m_pAnimationCom->AniMation_Init(9, 0.15f);	// 몇 열이고 출력 속도 몇인지
	m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Loop);
	m_pTransform->Set_Scale(_vec3{ 2.f, 2.f, 2.f });
	m_pAnimationCom->PlayAnimation();
}


void CBossBarrier::Free()
{
	CMonster::Free();
}