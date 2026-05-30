#include "ICam_OrbitalCam.h"
#include "CCamera.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CObjectManager.h"
#include "CObjectPoolingManager.h"
#include "CEventBus.h"
#include "CMonster.h"
#include "CUIManager.h"

void ICam_OrbitalCam::Enter(CCamera* pCamera, _vec3 vPos, _vec3 vTargetPos, float Distance, float YawAngle)
{
	// 1인칭 시점. Initialize 해야하는 것. 
	// 일단 1인칭 시점일 때는 나 자신이 보이지 않게 만들어보자.

	vWorldUp = { 0.f, 1.f, 0.f };
	m_tOrbit = { vPos, Distance, /*Yaw*/YawAngle, /*Pitch*/ 0.f, /*Speed*/1.f, _vec3{0.f, 1.f, 0.f}};
}

void ICam_OrbitalCam::Update(CCamera* pCamera, CGameObject* pOwner, _float fDeltaTime)
{
	if (pOwner == NULL || m_pPivotObject == NULL) return;

	m_vCameratoPivotDir = m_tOrbit.Pivot - vEye;
	D3DXVec3Normalize(&m_vCameratoPivotDir, &m_vCameratoPivotDir);

	Set_eye(pCamera, pOwner);
	Set_Look(pCamera, pOwner);
	Set_Matview(pCamera, pOwner);
	Set_MatProj(pCamera, pOwner);
}

void ICam_OrbitalCam::Camera_Anglepos(const float& fDeltaTime)
{
	m_tOrbit.yaw += m_tOrbit.AngVel * fDeltaTime;
}

void ICam_OrbitalCam::Exit(CCamera* pCamera, CGameObject* pOwner)
{
	m_tOrbit = { _vec3{0.f, 0.f, 0.f}, 0.f, 0.f, 0.f, 0.f, _vec3{0.f, 1.f, 0.f} };
	m_pPivotObject = nullptr;
}

void ICam_OrbitalCam::Set_eye(CCamera* pCamera, CGameObject* pOwner)
{
	// 오비탈 카메라 일단은 피벗의 worldup 기준으로 원을 그리며 공전하도록 설계.
	vEye.x = m_tOrbit.Pivot.x + m_tOrbit.radius * cos(D3DXToRadian(m_tOrbit.pitch)) * sin(D3DXToRadian(m_tOrbit.yaw));
	vEye.y = m_tOrbit.Pivot.y + m_tOrbit.radius * sin(D3DXToRadian(m_tOrbit.pitch)) + 0.3f;
	vEye.z = m_tOrbit.Pivot.z + m_tOrbit.radius * cos(D3DXToRadian(m_tOrbit.pitch)) * cos(D3DXToRadian(m_tOrbit.yaw));

	pCamera->Get_Transform()->Set_Pos(vEye.x, vEye.y, vEye.z);
}

void ICam_OrbitalCam::Set_Look(CCamera* pCamera, CGameObject* pOwner)
{
	// 현재 내 눈 위치의 바로 앞을 봄.
	//_vec3 offset{ 0.f, 0.9f, 0.f };
	vLook = m_tOrbit.Pivot /*+ offset*/;
}

void ICam_OrbitalCam::Set_Matview(CCamera* pCamera, CGameObject* pOwner)
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

void ICam_OrbitalCam::Set_MatProj(CCamera* pCamera, CGameObject* pOwner)
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