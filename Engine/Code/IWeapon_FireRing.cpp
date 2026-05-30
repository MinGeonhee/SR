#include "IWeapon_FireRing.h"
#include "CEventbus.h"
#include "CTransform.h"
#include "CGameObject.h"
#include "CCombatSystem.h"
#include "CObjectPoolingManager.h"
#include "CBullet.h"
#include "CColliderBase.h"
#include "CCollisionManager.h"
#include "CGraphicDev.h"
#include "CCameraManager.h"
#include "CInputManager.h"

void IWeapon_FireRing::Initialize(CWeaponController* pWeapon)	// 
{
	if (!pWeapon)
		return;

	MUZZLE muzzle{ pWeapon->Get_MyOwner()->Get_Transform(), {+0.5f, -0.5f, 0.5f} }; // 오른손
	MUZZLE muzzle1{ pWeapon->Get_MyOwner()->Get_Transform(), {-0.5f, -0.5f, 0.5f} }; // 왼손

	m_vMuzzleList.push_back(muzzle);
	m_vMuzzleList.push_back(muzzle1);

	m_fElapsedTime = 0.f;
	m_fFireRate = 0.5f; // 무기 발사 속도 0.5초로 고정
	m_FChargeFireRate = 1.f; // 무기 차지 속도 1초로 고정
	m_iMax_BulletCount = 200;	// 최대 불릿 카운트 100개
	m_iCurrent_BulletCount = 0;	// 처음에 보유중인 불릿 카운트 0개.

	m_bAmmoDepleted = false;
}

void IWeapon_FireRing::Enter(CWeaponController* pWeapon)
{
	m_fElapsedTime = 0.f;
	m_fFireRate = 0.5f; // 무기 발사 속도 0.5초로 고정
	m_FChargeFireRate = 1.f; // 무기 차지 속도 1초로 고정
	m_iMax_BulletCount = 200;	// 최대 불릿 카운트 100개
	//m_iCurrent_BulletCount = 0;	// 처음에 보유중인 불릿 카운트 0개.

	m_bAmmoDepleted = false;
}

void IWeapon_FireRing::Update(CWeaponController* pWeapon, float fDeltaTime)
{
	__super::IWeapon_Update(fDeltaTime);	// 기본적으로 돌아가야하는 로직들.
}

void IWeapon_FireRing::Exit(CWeaponController* pWeapon)
{

}

