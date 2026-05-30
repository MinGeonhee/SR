#include "ICam_ToolMode.h"
#include "CCamera.h"
#include "CGameObject.h"
#include "CTransform.h"

void ICam_ToolMode::Enter(CCamera* pCamera, _vec3 vPos, _vec3 vDir, float Distance, float fSpeed)
{
	vWorldUp = { 0.f , 1.f ,0.f };
}

void ICam_ToolMode::Update(CCamera* pCamera, CGameObject* pOwner, float fDeltaTime)
{
	//내가 지금 카메라맨 의 카메라 컴포트라는거고
	//Owner가 사실 Target 인데 나는 필요없다 찍고있을대상은 따로없으니까 
	Set_eye(pCamera,pOwner);
	Set_Look(pCamera,pOwner);

	Set_Matview(pCamera, pOwner);
	Set_MatProj(pCamera, pOwner);
}

void ICam_ToolMode::Exit(CCamera* pCamera, CGameObject* pOwner)
{
}

void ICam_ToolMode::Set_eye(CCamera* pCamera, CGameObject* pOwner)
{
	_vec3 Pos;
	vEye = *pCamera->Get_Transform()->GetInfoPos();
}

void ICam_ToolMode::Set_Look(CCamera* pCamera, CGameObject* pOwner)
{
	pCamera->Get_Transform()->Get_Info(INFO_LOOK, &vLook);
}

void ICam_ToolMode::Set_Matview(CCamera* pCamera, CGameObject* pOwner)
{
	_matrix MatView;
	::D3DXMatrixIdentity(&MatView);

	_vec3 vAt = vEye + vLook;

	D3DXMatrixLookAtLH(
		&MatView, // 뷰 행렬을 받을 인자
		&vEye,		// 현재 카메라 위치
		&vAt,		// 
		&vWorldUp);	// worldup벡터.

	pCamera->Set_ViewMatrix(MatView);
}

void ICam_ToolMode::Set_MatProj(CCamera* pCamera, CGameObject* pOwner)
{
	_matrix MatProj;
	::D3DXMatrixIdentity(&MatProj);

	D3DXMatrixPerspectiveFovLH(
		&MatProj,						// 투영 행렬을 담을 인자 넣기.
		D3DXToRadian(60.f),				// 카메라의 각도 FOV
		float(WINCX) / WINCY,			// 종횡비. 내 스크린 x를 스크린 y로 나눈 값.
		0.1f,							// 근거리 평면 Near 
		1000.f);

	pCamera->Set_ProjMatrix(MatProj);
}
