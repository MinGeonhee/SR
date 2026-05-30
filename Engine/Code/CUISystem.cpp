#include "CUISystem.h"
#include "CEventBus.h"
#include "CGameObject.h"
#include "CColliderBase.h"
#include "CObjectManager.h"
#include "CSystemManager.h"
#include "CUIManager.h"
#include "CSceneManager.h"
#include "CObjectPoolingManager.h"
#include "CItem.h"
#include "CSoundManager.h"

IMPLEMENT_SINGLETON(CUISystem)

CUISystem::CUISystem()
{

}

CUISystem::~CUISystem()
{
	Free();
}

_int CUISystem::System_Update(const float& fTimeDelta)
{

	return 0;
}

void CUISystem::System_LateUpdate(const float& fTimeDelta)
{
	Scene_Change_Button();
	UI_Open_Close();
	Interaction_UI();
}

void CUISystem::Scene_Change_Button()
{
	auto bus = CEventBus::GetInstance();

	EVENT event;

	if (bus->Subscribe(CHANNEL::UI, TOPIC::Go_Main, PHASE::PHASE_BEGIN, event, false))
	{
		CUIManager::GetInstance()->ForceUnpauseAll();
		CSceneManager::GetInstance()->Change_Scene(SCENETYPE::LOADING, LOADINGID::LOADING_LOGO);
	}

	if (bus->Subscribe(CHANNEL::UI, TOPIC::Go_1st, PHASE::PHASE_BEGIN, event, false))
	{
		CUIManager::GetInstance()->ForceUnpauseAll();
		CSceneManager::GetInstance()->Change_Scene(SCENETYPE::LOADING, LOADINGID::LOADING_STAGE);
	}

	if (bus->Subscribe(CHANNEL::UI, TOPIC::Go_2st, PHASE::PHASE_BEGIN, event, false))
	{
		CUIManager::GetInstance()->ForceUnpauseAll();
		CSceneManager::GetInstance()->Change_Scene(SCENETYPE::LOADING, LOADINGID::LOADING_STAGE2);
	}

	if (bus->Subscribe(CHANNEL::UI, TOPIC::Go_Boss, PHASE::PHASE_BEGIN, event, false))
	{
		CUIManager::GetInstance()->ForceUnpauseAll();
		CSceneManager::GetInstance()->Change_Scene(SCENETYPE::LOADING, LOADINGID::LOADING_BOSS);
	}
	if (bus->Subscribe(CHANNEL::UI, TOPIC::Go_Next, PHASE::PHASE_BEGIN, event, false))
	{
		CUIManager::GetInstance()->ForceUnpauseAll();
		switch (CSceneManager::GetInstance()->Get_CurrentSceneType())
		{
		case SCENETYPE::TUTORIAL:
			CSceneManager::GetInstance()->Change_Scene(SCENETYPE::LOADING, LOADINGID::LOADING_STAGE);
			break;
		case SCENETYPE::STAGE1:
			CSceneManager::GetInstance()->Change_Scene(SCENETYPE::LOADING, LOADINGID::LOADING_STAGE2);
			break;

		case SCENETYPE::STAGE2:
			CSceneManager::GetInstance()->Change_Scene(SCENETYPE::LOADING, LOADINGID::LOADING_BOSS);
			break;

		case SCENETYPE::BOSS2:
			CSceneManager::GetInstance()->Change_Scene(SCENETYPE::LOADING, LOADINGID::LOADING_BOSS2);
			break;
		}
	}
	if (bus->Subscribe(CHANNEL::UI, TOPIC::Stage_Restart, PHASE::PHASE_BEGIN, event, false))
	{
		CUIManager::GetInstance()->ForceUnpauseAll();
		switch (CSceneManager::GetInstance()->Get_CurrentSceneType())
		{
		case SCENETYPE::TUTORIAL:
			CSceneManager::GetInstance()->Change_Scene(SCENETYPE::LOADING, LOADINGID::LOADING_TUTORIALSTAGE);
			break;
		case SCENETYPE::STAGE1:
			CSceneManager::GetInstance()->Change_Scene(SCENETYPE::LOADING, LOADINGID::LOADING_STAGE);
			break;
		case SCENETYPE::BOSS:
			CSceneManager::GetInstance()->Change_Scene(SCENETYPE::LOADING, LOADINGID::LOADING_BOSS);
			break;

		case SCENETYPE::BOSS2:
			CSceneManager::GetInstance()->Change_Scene(SCENETYPE::LOADING, LOADINGID::LOADING_BOSS2);
			break;
		}
	}
}

