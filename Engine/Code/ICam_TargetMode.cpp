#include "ICam_TargetMode.h"
#include "CCamera.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CObjectManager.h"

void ICam_TargetMode::Enter(CCamera* pCamera, _vec3 vPos, _vec3 vTargetpPos, float Distance, float fSpeed)
{
	// 1인칭 시점. Initialize 해야하는 것. 
	// 일단 1인칭 시점일 때는 나 자신이 보이지 않게 만들어보자.
	vWorldUp = { 0.f , 1.f, 0.f };
	vEye = vPos;	vLook = vTargetpPos + _vec3{0.f, 1.f, 0.f};
}

void ICam_TargetMode::Update(CCamera* pCamera, CGameObject* pOwner, _float fDeltaTime)
{
	if (pOwner == nullptr)
		return;

	Set_eye(pCamera, pOwner);
	Set_Look(pCamera, pOwner);
	Set_Matview(pCamera, pOwner);
	Set_MatProj(pCamera, pOwner);
}


void ICam_TargetMode::Exit(CCamera* pCamera, CGameObject* pOwner)
{

}

void ICam_TargetMode::Set_eye(CCamera* pCamera, CGameObject* pOwner)
{
	pCamera->Get_Transform()->Set_Pos(vEye.x, vEye.y, vEye.z);
}

void ICam_TargetMode::Set_Look(CCamera* pCamera, CGameObject* pOwner)
{
}

void ICam_TargetMode::Set_Matview(CCamera* pCamera, CGameObject* pOwner)
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

void ICam_TargetMode::Set_MatProj(CCamera* pCamera, CGameObject* pOwner)
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