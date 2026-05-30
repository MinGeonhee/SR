#include "CEyeOfChaos.h"
#include "CProtoTypeManager.h"
#include "CEyeOfChaos_Eye.h"
#include "CResourceManager.h"
#include "CRendererManager.h"


CEyeOfChaos::CEyeOfChaos(LPDIRECT3DDEVICE9 Device)
	: CEventObject(Device) , m_pEye(nullptr) , m_pAnimationCom(nullptr)
{
	m_eObject_Event_Type = OBJECT_EVENT_TYPE::CHAOSOFEYE;
	m_ePlacement = MAPOBJECT_PLACEMENT::WALL;
	m_eCategory = MAPOBJECT_CATEGORY::EVENT;
}

CEyeOfChaos::CEyeOfChaos(const CEyeOfChaos& cpy)
	: CEventObject(cpy) , m_pEye(nullptr) , m_pAnimationCom(nullptr)
{
	m_eObject_Event_Type = OBJECT_EVENT_TYPE::CHAOSOFEYE;
	m_ePlacement = MAPOBJECT_PLACEMENT::WALL;
	m_eCategory = MAPOBJECT_CATEGORY::EVENT;
}

CEyeOfChaos::~CEyeOfChaos()
{
}

HRESULT CEyeOfChaos::Ready_GameObject()
{
	if (FAILED(CEyeOfChaos::Add_Component()))
		return E_FAIL;

	m_pEye = CEyeOfChaos_Eye::Create(m_pGraphicDev);

	if (m_pEye == nullptr)
		return E_FAIL;

	m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"EYEOFCHAOS");

	m_pAnimationCom->SetGrid(4, 4); // 열행 ㅋㅋ 열행이러고있네

	m_pAnimationCom->SetRow(1);      //

	m_pAnimationCom->SetCol(0,1);      // 맨 위 행
	m_pAnimationCom->AniMation_Init(2, 0.25f);	// 몇 열이고 출력 속도 몇인지
	m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Loop);

	m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
	m_pAnimationCom->PlayAnimation();


	return S_OK;
}

HRESULT CEyeOfChaos::Add_Component()
{

	CComponent* newCom = nullptr;

	//트랜스폼
	newCom = m_pTransform = CLONE_PROTOTYPE(PROTOTYPE::PROTO_TRANS, CTransform);
	if (newCom == nullptr)
		return E_FAIL;
	newCom->Set_MyOwner(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", newCom });


	//콜라이더
	newCom = nullptr;

	newCom = m_pColliderCom = CLONE_PROTOTYPE(PROTOTYPE::PROTO_COLLIDER_SPHERE, CSphere);
	if (newCom == nullptr)
		return E_FAIL;
	newCom->Set_MyOwner(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Collider", newCom });
	


	//애니매이션 버퍼
	newCom = m_pAnimationCom = CLONE_PROTOTYPE(PROTOTYPE::PROTO_ANIMATION, CAnimation);
	if (newCom == nullptr)
		return E_FAIL;

	newCom->Set_MyOwner(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Animation", newCom });


	return S_OK;
}

_int CEyeOfChaos::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);
	m_pEye->Update_GameObject(fTimeDelta);


	CRendererManager::GetInstance()->Add_RenderGroup(RENDERID::RENDER_ALPHATESTING, this);

	return 0;
}

void CEyeOfChaos::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
	m_pEye->LateUpdate_GameObject(fTimeDelta);

}

void CEyeOfChaos::Render_GameObject()
{
	//텍스처등록
	//해줄것 
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());

	m_pTextureCom->Set_Texture();
	m_pAnimationCom->Render_Buffer();


	m_pEye->Render_GameObject();
}

CEyeOfChaos* CEyeOfChaos::Create(LPDIRECT3DDEVICE9 Device)
{
	CEyeOfChaos* pObj = new CEyeOfChaos(Device);

	if (FAILED(pObj->Ready_GameObject()))
	{
		MSG_BOX("CEyeOfChaos_Frame Create Is Failed");
		Safe_Release(pObj);
		return nullptr;
	}


	return pObj;
}

void CEyeOfChaos::EyeEvent()
{
	m_pEye->Set_Active(false);

	m_pAnimationCom->SetGrid(4, 4); // 열행 ㅋㅋ 열행이러고있네

	m_pAnimationCom->SetRow(1);      //

	m_pAnimationCom->SetCol(2,2);      // 맨 위 행
	m_pAnimationCom->AniMation_Init(2, 0.25f);	// 몇 열이고 출력 속도 몇인지
	m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Loop);

	m_pAnimationCom->PlayAnimation();
}

void CEyeOfChaos::Setting_EventObject(const ObjectHandle& tHandle, const _vec3& vPos, const _vec3& vRot, const _vec3 vScale, const MAPOBJECT_CATEGORY& eCat, const MAPOBJECT_PLACEMENT& ePlacement, const EVENT_PATH& EventPath)
{
	CGameObject::Setting_GameObject(tHandle, vPos, vRot, vScale);

	m_pAnimationCom->Change_Buffer(m_ePlacement);

	//눈의 위치점 잡아주기
	m_pEye->Setting_GameObject(tHandle,vPos,vRot,vScale);
	m_pEye->SetControlPoint(vPos.x, vPos.y, vPos.z);


}

void CEyeOfChaos::Free()
{
	CGameObject::Free();
	Safe_Release(m_pEye);
}
