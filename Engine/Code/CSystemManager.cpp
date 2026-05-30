#include "CSystemManager.h"
#include "CDoorSystem.h"
#include "CCombatSystem.h"
#include "CCameraSystem.h"
#include "CTimeSystem.h"
#include "CUISystem.h"
#include "CInputSystem.h"
#include "CMonsterSystem.h"
#include "CItemSystem.h"
#include "CEventCubeSystem.h"
#include "CInteractionSystem.h"
#include "CEyeOfChaosSystem.h"
#include "CBossSystem.h"
#include "CSoundSystem.h"

IMPLEMENT_SINGLETON(CSystemManager)

CSystemManager::CSystemManager()
{

}

CSystemManager::~CSystemManager()
{
	Free();
}

_int CSystemManager::System_Update(const float& fTimeDelta)
{
	CItemSystem::GetInstance()->System_Update(fTimeDelta);
	CDoorSystem::GetInstance()->System_Update(fTimeDelta);
	CCombatSystem::GetInstance()->System_Update(fTimeDelta);
	CMonsterSystem::GetInstance()->System_Update(fTimeDelta);
	CBossSystem::GetInstance()->System_Update(fTimeDelta);
	CCameraSystem::GetInstance()->System_Update(fTimeDelta);
	CTimeSystem::GetInstance()->System_Update(fTimeDelta);
	CInputSystem::GetInstance()->System_Update(fTimeDelta);
	CInteractionSystem::GetInstance()->System_Update(fTimeDelta);
	CEventCubeSystem::GetInstance()->System_Update(fTimeDelta);
	CEyeOfChaosSystem::GetInstance()->System_Update(fTimeDelta);

	// 반드시 UI & sound가 마지막
	CUISystem::GetInstance()->System_Update(fTimeDelta);
	CSoundSystem::GetInstance()->System_Update(fTimeDelta);

	return 0;
}

void CSystemManager::System_LateUpdate(const float& fTimeDelta)
{
	CItemSystem::GetInstance()->System_LateUpdate(fTimeDelta);
	CDoorSystem::GetInstance()->System_LateUpdate(fTimeDelta);
	CCombatSystem::GetInstance()->System_LateUpdate(fTimeDelta);
	CInteractionSystem::GetInstance()->System_LateUpdate(fTimeDelta);
	CMonsterSystem::GetInstance()->System_LateUpdate(fTimeDelta);
	CBossSystem::GetInstance()->System_LateUpdate(fTimeDelta);
	CCameraSystem::GetInstance()->System_LateUpdate(fTimeDelta);
	CTimeSystem::GetInstance()->System_LateUpdate(fTimeDelta);
	CInputSystem::GetInstance()->System_LateUpdate(fTimeDelta);
	CEventCubeSystem::GetInstance()->System_LateUpdate(fTimeDelta);
	CEyeOfChaosSystem::GetInstance()->System_LateUpdate(fTimeDelta);

	// 반드시 UI & sound가 마지막
	CUISystem::GetInstance()->System_LateUpdate(fTimeDelta);
	CSoundSystem::GetInstance()->System_LateUpdate(fTimeDelta);

}

void CSystemManager::Free()
{
	CDoorSystem::GetInstance()->DestroyInstance();
	CCombatSystem::GetInstance()->DestroyInstance();
	CMonsterSystem::GetInstance()->DestroyInstance();
	CBossSystem::GetInstance()->DestroyInstance();
	CCameraSystem::GetInstance()->DestroyInstance();
	CTimeSystem::GetInstance()->DestroyInstance();
	CUISystem::GetInstance()->DestroyInstance();
	CInputSystem::GetInstance()->DestroyInstance();
	CItemSystem::GetInstance()->DestroyInstance();
	CInteractionSystem::GetInstance()->DestroyInstance();
	CEventCubeSystem::GetInstance()->DestroyInstance();
	CEyeOfChaosSystem::GetInstance()->DestroyInstance();
	CSoundSystem::GetInstance()->DestroyInstance();
}