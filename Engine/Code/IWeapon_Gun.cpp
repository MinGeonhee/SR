#include "IWeapon_Gun.h"
#include "CEventbus.h"
#include "CTransform.h"
#include "CGameObject.h"
#include "CCombatSystem.h"
#include "CObjectPoolingManager.h"
#include "CBullet.h"
#include "CColliderBase.h"

void IWeapon_Gun::Initialize(CWeaponController* pWeapon)	// 
{
	if (!pWeapon)
		return;

	MUZZLE muzzle{ pWeapon->Get_MyOwner()->Get_Transform(), {0.3f, -0.2f, 0.3f} }; // Offset을 X 0.3 / Z 0.3 만큼 준다.
	m_vMuzzleList.push_back(muzzle);

	m_fElapsedTime = 0.f;
	m_fFireRate = 1.5f; // 무기 발사 속도 0.5초로 고정
	m_iMax_BulletCount = 100;	// 최대 불릿 카운트 100개
	m_iCurrent_BulletCount = 30;	// 처음에 보유중인 불릿 카운트 0개.

	m_bAmmoDepleted = false;

	
}

void IWeapon_Gun::Enter(CWeaponController* pWeapon)
{
	m_fElapsedTime = 0.f;
	m_fFireRate = 1.5f; // 무기 발사 속도 0.5초로 고정
	m_iMax_BulletCount = 100;	// 최대 불릿 카운트 100개

	m_bAmmoDepleted = false;
}

void IWeapon_Gun::Update(CWeaponController* pWeapon, float fDeltaTime)
{
	__super::IWeapon_Update(fDeltaTime);	// 기본적으로 돌아가야하는 로직들.
}


void IWeapon_Gun::Exit(CWeaponController* pWeapon)
{

}

void IWeapon_Gun::Fire(CWeaponController* pWeapon)
{
	if (m_iCurrent_BulletCount <= 0)		// 메시지와 함께 무기 강제 교체
	{
		Publish_AmmoDepletedEvent();
		return;
	}

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

		_vec3 vNormDir;
		D3DXVec3Normalize(&vNormDir, &BulletDir);
		Fire_InitSmokeParticle(pos + vNormDir * 0.3f);
		bullet->Set_ObjectOwner(Owner);
		bullet->Get_Transform()->Set_Pos(pos.x, pos.y, pos.z);
		bullet->Set_BulletDir(BulletDir);
		bullet->Set_BulletStatus(BULLET_STATUS_IDLE);
		bullet->Change_BulletState(BULLET_PLAYER_GUN);

		static_cast<CColliderBase*>(bullet->Get_Component(L"Com_Collider", ID_DYNAMIC))->Set_ColliderType(COLLIDER_SPHERE);
		static_cast<CTransform*>(bullet->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Scale(_vec3{ 0.1f, 0.1f, 0.1f });
		// 두번째 총알
		auto bullet1 = static_cast<CBullet*>(CObjectPoolingManager::GetInstance()->Get(OBJ_BULLET));

		if (!bullet1) return;

		_matrix mat;
		_vec3	vUp{ 0,1,0 };
		D3DXMatrixRotationAxis(&mat, &vUp, D3DXToRadian(1.f));
		D3DXVec3TransformNormal(&BulletDir, &BulletDir, &mat);		// y축으로 1도 돌아간만큼 방향 설정

		bullet1->Get_Transform()->Set_Pos(pos.x, pos.y, pos.z);
		bullet1->Set_BulletDir(BulletDir);
		bullet1->Set_ObjectOwner(Owner);
		bullet1->Set_BulletStatus(BULLET_STATUS_IDLE);
		bullet1->Change_BulletState(BULLET_PLAYER_GUN);

		static_cast<CColliderBase*>(bullet1->Get_Component(L"Com_Collider", ID_DYNAMIC))->Set_ColliderType(COLLIDER_SPHERE);
		static_cast<CTransform*>(bullet1->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Scale(_vec3{ 0.1f, 0.1f, 0.1f });

		// 세번째 총알

		auto bullet2 = static_cast<CBullet*>(CObjectPoolingManager::GetInstance()->Get(OBJ_BULLET));

		if (!bullet2) return;

		D3DXMatrixRotationAxis(&mat, &vUp, D3DXToRadian(-2.f));
		D3DXVec3TransformNormal(&BulletDir, &BulletDir, &mat);		// y축으로 1도 돌아간만큼 방향 설정

		bullet2->Get_Transform()->Set_Pos(pos.x, pos.y, pos.z);
		bullet2->Set_BulletDir(BulletDir);
		bullet2->Set_ObjectOwner(Owner);
		bullet2->Set_BulletStatus(BULLET_STATUS_IDLE);
		bullet2->Change_BulletState(BULLET_PLAYER_GUN);

		static_cast<CColliderBase*>(bullet2->Get_Component(L"Com_Collider", ID_DYNAMIC))->Set_ColliderType(COLLIDER_SPHERE);
		static_cast<CTransform*>(bullet2->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Scale(_vec3{ 0.1f, 0.1f, 0.1f });

		m_fElapsedTime = 0.f;
		CSoundManager::GetInstance()->SoundPlay(L"gun_fire.wav", BULLET_SOUND, CSoundManager::GetInstance()->GetVol().sfx);

		// 다 쐈다면 현재 총알 감소
		Dicrease_Bullet(1);
	}
}

void IWeapon_Gun::Overdrive_Fire(CWeaponController* pWeapon)
{
	if (m_iCurrent_BulletCount <= 0)		// 메시지와 함께 무기 강제 교체
	{
		Publish_AmmoDepletedEvent();
		return;
	}

	if (m_bCanOverDrive && m_iWeaponLevel >=2)
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
		bullet->Set_ObjectOwner(Owner);

		bullet->Change_BulletState(BULLET_PLAYER_GUNSKILL);
		bullet->Set_BulletStatus(BULLET_STATUS_IDLE);
		static_cast<CColliderBase*>(bullet->Get_Component(L"Com_Collider", ID_DYNAMIC))->Set_ColliderType(COLLIDER_SPHERE);
		static_cast<CTransform*>(bullet->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Scale(_vec3{ 1.f, 1.f, 1.f });

		Dicrease_Bullet(1);
	}
}

void IWeapon_Gun::Fire_InitSmokeParticle(_vec3 _vPos)
{
	CParticle* pParticle = static_cast<CParticle*>(CObjectPoolingManager::GetInstance()->Get(OBJ_PARTICLE));

	pParticle->Set_Particle_Size_Constant(0.2f);
	pParticle->Make_Random_Position_XYZ({ 0.05f,0.05f,0.05f }, _vPos);
	pParticle->Set_ParticleType(PARTICLE_TYPE_INIT_SMOKE);
	pParticle->Set_IsCollisionOn(false);
	pParticle->Set_Collider_Type(COLLIDER_NONE);
	pParticle->Set_ParticleStatus(PARTICLE_STATUS_ACTIVE);
	pParticle->Set_Animation();
}