void IWeapon_FireRing::Fire(CWeaponController* pWeapon)
{
	if (m_iCurrent_BulletCount <= 0)		// 메시지와 함께 무기 강제 교체
	{
		Publish_AmmoDepletedEvent();
		return;
	}

	if (m_bCanFire)
	{
		_vec3 XAxisrotate, ZAxisrotate, YAxisrotate;
		m_vMuzzleList[0].tTranfrom->Get_Info(INFO_LOOK, &ZAxisrotate);
		m_vMuzzleList[0].tTranfrom->Get_Info(INFO_RIGHT, &XAxisrotate);
		m_vMuzzleList[0].tTranfrom->Get_Info(INFO_UP, &YAxisrotate);
		D3DXVec3Normalize(&ZAxisrotate, &ZAxisrotate);
		D3DXVec3Normalize(&XAxisrotate, &XAxisrotate);
		D3DXVec3Normalize(&YAxisrotate, &YAxisrotate);

		auto Owner = pWeapon->Get_MyOwner();

		_vec3 pos;	// 무기 발사할 위치 정해주는 _vec3 Pos

		if (!m_bisRightHandShoot)
		{
			pos = *(m_vMuzzleList[0].tTranfrom->GetInfoPos()) +
				m_vMuzzleList[0].tOffset.x * XAxisrotate
				+ m_vMuzzleList[0].tOffset.y * YAxisrotate
				+ m_vMuzzleList[0].tOffset.z * ZAxisrotate;

			m_bisRightHandShoot = true;
		}

		else
		{
			pos = *(m_vMuzzleList[1].tTranfrom->GetInfoPos()) +
				m_vMuzzleList[1].tOffset.x * XAxisrotate
				+ m_vMuzzleList[1].tOffset.y * YAxisrotate
				+ m_vMuzzleList[1].tOffset.z * ZAxisrotate;

			m_bisRightHandShoot = false;
		}

		_vec3 BulletDir;
		//CalculateFireDirection(pos, BulletDir);


		// 1) 뷰포트/행렬
		D3DVIEWPORT9 vp; ZeroMemory(&vp, sizeof(vp));
		CGraphicDev::GetInstance()->Get_GraphicDev()->GetViewport(&vp);

		D3DXMATRIX view = CCameraManager::GetInstance()->Get_MainViewMatrix();
		D3DXMATRIX proj = CCameraManager::GetInstance()->Get_MainProjMatrix();
		D3DXMATRIX world; D3DXMatrixIdentity(&world);

		// 2) 마우스 → 카메라 레이
		POINT m = CInputManager::GetInstance()->GetMousePos();

		D3DXVECTOR3 s0((float)m.x, (float)m.y, 0.f); // near
		D3DXVECTOR3 s1((float)m.x, (float)m.y, 1.f); // far
		D3DXVECTOR3 p0, p1;
		D3DXVec3Unproject(&p0, &s0, &vp, &proj, &view, &world);
		D3DXVec3Unproject(&p1, &s1, &vp, &proj, &view, &world);

		_vec3 rayO = p0;
		_vec3 rayD = p1 - p0;
		D3DXVec3Normalize(&rayD, &rayD);

		bool gotHit = false;

		_vec3 OutDir {0.f,0.f,0.f};
		_vec3 hit = { 0,0,0 };

		// 그래도 없으면 그냥 레이를 멀리 쏴서 임의 지점(항상 발사 보장)
		if (!gotHit) {
			const float kFar = 1000.f; // 원하는 사거리
			hit = rayO + rayD * kFar;
		}

		//  방향 계산(총구 기준)
		OutDir = hit - pos;
		if (D3DXVec3LengthSq(&OutDir) < 1e-8f) {
			// 혹시나 0벡터면 카메라 정면(또는 플레이어 LOOK)으로 대체
			D3DXMATRIX invV; D3DXMatrixInverse(&invV, 0, &view);
			_vec3 camFwd{ invV._31, invV._32, invV._33 }; // 카메라 월드 LOOK
			OutDir = camFwd;
		}
		D3DXVec3Normalize(&OutDir, &OutDir);



		// 현재 내 무기의 위치와 마우스 위치를 구해야함.
		auto bullet = static_cast<CBullet*>(CObjectPoolingManager::GetInstance()->Get(OBJ_BULLET));

		if (!bullet) return;

		bullet->Get_Transform()->Set_Pos(pos.x, pos.y, pos.z);
		bullet->Set_BulletDir(OutDir);
		bullet->Set_ObjectOwner(Owner);

		bullet->Set_BulletStatus(BULLET_STATUS_IDLE);
		bullet->Change_BulletState(BULLET_PLAYER_FIREBALL);
		static_cast<CColliderBase*>(bullet->Get_Component(L"Com_Collider", ID_DYNAMIC))->Set_ColliderType(COLLIDER_SPHERE);
		static_cast<CTransform*>(bullet->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Scale(_vec3{ 0.01f, 0.01f, 0.01f });
		CSoundManager::GetInstance()->SoundPlay(L"firerings_fire.wav", BULLET_SOUND, CSoundManager::GetInstance()->GetVol().sfx);

		Dicrease_Bullet(1);
		m_fElapsedTime = 0.f;
	}
}

void IWeapon_FireRing::ChargeFire(CWeaponController* pWeapon)
{
	if (m_iCurrent_EnergyBulletCount <= 0) return;

	if (m_bCanChargeFire)
	{
		//auto Owner = pWeapon->Get_MyOwner();
		//auto pos = *(m_vMuzzleList[0].tTranfrom->GetInfoPos()) + m_vMuzzleList[0].tOffset;

		//_vec3 BulletDir;
		//CalculateFireDirection(pos, BulletDir);
		//// 현재 내 무기의 위치와 마우스 위치를 구해야함.
		//auto bullet = static_cast<CBullet*>(CObjectPoolingManager::GetInstance()->Get(OBJ_BULLET));

		//if (!bullet) return;

		//bullet->Get_Transform()->Set_Pos(pos.x, pos.y, pos.z);
		//bullet->Set_BulletDir(BulletDir);
		//bullet->Set_ObjectOwner(Owner);

		//bullet->Change_BulletState(BULLET_PLAYER_CHARGEEMACE);
		//static_cast<CColliderBase*>(bullet->Get_Component(L"Com_Collider", ID_DYNAMIC))->Set_ColliderType(COLLIDER_SPHERE);
		//static_cast<CTransform*>(bullet->Get_Component(L"Com_Transform", ID_DYNAMIC))->Set_Scale(_vec3{ 1.f, 1.f, 1.f });

		//Decrease_EnergyBullet(5);
	}
}