#include "CDoor_Component.h"
#include "CCollisionManager.h"
#include "CObjectManager.h"
#include "CObjectPoolingManager.h"
#include "CEventBus.h"

CDoor_Component::CDoor_Component()
	:CComponent()
{

}

CDoor_Component::CDoor_Component(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev)
{

}

CDoor_Component::CDoor_Component(const CDoor_Component& rhs)
	:CComponent(rhs)
{

}

HRESULT CDoor_Component::Ready_DoorComponent()
{

	return S_OK;
}

_int CDoor_Component::Update_Component(const _float& iTimeDelta)
{
	// 내 체력을 고정 시켜주는 함수

	return 0;
}

void CDoor_Component::LateUpdate_Component(const _float& iTimeDelta)
{

}

void CDoor_Component::DoorType_Setting()
{
	auto Index = Get_MyOwner()->Get_HandleEventIndex();
	Index = 6;

	if (Index == 1) m_eDoorType = DOORTYPE::BRONZE_DOOR;
	if (Index == 2) m_eDoorType = DOORTYPE::SILVER_DOOR;
	if (Index == 3) m_eDoorType = DOORTYPE::GOLD_DOOR;
	if (Index == 4) m_eDoorType = DOORTYPE::BOSS_DOOR;

	if (Index == 5) m_eDoorType = DOORTYPE::DEFAULT_WOOD_DOOR;
	if (Index == 6) m_eDoorType = DOORTYPE::DEFAULT_IRON_DOOR;

	//이긴한데 안쓸듯?
	if (1501 < Index && Index < 1600) m_eDoorType = DOORTYPE::SWITCH_WOOD_DOOR;
	if (1601 < Index && Index < 1700) m_eDoorType = DOORTYPE::SWITCH_IRON_DOOR;

}

CComponent* CDoor_Component::Clone()
{
	return new CDoor_Component(*this);
}

CDoor_Component* CDoor_Component::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDoor_Component* pStatus = new CDoor_Component(pGraphicDev);

	if (FAILED(pStatus->Ready_DoorComponent()))
	{
		Safe_Release(pStatus);
		MSG_BOX("Status Create Failed");
		return nullptr;
	}

	return pStatus;
}

void CDoor_Component::Free()
{
	CComponent::Free();
}
