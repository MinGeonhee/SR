#include "CTransform.h"

CTransform::CTransform()
	:m_vScale(1.f, 1.f, 1.f), m_vAngle(0.f, 0.f, 0.f)
{
	ZeroMemory(m_vInfo, sizeof(m_vInfo));
	D3DXMatrixIdentity(&m_matWorld); // 월드 행렬을 단위벡터로 초기화 한다.
	D3DXMatrixIdentity(&m_matRotate);
}

CTransform::CTransform(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev), m_vScale(1.f, 1.f, 1.f), m_vAngle(0.f, 0.f, 0.f)
{
	ZeroMemory(m_vInfo, sizeof(m_vInfo));
	D3DXMatrixIdentity(&m_matWorld); // 월드 행렬을 단위벡터로 초기화 한다.
	D3DXMatrixIdentity(&m_matRotate);
}

CTransform::CTransform(const CTransform& rhs)
	: CComponent(rhs), m_vScale(rhs.m_vScale), m_vAngle(rhs.m_vAngle)
{
	for (_uint i = 0; i < INFO_END; i++)
		m_vInfo[i] = rhs.m_vInfo[i];

	m_matWorld = rhs.m_matWorld;
	m_matRotate = rhs.m_matRotate;
}

CTransform::~CTransform()
{
}

HRESULT CTransform::Ready_Transform()
{
	D3DXMatrixIdentity(&m_matWorld); // 한번더 초기화... 왜 계속 초기화 하는건지? 이해 X

	for (_uint i = 0; i < INFO_END; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3)); // m_vInfo를 단위 벡터로 초기화하기 위함.

	m_matRotate = m_matWorld;

	return S_OK;
}


int CTransform::Update_Component(const _float& fTimeDelta)
{
	// 매 프레임 월드 행렬을 계속 계산해주어야하기 때문에
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matRotate); // 단위 행렬로 만들어줌.
	D3DXMatrixIdentity(&m_matScale);
	D3DXMatrixIdentity(&m_matTrans);

	for (_uint i = 0; i < INFO_POS; ++i)
		memcpy(&m_vInfo[i], &m_matWorld.m[i][0], sizeof(_vec3));

	// 크기 변환

	// 각각의 단위벡터에 scale값을 곱해준다.

	Make_ScaleMatrix();
	Make_RotateMatrix();

	for (_uint i = 0; i < INFO_POS; ++i)
	{
		D3DXVec3TransformNormal(&m_vInfo[i], &m_vInfo[i], &m_matRotate);
	}

	for (_uint i = 0; i < INFO_END; ++i)
	{
		memcpy(&m_matWorld.m[i][0], &m_vInfo[i], sizeof(_vec3));
	}

	return 0;
}

void CTransform::LateUpdate_Component(const _float& fTimeDelta)
{

}

CComponent* CTransform::Clone()
{
	return new CTransform(*this);
}

CTransform* CTransform::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTransform* pTransform = new CTransform(pGraphicDev);

	if (FAILED(pTransform->Ready_Transform()))
	{
		Safe_Release(pTransform);
		MSG_BOX("Transform Create Failed");
		return nullptr;
	}

	return pTransform;
}

void CTransform::Free()
{
	CComponent::Free();
}

void CTransform::Make_RotateMatrix()
{
	// 회전 행렬을 구하고 해당 return 되는 회전 행렬을 
	// 
	_matrix			matRot[ROT_END];

	D3DXMatrixRotationX(&matRot[ROT_X], m_vAngle.x);
	D3DXMatrixRotationY(&matRot[ROT_Y], m_vAngle.y);
	D3DXMatrixRotationZ(&matRot[ROT_Z], m_vAngle.z);

	m_matRotate = matRot[ROT_X] * matRot[ROT_Y] * matRot[ROT_Z];
}

void CTransform::Make_ScaleMatrix()
{
	D3DXMatrixScaling(&m_matScale, m_vScale.x, m_vScale.y, m_vScale.z);

	for (_uint i = 0; i < INFO_POS; ++i)
	{
		D3DXVec3Normalize(&m_vInfo[i], &m_vInfo[i]);
		m_vInfo[i] *= *(((_float*)&m_vScale) + i);
	}
}

// Get_Set 함수들 모음.
