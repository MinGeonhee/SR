#include "ICam_UIMode.h"
#include "CCamera.h"
#include "CGameObject.h"
#include "CTransform.h"

// UI 
void ICam_UIMode::Enter(CCamera* pCamera, _vec3 vPos, _vec3 vDir, float Distance, float fSpeed)
{
	// 잘가 UI카메라~~~~~~~~~~
	m_vUIPos = { 0.f, 0.f, -1.f };
	vWorldUp = { 0.f , 1.f, 0.f };
}

void ICam_UIMode::Update(CCamera* pCamera, CGameObject* pTarget, _float fDeltaTime)
{
	Set_Matview(pCamera, pTarget);
	Set_MatProj(pCamera, pTarget);
}


void ICam_UIMode::Exit(CCamera* pCamera, CGameObject* pTarget)
{

}

void ICam_UIMode::Set_eye(CCamera* pCamera, CGameObject* pTarget)
{
}

void ICam_UIMode::Set_Look(CCamera* pCamera, CGameObject* pTarget)
{
}

void ICam_UIMode::Set_Matview(CCamera* pCamera, CGameObject* pTarget)
{
	_matrix MatView;
	::D3DXMatrixIdentity(&MatView);

	_vec3 vTarget = m_vUIPos + _vec3(0.f, 0.f, 1.f);

	D3DXMatrixLookAtLH(
		&MatView, // 뷰 행렬을 받을 인자
		&m_vUIPos,		// 현재 카메라 위치
		&vTarget,		// 100,100,100 에서 1앞
		&vWorldUp);	// worldup벡터.

	pCamera->Set_ViewMatrix(MatView);
}

void ICam_UIMode::Set_MatProj(CCamera* pCamera, CGameObject* pTarget)
{
	_matrix MatProj;
	::D3DXMatrixIdentity(&MatProj);

	// 직교 투영은 무조건 z값이 0 ~ 1
	// UI용 카메라일때는 반드시 
	// z버퍼 enable
	// zWriteEnable false
	// 조명도 꺼야한다.

	D3DXMatrixOrthoLH(
		&MatProj,						// 투영 행렬을 담을 인자 넣기.
		static_cast<float>(WINCX),				// 내가 바라볼 카메라의 각도 FOV
		static_cast<float>(WINCY),			// 종횡비. 내 스크린 x를 스크린 y로 나눈 값.
		0.f,							// 근거리 평면 Near 
		5.f);

	pCamera->Set_ProjMatrix(MatProj);
}