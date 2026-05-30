#include "CStatus.h"
#include "CCollisionManager.h"
#include "CObjectManager.h"
#include "CObjectPoolingManager.h"
#include "CEventBus.h"

CStatus::CStatus()
	:CComponent()
{

}

CStatus::CStatus(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
{

}

CStatus::CStatus(const CStatus& rhs)
	:CComponent(rhs)
{

}

HRESULT CStatus::Ready_Status()
{

	return S_OK;
}

_int CStatus::Update_Component(const _float& iTimeDelta)
{
	// 내 체력을 고정 시켜주는 함수
	ClampCurrentHp();
	Update_FrozenByIceStacks(iTimeDelta);

	return 0;
}

void CStatus::LateUpdate_Component(const _float& fTimeDelta)
{

}

void CStatus::ClampCurrentHp()
{
	if (m_tStatus.fMaxHp <= m_tStatus.fCurrentHp)
		m_tStatus.fCurrentHp = m_tStatus.fMaxHp;

	if (m_tStatus.fCurrentHp <= 0)
		m_tStatus.fCurrentHp = 0;

	if (m_tStatus.fDefence <= 0)
		m_tStatus.fDefence = 0;

}

void CStatus::Update_FrozenByIceStacks(const float& fTimeDelta)
{
	if (m_iFrozenStack >= 3)
		m_bFrozen = true;

	if (m_bFrozen)
	{
		m_fElapsedtime += fTimeDelta;

		if (!m_bFrozenEvent)	// 나 얼었어요 이벤트 안보냈었다면.
		{
			EVENT event
			{
				// 나 얼었엉
				PHASE::PHASE_BEGIN,
				TOPIC::FROZEN,
				CHANNEL::Combat,
				m_pMyOwner,
				nullptr,
				m_pMyOwner->Get_Handle(),
				{}
			};

			CEventBus::GetInstance()->publish(event);
			m_bFrozenEvent = true;
		}
	}

	if (m_bFrozen && m_fElapsedtime >= m_fFreezeTime)	// 현재 얼어있는 상태인데 시간이 3초 이상 경과되었다면
	{
		// 얼음 상태 해제
		EVENT event
		{
			// 나 얼은거 풀렸엉
			PHASE::PHASE_END,
			TOPIC::FROZEN,
			CHANNEL::Combat,
			m_pMyOwner,
			nullptr,
			m_pMyOwner->Get_Handle(),
			{}
		};

		CEventBus::GetInstance()->publish(event);

		m_fElapsedtime = 0.f;
		m_iFrozenStack = 0;
		m_bFrozen = false;
		m_bFrozenEvent = true;
	}
}

CComponent* CStatus::Clone()
{
	return new CStatus(*this);
}

CStatus* CStatus::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStatus* pStatus = new CStatus(pGraphicDev);

	if (FAILED(pStatus->Ready_Status()))
	{
		Safe_Release(pStatus);
		MSG_BOX("Status Create Failed");
		return nullptr;
	}

	return pStatus;
}

void CStatus::Free()
{
	CComponent::Free();
}
