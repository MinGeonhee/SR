#include "IWeapon_IceWand.h"
#include "CEventbus.h"
#include "CTransform.h"
#include "CGameObject.h"
#include "CCombatSystem.h"
#include "CObjectPoolingManager.h"
#include "CBullet.h"
#include "CColliderBase.h"

void IWeapon_IceWand::Initialize(CWeaponController* pWeapon)	// 
{
	if (!pWeapon)
		return;

	MUZZLE muzzle{ pWeapon->Get_MyOwner()->Get_Transform(), {0.f, -0.5f, 0.5f} }; // Offset을 X 0.3 / Z 0.3 만큼 준다.
	m_vMuzzleList.push_back(muzzle);

	m_fElapsedTime = 0.f; 
	m_fFireRate = 0.1f; // 무기 발사 속도 0.5초로 고정
	m_iMax_BulletCount = 100;	// 최대 불릿 카운트 100개
	m_iCurrent_BulletCount = 30;	// 처음에 보유중인 불릿 카운트 0개.

	m_bAmmoDepleted = false;
}

void IWeapon_IceWand::Enter(CWeaponController* pWeapon)
{
	m_fElapsedTime = 0.f;
	m_fFireRate = 0.1f; // 무기 발사 속도 0.5초로 고정
	m_iMax_BulletCount = 100;	// 최대 불릿 카운트 100개

	m_bAmmoDepleted = false;
}

void IWeapon_IceWand::Update(CWeaponController* pWeapon, float fDeltaTime)
{
	__super::IWeapon_Update(fDeltaTime);	// 기본적으로 돌아가야하는 로직들.
}


void IWeapon_IceWand::Exit(CWeaponController* pWeapon)
{

}

void IWeapon_IceWand::Fire(CWeaponController* pWeapon)
{
	if (m_bCanFire)	// 쏠 수 있는 상태입니다
	{
		auto Owner = pWeapon->Get_MyOwner();

		_vec3 XAxisrotate, ZAxisrotate, YAxisrotate;
		m_vMuzzleList[0].tTranfrom->Get_Info(INFO_LOOK, &ZAxisrotate);
		m_vMuzzleList[0].tTranfrom->Get_Info(INFO_RIGHT, &XAxisrotate);
		m_vMuzzleList[0].tTranfrom->Get_Info(INFO_UP, &YAxisrotate);
		D3DXVec3Normalize(&ZAxisrotate, &ZAxisrotate);
		D3DXVec3Normalize(&XAxisrotate, &XAxisrotate);
		D3DXVec3Normalize(&YAxisrotate, &YAxisrotate);

		auto pos = *(m_vMuzzleList[0].tTranfrom->GetInfoPos()) +
			m_vMuzzleList[0].tOffset.x * XAxisrotate
			+ m_vMuzzleList[0].tOffset.y * YAxisrotate
			+ m_vMuzzleList[0].tOffset.z * ZAxisrotate;

		_vec3 BulletDir;
		CalculateFireDirection(pos, BulletDir);
		// 현재 내 무기의 위치와 마우스 위치를 구해야함.
		auto bullet = static_cast<CBullet*>(CObjectPoolingManager::GetInstance()->Get(OBJ_BULLET));
		
		bullet->Set_ObjectOwner(Owner);
		bullet->Get_Transform()->Set_Pos(pos.x, pos.y, pos.z);
		bullet->Set_BulletDir(BulletDir);
		bullet->Set_BulletStatus(BULLET_STATUS_IDLE);
		bullet->Change_BulletState(BULLET_PLAYER_ICE);
		
		static_cast<CColliderBase*>(bullet->Get_Component(L"Com_Collider", ID_DYNAMIC))->Set_ColliderType(COLLIDER_SPHERE);
		static_cast<CTransform*>(bullet->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Scale(_vec3{ 0.5f, 0.5f, 0.5f });
		CSoundManager::GetInstance()->SoundPlay(L"ice_fire.wav", BULLET_SOUND, CSoundManager::GetInstance()->GetVol().sfx);

		m_fElapsedTime = 0.f;

		Dicrease_Bullet(1);
	}
}

void IWeapon_IceWand::Overdrive_Fire(CWeaponController* pWeapon)
{
	// 아이스 스태프의 특수 스킬은 Combatsystem안에서 얼어있는 아이가 있다고 판단 되면 true를 뱉어냄.
	// true 일때만 사용 가능하며 사용하면 얼어있는 몬스터들 즉사.

	if (!m_bCanOverDrive && m_iWeaponLevel <= 2)
		return;

	if (CCombatSystem::GetInstance()->HasFrozenObject()) // 만약에 얼어있는 아이가 있다면.
	{
		EVENT event
		{
			event.phase = PHASE::PHASE_UPDATE,
			event.topic = TOPIC::FROZEN,
			event.channel = CHANNEL::Combat,
			nullptr,
			nullptr,
			{},
			{}
		};

		CEventBus::GetInstance()->publish(event);
	}

	Set_OverideFire(false);	// 마우스 우클릭을 떼면 다시 true로 설정.

	Dicrease_Bullet(1);
}