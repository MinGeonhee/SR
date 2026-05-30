#include "ICam_ZoomOutMode.h"
#include "CCamera.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CCameraManager.h"
#include "CEventBus.h"

void ICam_ZoomOutMode::Enter(CCamera* pCamera, _vec3 vPos, _vec3 vDir, float fDistance, float fSpeed)
{
	vWorldUp = { 0.f , 1.f, 0.f };
	vEye = vPos;

	m_vStartEye = vEye;
	m_vZoomDir = vDir;

	m_tZoom.fDistance = fDistance;
	m_tZoom.fSpeed = fSpeed;
	m_fTravel = 0.f;
}

void ICam_ZoomOutMode::Update(CCamera* pCamera, CGameObject* pOwner, _float fDeltaTime)
{
	if (pOwner == nullptr) return;

	const float step = m_tZoom.fSpeed * fDeltaTime;
	const float remain = m_tZoom.fDistance - m_fTravel;
	const float advance = (remain > 0.f) ? min(step, remain) : 0.f;

	// 이동 및 누적
	vEye -= m_vZoomDir * advance;
	m_fTravel -= advance;

	Set_eye(pCamera, pOwner);
	Set_Look(pCamera, pOwner);
	Set_Matview(pCamera, pOwner);
	Set_MatProj(pCamera, pOwner);

	// 목표 도달 시
	if (m_fTravel >= m_tZoom.fDistance - 0.001f)
	{
		// 무언가 하면 좋지 않을까 나중에라도.
	}
}


void ICam_ZoomOutMode::Exit(CCamera* pCamera, CGameObject* pOwner)
{
	vEye = m_vStartEye;
	Set_eye(pCamera, pOwner);
	Set_Look(pCamera, pOwner);
	Set_Matview(pCamera, pOwner);
	Set_MatProj(pCamera, pOwner);
}

void ICam_ZoomOutMode::Set_eye(CCamera* pCamera, CGameObject* pOwner)
{

	//offset = *D3DXVec3Normalize(&pOwnerLook, &pOwnerLook)/* + *D3DXVec3Normalize(&pOwnerLook, &pOwnerLook)*/;
	//vEye += offset; // 위치에 Offset을 더함.

	pCamera->Get_Transform()->Set_Pos(vEye.x, vEye.y, vEye.z);
}

void ICam_ZoomOutMode::Set_Look(CCamera* pCamera, CGameObject* pOwner)
{
	vLook = vEye + m_vZoomDir;
}

void ICam_ZoomOutMode::Set_Matview(CCamera* pCamera, CGameObject* pOwner)
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

void ICam_ZoomOutMode::Set_MatProj(CCamera* pCamera, CGameObject* pOwner)
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