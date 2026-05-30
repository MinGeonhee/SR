#include "IWeapon_Goblin_FireBall.h"
#include "CEventbus.h"
#include "CTransform.h"
#include "CGameObject.h"
#include "CCombatSystem.h"
#include "CObjectPoolingManager.h"
#include "CBullet.h"
#include "CColliderBase.h"

void IWeapon_Goblin_FireBall::Initialize(CWeaponController* pWeapon)	// 
{
	if (!pWeapon)
		return;

	MUZZLE muzzle{ pWeapon->Get_MyOwner()->Get_Transform(), {0.f, 0.f, 0.f} }; // Offset을 X 0.3 / Z 0.3 만큼 준다.
	m_vMuzzleList.push_back(muzzle);

	m_fElapsedTime = 0.f;
	m_fFireRate = 3.f; // 무기 발사 속도 3초
	m_iMax_BulletCount = 100;	// 최대 불릿 카운트 100개
	m_iCurrent_BulletCount = 30;	// 처음에 보유중인 불릿 카운트 0개.

	m_bAmmoDepleted = false;
}

void IWeapon_Goblin_FireBall::Enter(CWeaponController* pWeapon)
{
	m_fElapsedTime = 0.f;
	m_fFireRate = 3.f; // 무기 발사 속도 3초
	m_iMax_BulletCount = 100;	// 최대 불릿 카운트 100개

	m_bAmmoDepleted = false;
}

void IWeapon_Goblin_FireBall::Update(CWeaponController* pWeapon, float fDeltaTime)
{
	__super::IWeapon_Update(fDeltaTime);	// 기본적으로 돌아가야하는 로직들.
}


void IWeapon_Goblin_FireBall::Exit(CWeaponController* pWeapon)
{

}

void IWeapon_Goblin_FireBall::Fire(CWeaponController* pWeapon)
{
	if (m_bCanFire)	// 쏠 수 있는 상태입니다
	{
		auto Owner = pWeapon->Get_MyOwner();
		auto pos = *(m_vMuzzleList[0].tTranfrom->GetInfoPos()) + m_vMuzzleList[0].tOffset;

		_vec3 BulletDir;
		MonsterBulletDirCalculator(pWeapon, BulletDir, pos);
		_vec3 BulletDirOffest = { 0.f,-0.087f,0.f };
		// 현재 내 무기의 위치와 마우스 위치를 구해야함.
		auto bullet = static_cast<CBullet*>(CObjectPoolingManager::GetInstance()->Get(OBJ_BULLET));

		if (!bullet) return;

		pos = pos + _vec3{ 0.f,0.3f,0.f } + BulletDir * 0.3f;
		bullet->Change_BulletState(BULLET_GOBLIN_FIREBALL);
		bullet->Set_BulletStatus(BULLET_STATUS_IDLE);
		bullet->Get_Transform()->Set_Pos(pos.x, pos.y, pos.z);
		bullet->Set_BulletDir(BulletDir + BulletDirOffest);
		bullet->Set_ObjectOwner(Owner);

		static_cast<CColliderBase*>(bullet->Get_Component(L"Com_Collider", ID_DYNAMIC))->Set_ColliderType(COLLIDER_SPHERE);
		static_cast<CTransform*>(bullet->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Scale(_vec3{ 0.2f, 0.2f, 0.2f });

		m_fElapsedTime = 0.f;
	}
}
