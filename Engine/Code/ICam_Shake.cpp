#include "ICam_Shake.h"
#include "CCamera.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CInputManager.h"

void ICam_Shake::Enter(CCamera* pCamera, _vec3 vPos, _vec3 vDir, float Duration, float power)
{
    // 1인칭 시점. Initialize 해야하는 것. 
    // 일단 1인칭 시점일 때는 나 자신이 보이지 않게 만들어보자.
    vWorldUp = { 0.f , 1.f, 0.f };

    m_fCameraShakeDuration = Duration;
    m_fCameraShakeIntensity = power;
}

void ICam_Shake::Update(CCamera* pCamera, CGameObject* pOwner, _float fDeltaTime)
{
    if (pOwner == nullptr)
        return;

    m_fCameraShakeIntensity -= fDeltaTime * 0.01f;
    if (m_fCameraShakeIntensity <= 0.f) m_fCameraShakeIntensity = 0.f;

    if (m_fCameraShakeDuration > 0.0f)
    {
        // Generate random offsets
        float offsetX = (rand() % 100 / 100.0f - 0.8f) * m_fCameraShakeIntensity;
        float offsetY = (rand() % 100 / 100.0f - 0.5f) * m_fCameraShakeIntensity;
        ShakeOffset = D3DXVECTOR3(offsetX, offsetY, 0);

        m_fCameraShakeDuration -= fDeltaTime;
        if (m_fCameraShakeDuration <= 0.0f)
        {
            ShakeOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // End shake
            pCamera->Set_CameraState(CAM_FIRSTPERSON);
            CInputManager::GetInstance()->Set_KeyLock(false);
        }
    }

    // Apply shake offset to camera position
    _vec3 originalCameraPos = *(pCamera->Get_Transform()->GetInfoPos());
    vEye = originalCameraPos + ShakeOffset;

    Set_Look(pCamera, pOwner);

    // Construct view matrix
    _matrix MatView;
    D3DXMatrixLookAtLH(&MatView, &vEye, &vLook, &vWorldUp);
    pCamera->Set_ViewMatrix(MatView);

    // Set the view transform in DirectX
    Set_MatProj(pCamera, pOwner);
}

void ICam_Shake::Exit(CCamera* pCamera, CGameObject* pOwner)
{
}

void ICam_Shake::Set_eye(CCamera* pCamera, CGameObject* pOwner)
{
    // 카메라의 위치가 주인 몸통으로부터 얼마만큼 떨어뜨려 둘 것인가.

    //_vec3 pOwnerUp, pOwnerLook;
    //pOwner->Get_Transform()->Get_Info(INFO_UP, &pOwnerUp);
    //pOwner->Get_Transform()->Get_Info(INFO_LOOK, &pOwnerLook);

    //offset = *D3DXVec3Normalize(&pOwnerLook, &pOwnerLook)/* + *D3DXVec3Normalize(&pOwnerLook, &pOwnerLook)*/;

    //pOwner->Get_Transform()->Get_Info(INFO_POS, &vEye);
    //vEye += offset; // 위치에 Offset을 더함.

    //pCamera->Get_Transform()->Set_Pos(vEye.x, vEye.y, vEye.z);
}

void ICam_Shake::Set_Look(CCamera* pCamera, CGameObject* pOwner)
{
    // 현재 내 눈 위치의 바로 앞을 봄.
    _vec3 pOwnerUp, pOwnerLook;
    pOwner->Get_Transform()->Get_Info(INFO_UP, &pOwnerUp);
    pOwner->Get_Transform()->Get_Info(INFO_LOOK, &pOwnerLook);

    vLook = vEye + *D3DXVec3Normalize(&pOwnerLook, &pOwnerLook); // 원래는 이거 Look이 맞음.
}

void ICam_Shake::Set_Matview(CCamera* pCamera, CGameObject* pOwner)
{
    _matrix MatView;
    ::D3DXMatrixIdentity(&MatView);

    D3DXMatrixLookAtLH(
        &MatView, // 뷰 행렬을 받을 인자
        &vEye,    // 현재 카메라 위치
        &vLook,   // 
        &vWorldUp // worldup벡터.
    );

    pCamera->Set_ViewMatrix(MatView);
}

void ICam_Shake::Set_MatProj(CCamera* pCamera, CGameObject* pOwner)
{
    _matrix MatProj;
    ::D3DXMatrixIdentity(&MatProj);

    D3DXMatrixPerspectiveFovLH(
        &MatProj,                   // 투영 행렬을 담을 인자 넣기.
        D3DXToRadian(60.f),         // 내가 바라볼 카메라의 각도 FOV
        float(WINCX) / WINCY,       // 종횡비. 내 스크린 x를 스크린 y로 나눈 값.
        0.1f,                       // 근거리 평면 Near 
        1000.f
    );

    pCamera->Set_ProjMatrix(MatProj);
}
