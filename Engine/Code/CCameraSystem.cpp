#include "CCameraSystem.h"
#include "CEventBus.h"
#include "CGameObject.h"
#include "CColliderBase.h"
#include "CObjectManager.h"
#include "CObjectPoolingManager.h"
#include "CSystemManager.h"
#include "CCameraManager.h"
#include "CCamera.h"
#include "CWeaponController.h"
#include "CTriggerEventManager.h"
#include "CInputManager.h"
#include "CUIManager.h"
#include "CTransform.h"
#include "CActionCamera_Helper.h"

IMPLEMENT_SINGLETON(CCameraSystem)

CCameraSystem::CCameraSystem()
{

}

CCameraSystem::~CCameraSystem()
{
	Free();
}

_int CCameraSystem::System_Update(const float& fTimeDelta)
{

	return 0;
}

void CCameraSystem::System_LateUpdate(const float& fTimeDelta)
{
	Boss_Room_Camera();
	DeathSequenceCam();
	IslandTourCam();
	Camera();
}

// EventBus에 Event가 생성되면 std::Function<void(const Event&)> 호출되게 만들까.
// -> 최적화 드럽게 안되면 CALLBACK 형식의 함수로 호출하자. GG
// 그냥 CALLBACK 형식 함수 호출이 더 나아보이는데.

void CCameraSystem::Camera()	// Camera
{
	_matrix view = CCameraManager::GetInstance()->Get_MainViewMatrix();	// 전 프레임의 카메라 뷰 행렬 가져오기.
	_matrix inv; D3DXMatrixInverse(&inv, nullptr, &view);
	_vec3   vEye = { inv._41, inv._42, inv._43 };

	auto bus = CEventBus::GetInstance();

	EVENT event;

	// 전투가 일어나서 누군가가 데미지를 입었다면, 카메라 쉐이킹.
	if(bus->Subscribe(CHANNEL::Combat, TOPIC::ONDAMAGED, PHASE::PHASE_BEGIN, event, false))
	{
		if (event.sourceHandle.m_eType == OBJ_PLAYER && static_cast<CWeaponController*>(event.sourcePtr->Get_Component(L"Com_Weapon", ID_DYNAMIC))->Get_WeaponType() == WEAPONSTATE::WEAPON_MACE)
		{
			auto camera = CCameraManager::GetInstance();
			static_cast<CCamera*>(
				camera->Get_Camera(L"Main_Camera")->
				Get_Component(L"Com_Camera", ID_DYNAMIC))->
				Set_CameraState(CAM_SHAKE, vEye, {}, 0.3f, 0.05f);
			CInputManager::GetInstance()->Set_KeyLock(true);
		}
	}

	// 플레이어가 벽을 떄린다던가. 바닥을 떄리면 카메라 쉐이킹.
	if(bus->Subscribe(CHANNEL::Player, TOPIC::COLLISION_ENTER, PHASE::PHASE_BEGIN, event, false))
	{
		auto camera = CCameraManager::GetInstance();
		static_cast<CCamera*>(
			camera->Get_Camera(L"Main_Camera")->
			Get_Component(L"Com_Camera", ID_DYNAMIC))->
			Set_CameraState(CAM_SHAKE, vEye, {}, 0.3f, 0.05f);
		CInputManager::GetInstance()->Set_KeyLock(true);
	}

	if (bus->Subscribe(CHANNEL::Boss, TOPIC::Boss_Gimmik, PHASE::PHASE_BEGIN, event, false))
	{
		auto camera = CCameraManager::GetInstance();
		static_cast<CCamera*>(
			camera->Get_Camera(L"Main_Camera")->
			Get_Component(L"Com_Camera", ID_DYNAMIC))->
			Set_CameraState(CAM_SHAKE, vEye, {}, 1.f, 0.2f);
		CInputManager::GetInstance()->Set_KeyLock(true);
	}
}

