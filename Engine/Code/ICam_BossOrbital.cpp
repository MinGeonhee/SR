#include "ICam_BossOrbital.h"
#include "CCamera.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CObjectManager.h"
#include "CObjectPoolingManager.h"
#include "CEventBus.h"
#include "CMonster.h"
#include "CUIManager.h"

void ICam_BossOrbital::Enter(CCamera* pCamera, _vec3 vPos, _vec3 vDir, float Distance, float fSpeed)
{
	// 1인칭 시점. Initialize 해야하는 것. 
	// 일단 1인칭 시점일 때는 나 자신이 보이지 않게 만들어보자.

	vWorldUp = { 0.f, 1.f, 0.f };
	m_pPivotObject = CObjectManager::GetInstance()->Get_StaticObjectList(OBJ_MONSTER_BOSS).front();		if (m_pPivotObject == NULL) return;
	CTransform* trans = m_pPivotObject->Get_Transform();	if (trans == NULL) return;
	m_tOrbit = { *(trans->GetInfoPos()), Distance, 180.f, 0.f, fSpeed, _vec3{0.f, 1.f, 0.f} };

	m_bfirst_Intro = false;	
	m_bSecond_Intro = false;	// 첫 진입 시 첫번째 인트로 재생.
}

void ICam_BossOrbital::Update(CCamera* pCamera, CGameObject* pOwner, _float fDeltaTime)
{
	if (pOwner == NULL || m_pPivotObject == NULL) return;

	if (!m_bfirst_Intro && !m_bSecond_Intro) FirstIntro_CameraAnglepos(fDeltaTime);
	else if (m_bfirst_Intro && !m_bSecond_Intro) SecondIntro_CameraAnglepos(fDeltaTime);

	m_vCameratoPivotDir = m_tOrbit.Pivot - vEye;
	D3DXVec3Normalize(&m_vCameratoPivotDir, &m_vCameratoPivotDir);

	Set_eye(pCamera, pOwner);
	Set_Look(pCamera, pOwner);
	Set_Matview(pCamera, pOwner);
	Set_MatProj(pCamera, pOwner);
}

void ICam_BossOrbital::FirstIntro_CameraAnglepos(const float& fDeltaTime)
{
	CUIManager::GetInstance()->SetCategoryTag(L"Dialogue_UI", L"BossIntro_Left");
	m_tOrbit.yaw += m_tOrbit.AngVel * fDeltaTime;

	if (m_tOrbit.yaw >= 230.f)	// 카메라 이동각이 45도를 넘었다면
	{
		m_tOrbit.yaw = 230.f; // 카메라를 45도 위치에 잠궈두고
		m_fCameraSwitchTime += fDeltaTime;
		m_fMonsterSpawnTime += fDeltaTime;

		if (m_fMonsterSpawnTime >= 0.4f && m_iMonsterCount <4)
		{
			CGameObject* skelleton = CObjectPoolingManager::GetInstance()->Get(OBJ_MONSTER_SKELETON_SWORD);		if (skelleton == NULL) return;
			skelleton->Get_Transform()->Set_Pos(m_tOrbit.Pivot.x - (m_vCameratoPivotDir.x * m_iMonsterCount * 1.3f), m_tOrbit.Pivot.y, m_tOrbit.Pivot.z - (m_vCameratoPivotDir.z * m_iMonsterCount * 0.6f));
			auto monster = static_cast<CMonster*>(skelleton);
			monster->Set_MonsterStatus(MONSTER_STATUS_IDLE);
			monster->Set_Target(CObjectManager::GetInstance()->Get(OBJ_PLAYER));
			static_cast<CStatus*>(monster->Get_Component(L"Com_Status", ID_DYNAMIC))->Reset_Hp();
			m_iMonsterCount++;
			m_fMonsterSpawnTime = 0.f;
		}

		if (m_fCameraSwitchTime >= 3.f) // 카메라 스위칭 대기시간 만큼 대기를 시킨다. (대사 출력이나 몬스터 스폰 떄문에)
		{
			m_fCameraSwitchTime = 0.f;
			m_fMonsterSpawnTime = 0.f;
			m_tOrbit.yaw = 180.f;
			m_iMonsterCount = 1;
			m_bfirst_Intro = true;
		}
	}
}

