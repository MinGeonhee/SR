#include "ICam_ThirdPerson.h"
#include "CCamera.h"
#include "CGameObject.h"
#include "CTransform.h"

void ICam_ThirdPerson::Enter(CCamera* pCamera, _vec3 vPos, _vec3 vDir, float Distance, float fSpeed)
{
	// 3인칭 시점. Initialize 해야하는 것. 
	// 3인칭이니까 Visible 세팅을 한다.
	vWorldUp = { 0.f , 1.f, 0.f };
}

void ICam_ThirdPerson::Update(CCamera* pCamera, CGameObject* pOwner, _float fDeltaTime)
{
	if (pOwner == NULL)
		return;

	Set_eye(pCamera, pOwner);
	Set_Look(pCamera, pOwner);
	Set_Matview(pCamera, pOwner);
	Set_MatProj(pCamera, pOwner);
}


void ICam_ThirdPerson::Exit(CCamera* pCamera, CGameObject* pOwner)
{

}

void ICam_ThirdPerson::Set_eye(CCamera* pCamera, CGameObject* pOwner)
{
	// 카메라의 위치가 주인 몸통으로부터 얼마만큼 떨어뜨려 둘 것인가.

	_vec3 pOwnerUp, pOwnerLook;
	pOwner->Get_Transform()->Get_Info(INFO_UP, &pOwnerUp);
	pOwner->Get_Transform()->Get_Info(INFO_LOOK, &pOwnerLook);

	offset = { 0, 5.f, -5.f };

	pOwner->Get_Transform()->Get_Info(INFO_POS, &vEye);
	vEye += offset; // 위치에 Offset을 더함.

	pCamera->Get_Transform()->Set_Pos(vEye.x, vEye.y, vEye.z);
}

void ICam_ThirdPerson::Set_Look(CCamera* pCamera, CGameObject* pOwner)
{
	// 현재 내 눈 위치의 바로 앞을 봄.
	_vec3 pOwnerUp, pOwnerLook;
	pOwner->Get_Transform()->Get_Info(INFO_UP, &pOwnerUp);
	pOwner->Get_Transform()->Get_Info(INFO_LOOK, &pOwnerLook);

	pOwner->Get_Transform()->Get_Info(INFO_POS, &vLook); // 원래는 이거 Look이 맞음.
}

void ICam_ThirdPerson::Set_Matview(CCamera* pCamera, CGameObject* pOwner)
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

void ICam_ThirdPerson::Set_MatProj(CCamera* pCamera, CGameObject* pOwner)
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
