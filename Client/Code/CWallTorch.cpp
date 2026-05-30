#include "pch.h"
#include "CWallTorch.h"
#include "CProtoTypeManager.h"
#include "CTransform.h"
#include "CRendererManager.h"
#include "CResourceManager.h"

CWallTorch::CWallTorch(LPDIRECT3DDEVICE9 Device)
	: CLightObject(Device)
{
	m_tHandle.m_eType = OBJTYPE::OBJ_LIGHT;
	m_eLight_Object_Type = LIGHT_OBJECT_TYPE::WALLTORCH;
	m_ePlacementType = MAPOBJECT_PLACEMENT::WALL;
}

CWallTorch::CWallTorch(const CWallTorch& cpy)
	: CLightObject(cpy)
{
	m_tHandle.m_eType = OBJTYPE::OBJ_LIGHT;
	m_eLight_Object_Type = LIGHT_OBJECT_TYPE::WALLTORCH;
	m_ePlacementType = MAPOBJECT_PLACEMENT::WALL;
}

CWallTorch::~CWallTorch()
{
}

HRESULT CWallTorch::Ready_GameObject()
{
	if (FAILED(CWallTorch::Add_Component()))
		return E_FAIL;


	m_pTextureCom = CResourceManager::GetInstance()->Get<CTexture>(L"WALLTORCH");


	m_pAnimationCom->SetGrid(4, 2); // 4행 8열
	m_pAnimationCom->SetRow(0);      // 맨 위 행
	m_pAnimationCom->AniMation_Init(0, 0.25f);	// 몇 열이고 출력 속도 몇인지
	m_pAnimationCom->SetPlayMode(CAnimation::PlayMode::Loop);

	m_pTransform->Set_Scale(_vec3{ 1.f, 1.f, 1.f });
	m_pAnimationCom->PlayAnimation();

	return S_OK;
}

_int CWallTorch::Update_GameObject(const _float& fTimeDelta)
{
	if (m_isEvent == true)
		CGameObject::Update_GameObject(fTimeDelta);

	m_pAnimationCom->Update_Component(fTimeDelta);

	CRendererManager::GetInstance()->Add_RenderGroup(RENDER_ALPHATESTING, this);

	return 0;
}

void CWallTorch::LateUpdate_GameObject(const _float& fTimeDelta)
{
	if (m_isEvent == true)
		CGameObject::LateUpdate_GameObject(fTimeDelta);


	m_pAnimationCom->LateUpdate_Component(fTimeDelta);
}

void CWallTorch::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_World());
	m_pTextureCom->Set_Texture();
	m_pAnimationCom->Render_Buffer();
}

CWallTorch* CWallTorch::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CWallTorch* pObj = new CWallTorch(pGraphicDev);

	if (FAILED(pObj->Ready_GameObject()))
	{
		Safe_Release(pObj);
		MSG_BOX("CWallTorch Create Failed");
		return nullptr;
	}


	return pObj;
}

HRESULT CWallTorch::Add_Component()
{
	//부모에는 트랜스폼 애니매이션  밖에없나 애니매이션 컴포넌트도 있다.
	if (FAILED(CLightObject::Add_Component()))
		return E_FAIL;

	CComponent* pComponent = NULL;


	return S_OK;

}

void CWallTorch::Free()
{
	CGameObject::Free();
}