void ICam_BossOrbital::SecondIntro_CameraAnglepos(const float& fDeltaTime)
{
	CUIManager::GetInstance()->SetCategoryTag(L"Dialogue_UI", L"BossIntro_Right");
	m_tOrbit.yaw -= m_tOrbit.AngVel * fDeltaTime;

	if (m_tOrbit.yaw <= 130.f)	// 카메라 이동각이 45도를 넘었다면
	{
		m_tOrbit.yaw = 130.f; // 카메라를 45도 위치에 잠궈두고
		m_fCameraSwitchTime += fDeltaTime;
		m_fMonsterSpawnTime += fDeltaTime;

		if (m_fMonsterSpawnTime >= 0.4f && m_iMonsterCount < 4)
		{
			CGameObject* skelleton = CObjectPoolingManager::GetInstance()->Get(OBJ_MONSTER_SKELETON_SWORD);		if (skelleton == NULL) return;
			skelleton->Get_Transform()->Set_Pos(m_tOrbit.Pivot.x - (m_vCameratoPivotDir.x * m_iMonsterCount * 1.3f), m_tOrbit.Pivot.y, m_tOrbit.Pivot.z - (m_vCameratoPivotDir.z * m_iMonsterCount * 0.6f));
			auto monster = static_cast<CMonster*>(skelleton);
			monster->Set_MonsterStatus(MONSTER_STATUS_IDLE);
			monster->Set_Target(CObjectManager::GetInstance()->Get(OBJ_PLAYER));
			static_cast<CStatus*>(monster->Get_Component(L"Com_Status", ID_DYNAMIC))->Reset_Hp();
			m_iMonsterCount++;
			m_fMonsterSpawnTime = 0.f;
		}

		if (m_fCameraSwitchTime >= 3.f) // 카메라 스위칭 대기시간 만큼 대기를 시킨다. (대사 출력이나 몬스터 스폰 떄문에)
		{
			CUIManager::GetInstance()->DeactivateUI(L"Dialogue_UI");
			m_fCameraSwitchTime = 0.f;
			m_fMonsterSpawnTime = 0.f;
			m_tOrbit.yaw = 180.f;
			m_iMonsterCount = 1;
			m_bSecond_Intro = true;

			EVENT event
			{
				PHASE::PHASE_END,
				TOPIC::Boss_Appear,
				CHANNEL::Boss,
				nullptr, nullptr, {}, {}
			};
			CEventBus::GetInstance()->publish(event);
		}
	}
}


void ICam_BossOrbital::Exit(CCamera* pCamera, CGameObject* pOwner)
{
	m_tOrbit = { _vec3{0.f, 0.f, 0.f}, 0.f, 0.f, 0.f, 0.f, _vec3{0.f, 1.f, 0.f} };
	m_pPivotObject = nullptr;
}

void ICam_BossOrbital::Set_eye(CCamera* pCamera, CGameObject* pOwner)
{
	// 오비탈 카메라 일단은 피벗의 worldup 기준으로 원을 그리며 공전하도록 설계.
	vEye.x = m_tOrbit.Pivot.x + m_tOrbit.radius * cos(D3DXToRadian(m_tOrbit.pitch)) * sin(D3DXToRadian(m_tOrbit.yaw));
	vEye.y = m_tOrbit.Pivot.y + m_tOrbit.radius * sin(D3DXToRadian(m_tOrbit.pitch)) + 0.3f;
	vEye.z = m_tOrbit.Pivot.z + m_tOrbit.radius * cos(D3DXToRadian(m_tOrbit.pitch)) * cos(D3DXToRadian(m_tOrbit.yaw));

	pCamera->Get_Transform()->Set_Pos(vEye.x, vEye.y, vEye.z);
}

void ICam_BossOrbital::Set_Look(CCamera* pCamera, CGameObject* pOwner)
{
	// 현재 내 눈 위치의 바로 앞을 봄.
	_vec3 offset{ 0.f, 0.9f, 0.f };
	vLook = m_tOrbit.Pivot + offset;
}

void ICam_BossOrbital::Set_Matview(CCamera* pCamera, CGameObject* pOwner)
{
	_matrix MatView;
	::D3DXMatrixIdentity(&MatView);

	D3DXMatrixLookAtLH(
		&MatView, // 뷰 행렬을 받을 인자
		&vEye,		// 현재 카메라 위치
		&vLook,		// 
		&vWorldUp);	// worldup벡터.

	pCamera->Set_ViewMatrix(MatView);
}

void ICam_BossOrbital::Set_MatProj(CCamera* pCamera, CGameObject* pOwner)
{
	_matrix MatProj;
	::D3DXMatrixIdentity(&MatProj);

	D3DXMatrixPerspectiveFovLH(
		&MatProj,						// 투영 행렬을 담을 인자 넣기.
		D3DXToRadian(60.f),				// 내가 바라볼 카메라의 각도 FOV
		float(WINCX) / WINCY,			// 종횡비. 내 스크린 x를 스크린 y로 나눈 값.
		0.1f,							// 근거리 평면 Near 
		1000.f);

	pCamera->Set_ProjMatrix(MatProj);
}