void CUISystem::UI_Open_Close()
{
	EVENT event;

	if (CEventBus::GetInstance()->Subscribe(CHANNEL::UI, TOPIC::Stage_Clear, PHASE::PHASE_BEGIN, event, false))
	{
		CUIManager::GetInstance()->DeactivateUI(L"PlayerStatPanel_UI");
		CUIManager::GetInstance()->SetCleared(true);
	}
	if (CEventBus::GetInstance()->Subscribe(CHANNEL::UI, TOPIC::Open_Shop, PHASE::PHASE_BEGIN, event, false))
	{
		CUIManager::GetInstance()->ActivateUI(L"Shop_UI");
	}
	if (CEventBus::GetInstance()->Subscribe(CHANNEL::UI, TOPIC::ONHEALEDFULL, PHASE::PHASE_BEGIN, event, false))
	{
		CUIManager::GetInstance()->SetCategoryTag(L"UI_NoticeBanner", L"Over Heal");
		CUIManager::GetInstance()->ActivateUI(L"UI_NoticeBanner");
	}
	if (CEventBus::GetInstance()->Subscribe(CHANNEL::UI, TOPIC::Stage_Fail, PHASE::PHASE_BEGIN, event, false))
	{
		CUIManager::GetInstance()->ActivateUI(L"GameOver");
	}
	if (CEventBus::GetInstance()->Subscribe(CHANNEL::UI, TOPIC::Eat_Effect, PHASE::PHASE_BEGIN, event, false))
	{
		CUIManager::GetInstance()->ActivateUI(L"PlayerStatPanel_UI");
	}
	if (CEventBus::GetInstance()->Subscribe(CHANNEL::Combat, TOPIC::ONDAMAGED, PHASE::PHASE_BEGIN, event, false))
	{
		CUIManager::GetInstance()->SetCategoryTag(L"PlayerStatPanel_UI", L"Hit_Effect");
		if (event.targetHandle.m_eOriginType == ORIGIN_DYNAMIC)
			event.targetPtr = CObjectPoolingManager::GetInstance()->Resolve(event.targetHandle);

		if (event.targetPtr && event.targetPtr->Get_OBJTYPE() == OBJ_PLAYER)
		{
			CUIManager::GetInstance()->ActivateUI(L"PlayerStatPanel_UI");
		}
	}
	if (CEventBus::GetInstance()->Subscribe(CHANNEL::UI, TOPIC::Get_Weapon, PHASE::PHASE_BEGIN, event, false))
	{
		CSoundManager::GetInstance()->SoundPlay(L"get_new_weapon.wav", ITEM_SOUND, CSoundManager::GetInstance()->GetVol().sfx);
		CUIManager::GetInstance()->ActivateUI(L"WeaponGet_UI");
	}
	if (CEventBus::GetInstance()->Subscribe(CHANNEL::UI, TOPIC::Get_Effect, PHASE::PHASE_BEGIN, event, false))
	{
		CUIManager::GetInstance()->SetCategoryTag(L"PlayerStatPanel_UI", L"Get_Effect");
		if (event.sourceHandle.m_eOriginType == ORIGIN_DYNAMIC)
			event.sourcePtr = CObjectPoolingManager::GetInstance()->Resolve(event.sourceHandle);

		if (event.sourcePtr && event.sourcePtr->Get_OBJTYPE() == OBJ_PLAYER)
		{
			CUIManager::GetInstance()->ActivateUI(L"ItemGet_UI");
			CUIManager::GetInstance()->ActivateUI(L"PlayerStatPanel_UI");
		}
	}
	if (CEventBus::GetInstance()->Subscribe(CHANNEL::UI, TOPIC::BOSS_CLEAR, PHASE::PHASE_BEGIN, event, false))
	{
		CUIManager::GetInstance()->DeactivateAll();
		CUIManager::GetInstance()->ActivateUI(L"UI_FadeIn");
		CUIManager::GetInstance()->ActivateUI(L"StoryScript_UI");
		CUIManager::GetInstance()->SetCategoryTag(L"StoryScript_UI", L"Epilogue");
	}
}

void CUISystem::Interaction_UI()
{
	EVENT event;

	if (CEventBus::GetInstance()->Subscribe(CHANNEL::UI, TOPIC::INTERACTNOTICE, PHASE::PHASE_BEGIN, event, false))
	{
		CUIManager::GetInstance()->ActivateUI(L"UI_Interaction");
	}
	else
	{
		CUIManager::GetInstance()->DeactivateUI(L"UI_Interaction");
	}
}

void CUISystem::Free()
{

}