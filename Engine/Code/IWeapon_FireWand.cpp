#include "IWeapon_FireWand.h"
#include "CEventbus.h"
#include "CTransform.h"
#include "CGameObject.h"
#include "CCombatSystem.h"
#include "CObjectPoolingManager.h"
#include "CBullet.h"
#include "CColliderBase.h"

void IWeapon_FireWand::Initialize(CWeaponController* pWeapon)	// 
{
	if (!pWeapon)
		return;

	MUZZLE muzzle{ pWeapon->Get_MyOwner()->Get_Transform(), {0.3f, -0.4f, 1.0f} }; // Offset을 X 0.3 / Z 0.3 만큼 준다.
	m_vMuzzleList.push_back(muzzle);

	m_fElapsedTime = 0.f;
	m_fFireRate = 0.2f; // 무기 발사 속도 0.5초로 고정
	m_FChargeFireRate = 0.5f;

	m_iMax_BulletCount = 100;	// 최대 불릿 카운트 100개
	m_iCurrent_BulletCount = 1;	// 처음에 보유중인 불릿 카운트 0개.

	m_bAmmoDepleted = false;
}

void IWeapon_FireWand::Enter(CWeaponController* pWeapon)
{
	m_fElapsedTime = 0.f;
	m_fFireRate = 0.2f; // 무기 발사 속도 0.5초로 고정
	m_FChargeFireRate = 0.5f;

	m_iMax_BulletCount = 100;	// 최대 불릿 카운트 100개

	m_bAmmoDepleted = false;
}

void IWeapon_FireWand::Update(CWeaponController* pWeapon, float fDeltaTime)
{
	__super::IWeapon_Update(fDeltaTime);	// 기본적으로 돌아가야하는 로직들.
}


void IWeapon_FireWand::Exit(CWeaponController* pWeapon)
{

}

void IWeapon_FireWand::Fire(CWeaponController* pWeapon)
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

		if (!bullet) return;

		bullet->Get_Transform()->Set_Pos(pos.x, pos.y, pos.z);
		bullet->Set_BulletDir(BulletDir);
		bullet->Change_BulletState(BULLET_PLAYER_EMBER);
		bullet->Set_BulletStatus(BULLET_STATUS_IDLE);
		bullet->Set_ObjectOwner(Owner);

		static_cast<CColliderBase*>(bullet->Get_Component(L"Com_Collider", ID_DYNAMIC))->Set_ColliderType(COLLIDER_SPHERE);
		static_cast<CTransform*>(bullet->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Scale(_vec3{ 0.01f, 0.01f, 0.01f });
		CSoundManager::GetInstance()->SoundPlay(L"wand_fire.wav", BULLET_SOUND, CSoundManager::GetInstance()->GetVol().sfx);

		m_fElapsedTime = 0.f;
	}
}

void IWeapon_FireWand::ChargeFire(CWeaponController* pWeapon)
{
	// 아이스 스태프의 특수 스킬은 Combatsystem안에서 얼어있는 아이가 있다고 판단 되면 true를 뱉어냄.
	// true 일때만 사용 가능하며 사용하면 얼어있는 몬스터들 즉사.

	if (m_bCanChargeFire && m_iWeaponLevel >=2)
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

		if (!bullet) return;

		bullet->Get_Transform()->Set_Pos(pos.x, pos.y, pos.z);
		bullet->Set_BulletDir(BulletDir);
		bullet->Change_BulletState(BULLET_PLAYER_EMBER);
		bullet->Set_BulletStatus(BULLET_STATUS_IDLE);
		bullet->Set_ObjectOwner(Owner);

		static_cast<CColliderBase*>(bullet->Get_Component(L"Com_Collider", ID_DYNAMIC))->Set_ColliderType(COLLIDER_SPHERE);
		static_cast<CTransform*>(bullet->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Scale(_vec3{ 0.01f, 0.01f, 0.01f });
		CSoundManager::GetInstance()->SoundPlay(L"wand_fire.wav", BULLET_SOUND, CSoundManager::GetInstance()->GetVol().sfx);

		// 두번째 총알
		auto bullet1 = static_cast<CBullet*>(CObjectPoolingManager::GetInstance()->Get(OBJ_BULLET));

		if (!bullet1) return;

		_matrix mat;
		_vec3	vUp{ 0,1,0 };
		D3DXMatrixRotationAxis(&mat, &vUp, D3DXToRadian(5.f));
		D3DXVec3TransformNormal(&BulletDir, &BulletDir, &mat);		// y축으로 1도 돌아간만큼 방향 설정

		bullet1->Get_Transform()->Set_Pos(pos.x, pos.y, pos.z);
		bullet1->Set_BulletDir(BulletDir);
		bullet1->Change_BulletState(BULLET_PLAYER_EMBER);
		bullet1->Set_BulletStatus(BULLET_STATUS_IDLE);
		bullet1->Set_ObjectOwner(Owner);

		static_cast<CColliderBase*>(bullet1->Get_Component(L"Com_Collider", ID_DYNAMIC))->Set_ColliderType(COLLIDER_SPHERE);
		static_cast<CTransform*>(bullet1->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Scale(_vec3{ 0.01f, 0.01f, 0.01f });

		// 세번째 총알

		auto bullet2 = static_cast<CBullet*>(CObjectPoolingManager::GetInstance()->Get(OBJ_BULLET));

		if (!bullet2) return;

		D3DXMatrixRotationAxis(&mat, &vUp, D3DXToRadian(-10.f));
		D3DXVec3TransformNormal(&BulletDir, &BulletDir, &mat);		// y축으로 1도 돌아간만큼 방향 설정

		bullet2->Get_Transform()->Set_Pos(pos.x, pos.y, pos.z);
		bullet2->Set_BulletDir(BulletDir);
		bullet2->Change_BulletState(BULLET_PLAYER_EMBER);
		bullet2->Set_BulletStatus(BULLET_STATUS_IDLE);
		bullet2->Set_ObjectOwner(Owner);

		static_cast<CColliderBase*>(bullet2->Get_Component(L"Com_Collider", ID_DYNAMIC))->Set_ColliderType(COLLIDER_SPHERE);
		static_cast<CTransform*>(bullet2->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Scale(_vec3{ 0.01f, 0.01f, 0.01f });

		m_fElapsedTime = 0.f;
		m_fChargeElapsedTime = 0.f;
	}
}