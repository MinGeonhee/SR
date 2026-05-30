#include "pch.h"
#include "CCameraMan.h"
#include "CProtoTypeManager.h"
#include "CInputManager.h"
#include "CToolManager.h"
#include "CCollisionManager.h"

CCameraMan::CCameraMan(const CCameraMan& rhs)
	: CGameObject(rhs)
{
	m_tHandle.m_eType = OBJ_END;
}

CCameraMan::CCameraMan(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev) , m_pCameraCom(nullptr)
{
}

CCameraMan::~CCameraMan()
{
}

void CCameraMan::Free()
{
	CGameObject::Free();
}

HRESULT CCameraMan::Ready_GameObject()
{
	// 이니셜라이즈 부분에서 컴포넌트 추가를 한다.
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_fSpeed = 10.f;
	m_iPosYFixedFloor = 3;
	m_isCameraFixed = false;
	//CAM_CINEMATIC
	m_pCameraCom->Set_CameraState(CAMERASTATE::CAM_TOOLMODE);
	m_pCameraCom->Set_MyOwner(this);
	m_pCameraCom->Set_MyTransform(m_pTransform);
	m_pCameraCom->Camera_Inintialize();


	m_pTransform->Set_Pos( 0.f , 10.f , -8.f );


	return S_OK;
}

_int CCameraMan::Update_GameObject(const _float& fTimeDelta)
{
	Key_Update();

	CGameObject::Update_GameObject(fTimeDelta);

	return 0;
}

void CCameraMan::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Move_Camera(fTimeDelta);
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CCameraMan::Render_GameObject()
{
	CGameObject::Render_GameObject();
}

HRESULT CCameraMan::Add_Component()
{
	CComponent* newCom = nullptr;

	newCom = m_pTransform = CLONE_PROTOTYPE(PROTOTYPE::PROTO_TRANS,CTransform);
	if (newCom == nullptr)
		return E_FAIL;

	m_mapComponent[ID_DYNAMIC].insert({L"Com_Transform",newCom });


	newCom = m_pCameraCom = CLONE_PROTOTYPE(PROTO_CAMERA, CCamera);

	if (newCom == nullptr)
		return E_FAIL;

	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Camera",newCom });

	return S_OK;
}

void CCameraMan::Move_Camera(const _float& fTimeDelta)
{

	_vec3 vRight;
	m_pTransform->Get_Info(INFO_RIGHT, &vRight);
	_vec3 vLook;
	m_pTransform->Get_Info(INFO_LOOK, &vLook);

	if (m_isCameraFixed)
	{
		// Y축 고정 (즉, 수평 이동만 가능)
		vRight.y = 0.f;
		vLook.y = 0.f;

		D3DXVec3Normalize(&vRight, &vRight);
		D3DXVec3Normalize(&vLook, &vLook);
	}



	D3DXVec3Normalize(&vRight,&vRight);
	D3DXVec3Normalize(&vLook,&vLook);

	
	
	if (CInputManager::GetInstance()->GetButtonPress(KEY_TYPE::W))
	{
		m_pTransform->Move_Pos(&vLook, +m_fSpeed , fTimeDelta);
	}
	else if (CInputManager::GetInstance()->GetButtonPress(KEY_TYPE::S))
	{
		m_pTransform->Move_Pos(&vLook, -m_fSpeed, fTimeDelta);
	}
	else if (CInputManager::GetInstance()->GetButtonPress(KEY_TYPE::A))
	{
		m_pTransform->Move_Pos(&vRight, -m_fSpeed, fTimeDelta);
	}
	else if (CInputManager::GetInstance()->GetButtonPress(KEY_TYPE::D))
	{
		m_pTransform->Move_Pos(&vRight, +m_fSpeed, fTimeDelta);
	}

	if (CInputManager::GetInstance()->GetButtonPress(KEY_TYPE::Q))
	{
		m_pTransform->Rotation(ROTATION::ROT_Y, -D3DXToRadian(m_fSpeed) * fTimeDelta * 10);
		//m_pCameraCom->Get_Transform()->Rotation(ROTATION::ROT_Y, -D3DXToRadian(m_fSpeed) * fTimeDelta * 4);
	}
	else if (CInputManager::GetInstance()->GetButtonPress(KEY_TYPE::E))
	{
		m_pTransform->Rotation(ROTATION::ROT_Y, +D3DXToRadian(m_fSpeed) * fTimeDelta * 10);
		//m_pCameraCom->Get_Transform()->Rotation(ROTATION::ROT_Y,+D3DXToRadian(m_fSpeed) * fTimeDelta * 4);
	}
	else if (CInputManager::GetInstance()->GetButtonPress(KEY_TYPE::Z))
	{
		m_pTransform->Rotation(ROTATION::ROT_X, +D3DXToRadian(m_fSpeed) * fTimeDelta * 10);
		//m_pCameraCom->Get_Transform()->Rotation(ROTATION::ROT_X, +D3DXToRadian(m_fSpeed) * fTimeDelta * 4);
	}
	else if (CInputManager::GetInstance()->GetButtonPress(KEY_TYPE::X))
	{
		m_pTransform->Rotation(ROTATION::ROT_X, -D3DXToRadian(m_fSpeed) * fTimeDelta * 10);
		//m_pCameraCom->Get_Transform()->Rotation(ROTATION::ROT_X, -D3DXToRadian(m_fSpeed) * fTimeDelta * 4);
	}


	if (m_isCameraFixed)
	{
		_vec3 vPos = *m_pTransform->GetInfoPos();
		vPos.y = m_iPosYFixedFloor + 0.5f;
		m_pTransform->Set_Pos(vPos.x , vPos.y , vPos.z);
	}
}

void CCameraMan::Change_CameraState()
{

}

void CCameraMan::Key_Update()
{
	if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::F6))
		m_isCameraFixed = !m_isCameraFixed;


	if (m_isCameraFixed == true)
	{

		if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::UP))
		{
			if (m_iPosYFixedFloor == 10)
				return;

			m_iPosYFixedFloor = m_iPosYFixedFloor + 1;
		}
		else if (CInputManager::GetInstance()->GetButtonDown(KEY_TYPE::DOWN))
		{
			if (m_iPosYFixedFloor == (int)FLOOR::FLOOR_B3)
				return;

			m_iPosYFixedFloor = m_iPosYFixedFloor - 1;

		}
	}
}

CCameraMan* CCameraMan::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCameraMan* pObj = new CCameraMan(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		// 만약에 제대로 생성이 되지 않았다면 바로 삭제.
		Safe_Release(pObj);
		MSG_BOX("Player Create Failed");
		return nullptr;
	}
	return pObj;
}
