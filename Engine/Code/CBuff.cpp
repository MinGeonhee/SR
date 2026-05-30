#include "CBuff.h"
#include "CBillboard.h"
#include "CObjectManager.h"
#include "CResourceManager.h"
#include "CRendererManager.h"

CBuff::CBuff(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{

}

CBuff::CBuff(const CBuff& rhs)
	:CGameObject(rhs)
{

}

CBuff::~CBuff()
{

}

HRESULT	CBuff::Ready_GameObject()
{
	// 이니셜라이즈 부분에서 컴포넌트 추가를 한다.
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_tHandle.m_eType = OBJ_NONE;

	m_pTransform->Set_Scale({ 0.2f,0.2f,0.2f });

	m_pAnimationCom->SetGrid(8, 8); // 4행 8열
	m_pAnimationCom->SetRow(0);      // 맨 위 행
	m_pAnimationCom->AniMation_Init(6, 0.07f);	// 몇 열이고 출력 속도 몇인지
	m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Loop);
	m_pAnimationCom->PlayAnimation();

	return S_OK;
}

_int CBuff::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);
	CRendererManager::GetInstance()->Add_RenderGroup(RENDER_ALPHATESTING, this);

	_vec3 TargetPos = *(m_pTarget->Get_Transform()->GetInfoPos());
	m_pTransform->Set_Pos(TargetPos.x, TargetPos.y + m_fOffset_Y, TargetPos.z);

	_vec3 vPos = *this->Get_Transform()->GetInfoPos();
	CGameObject::Compute_ViewZ(&vPos);

	return 0;
}

void CBuff::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);


}

void CBuff::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
	CResourceManager::GetInstance()->Get<CTexture>(L"Buff_Anim")->Set_Texture();
	m_pAnimationCom->Render_Buffer();
}

HRESULT CBuff::Add_Component()
{
	CComponent* pComponent = NULL;

	//================ 트랜스폼 컴포넌트 ===============
	pComponent = m_pTransform = CLONE_PROTOTYPE(PROTO_TRANS, CTransform);

	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });


	// =============== 빌보드 컴포넌트 ==================

	pComponent = m_pBillboardCom = CLONE_PROTOTYPE(PROTO_BILLBOARD, CBillboard);

	if (nullptr == pComponent)
		return E_FAIL;

	m_pBillboardCom->Set_MyOwner(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Billboard",pComponent });

	//================ 애니메이션 컴포넌트 ===============

	pComponent = m_pAnimationCom = CLONE_PROTOTYPE(PROTO_ANIMATION, CAnimation);

	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Animation",pComponent });

	return S_OK;
}


void CBuff::Free()
{
	Engine::CGameObject::Free();
}


CBuff* CBuff::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBuff* pItem = new CBuff(pGraphicDev);

	if (FAILED(pItem->Ready_GameObject()))
	{
		// 만약에 제대로 생성이 되지 않았다면 바로 삭제.
		Safe_Release(pItem);
		MSG_BOX("CBuff Create Failed");
		return nullptr;
	}

	return pItem;
}


