#include "CDropItem.h"
#include "CTransform.h"
#include "CGameObject.h"
#include "CObjectManager.h"
#include "CSceneManager.h"

CDropItem::CDropItem()
	:  m_pTargetTransform(nullptr)
{

}

CDropItem::CDropItem(LPDIRECT3DDEVICE9 pGrapahicDev)
	:CComponent(pGrapahicDev)
{

}

CDropItem::CDropItem(const CDropItem& rhs)
	:CComponent(rhs)
{

}


_int CDropItem::Update_Component(const _float& fTimeDelta)
{
	Drop_Update(fTimeDelta);
	return 0;
}

void CDropItem::LateUpdate_Component(const _float& fTimeDelta)
{
	
}


HRESULT CDropItem::Ready_DropItem()
{
	return S_OK;
}

CComponent* CDropItem::Clone()
{
	return new CDropItem(*this);
}

CDropItem* CDropItem::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDropItem* pHover = new CDropItem(pGraphicDev);

	if (FAILED(pHover->Ready_DropItem()))
	{
		Safe_Release(pHover);
		MSG_BOX("DropItem Create Failed");
		return nullptr;
	}

	return pHover;
}

void CDropItem::Free()
{
	CComponent::Free();
}

void CDropItem::Drop_Update(const _float& fTimeDelta)
{
	if (m_bIsOnFloor)
	{
		m_bIsDropped = false;
		m_fDropTimer = 0.f;
		return;
	}
	// 최초로 들어왓을때
	if (!m_bIsDropped)
	{
		// 시드 생성 (시간 기반으로 고유하게 생성)
		unsigned seed = static_cast<unsigned>(
			chrono::high_resolution_clock::now().time_since_epoch().count());
		mt19937 rng(seed);  // 난수 생성기

		// 랜덤각 범위 지정
		uniform_real_distribution<float> XAngle(0.f, (2 * 3.14f));
		uniform_real_distribution<float> ZAngle(0.f, (2 * 3.14f));

		// 랜덤각 생성
		float fRandXAngle = XAngle(rng);
		float fRandZAngle = ZAngle(rng);

		float dx = cosf(fRandXAngle);
		float dz = sinf(fRandZAngle);
		m_vDir_Drop = { dx, 0.f, dz };

		CGameObject* owner = Get_MyOwner();
		_vec3 vDropInitPos = *(m_pTargetTransform->GetInfoPos());
		owner->Get_Transform()->Set_Pos(vDropInitPos.x, vDropInitPos.y, vDropInitPos.z);
	}

	m_bIsDropped = true;

	// 활성화 타이머 증가
	m_fDropTimer += fTimeDelta;

	// 시간
	float t = m_fDropTimer;

	if (CSceneManager::GetInstance()->Get_CurrentSceneType() == SCENETYPE::BOSS ||
		CSceneManager::GetInstance()->Get_CurrentSceneType() == SCENETYPE::BOSS2)
	{
		m_fSpeed_XZ_Drop = 2.f;
		m_fSpeed_Y_Drop = 9.f;
		m_fSpeed = 2.5f;
	}
	else if (CSceneManager::GetInstance()->Get_CurrentSceneType() == SCENETYPE::STAGE1 ||
		CSceneManager::GetInstance()->Get_CurrentSceneType() == SCENETYPE::STAGE2)
	{
		m_fSpeed_XZ_Drop = 2.5f;
		m_fSpeed_Y_Drop = 5.f;
		m_fSpeed = 1.f;
	}

	_vec3 pos;

	// 포물선 공식
	pos.x = m_fSpeed_XZ_Drop * m_vDir_Drop.x * t;
	pos.y = m_fSpeed_Y_Drop * t - (1.f * m_fGravity_Drop * t * t);
	pos.z = m_fSpeed_XZ_Drop * m_vDir_Drop.z * t;

	CGameObject* owner = Get_MyOwner();
	owner->Get_Transform()->Move_Pos(&pos, m_fSpeed, fTimeDelta); // 위로 이동
	_vec3 vPlayerPos = *(CObjectManager::GetInstance()->Get(OBJ_PLAYER)->Get_Transform()->GetInfoPos());
	if (owner->Get_Transform()->GetInfoPos()->y < vPlayerPos.y-0.51f)
	{
		m_bIsOnFloor = true;
	}
}