void CCameraSystem::Boss_Room_Camera()
{
	_matrix view = CCameraManager::GetInstance()->Get_MainViewMatrix();	// 전 프레임의 카메라 뷰 행렬 가져오기.
	_matrix inv; D3DXMatrixInverse(&inv, nullptr, &view);
	_vec3   vEye = { inv._41, inv._42, inv._43 };

	auto bus = CEventBus::GetInstance();
	auto camera = CCameraManager::GetInstance();
	auto objmng = CObjectManager::GetInstance();

	EVENT event;

	// 보스방 진입시 5초동안 카메라가 흔들림.
	if (bus->Subscribe(CHANNEL::Boss, TOPIC::DOOR_OPEN, PHASE::PHASE_BEGIN, event, false))
	{
		static_cast<CCamera*>(
			camera->Get_Camera(L"Main_Camera")->
			Get_Component(L"Com_Camera", ID_DYNAMIC))->
			Set_CameraState(CAM_SHAKE, vEye, {}, 5.f, 0.25f);
		CTriggerEventManager::GetInstance()->Set_BossDooropen(true);
		CInputManager::GetInstance()->Set_KeyLock(true);
	}

	if (bus->Subscribe(CHANNEL::Boss, TOPIC::DOOR_OPEN, PHASE::PHASE_UPDATE, event, false))
	{
		CUIManager::GetInstance()->DeactivateAll();
		CUIManager::GetInstance()->ActivateUI(L"UI_FadeIn");
		CUIManager::GetInstance()->ActivateUI(L"Dialogue_UI");
		CUIManager::GetInstance()->SetCategoryTag(L"Dialogue_UI", L"BossIntro_Zoom");
		static_cast<CCamera*>(
			camera->Get_Camera(L"Main_Camera")->
			Get_Component(L"Com_Camera", ID_DYNAMIC))->
			Set_CameraState(CAM_ZOOM, _vec3{18.5f, 2.5f, 42.5f}, _vec3{1.0f, 0.f, 0.f}, /*이동거리*/ 5.3f, /*스피드*/ 0.5f);
		CInputManager::GetInstance()->Set_KeyLock(true);
		CTriggerEventManager::GetInstance()->Set_BossZoomCutSceneActive(true);
		CObjectManager::GetInstance()->Get(OBJ_PLAYER)->Get_Transform()->Set_Pos(1.f, 1.f, 1.f);
	}

	if (bus->Subscribe(CHANNEL::Boss, TOPIC::DOOR_OPEN, PHASE::PHASE_END, event, false))
	{
		CUIManager::GetInstance()->ActivateUI(L"UI_FadeIn");	// 하얗게 번쩍이는걸로 교체할 것.
		CGameObject* boss = CObjectManager::GetInstance()->Get(OBJ_MONSTER_BOSS);	if (boss == NULL) return;
		CObjectManager::GetInstance()->Get(OBJ_MONSTER_BOSS)->Set_Active(true);
		CTransform* Trans = static_cast<CTransform*>(boss->Get_Component(L"Com_Transform", ID_DYNAMIC));	if (Trans == NULL) return;

		static_cast<CCamera*>(
			camera->Get_Camera(L"Main_Camera")->
			Get_Component(L"Com_Camera", ID_DYNAMIC))->
			Set_CameraState(CAM_BOSS_TARGET, vEye, *Trans->GetInfoPos());
		CInputManager::GetInstance()->Set_KeyLock(true);
		CTriggerEventManager::GetInstance()->Set_Boss_AppearSceneActive(true);
		objmng->ObjectBillboard_OnOff(OBJ_MONSTER_BOSS, true);
	}

	if (bus->Subscribe(CHANNEL::Boss, TOPIC::Boss_Appear, PHASE::PHASE_BEGIN, event, false))
	{
		static_cast<CCamera*>(
			camera->Get_Camera(L"Main_Camera")->
			Get_Component(L"Com_Camera", ID_DYNAMIC))->
			Set_CameraState(CAM_BOSSORBITAL, {}, {}, /*공전거리*/ 4.f,  /*스피드*/ 6.f);
		CInputManager::GetInstance()->Set_KeyLock(true);

		objmng->ObjectBillboard_OnOff(OBJ_MONSTER_BOSS, false);
	}

	if (bus->Subscribe(CHANNEL::Boss, TOPIC::Boss_Appear, PHASE::PHASE_END, event, false))
	{
		CUIManager::GetInstance()->ActivateUI(L"UI_FadeIn");	// 하얗게 번쩍이는걸로 교체할 것.
		CObjectManager::GetInstance()->Get(OBJ_BARRIER)->Set_Active(true);
		CObjectManager::GetInstance()->Get(OBJ_PLAYER)->Get_Transform()->Set_Pos(26.5f, 2.5f, 49.5f);
		CObjectManager::GetInstance()->Get(OBJ_PLAYER)->Get_Transform()->Reset_Angle();
		CObjectManager::GetInstance()->Get(OBJ_PLAYER)->Get_Transform()->Set_Angle(0.f, D3DXToRadian(180.f), 0.f);

		static_cast<CCamera*>(
			camera->Get_Camera(L"Main_Camera")->
			Get_Component(L"Com_Camera", ID_DYNAMIC))->
			Set_CameraState(CAM_FIRSTPERSON);
		CInputManager::GetInstance()->Set_KeyLock(false);

		objmng->ObjectBillboard_OnOff(OBJ_MONSTER_BOSS, true);
		CUIManager::GetInstance()->ActivateUI(L"PlayerTex");
		CUIManager::GetInstance()->ActivateUI(L"PlayerUI");
		CUIManager::GetInstance()->ActivateUI(L"Boss_UI");
		CUIManager::GetInstance()->ActivateUI(L"UI_Weapon");
		CUIManager::GetInstance()->ActivateUI(L"Map_UI");
	}

	if (bus->Subscribe(CHANNEL::Boss, TOPIC::Boss_Gimmik, PHASE::PHASE_UPDATE, event, false))
	{
		// 보스 베리어 기믹이 다 끝났다면 발송되는 이벤트 구독
		CUIManager::GetInstance()->DeactivateAll();
		CUIManager::GetInstance()->ActivateUI(L"UI_FadeIn");	// 하얗게 번쩍이는걸로 교체할 것.

		CGameObject* boss = CObjectManager::GetInstance()->Get(OBJ_MONSTER_BOSS);	if (boss == NULL) return;
		CTransform* Trans = static_cast<CTransform*>(boss->Get_Component(L"Com_Transform", ID_DYNAMIC));	if (Trans == NULL) return;

		static_cast<CCamera*>(
			camera->Get_Camera(L"Main_Camera")->
			Get_Component(L"Com_Camera", ID_DYNAMIC))->
			Set_CameraState(CAM_BOSS_TARGET, *Trans->GetInfoPos() + _vec3{7.f, 20.f, 0.f}, *Trans->GetInfoPos());
		CInputManager::GetInstance()->Set_KeyLock(true);
		objmng->ObjectBillboard_OnOff(OBJ_MONSTER_BOSS, false);

		CTriggerEventManager::GetInstance()->Set_Boss_GimmikSceneEnd(true);
		m_vSavePlayerPos = *(CObjectManager::GetInstance()->Get(OBJ_PLAYER)->Get_Transform()->GetInfoPos());
		CObjectManager::GetInstance()->Get(OBJ_PLAYER)->Get_Transform()->Set_Pos(1.f, 1.f, 1.f);
	}

	if (bus->Subscribe(CHANNEL::Boss, TOPIC::Boss_Gimmik, PHASE::PHASE_END, event, false))
	{
		// 보스 베리어 기믹이 다 끝났다면 발송되는 이벤트 구독
		CUIManager::GetInstance()->ActivateUI(L"PlayerTex");
		CUIManager::GetInstance()->ActivateUI(L"PlayerUI");
		CUIManager::GetInstance()->ActivateUI(L"Boss_UI");
		CUIManager::GetInstance()->ActivateUI(L"UI_Weapon");
		CUIManager::GetInstance()->ActivateUI(L"Map_UI");
		CUIManager::GetInstance()->ActivateUI(L"UI_FadeIn");

		CObjectManager::GetInstance()->Get(OBJ_PLAYER)->Get_Transform()->Set_Pos(m_vSavePlayerPos.x, m_vSavePlayerPos.y, m_vSavePlayerPos.z);

		static_cast<CCamera*>(
			camera->Get_Camera(L"Main_Camera")->
			Get_Component(L"Com_Camera", ID_DYNAMIC))->
			Set_CameraState(CAM_FIRSTPERSON);

		CInputManager::GetInstance()->Set_KeyLock(false);
		objmng->ObjectBillboard_OnOff(OBJ_MONSTER_BOSS, true);
	}

	if (bus->Subscribe(CHANNEL::Boss, TOPIC::Boss1Phaseto2, PHASE::PHASE_BEGIN, event, false))
	{
		// 보스 베리어 기믹이 다 끝났다면 발송되는 이벤트 구독
		CUIManager::GetInstance()->DeactivateAll();
		CTriggerEventManager::GetInstance()->Set_Boss_1Phaseto2CutsceneBegin(true);

		CInputManager::GetInstance()->Set_KeyLock(true);
	}

	if (bus->Subscribe(CHANNEL::Boss, TOPIC::Boss1Phaseto2, PHASE::PHASE_UPDATE, event, false))
	{
		// 보스 베리어 기믹이 다 끝났다면 발송되는 이벤트 구독
		CUIManager::GetInstance()->ActivateUI(L"UI_FadeIn");
		CUIManager::GetInstance()->SetCategoryTag(L"Dialogue_UI", L"BossPhase2_Intro");
		CGameObject* boss = CObjectManager::GetInstance()->Get(OBJ_MONSTER_BOSS);	if (boss == NULL) return;
		CObjectManager::GetInstance()->Get(OBJ_MONSTER_BOSS)->Set_Active(true);
		CTransform* Trans = static_cast<CTransform*>(boss->Get_Component(L"Com_Transform", ID_DYNAMIC));	if (Trans == NULL) return;

		static_cast<CCamera*>(
			camera->Get_Camera(L"Main_Camera")->
			Get_Component(L"Com_Camera", ID_DYNAMIC))->
			Set_CameraState(CAM_ZOOM, _vec3{ 21.5f, 2.5f, 42.5f }, _vec3{ 1.0f, 0.f, 0.f }, /*이동거리*/ 3.3f, /*스피드*/ 0.05f);

		m_vSavePlayerPos = *(CObjectManager::GetInstance()->Get(OBJ_PLAYER)->Get_Transform()->GetInfoPos());
		CObjectManager::GetInstance()->Get(OBJ_PLAYER)->Get_Transform()->Set_Pos(1.f, 1.f, 1.f);

		CTriggerEventManager::GetInstance()->Set_Boss_1Phaseto2CutsceneUpdate(true);
		CInputManager::GetInstance()->Set_KeyLock(true);
	}

	if (bus->Subscribe(CHANNEL::Boss, TOPIC::Boss1Phaseto2, PHASE::PHASE_END, event, false))
	{
		CUIManager::GetInstance()->SetCategoryTag(L"Dialogue_UI", L"BossPhase2_IntroZoom");
		CGameObject* boss = CObjectManager::GetInstance()->Get(OBJ_MONSTER_BOSS);	if (boss == NULL) return;
		CObjectManager::GetInstance()->Get(OBJ_MONSTER_BOSS)->Set_Active(true);
		CTransform* Trans = static_cast<CTransform*>(boss->Get_Component(L"Com_Transform", ID_DYNAMIC));	if (Trans == NULL) return;

		static_cast<CCamera*>(
			camera->Get_Camera(L"Main_Camera")->
			Get_Component(L"Com_Camera", ID_DYNAMIC))->
			Set_CameraState(CAM_BOSS_TARGET, *Trans->GetInfoPos() + _vec3{0.5f, 1.8f, 0.f}, * Trans->GetInfoPos()+_vec3 { 0.f, 0.8f, 0.f });

		CInputManager::GetInstance()->Set_KeyLock(true);
		CTriggerEventManager::GetInstance()->Set_Boss_2PhaseBegin(true);
		objmng->ObjectBillboard_OnOff(OBJ_MONSTER_BOSS, true);
	}

	if (bus->Subscribe(CHANNEL::Boss, TOPIC::Boss2Phase, PHASE::PHASE_BEGIN, event, false))
	{
		CUIManager::GetInstance()->DeactivateUI(L"Dialogue_UI");

		static_cast<CCamera*>(
			camera->Get_Camera(L"Main_Camera")->
			Get_Component(L"Com_Camera", ID_DYNAMIC))->
			Set_CameraState(CAM_FIRSTPERSON);
		CInputManager::GetInstance()->Set_KeyLock(false);

		//CInputManager::GetInstance()->Set_KeyLock(true);
		objmng->ObjectBillboard_OnOff(OBJ_MONSTER_BOSS, true);
	}



}

