#include "CLightObject.h"
#include "CProtoTypeManager.h"


CLightObject::CLightObject(LPDIRECT3DDEVICE9 pGraphicDev)
    : CGameObject(pGraphicDev), m_eLight_Object_Type(LIGHT_OBJECT_TYPE::NONE) ,  m_isEvent(false) , m_ePlacementType(MAPOBJECT_PLACEMENT::_END)
{
    m_tHandle.m_eType = OBJTYPE::OBJ_LIGHT;
}


CLightObject::CLightObject(const CLightObject& cpy)
    : CGameObject(cpy), m_eLight_Object_Type(cpy.m_eLight_Object_Type), m_ePlacementType(MAPOBJECT_PLACEMENT::_END)
{
}

CLightObject::~CLightObject()
{
}

_int CLightObject::Update_GameObject(const _float& fTimeDelta)
{
    return 0;
}

void CLightObject::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CLightObject::Render_GameObject()
{
}

CLightObject* CLightObject::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    return nullptr;
}

HRESULT CLightObject::Ready_GameObject()
{
    return S_OK;
}


void CLightObject::Setting(const ObjectHandle& tHandle, const _vec3 vPos, const _vec3& vRot, const _vec3& vScale, const MAPOBJECT_PLACEMENT& ePlacement)
{
	CGameObject::Setting_GameObject(tHandle, vPos, vRot, vScale);

	m_ePlacementType = ePlacement;

	if (m_pColliderCom != nullptr)
	{
		if (m_ePlacementType == MAPOBJECT_PLACEMENT::GROUND)
		{
			m_pColliderCom->Set_ColliderScale(vScale * 0.8f);
			m_pColliderCom->Set_Offset(_vec3(0.f,vScale.y * 0.5f , 0.f) );
		}
	}

	m_pAnimationCom->Change_Buffer(ePlacement);

	if (m_eLight_Object_Type == LIGHT_OBJECT_TYPE::STATUE)
	{
		_vec3 Scale = m_pTransform->Get_Scale();
		Scale.x = 1;
		m_pTransform->Set_Scale(Scale);
	}
}

HRESULT CLightObject::Add_Component()
{
	CComponent* pComponent = NULL;
	// transform 컴포넌트.

	pComponent = m_pTransform = CLONE_PROTOTYPE(PROTO_TRANS, CTransform);

	if (pComponent == NULL)
		return E_FAIL;

	m_pTransform->Set_MyOwner(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	//=========== 애니메이션 컴포넌트 생성 ===========

	pComponent = m_pAnimationCom = CLONE_PROTOTYPE(PROTO_ANIMATION, CAnimation);

	if (nullptr == pComponent)
		return E_FAIL;

	m_pAnimationCom->Set_MyOwner(this);
	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Animation",pComponent });

    return S_OK;
}

void CLightObject::Free()
{
}
