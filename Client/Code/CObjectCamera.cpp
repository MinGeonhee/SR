#include "pch.h"
#include "CObjectCamera.h"
#include "CRendererManager.h"
#include "CInputManager.h"

CObjectCamera::CObjectCamera(const CObjectCamera& rhs)
	:CGameObject(rhs), m_pTarget(nullptr), m_pCameraCom(nullptr)
{
}

CObjectCamera::CObjectCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev), m_pTarget(nullptr), m_pCameraCom(nullptr)
{
}

CObjectCamera::~CObjectCamera()
{

}

HRESULT	CObjectCamera::Ready_GameObject()
{
	// 이니셜라이즈 부분에서 컴포넌트 추가를 한다.
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fSpeed = 10.f;
	m_pCameraCom->Set_MyOwner(this);
	m_pCameraCom->Set_MyTransform(m_pTransform);
	m_pCameraCom->Camera_Inintialize();

	return S_OK;
}

_int CObjectCamera::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);
	return 0;
}

void CObjectCamera::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);

	// UI 카메라 아니면 언제든지 시점 변환 가능
	if(m_pCameraCom->Get_CameraState() != CAM_UIMODE)
	Change_CameraState();

	// 자유시점 카메라일 때만 움직임 키 입력을 받음.
	if(m_pCameraCom->Get_CameraState() == CAM_FREE)
		Move_Camera(fTimeDelta);
}

void CObjectCamera::Render_GameObject()
{
	// 컬링 꺼주고
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
	// 다시 켜줌.
}

HRESULT CObjectCamera::Add_Component()
{
	CComponent* pComponent = NULL;
	// transform 컴포넌트.

	pComponent = m_pTransform = CLONE_PROTOTYPE(PROTO_TRANS, CTransform);

	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	// 카메라 컴포넌트 생성.
	pComponent = m_pCameraCom = CLONE_PROTOTYPE(PROTO_CAMERA, CCamera);

	if (pComponent == NULL)
		return E_FAIL;

	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Camera", pComponent });

	return S_OK;
}

void CObjectCamera::Move_Camera(const _float& fTimeDelta)
{
	// 회전한 상태의 축을 가져온다??
	// 회전한 y축 방향대로 나아가면 되니까??
	_vec3 vLook;
	m_pTransform->Get_Info(INFO_LOOK, &vLook);

	if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::K8))
	{
		// 회전된 내 local Y축 방향으로 내가 움직이는데 그걸 단위벡터로 만들어서
		// 방향값만 가져오고 내 speed만큼 움직이겠다.
		m_pTransform->Move_Pos(D3DXVec3Normalize(&vLook, &vLook), m_fSpeed, fTimeDelta);
	}

	if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::K2))
	{
		// 뒤로도 가보자.
		m_pTransform->Move_Pos(D3DXVec3Normalize(&vLook, &vLook), -m_fSpeed, fTimeDelta);
	}

	if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::K7))
	{
		m_pTransform->Rotation(ROT_X, D3DXToRadian(180 * fTimeDelta));
	}

	if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::K9))
	{
		m_pTransform->Rotation(ROT_X, D3DXToRadian(-180 * fTimeDelta));
	}

	if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::K4))
	{
		m_pTransform->Rotation(ROT_Y, D3DXToRadian(180 * fTimeDelta));
	}

	if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::K6))
	{
		m_pTransform->Rotation(ROT_Y, D3DXToRadian(-180 * fTimeDelta));
	}

	if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::K1))
	{
		m_pTransform->Rotation(ROT_Z, D3DXToRadian(180 * fTimeDelta));
	}

	if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::K3))
	{
		m_pTransform->Rotation(ROT_Z, D3DXToRadian(-180 * fTimeDelta));
	}
}

void CObjectCamera::Change_CameraState()
{
	if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::R))
	{
		m_pCameraCom->Set_CameraState(CAM_FIRSTPERSON);
		m_pCameraCom->Camera_Inintialize();
	}

	if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::T))
	{
		m_pCameraCom->Set_CameraState(CAM_THIRDPERSON);
		m_pCameraCom->Camera_Inintialize();
	}

	//if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::F))
	//{
	//	m_pCameraCom->Set_CameraState(CAM_FREE);
	//	m_pCameraCom->Camera_Inintialize();
	//}
}

CObjectCamera* CObjectCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CObjectCamera* pCamera = new CObjectCamera(pGraphicDev);

	if (FAILED(pCamera->Ready_GameObject()))
	{
		// 만약에 제대로 생성이 되지 않았다면 바로 삭제.
		Safe_Release(pCamera);
		MSG_BOX("Camera Create Failed");
		return nullptr;
	}

	return pCamera;
}

void CObjectCamera::Free()
{
	Engine::CGameObject::Free();
}