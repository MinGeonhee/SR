#include "IWeapon_Mace.h"
#include "CEventbus.h"
#include "CTransform.h"
#include "CGameObject.h"
#include "CCombatSystem.h"
#include "CObjectPoolingManager.h"
#include "CBullet.h"
#include "CColliderBase.h"
#include "CCollisionManager.h"

void IWeapon_Mace::Initialize(CWeaponController* pWeapon)	// 
{
	if (!pWeapon)
		return;

	MUZZLE muzzle{ pWeapon->Get_MyOwner()->Get_Transform(), {0.f, 0.f, 0.f} }; // Offset을 X 0.3 / Z 0.3 만큼 준다.
	m_vMuzzleList.push_back(muzzle);

	m_fElapsedTime = 0.f;
	m_fFireRate = 0.5f; // 무기 발사 속도 0.5초로 고정
	m_FChargeFireRate = 1.f; // 무기 차지 속도 1초로 고정
	m_iMax_EnergyBulletCount = 100;	// 최대 불릿 카운트 100개
	m_iCurrent_BulletCount = 1;	// 기본 무기 그냥 이거 1개 주자
	m_iCurrent_EnergyBulletCount = 0;	// 처음에 보유중인 불릿 카운트 0개.

	m_bAmmoDepleted = false;
}

void IWeapon_Mace::Enter(CWeaponController* pWeapon)
{
	m_fElapsedTime = 0.f;
	m_fFireRate = 0.5f; // 무기 발사 속도 0.5초로 고정
	m_FChargeFireRate = 1.f; // 무기 차지 속도 1초로 고정
	m_iMax_EnergyBulletCount = 100;	// 최대 불릿 카운트 100개
	m_iCurrent_EnergyBulletCount = 0;

	m_bAmmoDepleted = false;
}

void IWeapon_Mace::Update(CWeaponController* pWeapon, float fDeltaTime)
{
	__super::IWeapon_Update(fDeltaTime);	// 기본적으로 돌아가야하는 로직들.
}


void IWeapon_Mace::Exit(CWeaponController* pWeapon)
{

}

void IWeapon_Mace::Fire(CWeaponController* pWeapon)
{
	if (!m_bCanFire) return;

	CSoundManager::GetInstance()->SoundPlay(
		L"mace_fire.wav", BULLET_SOUND, CSoundManager::GetInstance()->GetVol().sfx);

	m_bCanFire = false;
	m_fElapsedTime = 0.f;
}

void IWeapon_Mace::ChargeFire(CWeaponController* pWeapon)
{
	//if (m_iCurrent_EnergyBulletCount <= 0) return;
	
	if (m_bCanChargeFire)
	{
		auto Owner = pWeapon->Get_MyOwner();
		auto pos = *(m_vMuzzleList[0].tTranfrom->GetInfoPos()) + m_vMuzzleList[0].tOffset;

		_vec3 BulletDir;
		CalculateFireDirection(pos, BulletDir);
		// 현재 내 무기의 위치와 마우스 위치를 구해야함.
		auto bullet = static_cast<CBullet*>(CObjectPoolingManager::GetInstance()->Get(OBJ_BULLET));

		if (!bullet) return;

		bullet->Get_Transform()->Set_Pos(pos.x, pos.y, pos.z);
		bullet->Set_BulletDir(BulletDir);
		bullet->Set_ObjectOwner(Owner);

		bullet->Change_BulletState(BULLET_PLAYER_CHARGEEMACE);
		static_cast<CColliderBase*>(bullet->Get_Component(L"Com_Collider", ID_DYNAMIC))->Set_ColliderType(COLLIDER_SPHERE);
		static_cast<CTransform*>(bullet->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
		CSoundManager::GetInstance()->SoundPlay(
			L"mace_fire.wav", BULLET_SOUND, CSoundManager::GetInstance()->GetVol().sfx);

		Dicrease_EnergyBullet(5);
	}
}