#include "CBillboard.h"
#include "CTransform.h"
#include "CGameObject.h"
#include "CCameraManager.h"

CBillboard::CBillboard()
{

}

CBillboard::~CBillboard()
{

}

CBillboard::CBillboard(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
{

}

CBillboard::CBillboard(const CBillboard& rhs)
	:CComponent(rhs)
{

}

HRESULT CBillboard::Ready_Billboard()
{


	return S_OK;
}

_int CBillboard::Update_Component(const _float& fTimeDelta)
{


	return 0;
}

void CBillboard::LateUpdate_Component(const _float& fTimeDelta)
{
	if (m_bActive)
	{
		Set_Billboard();
	}
}

void CBillboard::Set_Billboard()
{
	_matrix matWorld, matView, matBill;

	// 현재 월드, 뷰 행렬 가져오기
	m_pMyOwner->Get_Transform()->Get_World(&matWorld);
	matView = CCameraManager::GetInstance()->Get_MainViewMatrix();

	// 기존 월드 행렬 분해
	_vec3 vScale, vTrans;
	D3DXQUATERNION qRot;
	D3DXMatrixDecompose(&vScale, &qRot, &vTrans, &matWorld);

	// 스케일, 이동 행렬 생성
	_matrix matScale, matTrans;
	D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);
	D3DXMatrixTranslation(&matTrans, vTrans.x, vTrans.y, vTrans.z);

	// 뷰 행렬에서 회전만 추출 후 역행렬 → 카메라의 월드 회전
	D3DXMatrixIdentity(&matBill);
	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;
	D3DXMatrixInverse(&matBill, 0, &matBill);

	// 최종 월드 행렬 재조합 (스케일 → 빌보드 회전 → 이동)
	matWorld = matScale * matBill * matTrans;

	m_pMyOwner->Get_Transform()->Set_World(&matWorld);
}


CComponent* CBillboard::Clone()
{
	return new CBillboard(*this);
}

CBillboard* CBillboard::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBillboard* pBillboard = new CBillboard(pGraphicDev);

	if (FAILED(pBillboard->Ready_Billboard()))
	{
		Safe_Release(pBillboard);
		MSG_BOX("Billboard Create Failed");
		return nullptr;
	}

	return pBillboard;
}

void CBillboard::Free()
{
	CComponent::Free();
}