void CCameraSystem::DeathSequenceCam()
{
	_matrix view = CCameraManager::GetInstance()->Get_MainViewMatrix();	// 전 프레임의 카메라 뷰 행렬 가져오기.
	_matrix inv; D3DXMatrixInverse(&inv, nullptr, &view);
	_vec3   vEye = { inv._41, inv._42, inv._43 };

	auto bus = CEventBus::GetInstance();
	auto camera = CCameraManager::GetInstance();
	auto objmng = CObjectManager::GetInstance();

	EVENT event;

	if (bus->Subscribe(CHANNEL::Camera, TOPIC::DeathSequenceCAM, PHASE::PHASE_BEGIN, event, false))
	{
		CUIManager::GetInstance()->DeactivateUI(L"PlayerTex");

		static_cast<CCamera*>(
			camera->Get_Camera(L"Main_Camera")->
			Get_Component(L"Com_Camera", ID_DYNAMIC))->
			Set_CameraState(CAM_FIRSTPERSON);
		CInputManager::GetInstance()->Set_KeyLock(true);
	}

	if (bus->Subscribe(CHANNEL::Camera, TOPIC::DeathSequenceCAM, PHASE::PHASE_UPDATE, event, false))
	{
		_vec3 DeathObjectPos = *(event.sourcePtr->Get_Transform()->GetInfoPos());
		_vec3 objectDir = vEye - DeathObjectPos;

		_vec3 EyeOffset = {};
		_vec3 LookOffset = {};
		float DirScale = 0.f;

		switch (event.sourceHandle.m_eType)
		{
		case OBJTYPE::OBJ_MONSTER_GOBLIN_FIREBALL:
		case OBJTYPE::OBJ_MONSTER_GOBLIN_HAMMER:
		case OBJTYPE::OBJ_MONSTER_ORGE_AXE:
		case OBJTYPE::OBJ_MONSTER_RAT:
		case OBJTYPE::OBJ_MONSTER_SKELETON_SWORD:
		case OBJTYPE::OBJ_MONSTER_WIZARD:
			EyeOffset = { 0.f, +0.3f, 0.f };
			LookOffset = { 0.f, -0.5f, 0.f };
			DirScale = 1.f;
			break;

		case OBJTYPE::OBJ_MONSTER_BOSS:
		case OBJTYPE::OBJ_MONSTER_BOSS_CLONE:
			EyeOffset = { 0.f, +1.2f, 0.f };
			LookOffset = { 0.f, 0.3f, 0.f };
			DirScale = 2.f;
			break;
		}

		D3DXVec3Normalize(&objectDir, &objectDir);

		static_cast<CCamera*>(
			camera->Get_Camera(L"Main_Camera")->
			Get_Component(L"Com_Camera", ID_DYNAMIC))->
			Set_CameraState(CAM_BOSS_TARGET, DeathObjectPos + objectDir * DirScale +EyeOffset, DeathObjectPos + LookOffset);

		CInputManager::GetInstance()->Set_KeyLock(true);
	}

	if (bus->Subscribe(CHANNEL::Camera, TOPIC::DeathSequenceCAM, PHASE::PHASE_END, event, false))
	{
		CUIManager::GetInstance()->ActivateUI(L"PlayerTex");

		static_cast<CCamera*>(
			camera->Get_Camera(L"Main_Camera")->
			Get_Component(L"Com_Camera", ID_DYNAMIC))->
			Set_CameraState(CAM_FIRSTPERSON);
		CInputManager::GetInstance()->Set_KeyLock(false);
	}

}

