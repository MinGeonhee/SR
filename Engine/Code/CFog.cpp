#include "CFog.h"
#include "CBillboard.h"
#include "CObjectManager.h"
#include "CRendererManager.h"
#include "CTexture.h"
#include "CResourceManager.h"

CFog::CFog(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{

}

CFog::CFog(const CFog& rhs)
	:CGameObject(rhs)
{

}

CFog::~CFog()
{

}

HRESULT	CFog::Ready_GameObject()
{
	// 이니셜라이즈 부분에서 컴포넌트 추가를 한다.
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_tHandle.m_eType = OBJ_FOG;

	m_pTransform->Set_Scale({ 1.8f, 1.8f, 1.8f });

	return S_OK;
}

_int CFog::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);

	CRendererManager::GetInstance()->Add_RenderGroup(RENDER_ALPHABLEND, this);

	_vec3 vPos = *this->Get_Transform()->GetInfoPos();

	CGameObject::Compute_ViewZ(&vPos);


	return 0;
}

void CFog::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CFog::Render_GameObject()
{
	// 현재 블렌드 상태 백업
	DWORD oldSrc, oldDst, oldOp;
	m_pGraphicDev->GetRenderState(D3DRS_SRCBLEND, &oldSrc);
	m_pGraphicDev->GetRenderState(D3DRS_DESTBLEND, &oldDst);
	m_pGraphicDev->GetRenderState(D3DRS_BLENDOP, &oldOp);

	// 가산 혼합: src * α + dst * 1  (프리멀티플라이는 ONE, ONE 권장)
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA); // 또는 D3DBLEND_ONE
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	m_pGraphicDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);


	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
	CResourceManager::GetInstance()->Get<CTexture>(L"Fog")->Set_Texture();
	m_pBufferCom->Render_Buffer();

	// 블렌드 상태 원복
	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, oldSrc);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, oldDst);
	m_pGraphicDev->SetRenderState(D3DRS_BLENDOP, oldOp);
}


HRESULT CFog::Add_Component()
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

	// =============== 버퍼 컴포넌트 ==================
	pComponent = m_pBufferCom = CLONE_PROTOTYPE(PROTO_RECTEX, CRecTex);

	if (nullptr == pComponent)
		return E_FAIL;

	m_pBufferCom->Set_MyOwner(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Buffer",pComponent });

	return S_OK;
}


CFog* CFog::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFog* pfog = new CFog(pGraphicDev);

	if (FAILED(pfog->Ready_GameObject()))
	{
		// 만약에 제대로 생성이 되지 않았다면 바로 삭제.
		Safe_Release(pfog);
		MSG_BOX("fog Create Failed");
		return nullptr;
	}

	return pfog;
}

void CFog::Free()
{
	Engine::CGameObject::Free();
}

void CFog::Make_Random_Position(_vec3 _vRange, _vec3 _vPos)
{
	// 시드 생성 (시간 기반으로 고유하게 생성)
	unsigned seed = static_cast<unsigned>(
		chrono::high_resolution_clock::now().time_since_epoch().count());
	mt19937 rng(seed);  // 난수 생성기

	// 각 축(X, Y, Z)에 대해 범위 지정
	uniform_real_distribution<float> distX(_vPos.x - _vRange.x / 2, _vPos.x + _vRange.x);
	uniform_real_distribution<float> distY(-0.4f, _vRange.y);
	uniform_real_distribution<float> distZ(_vPos.z - _vRange.z / 2, _vPos.z + _vRange.z);

	// 랜덤 좌표 생성
	float fRandX = distX(rng);
	float fRandY = distY(rng);
	float fRandZ = distZ(rng);

	m_pTransform->Set_Pos(fRandX , fRandY + _vPos.y, fRandZ );
}


