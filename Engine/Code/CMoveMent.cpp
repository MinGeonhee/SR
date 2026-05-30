#include "CMoveMent.h"
#include "CProtoTypeManager.h"

CMoveMent::CMoveMent()
	: m_pTransform(nullptr)
{
	m_pMyOwner = nullptr;
	// 트랜스폼 미리 캐싱해두기.
	m_pTransform = CLONE_PROTOTYPE(PROTO_TRANS, CTransform);
}

CMoveMent::CMoveMent(const CMoveMent& rhs)
	:CComponent(rhs), m_pTransform(nullptr)
{
	m_pMyOwner = nullptr;
	m_pTransform = CLONE_PROTOTYPE(PROTO_TRANS, CTransform);
}

CMoveMent::CMoveMent(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev), m_pTransform(nullptr)
{
	m_pMyOwner = nullptr;
	m_pTransform = CLONE_PROTOTYPE(PROTO_TRANS, CTransform);
}

CMoveMent::~CMoveMent()
{

}

HRESULT CMoveMent::Ready_MoveMent()
{
	return S_OK;
}

_int CMoveMent::Update_Component(const _float& fTimeDelta)
{
	m_pTransform->Update_Component(fTimeDelta);
	Move(fTimeDelta);
	return 0;
}

void CMoveMent::LateUpdate_Component(const _float& fTimeDelta)
{

}

CComponent* CMoveMent::Clone()
{
	return new CMoveMent(*this);
}

CMoveMent* CMoveMent::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMoveMent* _pMovement = new CMoveMent(pGraphicDev);

	if (_pMovement == NULL)
		return nullptr;

	if (FAILED(_pMovement->Ready_MoveMent()))
	{
		Safe_Release(_pMovement);
		MSG_BOX("CMoveMent Create Failed");
		return nullptr;
	}

	return _pMovement;
}

void CMoveMent::Free()
{
	CComponent::Free();
}

void CMoveMent::Move(const _float& fTimeDelta)
{
	switch (m_eMoveMentState)
	{
		case MOVE_STRAIGHT:
		{
			break;
		}

		case MOVE_CHASE:
		{
			D3DXMATRIX Rotate;
			_vec3 Axis, vDir, vUp;
			m_pTargetTranform->Get_Info(INFO_POS, &vDir);
			m_pTransform->Get_Info(INFO_POS, &vUp);

			D3DXMatrixRotationAxis(&Rotate,
				D3DXVec3Cross(&Axis, D3DXVec3Normalize(&vUp, &vUp), D3DXVec3Normalize(&vDir, &vDir))
				, acosf(D3DXVec3Dot(D3DXVec3Normalize(&vDir, &vDir), D3DXVec3Normalize(&vUp, &vUp))));

			m_pTransform->Set_World(m_pTransform->Get_ScaleMatrix() * Rotate * m_pTransform->Get_TransMatrix());
			break;
		}
	}

	_vec3 vUp;
	m_pTransform->Get_Info(INFO_UP, &vUp);
	m_pTransform->Move_Pos(D3DXVec3Normalize(&vUp, &vUp), m_fSpeed, fTimeDelta);
}
