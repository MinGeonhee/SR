#include "CCamera.h"
#include "ICameraState.h"
#include "CTransform.h"
#include "CGameObject.h"
#include "ICam_FirstPerson.h"
#include "ICam_ThirdPerson.h"
#include "ICam_UIMode.h"
#include "ICam_EffectToolMode.h"
#include "CProtoTypeManager.h"
#include "ICam_ToolMode.h"
#include "ICam_Shake.h"
#include "ICam_BossIntro.h"
#include "ICam_BossOrbital.h"
#include "ICam_Zoom.h"
#include "ICam_TargetMode.h"
#include "ICam_OrbitalCam.h"
#include "ICam_ZoomOutMode.h"

CCamera::CCamera()
    : m_pTargetTransform(nullptr)
    , m_pCurState(nullptr)
{
}

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
    : CComponent(pGraphicDev)
    , m_pTargetTransform(nullptr)
    , m_pCurState(nullptr)
{
}

CCamera::CCamera(const CCamera& rhs)
    : CComponent(rhs)
    , m_pTargetTransform(rhs.m_pTargetTransform)
    , m_pCurState(rhs.m_pCurState)
    , m_eCamState(rhs.m_eCamState)
    , m_mCameraState(rhs.m_mCameraState)
{
}

CCamera::~CCamera()
{
}

HRESULT CCamera::Ready_Camera()
{
    m_mCameraState.insert({ CAM_FIRSTPERSON,          new ICam_FirstPerson() });
    m_mCameraState.insert({ CAM_THIRDPERSON,          new ICam_ThirdPerson() });
    m_mCameraState.insert({ CAM_SHAKE,                new ICam_Shake() });
    m_mCameraState.insert({ CAM_BOSS_CUTSCENE,        new ICam_BossIntro() });      // 범용성 x 추후 삭제
    m_mCameraState.insert({ CAM_BOSSORBITAL,          new ICam_BossOrbital() });   // 범용성 x 추후 삭제

    m_mCameraState.insert({ CAM_ORBITAL,              new ICam_OrbitalCam() });
    m_mCameraState.insert({ CAM_ZOOM,                 new ICam_Zoom() });
    m_mCameraState.insert({ CAM_ZOOMOUT,              new ICam_ZoomOutMode() });
    m_mCameraState.insert({ CAM_BOSS_TARGET,          new ICam_TargetMode() });     // 추후 닉변 CAM_TARGET으로

    m_mCameraState.insert({ CAM_UIMODE,               new ICam_UIMode() });
    m_mCameraState.insert({ CAM_TOOLMODE,             new ICam_ToolMode() });
    m_mCameraState.insert({ CAM_TOOLMODE_EFFECT,      new ICam_EffectToolMode() });

    D3DXMatrixIdentity(&m_ViewMatrix);
    D3DXMatrixIdentity(&m_ProjMatrix);

    // 초기 카메라 상태값 설정. (현재 1인칭 시점)
    auto iter = m_mCameraState.find(CAM_FIRSTPERSON);
    m_pCurState = (*iter).second;

    return S_OK;
}

void CCamera::Camera_Inintialize()
{
    m_pCurState->Enter(this);
}

void CCamera::Set_CameraState(CAMERASTATE eState, _vec3 vPOs, _vec3 vDir, float duration, float power)
{
    m_pCurState->Exit(this, m_pTarget);
    
    auto iter = m_mCameraState.find(eState);
    m_pCurState = (*iter).second;

    m_pCurState->Enter(this, vPOs, vDir, duration, power);
}

_int CCamera::Update_Component(const _float& fTimeDelta)
{
    if (m_pMyOwner == NULL)
        return 0;

    m_pTarget = m_pMyOwner->Get_Target();

    if (m_pCurState)
        m_pCurState->Update(this, m_pTarget, fTimeDelta);

    return 0;
}

void CCamera::LateUpdate_Component(const _float& fTimeDelta)
{
    // 어차피 Update에서 ICameraState에서 계산 다 해주니까.
    // 행렬 그대로 다 곱해준다.
    if (FAILED(m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_ViewMatrix))) // 모든 객체한테 이 뷰 행렬 적용할랭
        MSG_BOX("카메라 뷰 행렬 적용 실패");

    if (FAILED(m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_ProjMatrix)))
        MSG_BOX("카메라 투영 행렬 적용 실패");
}

CComponent* CCamera::Clone()
{
    return new CCamera(*this);
}

CCamera* CCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CCamera* pInstance = new CCamera(pGraphicDev);
    if (FAILED(pInstance->Ready_Camera()))
    {
        delete pInstance;
        return nullptr;
    }
    return pInstance;
}

void CCamera::Free()
{
    for_each(m_mCameraState.begin(), m_mCameraState.end(), CDeleteMap());
    m_mCameraState.clear();

    CComponent::Free();
}