void CCameraSystem::IslandTourCam()
{
	_matrix view = CCameraManager::GetInstance()->Get_MainViewMatrix();	// 전 프레임의 카메라 뷰 행렬 가져오기.
	_matrix inv; D3DXMatrixInverse(&inv, nullptr, &view);
	_vec3   vEye = { inv._41, inv._42, inv._43 };

	auto bus = CEventBus::GetInstance();
	auto camera = CCameraManager::GetInstance();
	auto objmng = CObjectManager::GetInstance();

	EVENT event;

	 //오비탈 카메라 
	if (bus->Subscribe(CHANNEL::Camera, TOPIC::IslandTourCam, PHASE::PHASE_BEGIN, event, false))
	{
		_vec3 targetPos = { 48.f, 10.f, 49.5f };

		CUIManager::GetInstance()->ActivateUI(L"UI_FadeIn");
		static_cast<CCamera*>(
			camera->Get_Camera(L"Main_Camera")->
			Get_Component(L"Com_Camera", ID_DYNAMIC))->
			Set_CameraState(CAM_ORBITAL, targetPos, {}, /*Distance*/10.f, /*Yaw Angle*/0.f);
		CInputManager::GetInstance()->Set_KeyLock(true);	// 키 잠금
	}

	if (bus->Subscribe(CHANNEL::Camera, TOPIC::IslandTourCam, PHASE::PHASE_UPDATE, event, false))
	{
		_vec3 targetPos = { 48.f, 10.f, 49.5f };

		static_cast<CCamera*>(
			camera->Get_Camera(L"Main_Camera")->
			Get_Component(L"Com_Camera", ID_DYNAMIC))->
			Set_CameraState(CAM_ZOOMOUT, targetPos, {}, /*Distance*/10.f, /*Speed*/1.f);
		CInputManager::GetInstance()->Set_KeyLock(true);	// 키 잠금
	}

	if (bus->Subscribe(CHANNEL::Camera, TOPIC::IslandTourCam, PHASE::PHASE_END, event, false))
	{
		static_cast<CCamera*>(
			camera->Get_Camera(L"Main_Camera")->
			Get_Component(L"Com_Camera", ID_DYNAMIC))->
			Set_CameraState(CAM_FIRSTPERSON);
		CInputManager::GetInstance()->Set_KeyLock(false);	// 키 잠금 해제
	}

}

void CCameraSystem::Free()
{

}