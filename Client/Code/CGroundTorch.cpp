#include "pch.h"
#include "CGroundTorch.h"
#include "CProtoTypeManager.h"
#include "CTransform.h"
#include "CResourceManager.h"
#include "CRendererManager.h"



CGroundTorch::CGroundTorch(LPDIRECT3DDEVICE9 Device)
	: CLightObject(Device)
{
	m_tHandle.m_eType = OBJTYPE::OBJ_LIGHT;
	m_eLight_Object_Type = LIGHT_OBJECT_TYPE::GROUNDTORCH;
	m_ePlacementType = MAPOBJECT_PLACEMENT::WALL;
}

CGroundTorch::CGroundTorch(const CGroundTorch& cpy)
	: CLightObject(cpy)
{
	m_tHandle.m_eType = OBJTYPE::OBJ_LIGHT;
	m_eLight_Object_Type = LIGHT_OBJECT_TYPE::GROUNDTORCH;
	m_ePlacementType = MAPOBJECT_PLACEMENT::WALL;
}

CGroundTorch::~CGroundTorch()
{
}

HRESULT CGroundTorch::Ready_GameObject()
{
	if (FAILED(CGroundTorch::Add_Component())) return E_FAIL;


	m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"GROUNDTORCH");


	m_pAnimationCom->SetGrid(6,1); // 4열 2행
	m_pAnimationCom->SetRow(0);      // 맨 위 행
	m_pAnimationCom->SetCol(0,5);
	m_pAnimationCom->AniMation_Init(2, 0.25f);	// 몇 열이고 출력 속도 몇인지
	m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Loop);
	m_pAnimationCom->PlayAnimation();

	m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });

	return S_OK;
}

_int CGroundTorch::Update_GameObject(const _float& fTimeDelta)
{
	//if (m_isEvent == true)
	CGameObject::Update_GameObject(fTimeDelta);

	m_pAnimationCom->Update_Component(fTimeDelta);

	CRendererManager::GetInstance()->Add_RenderGroup(RENDERID::RENDER_ALPHATESTING, this);


	return 0;
}

void CGroundTorch::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);

}

void CGroundTorch::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
	m_pTextureCom->Set_Texture();
	m_pAnimationCom->Render_Buffer();
}

CGroundTorch* CGroundTorch::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGroundTorch* pObj = new CGroundTorch(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		Safe_Release(pObj);
		MSG_BOX("CGroundTorch Create Failed");
		return nullptr;
	}


	return pObj;
}

HRESULT CGroundTorch::Add_Component()
{
	//부모에는 트랜스폼 밖에없나 애니매이션 컴포넌트도 있다.
	if (FAILED(CLightObject::Add_Component()))
		return E_FAIL;

	CComponent* pComponent = nullptr;

	return S_OK;

}

void CGroundTorch::Free()
{
	CGameObject::Free();
}
